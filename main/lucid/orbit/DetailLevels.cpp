#include "DetailLevels.h"
#include <lucid/gigl/Model.h>
#include <lucid/core/Reader.h>
#include <lucid/core/Error.h>

namespace core = ::lucid::core;
namespace  gal = ::lucid:: gal;
namespace gigl = ::lucid::gigl;

namespace lucid {
namespace orbit {

	DetailLevels::DetailLevels(core::Reader &reader)
	{
		read(reader);
	}

	size_t DetailLevels::level(float32_t distance)
	{
		///	start over if there was no matching level last time
		index = (INVALID_LEVEL == index) ? 0 : index;

		///	check if the current index is still valid...
		Level const &current = levels[index];
		if ((current.range[0] <= distance) && (distance < current.range[1]))
			return index;

		size_t delta = (distance < current.range[0]) ? -1 : +1;

		///	linear scan for valid level...
		size_t count = levels.size();
		for (index = index + delta; (INVALID_LEVEL != index) && (index < count); index = index + delta)
		{
			Level const &level = levels[index];
			if ((level.range[0] <= distance) && (distance < level.range[1]))
				return index;
		}

		///	viewer is too far away...
		index = INVALID_LEVEL;
		return index;
	}

	void DetailLevels::read(core::Reader &reader)
	{
		size_t count = reader.read<int32_t>();
		LUCID_VALIDATE(0 < count, "must define at least one detail level");

		levels.resize(count);
		for (size_t i = 0; i < count; ++i)
		{
			Level &level = levels[i];
			reader.read(level.range, 2 * sizeof(float32_t));

			level.model.reset(new gigl::Model(reader));

			reader.read(&level.color, sizeof(gal::Color));
			reader.read(level.scale);
			reader.read(&level.parameters, sizeof(gal::Vector4));
		}

		/// default to last level
		index = count - 1;
	}

}	///	orbit
}	///	lucid
