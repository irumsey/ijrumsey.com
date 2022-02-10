#pragma once

#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/rm/Mind.h>

namespace lucid {
namespace gal {

	class VertexFormat;
	class VertexBuffer;
	
	class Program;
	class Parameter;

}	///	gal
}	///	lucid

namespace lucid {
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

class Session
{
public:
	Session() = default;

	virtual ~Session() = default;

	void initialize();

	void shutdown();

	void update(float64_t t, float64_t dt);

	void render(float64_t t, float32_t interpolant);

private:
	enum { INSTANCE_MAXIMUM = 2048 };

	struct Instance
	{
		::lucid::gal::Vector3 position[2];
		float32_t scale[2];
	};

	bool _initialized = false; // no state classes (yet)

	::lucid::rm::Mind *_mind = nullptr;

	::lucid::gigl::Context _context;

	::lucid::gigl::Mesh *_mesh = nullptr;
	::lucid::gal::VertexBuffer *_instances = nullptr;

	size_t _instanceCount = 0;
};
