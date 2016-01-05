#include "TargetReader2D.h"
#include "RenderTarget2D.h"
#include "System.h"
#include "Utility.h"
#include <lucid/math/Algorithm.h>

namespace /* anonymous */
{

	namespace math = ::lucid::math;
	namespace gal = ::lucid::gal;

}	///	anonymous

///
///
///
namespace lucid {
namespace gal {

	TargetReader2D *TargetReader2D::create(gal::RenderTarget2D const *target, int32_t width, int32_t height)
	{
		return new gal::d3d11::TargetReader2D(target, width, height);
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	TargetReader2D::TargetReader2D(gal::RenderTarget2D const *target, int32_t width, int32_t height)
		: _width(width), _height(height)
	{
		try
		{
			gal::d3d11::RenderTarget2D const *concrete = static_cast<gal::d3d11::RenderTarget2D const *>(target);
			_srcTexture = concrete->d3dTexture();

			D3D11_TEXTURE2D_DESC descTarget;
			_srcTexture->GetDesc(&descTarget);

			_srcWidth = descTarget.Width;
			_srcHeight = descTarget.Height;

			D3D11_TEXTURE2D_DESC descReader;
			::memset(&descReader, 0, sizeof(D3D11_TEXTURE2D_DESC));

			descReader.Usage = D3D11_USAGE_STAGING;
			descReader.Format = descTarget.Format;
			descReader.Width = width;
			descReader.Height = height;
			descReader.ArraySize = 1;
			descReader.SampleDesc = descTarget.SampleDesc;
			descReader.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

			HRESULT hResult = d3d11ConcreteDevice->CreateTexture2D(&descReader, nullptr, &_stagingTexture);
			GAL_VALIDATE_HRESULT(hResult, "unable to create render target reader");

			_data = new uint8_t[TEXEL_SIZE * _width * _height];
		}
		catch (...)
		{
			shutdown();
			throw;
		}
	}

	TargetReader2D::~TargetReader2D()
	{
		shutdown();
	}

	uint8_t const *TargetReader2D::sample(int32_t x, int32_t y, int32_t width, int32_t height) const
	{
		x = math::clamp(x, 0,  _srcWidth - 1);
		y = math::clamp(y, 0, _srcHeight - 1);

		D3D11_BOX box = { UINT(x), UINT(y), 0, UINT(x + width), UINT(y + height), 1, };
		d3d11ConcreteContext->CopySubresourceRegion(_stagingTexture, 0, 0, 0, 0, _srcTexture, 0, &box);

		D3D11_MAPPED_SUBRESOURCE mapped = {};
		HRESULT hResult = d3d11ConcreteContext->Map(_stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);
		GAL_VALIDATE_HRESULT(hResult, "unable to map staging resource");

		::memcpy(_data, mapped.pData, TEXEL_SIZE * _width * _height);

		d3d11ConcreteContext->Unmap(_stagingTexture, 0);

		return _data;
	}

	void TargetReader2D::shutdown()
	{
		safeRelease(_stagingTexture);

		delete [] _data;
	}

}	///	d3d11
}	///	gal
}	///	lucid
