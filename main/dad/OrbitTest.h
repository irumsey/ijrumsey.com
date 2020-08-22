#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>
#include <dad/Test.h>

///
///
///
namespace lucid {
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

struct MouseEvent;

///
///
///
class OrbitTest : public Test
{
public:
	OrbitTest() = default;

	virtual ~OrbitTest() = default;

	virtual bool passed() const override;

	virtual void begin(float64_t t) override;

	virtual void onInput(MouseEvent const &event) override;

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	typedef ::lucid::math::Vector<float32_t,2> Vector2;
	typedef ::lucid::math::Vector<float32_t,3> Vector3;
	typedef ::lucid::math::Vector<float32_t,4> Vector4;
	typedef ::lucid::math::Matrix<float32_t,4,4> Matrix4x4;

	bool _passed = true;

	::lucid::orbit::Properties _bodyProperties;
	::lucid::orbit::Elements _orbitElements;

	Vector3 _viewPosition;
	Vector3 _viewDirection;

	lucid::gigl::Context _context;
	std::shared_ptr<lucid::gigl::Mesh> _orbit;

	LUCID_PREVENT_COPY(OrbitTest);
	LUCID_PREVENT_ASSIGNMENT(OrbitTest);
};

inline bool OrbitTest::passed() const
{
	return _passed;
}
