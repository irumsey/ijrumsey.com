#pragma once

#include <lucid/core/Types.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Camera2D.h>

namespace Lucid {
namespace Math {

	ref class Vector2;
	ref class Vector3;
	ref class Vector4;
	ref class Quaternion;
	ref class Matrix4x4;

} /// Math
} /// Lucid

namespace Lucid {
namespace GIGL {

	/// 
	/// 
	/// 
	public ref class Camera2D
	{
	public:
		Camera2D(System::String ^path);

		Camera2D(LUCID_GIGL::Camera2D const &camera);

		Camera2D();

		~Camera2D();

		!Camera2D();

		void InitPerspective(float fov, float aspect, float znear, float zfar);

		void InitOrthographic(float width, float height, float znear, float zfar);

		void Look(Math::Vector3 ^position, Math::Vector3 ^target, Math::Vector3 ^up);

		property Math::Vector3 ^position { Math::Vector3 ^get(); }

		property Math::Vector3 ^right { Math::Vector3 ^get(); }

		property Math::Vector3 ^forward { Math::Vector3 ^get(); }

		property Math::Vector3 ^up { Math::Vector3 ^get(); }

		property Math::Matrix4x4 ^viewMatrix { Math::Matrix4x4 ^get(); }

		property Math::Matrix4x4 ^projMatrix { Math::Matrix4x4 ^get(); }

		property Math::Matrix4x4 ^viewProjMatrix { Math::Matrix4x4 ^get(); }

		property Math::Matrix4x4 ^invViewProjMatrix { Math::Matrix4x4 ^get(); }

		property LUCID_GIGL::Camera2D &ref { LUCID_GIGL::Camera2D &get() { return *ptr; } }

		property LUCID_GIGL::Camera2D *ptr { LUCID_GIGL::Camera2D *get() { return _internal; } }

	private:
		LUCID_GIGL::Camera2D *_internal = nullptr;

	};

} /// GIGL
} /// Lucid
