#pragma once

//#pragma comment (lib,"FW1FontWrapper.lib")

//#include <D3D10_1.h>
//#include <DXGI.h>
//#include <D2D1.h>
//#include <dwrite.h>



class DwriteText
{

public:
	DwriteText();
	~DwriteText();

public:
	void			Render(std::wstring wtext, int numex);

private:
	HRESULT			Initialize();
//	HRESULT			Init2dScreenTexture();

	void			Release();

private:
	IFW1Factory*		m_factory = nullptr;
	IFW1FontWrapper*	m_fontwrap = nullptr;

//private:
//	IDXGIAdapter1*				Adapter				= nullptr;
//	ID3D10Device1*				d3d101Device		= nullptr;
//	IDXGIKeyedMutex*			keyedMutex11		= nullptr;
//	IDXGIKeyedMutex*			keyedMutex10		= nullptr;
//	ID2D1RenderTarget*			D2DRenderTarget		= nullptr;
//	ID2D1SolidColorBrush*		Brush				= nullptr;
//	ID3D11Texture2D*			sharedTex11			= nullptr;
//	IDWriteFactory*				DWriteFactory		= nullptr;
//	IDWriteTextFormat*			TextFormat			= nullptr;
//	ID3D11Buffer*				d2dVertBuffer		= nullptr;
//	ID3D11Buffer*				d2dIndexBuffer		= nullptr;
//	ID3D11Buffer*				cbPerObjectBuffer	= nullptr;
//	ID3D11ShaderResourceView*	d2dTexture			= nullptr;
//	ID3D11SamplerState*			TexSamplerState		= nullptr;
//	ID3D11RasterizerState*		CCWcullMode			= nullptr;
//	ID3D11RasterizerState*		CWcullMode			= nullptr;
//
//	ID3D11VertexShader*			D2D_VS				= nullptr;
//	ID3D11PixelShader*			D2D_PS				= nullptr;
//	ID3D11BlendState*			d2dTransparency		= nullptr;
//	ID3D11BlendState*			Transparency		= nullptr;
//
//	INT							LayoutWidth			= 300;
//	INT							LayoutHeight		= 300;
	
};

