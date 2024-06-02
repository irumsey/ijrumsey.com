#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/core/Reader.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/DetailLevels.h>

namespace lucid {
namespace orbit {

	///	PhysicalProperties
	///
	///
	struct PhysicalProperties
	{
		scalar_t GM = 0.0;
		scalar_t mass = 0.0;
		scalar_t radius = 0.0;

		PhysicalProperties() = default;

		PhysicalProperties(core::Reader &reader)
		{
			read(reader);
		}

		~PhysicalProperties() = default;

		void read(core::Reader &reader)
		{
			GM     = reader.read<scalar_t>();
			mass   = reader.read<scalar_t>();
			radius = reader.read<scalar_t>();
		}

	};

	///
	///
	///
	struct RenderProperties
	{
		bool showOrbit = false;			//	the default is provided by the ephemeris
		bool bodyHighlight = false;		//	used only at runtime to toggle highlighting
		bool orbitHighlight = false;	//	used only at runtime to toggle highlighting 

		DetailLevels detailLevels;

		RenderProperties() = default;

		RenderProperties(core::Reader &reader)
		{
			read(reader);
		}

		~RenderProperties() = default;

		void read(core::Reader &reader)
		{
			showOrbit = reader.read<bool>();

			detailLevels.read(reader);
		}
	};

}	///	orbit
}	///	lucid
