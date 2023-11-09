#pragma once

#include "SubObject.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <Uxtheme.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#pragma comment(lib,"uxtheme.lib")

namespace Win32 {

	class PAGE_API Window3D : public SubObject {

	public:
		Window3D(WSTRING title, HICON icon, WindowType type = RESIZABLE);
		~Window3D();

		virtual VOID Initialize() override;

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		VOID InitD3D(HWND hWnd);

		VOID CleanD3D(void);

		VOID RenderFrame(void);

	private:
		BOOL m_shouldQuit = false;

	protected:
		SIZE		m_Size;
		WindowType	m_Type;

		BOOL		m_Active;

	public:
		IDXGISwapChain* m_SwapChain;             // the pointer to the swap chain interface
		ID3D11Device* m_D3DDevice;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext* m_D3DDeviceContext;           // the pointer to our Direct3D device context
		ID3D11RenderTargetView* m_Backbuffer;

	public:
		SIZE		Size() { return m_Size; }
		BOOL		Active() { return m_Active; }

	public:
		VOID		Size(SIZE size) { m_Size = size; }
		VOID		Size(INT cx, INT cy) { m_Size.cx = cx; m_Size.cy = cy; }
		VOID		Active(BOOL active) { m_Active = active; }
	};

}