#pragma once

#include <ctime>
#include <cmath>
#include <lucid/math/Math.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>

LUCID_ORBIT_BEGIN

///
///
///

inline float32_t cast(scalar_t rhs)
{
	return float32_t(rhs);
}

inline LUCID_GAL::Vector2 cast(vector2_t const &rhs)
{
	return LUCID_GAL::Vector2(float32_t(rhs.x), float32_t(rhs.y));
}

inline LUCID_GAL::Vector3 cast(vector3_t const &rhs)
{
	return LUCID_GAL::Vector3(float32_t(rhs.x), float32_t(rhs.y), float32_t(rhs.z));
}

inline LUCID_GAL::Vector4 cast(vector4_t const &rhs)
{
	return LUCID_GAL::Vector4(float32_t(rhs.x), float32_t(rhs.y), float32_t(rhs.z), float32_t(rhs.w));
}

inline LUCID_GAL::Quaternion cast(quaternion_t const &rhs)
{
	return LUCID_GAL::Quaternion(float32_t(rhs.x), float32_t(rhs.y), float32_t(rhs.z), float32_t(rhs.w));
}

inline LUCID_GAL::Matrix3x3 cast(matrix3x3_t const &rhs)
{
	return LUCID_GAL::Matrix3x3(
		float32_t(rhs.xx), float32_t(rhs.xy), float32_t(rhs.xz),
		float32_t(rhs.yx), float32_t(rhs.yy), float32_t(rhs.yz),
		float32_t(rhs.zx), float32_t(rhs.zy), float32_t(rhs.zz)
	);
}

inline LUCID_GAL::Matrix4x4 cast(matrix4x4_t const &rhs)
{
	return LUCID_GAL::Matrix4x4(
		float32_t(rhs.xx), float32_t(rhs.xy), float32_t(rhs.xz), float32_t(rhs.xw),
		float32_t(rhs.yx), float32_t(rhs.yy), float32_t(rhs.yz), float32_t(rhs.yw),
		float32_t(rhs.zx), float32_t(rhs.zy), float32_t(rhs.zz), float32_t(rhs.zw),
		float32_t(rhs.wx), float32_t(rhs.wy), float32_t(rhs.wz), float32_t(rhs.ww)
	);
}

///
///	TBD: i would like these removed
/// the above casts take more precision to less for rendering purposes.
/// i don't like the chance of less being cast to "more" without the more.
///

inline scalar_t cast(float32_t rhs)
{
	return scalar_t (rhs);
}

inline vector2_t cast(LUCID_GAL::Vector2 const &rhs)
{
	return vector2_t(scalar_t(rhs.x), scalar_t(rhs.y));
}

inline vector3_t cast(LUCID_GAL::Vector3 const &rhs)
{
	return vector3_t(scalar_t(rhs.x), scalar_t(rhs.y), scalar_t(rhs.z));
}

inline vector4_t cast(LUCID_GAL::Vector4 const &rhs)
{
	return vector4_t(scalar_t(rhs.x), scalar_t(rhs.y), scalar_t(rhs.z), scalar_t(rhs.w));
}

/// JDN
///
/// Used to compute the Julian Day Number.  All times are UTC.
/// The idea is for the simulation to keep time in terms of Julian Day Numbers
/// measured in Universal Coordinated Time.  This will allow the simulator to
/// simply "tick" off time in seconds.  Then, at each simulation step convert
/// JDN in UTC to JDN in TDB for the updates.
/// 
/// See: TDB
struct JDN
{
	// since the JPL Horizons database is where I have been getting the orbital elements,
	// I have tested the following against the Horizons website which provides a JD date/time
	// conversion.
	static scalar_t from(Date const &date, Time const &time = Time())
	{
		int32_t  year = (2 < date.month) ? date. year : date. year -  1;
		int32_t month = (2 < date.month) ? date.month : date.month + 12;
		int32_t   day = date.day;

		int32_t A = year / 100;
		int32_t B = 2 - A + (A / 4);

		scalar_t frac = (3600.0 * time.hour + 60.0 * time.minute + time.second) / constants::seconds_per_day;

		return int32_t(365.25 * (year + 4716)) + int32_t(30.6001 * (month + 1)) + day + B - 1524.5 + frac;
	}

	static scalar_t now()
	{
		std::time_t localTime = std::time(nullptr);
		std::tm utcTime;

		::gmtime_s(&utcTime, &localTime);

		Date date { utcTime.tm_year + 1900, utcTime.tm_mon + 1, utcTime.tm_mday };
		Time time { utcTime.tm_hour, utcTime.tm_min, scalar_t(utcTime.tm_sec) };

		return from(date, time);
	}

	static scalar_t toCentury2000(scalar_t jdn)
	{
		return (jdn - constants::J2000) / constants::days_per_century;
	}

};

///	ERA
/// 
/// Earth Rotation Angle
struct ERA
{
	static scalar_t now()
	{
		return from(JDN::now());
	}

	static scalar_t from(Date const &date, Time const &time = Time())
	{
		return from(JDN::from(date, time));
	}

	static scalar_t from(scalar_t jdn)
	{
		// TBD: (UT1 - UTC) is a result of a table lookup
		// implement it
		scalar_t days = jdn - constants::J2000;
		scalar_t UT1 = days /* - (UT1 - UTC) */ ;
		return constants::two_pi * (0.779057273264 + 1.00273781191135448 * UT1);
	}
};

///
/// 
/// 
struct NameThis
{

