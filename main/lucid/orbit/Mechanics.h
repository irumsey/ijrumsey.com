#pragma once

#include <string>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Selection.h>
#include <lucid/orbit/Simulator.h>
#include <lucid/orbit/Renderer.h>

///
///
///

namespace lucid {
namespace core{

	class Clock;

}	/// core
}	///	lucid

///
///
///

namespace lucid {
namespace orbit {

	class Frame;
	class CameraFrame;

	///	Mechanics
	///
	/// 
	class Mechanics
	{
	public:
		Mechanics(scalar_t dayNumber);

		virtual ~Mechanics();

		void initialize(scalar_t dayNumber /* add filter for frame inclusion/exclusion */);

		void shutdown();

		scalar_t dayNumber() const;

		Frame *root() const;

		Frame *frame(size_t id) const;

		void attach(Frame *center, Frame *frame);

		void detach(Frame *frame);

		void update();

		void render(CameraFrame *cameraFrame, bool useFXAA = false);

		Selection hit(int32_t x, int32_t y) const;

	private:
		typedef std::unordered_map<size_t, Frame *> frame_map_t;

		scalar_t _dayNumber[2] = { constants::J2000<float64_t>, constants::J2000<float64_t>, };

		scalar_t const    TIME_STEP = scalar_t(0.1);
		scalar_t const   TIME_LIMIT = scalar_t(0.3);

		scalar_t         _wallTime;
		scalar_t     _wallTimeLast;
		scalar_t    _wallTimeAccum;
		scalar_t          _simTime;
		scalar_t _frameInterpolant;

		core::Clock *_clock = nullptr;

		Simulator _simulator;
		Renderer _renderer;
		
		Frame *_root = nullptr;
		frame_map_t _frames;

		void update(scalar_t delta);

		LUCID_PREVENT_COPY(Mechanics);
		LUCID_PREVENT_ASSIGNMENT(Mechanics);
	};

	inline scalar_t Mechanics::dayNumber() const
	{
		return _dayNumber[1];
	}

	inline Frame *Mechanics::root() const
	{
		return _root;
	}

	inline Frame *Mechanics::frame(size_t id) const
	{
		auto iter = _frames.find(id);
		LUCID_VALIDATE(iter != _frames.end(), "unknown frame id specified");

		return iter->second;
	}

}	///	orbit
}	///	lucid
