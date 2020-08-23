#include "Ephemeris.h"
#include "Algorithm.h"
#include "Elements.h"
#include "Properties.h"
#include "Constants.h"
#include <lucid/math/Algorithm.h>
#include <lucid/math/Constants.h>
#include <lucid/core/FileReader.h>

namespace  core = ::lucid:: core;
namespace  math = ::lucid:: math;
namespace orbit = ::lucid::orbit;

///
///
///

namespace { /// anonymous

}	///	anonymous

///
///
///

namespace lucid {
namespace orbit {

	Ephemeris::Ephemeris()
	{
		///	create the 'special' entry that defines the 'center' of the system

		Properties properties;
		properties.description = "solar system barycenter";

		_names.insert(std::make_pair("SSB", 0));
		_properties.insert(std::make_pair(0, properties));
		_entries.insert(std::make_pair(0, Entry()));
	}

	Ephemeris::~Ephemeris()
	{
	}

	void Ephemeris::initialize(std::string const &path)
	{
		core::FileReader reader(path);

		size_t bodyCount = 0;
		reader.read(bodyCount);
		for (size_t bodyIndex = 0; bodyIndex < bodyCount; ++bodyIndex)
		{
			Properties properties;
			Entry entry;

			size_t hid = 0;
			reader.read(hid);

			std::string target;
			reader.read(target);

			reader.read(entry.center);
			LUCID_VALIDATE(_properties.end() != _properties.find(entry.center), "'" + target + "' specifies unknown center object");

			reader.read(properties.description);
			reader.read(properties.GM);
			reader.read(properties.mass);
			reader.read(properties.radius);

			size_t elementsCount = 0;
			reader.read(elementsCount);

			for (size_t elementsIndex = 0; elementsIndex < elementsCount; ++elementsIndex)
			{
				Elements elements;
				reader.read(&elements, sizeof(elements));
				entry.elements.push_back(elements);
			}

			_names.insert(std::make_pair(target, hid));
			_properties.insert(std::make_pair(hid, properties));
			_entries.insert(std::make_pair(hid, entry));
		}
	}

	size_t Ephemeris::lookup(std::string const &target) const
	{
		auto iter = _names.find(target);
		LUCID_VALIDATE(iter != _names.end(), "unknown target '" + target + "' specified");

		return iter->second;
	}

	void Ephemeris::lookup(Properties &properties, std::string const &target) const
	{
		lookup(properties, lookup(target));
	}

	void Ephemeris::lookup(Properties &properties, size_t target) const
	{
		auto iter = _properties.find(target);
		LUCID_VALIDATE(iter != _properties.end(), "unknown target id specified");

		properties = iter->second;
	}

	size_t Ephemeris::lookup(Elements &elements, std::string const &target, scalar_t jdn) const
	{
		return lookup(elements, lookup(target), jdn);
	}

	size_t Ephemeris::lookup(Elements &elements, size_t target, scalar_t jdn) const
	{
		auto iter = _entries.find(target);
		LUCID_VALIDATE(iter != _entries.end(), "unknown target id specified");

		Entry const &entry = iter->second;
		size_t const count = entry.elements.size();

		LUCID_VALIDATE(0 < count, "target does not define orbital elements");

		///	find the closest entry to the given julian day.
		///	for now, it is a simple linear scan through the list which
		///	exits when the difference would increase.
		///	test { 

		size_t index = 0;
		scalar_t a = ::fabs(entry.elements[index].JDN - jdn);
		for (size_t i = 1; i < count; ++i)
		{
			scalar_t b = ::fabs(entry.elements[i].JDN - jdn);
			if (b < a)
			{
				index = i;
				a = b;
			}
			else
			{
				break;
			}
		}

		///	} test

		elements = entry.elements[index];

		return entry.center;
	}

	void Ephemeris::compute(vector3_t &position, vector3_t &velocity, std::string const &target, scalar_t jdn) const
	{
		compute(position, velocity, lookup(target), jdn);
	}

	void Ephemeris::compute(vector3_t &position, vector3_t &velocity, size_t target, scalar_t jdn) const
	{
		Elements targetElements;
		size_t cid = lookup(targetElements, target, jdn);

		Properties centerProperties;
		lookup(centerProperties, cid);

		computePositionVelocity(position, velocity, centerProperties, targetElements, jdn);
	}

	Ephemeris &Ephemeris::instance()
	{
		static Ephemeris theInstance;
		return theInstance;
	}

}	///	orbit
}	///	lucid
