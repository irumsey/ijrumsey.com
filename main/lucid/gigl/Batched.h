#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <lucid/core/Error.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Mesh.h>

namespace lucid {
namespace gigl {

	///	Batched
	///
	///	Instanced batching
	///	Note: first, batches are rendered in the order of batch creation.
	///	Note: second, all instances within the batch are rendered in the order determined using the supplied predicate.
	class Batched
	{
	public:
		Batched();

		virtual ~Batched();

		void initialize();

		void shutdown();

		template<typename I, typename Pred> void createBatch(std::shared_ptr<Mesh> mesh, size_t maximum);

		template<typename I> void addInstance(std::shared_ptr<Mesh> mesh, I const &instance);

		void render(Context const &context);

		void clear();

	private:
		///
		///
		///
		struct TypeID
		{
			static size_t counter;

			template<typename I> static size_t value()
			{
				static size_t id = ++counter;
				return id;
			}
		};


		///
		///
		///
		struct Key
		{
			std::shared_ptr<Mesh> mesh;
			size_t iid = 0;

			Key(std::shared_ptr<Mesh> mesh, size_t iid)
				: mesh(mesh)
				, iid(iid)
			{
			}

			size_t mid() const
			{
				return size_t(mesh.get());
			}

		};

		struct BatchHash
		{
			size_t operator()(Key const &key) const
			{
				return ((key.mid() << 16) |  key.iid);
			}
		};

		struct BatchEqual
		{
			bool operator()(Key const &lhs, Key const &rhs) const
			{
				return (lhs.iid == rhs.iid) && (lhs.mid() == rhs.mid());
			}
		};

		///
		///
		///
		struct BatchBase
		{
			BatchBase() = default;

			virtual ~BatchBase() = default;

			virtual void clear() = 0;

			virtual void push(void const *opaque) = 0;

			virtual void render(Context const &context, std::shared_ptr<Mesh> mesh) = 0;
		};

		typedef std::vector<Key> key_vec_t;
		key_vec_t _order;

		typedef std::unordered_map<Key, BatchBase*, BatchHash, BatchEqual> batch_map_t;
		batch_map_t _batches;

		///
		///
		///
		template<typename I, typename Pred> struct Batch : public BatchBase
		{
			size_t maximum = 0;
			std::shared_ptr<lucid::gal::VertexBuffer> batch;
			std::vector<I> instances;

			Batch(size_t maximum)
				: maximum(maximum)
			{
				batch.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, maximum, sizeof(I)));
			}

			virtual ~Batch() = default;

			virtual void clear() override
			{
				instances.clear();
			}

			virtual void push(void const *opaque) override
			{
				I const &instance = *((I const *)opaque);
				instances.push_back(instance);
			}

			virtual void render(Context const &context, std::shared_ptr<Mesh> mesh) override
			{
				lucid::gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

				gal::Vector3 const &viewPosition = context.lookup("viewPosition").as<gal::Vector3>();
				std::sort(instances.begin(), instances.end(), Pred(viewPosition));

				size_t totalCount = instances.size();
				size_t index = 0;

				std::shared_ptr<Material> material = mesh->material();
				material->begin(context);
				while (index < totalCount)
				{
					size_t count = math::min(totalCount - index, maximum);

					::memcpy((I*)(batch->lock()), &instances[index], count * sizeof(I));
					batch->unlock();

					pipeline.setVertexStream(1, batch.get());
					mesh->drawInstanced(count);

					index += count;
				}
				material->end();
			}
		};
	};

	template<typename I, typename Pred> inline void Batched::createBatch(std::shared_ptr<Mesh> mesh, size_t maximum)
	{
		Key key = Key(mesh, TypeID::value<I>());
		if (_batches.end() != _batches.find(key))
			return;

		_order.push_back(key);
		_batches.insert(std::make_pair(key, new Batch<I,Pred>(maximum)));
	}

	template<typename I> inline void Batched::addInstance(std::shared_ptr<Mesh> mesh, I const &instance)
	{
		auto iter = _batches.find(Key(mesh, TypeID::value<I>()));
		LUCID_VALIDATE(iter != _batches.end(), "batched mesh/instance pair not registered");

		iter->second->push(&instance);
	}

}	///	gigl
}	///	lucid
