#include "MathTest.h"
#include "Utility.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/Types.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Constants.h>

///
///
///

namespace constants = lucid::math::constants;
namespace math = lucid::math;

namespace /* anonymous */
{

	template<class T> inline bool validate(std::string const &test, T const &value, T const &target)
	{
		bool passed = math::equ(value, target);

		std::string result = passed ? "PASS" : "FAIL";
		::log(result, test);

		return passed;
	}

}	///	anonymous

///
///
///

void MathTest::begin(float64_t t)
{
	::log("INFO", "starting math test...");
}

bool MathTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	float32_t const theta = 37.3f * constants::pi<float32_t>() / 180.f;
	float32_t const half_pi = constants::half_pi<float32_t>();
	float32_t const quarter_pi = constants::quarter_pi<float32_t>();

	///
	///	Vector tests...
	///
	typedef math::Vector<float32_t, 3> Vector3;

	Vector3 e0 = math::normalize(Vector3(1, 1, 0));
	Vector3 e2(0, 0, 1);
	Vector3 e1 = math::cross(e2, e0);

	_passed &= validate("vector dot product", dot(e0, e1), math::cos(constants::half_pi<float32_t>()));

	///
	///	quaternion tests...
	///
	typedef math::Quaternion<float32_t> Quaternion;

	Quaternion q = math::rotateUsingAxis(Vector3(0, 0, 1), theta);
	_passed &= validate("rotate using quaternion", math::transformDirection(q, Vector3(1, 0, 0)), math::rotateAboutZ(theta) * Vector3(1, 0, 0));

	q =  math::rotateUsingAxis(Vector3(0, 0, 1), quarter_pi);
	_passed &= validate("rotate using quaternion", math::transformDirection(q, Vector3(1, 0, 0)), math::rotateAboutZ(quarter_pi) * Vector3(1, 0, 0));

	q =  math::rotateUsingAxis(Vector3(0, 0, 1), half_pi);
	_passed &= validate("rotate using quaternion", math::transformDirection(q, Vector3(1, 0, 0)), math::rotateAboutZ(half_pi) * Vector3(1, 0, 0));

	///
	///	Matrix tests...
	///
	typedef math::Matrix<float32_t, 3, 3> Matrix3x3;

	Matrix3x3 const I = Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	Matrix3x3 R(e0.x, e1.x, e2.x, e0.y, e1.y, e2.y, e0.z, e1.z, e2.z);

	_passed &= validate("rotate using matrix", R, math::rotateAboutZ(quarter_pi));
	_passed &= validate("matrix inverse", R * math::inverse(R), I);
	_passed &= validate("matrix transpose", R * math::transpose(R), I);

	return true;
}

void MathTest::render(float32_t time, float32_t interpolant)
{
	lucid::gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	pipeline.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}