	// obliquity of the ecliptic
	static scalar_t epsilon(scalar_t jdn)
	{
		scalar_t const c[] = {
			LUCID_MATH::deg2rad(23.0 + 26.0 / 60.0 + 21.45000 / 3600.0),
			LUCID_MATH::deg2rad( 0.0 +  0.0 / 60.0 - 46.81500 / 3600.0),
			LUCID_MATH::deg2rad( 0.0 +  0.0 / 60.0 -  0.00060 / 3600.0),
			LUCID_MATH::deg2rad( 0.0 +  0.0 / 60.0 +  0.00181 / 3600.0),
		};

		scalar_t const T = JDN::toCentury2000(jdn);
		scalar_t const TT = T * T;
		scalar_t const TTT = TT * T;

		return c[0] + c[1] * T + c[2] * TT + c[3] * TTT;
	}

	static matrix3x3_t BPN(vector3_t cip)
	{
		scalar_t a = 1.0 / (1.0 + cip.z);

		matrix3x3_t BPN = matrix3x3_t(
			1 - a * cip.x * cip.x,    -a * cip.x * cip.y,                                  cip.x,
			   -a * cip.x * cip.y, 1 - a * cip.y * cip.y,                                  cip.y,
						   -cip.x,                -cip.y, 1 - a * (cip.x * cip.x + cip.y * cip.y)
		);

		// TBD: CIO locator s matrix
		matrix3x3_t Rs(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		);

		return BPN * Rs;
	}

};

///
/// 
/// 
inline vector2_t computeConicPoint(scalar_t hu, scalar_t e, scalar_t theta)
{
	scalar_t c = LUCID_MATH::cos(theta);
	scalar_t s = LUCID_MATH::sin(theta);
	scalar_t r = hu / (1.0 + e * c);

	return r * vector2_t(c, s);
}

///
///
///
inline matrix3x3_t rotationFromElements(OrbitalElements const &elements)
{
	return
		LUCID_MATH::rotateAboutZ(elements.OM) *
		LUCID_MATH::rotateAboutX(elements.IN) *
		LUCID_MATH::rotateAboutZ(elements. W);
}

///	kinematicsFromElements
///
///	Note: the jdn parameter must be in TDB.
inline void kinematicsFromElements(vector3_t &position, vector3_t &velocity, OrbitalElements const &elements, PhysicalProperties const &centerProperties, scalar_t jdn)
{
	scalar_t const    GM = centerProperties.GM;
	scalar_t const     e = elements.EC;
	scalar_t const     a = elements.A;

	scalar_t const dt = jdn - elements.JDN;
	scalar_t MA = elements.MA + elements.N * dt;

	MA = std::fmod(MA, constants::two_pi);
	MA = (MA < 0.0) ? MA + constants::two_pi: MA;

	scalar_t EA[2] = { MA, 0.0 };

	size_t const limit = 10;
	size_t iter = 0;

	scalar_t err = EA[0] - EA[1];
	while (((err * err) > constants::tolsq) && (iter < limit))
	{
		EA[1] = EA[0] - (EA[0] - e * LUCID_MATH::sin(EA[0]) - MA) / (1.0 - e * LUCID_MATH::cos(EA[0]));
		err = EA[1] - EA[0];

		std::swap(EA[0], EA[1]);
		++iter;
	}

	scalar_t TA = 2.0 * LUCID_MATH::atan2(LUCID_MATH::sqrt(1.0 + e) * LUCID_MATH::sin(0.5 * EA[0]), LUCID_MATH::sqrt(1.0 - e) * LUCID_MATH::cos(0.5 * EA[0]));
	scalar_t  r = a * (1.0 - e * LUCID_MATH::cos(EA[0]));

	position = r * vector3_t(LUCID_MATH::cos(TA), LUCID_MATH::sin(TA), 0.0);
	velocity = LUCID_MATH::sqrt(GM * a) / r * vector3_t(-LUCID_MATH::sin(EA[0]), LUCID_MATH::sqrt(1.0 - e * e) * LUCID_MATH::cos(EA[0]), 0.0);

	matrix3x3_t R = rotationFromElements(elements);

	position = R * position;
	velocity = R * velocity;
}

///	rotationFromElements
///
///	Note: the jdn parameter must be in TDB.
inline void rotationFromElements(matrix3x3_t &rotation, RotationalElements const &elements, scalar_t jdn)
{
	matrix3x3_t const R_y = LUCID_MATH::rotateAboutY(0.5 * constants::pi);
	matrix3x3_t const R_x = LUCID_MATH::rotateAboutX(0.5 * constants::pi);
	matrix3x3_t const R_align = R_x * R_y;

	scalar_t const  T = JDN::toCentury2000(jdn);
	scalar_t const TT = T * T;
	scalar_t const  d = jdn - constants::J2000;

	scalar_t  ra = elements. ra[0] + elements. ra[1] * T + elements. ra[2] * TT;
	scalar_t dec = elements.dec[0] + elements.dec[1] * T + elements.dec[2] * TT;
	scalar_t   w = elements. pm[0] + elements. pm[1] * d;
	
	matrix3x3_t R_w  = LUCID_MATH::rotateAboutX(w);
	matrix3x3_t R_dec = LUCID_MATH::rotateAboutY(-dec);
	matrix3x3_t R_ra  = LUCID_MATH::rotateAboutZ(ra);

	matrix3x3_t R_ecliptic = LUCID_MATH::rotateAboutX(-NameThis::epsilon(jdn));

	rotation = R_ecliptic * R_ra * R_dec * R_w * R_align;
}

LUCID_ORBIT_END
