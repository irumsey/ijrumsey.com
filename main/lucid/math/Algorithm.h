#pragma once

#include <algorithm>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Quaternion.h>
#include <lucid/math/Sphere.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Frustum.h>

#ifdef min
#	undef min
#	undef max
#endif

#pragma push_macro("SCALAR")
#define SCALAR(T, S, Q) ::lucid::math::Scalar<T, ::lucid::units::Unit<S, Q> >

#pragma push_macro("VECTOR")
#define VECTOR(T, DIM, S, Q) ::lucid::math::Vector<T, DIM, ::lucid::units::Unit<S, Q> >

#pragma push_macro("MATRIX")
#define MATRIX(T, ROWS, COLS, S, Q) ::lucid::math::Matrix<T, ROWS, COLS, ::lucid::units::Unit<S, Q> >

#pragma push_macro("QUATERNION")
#define QUATERNION(T) ::lucid::math::Quaternion<T>

#pragma push_macro("SPHERE")
#define SPHERE(T, S, Q) ::lucid::math::Sphere<T, 3, ::lucid::units::Unit<S, Q> >

#pragma push_macro("PLANE")
#define PLANE(T, S, Q) ::lucid::math::Plane<T, 3, S, Q>

#pragma push_macro("FRUSTUM")
#define FRUSTUM(T, S, Q) ::lucid::math::Frustum<T, S, Q>

#pragma push_macro("BOX")
#define BOX(T, S, Q) ::lucid::math::AABB<T, 3, ::lucid::units::Unit<S, Q> >

namespace lucid {
namespace math {

	/// min / max
	///
	///

	template<typename T> inline T min(T const &x, T const &y)
	{
		return (x <= y) ? x : y;
	}

	template<typename T> inline T max(T const &x, T const &y)
	{
		return (x >= y) ? x : y;
	}

	///	interpolate
	///
	///	linear interpolation from a to b.
	template<typename T, typename U> inline U lerp(T const &t, U const &a, U const &b)
	{
		return (b - a) * t + a;
	}

	///	clamp
	///
	///	clamp value between values a and b
	///	where a < b
	template<typename T> inline T clamp(T const &x, T const &a, T const &b)
	{
		return std::min(std::max(a, x), b);
	}

	///	exclude
	///
	///	range of exclusion for the value x defined by a and b
	///	where a < b
	template<typename T> inline T exclude(T const &x, T const &a, T const &b)
	{
		if ((x < a) || (b < x))
			return x;

		if ((b - x) > (x - a))
			return a;

		return b;
	}

	///
	///	power "wrapper"
	///

	template<typename T> inline T pow(T x, T e)
	{
		return std::pow(x, e);
	}

	///
	///	square root "wrapper"
	///

	template<typename T> inline T sqrt(T x)
	{
		return std::sqrt(x);
	}

	///
	///	cosine "wrappers"
	///

	template<typename T> inline T cos(T theta)
	{
		return std::cos(theta);
	}

	template<typename T> inline T acos(T x)
	{
		return std::acos(x);
	}

	///
	///	sine "wrappers"
	///

	template<typename T> inline T sin(T theta)
	{
		return std::sin(theta);
	}

	template<typename T> inline T asin(T x)
	{
		return std::asin(x);
	}

	///
	///	tangent "wrappers"
	///

	template<typename T> inline T tan(T theta)
	{
		return std::tan(theta);
	}

	template<typename T> inline T atan(T x)
	{
		return std::atan(x);
	}

	template<typename T> inline T atan2(T y, T x)
	{
		return std::atan2(y, x);
	}

	///	quaternion to matrix
	///
	///
	template<typename T, typename S, typename Q>
	inline MATRIX(T, 3, 3, S, Q) matrixFromQuaternion(QUATERNION(T) const &q) 
	{
		T xx = q.x * q.x;
		T xy = q.x * q.y;
		T xz = q.x * q.z;
		T xw = q.x * q.w;
		T yy = q.y * q.y;
		T yz = q.y * q.z;
		T yw = q.y * q.w;
		T zz = q.z * q.z;
		T zw = q.z * q.w;

		return MATRIX(T, 3, 3, S, Q)(
			T(1) - T(2) * ( yy + zz ),        T(2) * ( xy - zw ),        T(2) * ( xz + yw ), T(0),
			       T(2) * ( xy + zw ), T(1) - T(2) * ( xx + zz ),        T(2) * ( yz - xw ), T(0),
			       T(2) * ( xz - yw ),        T(2) * ( yz + xw ), T(1) - T(2) * ( xx + yy ), T(0),
			                     T(0),                      T(0),                      T(0), T(1)
		);
	}

	///	matrix to quaternion
	///
	///
	template<typename T, typename S, typename Q>
	inline QUATERNION(T) quaternionFromMatrix(MATRIX(T, 3, 3, S, Q) const &R)
	{
		QUATERNION(T) q;

		q.w = T(0.5) * sqrt(T(1) + R.xx + R.yy + R.zz);
		T coeff = T(1) / (T(4) * q.w);

		q.x = coeff * (R.zy - R.yz);
		q.y = coeff * (R.xz - R.zx);
		q.z = coeff * (R.yx - R.xy);

		return q;
	}

