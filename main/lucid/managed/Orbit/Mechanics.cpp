#include "Mechanics.h"
#include "Frame.h"
#include "Selection.h"
#include <lucid/orbit/Mechanics.h>
#include <lucid/orbit/Selection.h>
#include <lucid/orbit/Utility.h>
#include <lucid/math/Algorithm.h>

#include <lucid/managed/GIGL/Context.h>
#include <lucid/managed/Math/Types.h>

namespace /* anonymous */ {

	namespace  core = ::lucid:: core;
	namespace  math = ::lucid:: math;
	namespace orbit = ::lucid::orbit;

}	///	anonymous

namespace Lucid {
namespace Orbit {

	Mechanics::Mechanics(double dayNumber)
	{
		Initialize(dayNumber);
	}

	Mechanics::~Mechanics()
	{
		this->!Mechanics();
	}

	Mechanics::!Mechanics()
	{
		Shutdown();
	}

	void Mechanics::Initialize(double dayNumber)
	{
		Shutdown();

		_internal = new orbit::Mechanics(dayNumber);
	}

	void Mechanics::Shutdown()
	{
		delete _internal;
		_internal = nullptr;
	}

	Frame ^Mechanics::Root::get()
	{
		return Frame::Wrap(_internal->root());
	}

	Frame ^Mechanics::default::get(size_t id)
	{
		return Frame::Wrap(_internal->frame(id));
	}

	void Mechanics::Attach(Frame ^center, Frame ^frame)
	{
		_internal->attach(center->ptr, frame->ptr);
	}

	void Mechanics::Detach(Frame ^frame)
	{
		_internal->detach(frame->ptr);
	}

	void Mechanics::Update()
	{
		_internal->update();
	}

	void Mechanics::Render(GIGL::Context ^context)
	{
		_internal->render(context->ref);
	}

	Selection ^Mechanics::Hit(int x, int y)
	{
		return gcnew Selection(_internal->hit(x, y));
	}

	Math::Vector3 ^Mechanics::InterpolatedPosition(Frame ^frame)
	{
		/// TBD: scaling from meters to render units...
		return gcnew Math::Vector3(::lucid::orbit::scale(_internal->interpolatedPosition(frame->ref)));
	}

}	///	Orbit
}	///	Lucid
