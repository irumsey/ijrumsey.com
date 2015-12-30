#include "Program.h"
#include "RenderTarget2D.h"
#include "Texture2D.h"
#include "Unordered2D.h"
#include "Parameter.h"
#include "Types.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Program.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

namespace MI = msclr::interop;

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;
	namespace gigl = ::lucid::gigl;

}	///	anonymous

namespace lucid {

	Program::Program(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<gal::Program>(gigl::Resources::get<gal::Program>(MI::marshal_as<std::string>(path)));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
			throw;
		}
	}

	Program::Program(std::shared_ptr<::lucid::gal::Program> program)
	{
		_internal = new std::shared_ptr<gal::Program>(program);
	}

	Program::~Program()
	{
		this->!Program();
	}

	Program::!Program()
	{
		_internal->reset();
		delete _internal;
	}

	Parameter ^Program::lookup(System::String ^name)
	{
		return gcnew Parameter((*_internal)->lookup(MI::marshal_as<std::string>(name)));
	}

	void Program::set(Parameter ^parameter, float value)
	{
		(*_internal)->set(parameter->ptr, value);
	}

	void Program::set(Parameter ^parameter, Vector2 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Vector3 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Vector4 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Quaternion ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Matrix2x2 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Matrix3x3 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Matrix4x4 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Color ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Unordered2D ^value)
	{
		(*_internal)->set(parameter->ptr, value->ptr);
	}

	void Program::set(Parameter ^parameter, Texture2D ^value)
	{
		(*_internal)->set(parameter->ptr, value->ptr);
	}

	void Program::set(Parameter ^parameter, RenderTarget2D ^value)
	{
		(*_internal)->set(parameter->ptr, value->ptr);
	}

}	///  lucid