	///	intersects
	///
	///	Sphere intersects AABB
	template<typename T, typename S, typename Q>
	inline bool intersects(SPHERE(T, S, Q) const &sphere, BOX(T, S, Q) const &aabb)
	{
		T RR = sphere.radius * sphere.radius;

		auto S = [](T x) { return x * x; };

		if (sphere.center.x < aabb.min.x)
		{
			RR -= S(sphere.center.x - aabb.min.x);
		}
		else if (sphere.center.x > aabb.max.x)
		{
			RR -= S(sphere.center.x - aabb.max.x);
		}

		if (sphere.center.y < aabb.min.y)
		{
			RR -= S(sphere.center.y - aabb.min.y);
		}
		else if (sphere.center.y > aabb.max.y)
		{
			RR -= S(sphere.center.y - aabb.max.y);
		}

		if (sphere.center.z < aabb.min.z)
		{
			RR -= S(sphere.center.z - aabb.min.z);
		}
		else if (sphere.center.z > aabb.max.z)
		{
			RR -= S(sphere.center.z - aabb.max.z);
		}

		return RR > T(0);
	}

	///	intersects
	///
	///	Frustum intersects AABB
	template<typename T, typename S, typename Q>
	inline bool intersects(FRUSTUM(T, S, Q) const &frustum, BOX(T, S, Q) const &aabb)
	{
		int const ELEMENT_COUNT = 3;
		int const CORNER_COUNT = Frustum<T, S, Q>::CORNER_COUNT;

		for (int i = 0; i < ELEMENT_COUNT; ++i)
		{
			int left = 0;
			int right = 0;

			for (int j = 0; j < CORNER_COUNT; ++j)
			{
				left = (frustum.corners[j].elements[i] < aabb.min[i]) ? left + 1 : left;
				right = (aabb.max[i] < frustum.corners[j].elements[i]) ? right + 1 : right;
			}

			if ((CORNER_COUNT == left) || (CORNER_COUNT == right))
			{
				return false;
			}
		}

		VECTOR(T, 3, S, Q) const boxCorners[] =
		{
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.min.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.min.y, aabb.max.z),
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.max.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.max.y, aabb.max.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.min.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.min.y, aabb.max.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.max.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.max.y, aabb.max.z),
		};

		for (int i = 0; i < Frustum<T, S, Q>::PLANE_COUNT; ++i)
		{
			PLANE(T, S, Q) const &plane = frustum.planes[i];

			int behind = 0;
			for (int j = 0; j < CORNER_COUNT; ++j)
			{
				if (dot(plane, boxCorners[j]) > 0.f)
					break;
				++behind;
			}

			if (CORNER_COUNT == behind)
			{
				return false;
			}
		}

