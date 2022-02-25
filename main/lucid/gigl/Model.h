#pragma once

#include <memory>
#include <vector>
#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Identity.h>
#include <lucid/gigl/Mesh.h>

///
///
///
namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

///
///
///
namespace lucid {
namespace gigl {

	class Batched;

	///	Model
	///
	///	Collection of meshes that make up a more complicated object.
	/// 
	///	Note: one caveat is that all meshes must use the same per-instance-vertex.
	class Model final
	{
	public:
		Model(core::Reader &reader);

		virtual ~Model();

		core::Identity const &identity() const;

		static Model *create(std::string const &path);

		static Model *create(core::Reader &reader);

	private:
		typedef std::shared_ptr<Mesh> mesh_ptr_t;
		typedef std::vector<mesh_ptr_t> mesh_vec_t;

		friend class Batched;

		core::Identity _identity;
		mesh_vec_t _meshes;

		void initialize(core::Reader &reader);

		void shutdown();

		LUCID_PREVENT_COPY(Model);
		LUCID_PREVENT_ASSIGNMENT(Model);
	};

	inline core::Identity const &Model::identity() const
	{
		return _identity;
	}

}	///	gigl
}	///	lucid
