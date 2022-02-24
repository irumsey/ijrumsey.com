#include "Model.h"
#include "Resources.h"
#include <lucid/core/FileReader.h>
#include <lucid/core/Reader.h>

namespace lucid {
namespace gigl {

	Model::Model(core::Reader &reader)
	{
		initialize(reader);
	}

	Model::~Model()
	{
		shutdown();
	}

	void Model::initialize(core::Reader &reader)
	{
		shutdown();

		int32_t count = reader.read<int32_t>();
		_meshes.resize(count);

		for (int32_t i = 0; i < count; ++i)
		{
			std::string path = reader.read<std::string>();

			_meshes[i] = Resources::get<Mesh>(path);
		}
	}

	void Model::shutdown()
	{
		_meshes.clear();
	}

	Model *Model::create(std::string const &path)
	{
		return create(core::FileReader(path));
	}

	Model *Model::create(core::Reader &reader)
	{
		return new Model(reader);
	}

}	///	gigl
}	///	lucid
