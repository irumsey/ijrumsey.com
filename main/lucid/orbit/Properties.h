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
		scalar_t GM;
		scalar_t mass;
		scalar_t radius;

		PhysicalProperties() = default;

		PhysicalProperties(core::Reader &reader)
		{
			read(reader);
		}

		~PhysicalProperties() = default;

		void read(core::Reader &reader)
		{
			reader.read(    &GM, sizeof(scalar_t));
			reader.read(  &mass, sizeof(scalar_t));
			reader.read(&radius, sizeof(scalar_t));
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