		return true;
	}

	///	project
	///
	///	project a specified number of 3D points onto an axis and return a min/max range.
	template<typename T, typename S, typename Q>
	inline std::pair<T, T> project(VECTOR(T, 3, S, Q) const *vertices, size_t vertexCount, VECTOR(T, 3, S, Q) const &axis)
	{
		std::pair<T, T> result(0, 0);

		result.first = dot(vertices[0], axis);
		result.second = result.first;

		for (int32_t i = 1; i < vertexCount; ++i)
		{
			SCALAR(T, S, Q) D = dot(vertices[i], axis);

			result.first = std::min(result.first, D.value);
			result.second = std::max(result.second, D.value);
		}

		return result;
	}

	///	intersect
	///
	///	3D AABB intersects 3D triangle
	template<typename T, typename S, typename Q>
	inline bool intersect(BOX(T, S, Q) const &box, VECTOR(T, 3, S, Q) const &v_i, VECTOR(T, 3, S, Q) const &v_j, VECTOR(T, 3, S, Q) const &v_k)
	{
		VECTOR(T, 3, S, Q) const boxNormals[] =
		{
			VECTOR(T, 3, S, Q)(1, 0, 0),
			VECTOR(T, 3, S, Q)(0, 1, 0),
			VECTOR(T, 3, S, Q)(0, 0, 1),
		};

		VECTOR(T, 3, S, Q) const faceVertices[] = { v_i, v_j, v_k, };

		for (size_t i = 0; i < 3; ++i)
		{
			std::pair<T, T> range = project(faceVertices, 3, boxNormals[i]);
			if ((range.second < box.min[i]) || (box.max[i] < range.first))
			{
				return false;
			}
		}

		VECTOR(T, 3, S, Q) const boxVertices[] =
		{
			VECTOR(T, 3, S, Q)(box.min.x, box.min.y, box.min.z),
			VECTOR(T, 3, S, Q)(box.min.x, box.min.y, box.max.z),
			VECTOR(T, 3, S, Q)(box.min.x, box.max.y, box.min.z),
			VECTOR(T, 3, S, Q)(box.min.x, box.max.y, box.max.z),
			VECTOR(T, 3, S, Q)(box.max.x, box.min.y, box.min.z),
			VECTOR(T, 3, S, Q)(box.max.x, box.min.y, box.max.z),
			VECTOR(T, 3, S, Q)(box.max.x, box.max.y, box.min.z),
			VECTOR(T, 3, S, Q)(box.max.x, box.max.y, box.max.z),
		};

		{
			VECTOR(T, 3, S, Q) const faceNormal = normalize(cross(v_j - v_i, v_k - v_i));
			SCALAR(T, S, Q) D = dot(faceNormal, v_i);

			std::pair<T, T> range = project(boxVertices, 8, faceNormal);
			if ((range.second < D) || (D < range.first))
			{
				return false;
			}
		}

		VECTOR(T, 3, S, Q) const faceEdges[] = { v_j - v_i, v_k - v_j, v_i - v_k, };

		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				VECTOR(T, 3, S, Q) axis = cross(faceEdges[i], boxNormals[j]);

				std::pair<T, T> boxRange = project(boxVertices, 8, axis);
				std::pair<T, T> faceRange = project(faceVertices, 3, axis);
				if ((boxRange.second < faceRange.first) || (faceRange.second < boxRange.first))
				{
					return false;
				}
			}
		}

		return true;
	}

	///	contains
	///
	///	3D Frustum contains 3D AABB
	template<typename T, typename S, typename Q>
	inline bool contains(FRUSTUM(T, S, Q) const &frustum, BOX(T, S, Q) const &aabb)
	{
		int const CORNER_COUNT = 8;

		VECTOR(T, 3, S, Q) const corners[] =
		{
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.min.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.min.y, aabb.max.z),
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.max.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.min.x, aabb.max.y, aabb.max.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.min.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.min.y, aabb.max.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.max.y, aabb.min.z),
			VECTOR(T, 3, S, Q)(aabb.max.x, aabb.max.y, aabb.max.z),
		};

		for (size_t i = 0; i < FRUSTUM(T, S, Q)::PLANE_COUNT; ++i)
		{
			PLANE(T, S, Q) const &plane = frustum.planes[i];

			for (size_t j = 0; j < CORNER_COUNT; ++j)
			{
				if (dot(plane, corners[j]) < T(0))
					return false;
			}
		}

		return true;
	}

	///	areaProjected
	///
	///
	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, Q) areaProjected(MATRIX(T, 4, 4, S, Q) const &viewProjMatrix, SCALAR(T, S, Q) const &znear, BOX(T, S, Q) const &aabb)
	{
		size_t const CORNER_COUNT = 8;

		VECTOR(T, 4, S, Q) const corners[] =
		{
			VECTOR(T, 4, S, Q)(aabb.min.x, aabb.min.y, aabb.min.z, T(1)),
			VECTOR(T, 4, S, Q)(aabb.min.x, aabb.min.y, aabb.max.z, T(1)),
			VECTOR(T, 4, S, Q)(aabb.min.x, aabb.max.y, aabb.min.z, T(1)),
			VECTOR(T, 4, S, Q)(aabb.min.x, aabb.max.y, aabb.max.z, T(1)),
			VECTOR(T, 4, S, Q)(aabb.max.x, aabb.min.y, aabb.min.z, T(1)),
			VECTOR(T, 4, S, Q)(aabb.max.x, aabb.min.y, aabb.max.z, T(1)),
			VECTOR(T, 4, S, Q)(aabb.max.x, aabb.max.y, aabb.min.z, T(1)),
			VECTOR(T, 4, S, Q)(aabb.max.x, aabb.max.y, aabb.max.z, T(1)),
		};

		VECTOR(T, 4, S, Q) projected = viewProjMatrix * corners[0];
		projected = projected / exclude(SCALAR(T, S, Q)(projected.w), -znear, znear);

		T xmin = projected.x;
		T ymin = projected.y;

		T xmax = projected.x;
		T ymax = projected.y;

		for (size_t i = 1; i < CORNER_COUNT; ++i)
		{
			projected = viewProjMatrix * corners[i];
			projected = projected / exclude(SCALAR(T, S, Q)(projected.w), -znear, znear);

			xmin = std::min(xmin, projected.x);
			ymin = std::min(ymin, projected.y);

			xmax = std::max(xmax, projected.x);
			ymax = std::max(ymax, projected.y);
		}

		return (xmax - xmin) * (ymax - ymin);
	}

}	///	math
}	///	lucid

#undef BOX
#pragma pop_macro("BOX")

#undef FRUSTUM
#pragma pop_macro("FRUSTUM")

#undef SPHERE
#pragma pop_macro("SPHERE")

#undef PLANE
#pragma pop_macro("PLANE")

#undef QUATERNION
#pragma pop_macro("QUATERNION")

#undef MATRIX
#pragma pop_macro("MATRIX")

#undef VECTOR
#pragma pop_macro("VECTOR")

#undef SCALAR
#pragma pop_macro("SCALAR")

