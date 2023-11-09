#include "page.h"
#include "Window.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#define DCX_USESTYLE 0x00010000

namespace Win32 {

	Window3D::Window3D(WSTRING title, HICON icon, WindowType type)
		:Win32::SubObject(title, title, icon), m_Type(type), m_SwapChain(nullptr), m_D3DDevice(nullptr), m_D3DDeviceContext(nullptr)
	{
		Size(1366, 720);
	}

	Window3D::~Window3D()
	{
	}

	VOID Window3D::Initialize()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT R = { 0, 0, Size().cx, Size().cy };
		AdjustWindowRect(&R, m_Type, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		m_Handle = CreateWindow(m_Class.c_str(), m_Title.c_str(),
			m_Type, ((desktop.right / 2) - (Size().cx / 2)), ((desktop.bottom / 2) - (Size().cy / 2)), Size().cx, Size().cy, NULL, NULL, HInstance(), (void*)this);

		ShowWindow(m_Handle, SW_SHOW);

		InitD3D(m_Handle);
	}

	LRESULT Window3D::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {
		case WM_PAINT: {RenderFrame(); } break;
		}
		return SubObject::MessageHandler(hwnd, message, wParam, lParam);
	}

	// this function initializes and prepares Direct3D for use
	VOID Window3D::InitD3D(HWND hWnd)
	{
		// DXGI_SWAP_CHAIN_DESC structure for swap chain configuration
		DXGI_SWAP_CHAIN_DESC scd = {};
		scd.BufferCount = 1;                                    // one back buffer
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
		scd.OutputWindow = hWnd;                                // the window to be used
		scd.SampleDesc.Count = 1;                               // no multisampling (change as needed)
		scd.Windowed = TRUE;                                    // windowed mode

		// Create Direct3D device, device context, and swap chain
// Specify the desired feature levels in an array
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// Specify the number of feature levels in the array
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		// Create Direct3D device, device context, and swap chain
		HRESULT result = D3D11CreateDeviceAndSwapChain(
			nullptr,                        // Use default adapter
			D3D_DRIVER_TYPE_HARDWARE,       // Use hardware graphics acceleration
			nullptr,                        // No software device
			D3D11_CREATE_DEVICE_DEBUG,       // Include debug layer for debugging (optional)
			featureLevels,                  // Array of feature levels to attempt to create
			numFeatureLevels,               // Number of feature levels in the array
			D3D11_SDK_VERSION,              // SDK version
			&scd,                           // Swap chain description
			&m_SwapChain,                   // Swap chain output
			&m_D3DDevice,                   // Device output
			NULL,                  // Actual feature level created
			&m_D3DDeviceContext);           // Device context output

		if (FAILED(result)) {
			// Handle initialization failure (log, throw exception, etc.)
			// Example: throw std::runtime_error("Direct3D initialization failed");
			return;
		}

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_Backbuffer);
		pBackBuffer->Release();

		// set the render target as the back buffer
		m_D3DDeviceContext->OMSetRenderTargets(1, &m_Backbuffer, NULL);

		// Set the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = 800;
		viewport.Height = 600;

		m_D3DDeviceContext->RSSetViewports(1, &viewport);

	}


	// this is the function that cleans up Direct3D and COM
	VOID Window3D::CleanD3D(void)
	{
		// close and release all existing COM objects
		m_SwapChain->Release();
		m_Backbuffer->Release();
		m_D3DDevice->Release();
		m_D3DDeviceContext->Release();
	}

	// Implementation of RenderFrame function (for rendering)
	VOID Window3D::RenderFrame(void) {
		// clear the back buffer to a deep blue
		m_D3DDeviceContext->ClearRenderTargetView(m_Backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

		// do 3D rendering on the back buffer here

		// switch the back buffer and the front buffer
		m_SwapChain->Present(0, 0);
	}
}
