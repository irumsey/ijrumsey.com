#include "Compositor.h"
#include "StarCatalog.h"
#include "Frame.h"
#include "Selection.h"
#include "Utility.h"
#include <lucid/gigl/Model.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/math/Algorithm.h>
#include <algorithm>

LUCID_ANONYMOUS_BEGIN

template<typename T> struct Back2Front
{
	bool operator()(T const &lhs, T const &rhs) const
	{
		return lhs.distance > rhs.distance;
	}
};

LUCID_ANONYMOUS_END

LUCID_ORBIT_BEGIN

Compositor::Compositor()
{
}

Compositor::~Compositor()
{
	shutdown();
}

void Compositor::initialize(size_t passMaximum, float32_t midRange)
{
	shutdown();

	_passMaximum = passMaximum;
	_midRange = midRange;

	_starCount = LUCID_ORBIT_STARCATALOG.count();
	_starMesh.reset(LUCID_GIGL::Mesh::create("content/star.mesh"));
	_starInstances.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_STATIC, int32_t(_starCount), sizeof(StarInstance)));

	StarInstance *starInstances = (StarInstance *)(_starInstances->lock());
	for (size_t i = 0; i < _starCount; ++i)
	{
		StarCatalog::Entry const &entry = LUCID_ORBIT_STARCATALOG[i];
		StarInstance &instance = starInstances[i];

		instance.          id = uint32_t((Selection::TYPE_STAR << Selection::SELECT_SHIFT) | i);
		instance.parameters.x = /* unused */ 0.f;
		instance.parameters.y = float32_t(entry.right_ascension);
		instance.parameters.z = float32_t(entry.declination);
		instance.parameters.w = 0.2f * entry.magnitude;
		instance.       color = entry.color;
	}
	_starInstances->unlock();

	_meshInstances.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, int32_t(_passMaximum), sizeof(MeshInstance)));
}

void Compositor::shutdown()
{
	_passMaximum = 0;
	_midRange = 0.f;

	_meshInstances.reset();

	_starCount = 0;
	_starInstances.reset();
	_starMesh.reset();
}

void Compositor::process(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t interpolant)
{
	_interpolant = interpolant;
	_cameraPosition = LUCID_MATH::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);

	_passes.clear();
	process(rootFrame);

	copyInstances();
}

void Compositor::render(LUCID_GIGL::Context const &context)
{
	LUCID_GAL_PIPELINE.setVertexStream(1, _starInstances.get());
	_starMesh->renderInstanced(context, int32_t(_starCount));

	size_t count = std::min(_passes.size(), _passMaximum);
	for (size_t i = 0; i < count; ++i)
	{
		Pass const &pass = _passes[i];
		
		pass.model->renderInstanced(context, _meshInstances.get(), int32_t(i), 1);
		LUCID_GAL_PIPELINE.clear(false, true, true);
	}

	if (0 == count)
	{
		LUCID_GAL_PIPELINE.clear(false, true, true);
	}
}

void Compositor::evaluate(DynamicPoint *point)
{
	/// TBD: implement...
}

void Compositor::evaluate(OrbitalBody *body)
{
	PhysicalProperties const &physicalProperties = body->physicalProperties;
	RenderProperties const &renderProperties = body->renderProperties;

	Pass pass;

	pass.      model = renderProperties.model;
	pass.         id = uint32_t((Selection::TYPE_FRAME << Selection::SELECT_SHIFT) | body->id);
	pass.   position = LUCID_MATH::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition;
	pass.   distance = LUCID_MATH::len(pass.position);
	pass.   rotation = LUCID_MATH::slerp(_interpolant, body->absoluteRotation[0], body->absoluteRotation[1]);
	pass.scaleFactor = physicalProperties.radius / pass.distance;
	pass.   channel0 = renderProperties.channel0;
	pass.   channel1 = renderProperties.channel1;
	pass.   channel2 = renderProperties.channel2;

	_passes.push_back(pass);
}

void Compositor::evaluate(DynamicBody *body)
{
	/// TBD: implement...
}

void Compositor::evaluate(CameraFrame *camera)
{
	/// TBD: implement...
}

void Compositor::process(Frame *frame)
{
	if (nullptr == frame)
		return;

	if (Frame::CULL_STATE_PRUNED == frame->cullState)
		return;

	if (Frame::CULL_STATE_VISIBLE == frame->cullState)
		frame->apply(this);

	for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		process(child);
}

void Compositor::copyInstances()
{
	size_t count = std::min(_passes.size(), _passMaximum);
	if (0 == count)
		return;

	std::sort(_passes.begin(), _passes.end(), Back2Front<Pass>());

	MeshInstance *instances = (MeshInstance *)(_meshInstances->lock());
	for (size_t i = 0; i < count; ++i)
	{
		Pass const &pass = _passes[i];

		instances[i].id = pass.id;
		instances[i].position = _midRange * cast(pass.position / pass.distance);
		instances[i].scale = _midRange * cast(pass.scaleFactor);
		instances[i].rotation = cast(pass.rotation);
		instances[i].channel0 = pass.channel0;
		instances[i].channel1 = pass.channel1;
		instances[i].channel2 = pass.channel2;

		/// test {
		/// everything is rendered in "camera space" with the origin at (0, 0, 0)
		/// the sun is at (0, 0, 0) in world corrdinates. therefore, the
		/// light distance is the magnitude of the view position.
		instances[i].channel2.x = cast(LUCID_MATH::len(_cameraPosition) * _midRange / pass.distance);
		/// } test
	}
	_meshInstances->unlock();
}

LUCID_ORBIT_END
