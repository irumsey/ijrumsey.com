#pragma once

#include <lucid/orbit/Types.h>

namespace lucid {
namespace core {

	class Clock;

}	///	orbit
}	///	lucid

namespace lucid {
namespace orbit {

	class Mechanics;

}	///	orbit
}	///	lucid

namespace Lucid {
namespace Math {

	ref class Vector3;

}	///	Math
}	///	Lucid

namespace Lucid {
namespace Orbit {

	ref class Selection;
	ref class Frame;
	ref class CameraFrame;

	///	Mechanics
	///
	///	Note: currently breaking the design choice of the
	/// managed library only wrapping non-managed.  this
	/// is adding functionality in the form of time-keeping
	/// which should either be pushed up or down eventually.
	public ref class Mechanics
	{
	public:
		Mechanics(double dayNumber);

		~Mechanics();

		!Mechanics();

		void Initialize(double dayNumber);

		void Shutdown();

		property float DayNumber { float get(); }

		property Frame ^Root { Frame ^get(); };

		property Frame ^default[size_t] { Frame ^get(size_t id); };

		void Attach(Frame ^center, Frame ^frame);

		void Detach(Frame ^frame);

		void Update();

		void Render(CameraFrame ^cameraFrame, bool useFXAA);

		Selection ^Hit(int x, int y);

	private:
		::lucid::orbit::Mechanics *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid
