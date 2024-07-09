#include "Primitive.h"
#include "Resources.h"
#include <lucid/gal/RenderTarget2D.h>
#include <lucid/gal/Texture2D.h>
#include <lucid/gal/Unordered2D.h>
#include <lucid/core/Reader.h>

LUCID_ANONYMOUS_BEGIN

typedef LUCID_CORE::Reader reader_t;

typedef LUCID_GAL::Unordered2D unordered2d_t;
typedef std::shared_ptr<unordered2d_t> shared_unordered2d_t;

typedef LUCID_GAL::Texture2D texture2d_t;
typedef std::shared_ptr<texture2d_t> shared_texture2d_t;
	
typedef LUCID_GAL::RenderTarget2D target2d_t;
typedef std::shared_ptr<target2d_t> shared_target2d_t;
	
typedef LUCID_GIGL::Primitive primitive_t;

struct UNSUPPORTED final {};

/// ENUM LOOKUP
LUCID_GAL::RenderTarget2D::FORMAT const targetFormat[] =
{
	target2d_t::FORMAT_UNORM_R8G8B8A8,
	target2d_t::FORMAT_UINT_R16G16,
	target2d_t::FORMAT_UINT_R10G10B10A2,
	target2d_t::FORMAT_FLOAT_R16G16,
	target2d_t::FORMAT_FLOAT_R32,
	target2d_t::FORMAT_FLOAT_R32G32,
	target2d_t::FORMAT_UINT_R32,
};

typedef primitive_t (*read_func_t)(reader_t &reader);
extern read_func_t const read_jump[primitive_t::TYPE_COUNT];

LUCID_ANONYMOUS_END

LUCID_GIGL_BEGIN

Primitive::Info const Primitive::infos[TYPE_COUNT] =
{
	{ Type<             UNKNOWN>::VALUE,      "<unknown>" },
	{ Type<           UNDEFINED>::VALUE,    "<undefined>" },
	{ Type<                bool>::VALUE,           "bool" },
	{ Type<             int32_t>::VALUE,            "int" },
	{ Type<              Scalar>::VALUE,          "float" },
	{ Type< LUCID_GAL::Color>::VALUE,          "Color" },
	{ Type<             Vector2>::VALUE,        "Vector2" },
	{ Type<             Vector3>::VALUE,        "Vector3" },
	{ Type<             Vector4>::VALUE,        "Vector4" },
	{ Type<          Quaternion>::VALUE,     "Quaternion" },
	{ Type<           Matrix2x2>::VALUE,      "Matrix2x2" },
	{ Type<           Matrix3x3>::VALUE,      "Matrix3x3" },
	{ Type<           Matrix4x4>::VALUE,      "Matrix4x4" },
	{ Type<  shared_texture2d_t>::VALUE,      "Texture2D" },
	{ Type<   shared_target2d_t>::VALUE, "RenderTarget2D" },
	{ Type<     std::    string>::VALUE,         "string" },

	{ Type<shared_unordered2d_t>::VALUE,    "Unordered2D" },
};

void Primitive::coerceFrom(Primitive const &rhs)
{
	_tid = rhs._tid;
	rhs._any ? _any.reset(rhs._any->clone()) : _any.reset();
}

void Primitive::readFrom(LUCID_CORE::Reader &reader)
{
	int32_t tid = reader.read<int32_t>();
	LUCID_VALIDATE(tid < primitive_t::TYPE_COUNT, "invalid primitive type id");

	///	TBD: this creates an intermediate primitive instance from which to coerce.
	///	use Primitive::Any directly to emlimiate this if it becomes an issue (which i doubt).
	coerceFrom(read_jump[tid](reader));
}

LUCID_GIGL_END

LUCID_ANONYMOUS_BEGIN

template<class T> primitive_t read(reader_t &reader)
{
	T value;
	reader.read(&value, sizeof(T));

	return primitive_t(value);
}

template<> primitive_t read<UNSUPPORTED>(reader_t &)
{
	LUCID_THROW("unsupported gigl primitive type");
}
	
template<> primitive_t read<texture2d_t>(reader_t &reader)
{
	std::string path = reader.read<std::string>();

	return primitive_t(LUCID_GIGL::Resources::get<texture2d_t>(path));
}

template<> primitive_t read<target2d_t>(reader_t &reader)
{
	int32_t format = reader.read<int32_t>();

	int32_t width = reader.read<int32_t>();
	int32_t height = reader.read<int32_t>();

	return primitive_t(shared_target2d_t(target2d_t::create(targetFormat[format], width, height)));
}

template<> primitive_t read<std::string>(reader_t &reader)
{
	std::string query = reader.read<std::string>();

	return primitive_t(query);
}

read_func_t const read_jump[primitive_t::TYPE_COUNT] =
{
	read<UNSUPPORTED>,
	read<UNSUPPORTED>,
	read<bool>,
	read<int32_t>,
	read<LUCID_GIGL::Scalar>,
	read<LUCID_GIGL::Color>,
	read<LUCID_GIGL::Vector2>,
	read<LUCID_GIGL::Vector3>,
	read<LUCID_GIGL::Vector4>,
	read<LUCID_GIGL::Quaternion>,
	read<LUCID_GIGL::Matrix2x2>,
	read<LUCID_GIGL::Matrix3x3>,
	read<LUCID_GIGL::Matrix4x4>,
	read<texture2d_t>,
	read<target2d_t>,
	read<std::string>,

	read<UNSUPPORTED>,
};

LUCID_ANONYMOUS_END
