#include "page.h"
#include "Window.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context


#define DCX_USESTYLE 0x00010000

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void CleanD3D(void);        // closes Direct3D and releases memory

namespace Win32{

	Window::Window(WSTRING title, HICON icon, WindowType type)
		:Win32::SubObject(title, title, icon), m_Type(type)
	{

		Size(DEFAULTWIDTH, DEFAULTHEIGHT);
	}

	Window::~Window()
	{
	}

	VOID Window::Initialize()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT R = { 0, 0, Size().cx, Size().cy};
		AdjustWindowRect(&R, m_Type, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		m_Handle = CreateWindow(m_Class.c_str(), m_Title.c_str(),
			m_Type, ((desktop.right / 2) - (Size().cx / 2)), ((desktop.bottom / 2) - (Size().cy / 2)), Size().cx, Size().cy, 0, 0, HInstance(), (void*)this);

		ShowWindow(m_Handle, SW_SHOW);
		UpdateWindow(m_Handle);
	}

	LRESULT Window::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {
		
			case WM_NCCREATE:				{ OnNonClientCreate();								  }			return TRUE;
			case WM_NCACTIVATE:				{ OnNonClientActivate(LOWORD(wParam) != WA_INACTIVE); }			return TRUE;
			case WM_NCPAINT:				{ OnNonClientPaint((HRGN)wParam);					  }			return FALSE;
			case WM_NCLBUTTONDOWN:			{ OnNonClientLeftMouseButtonDown();					  }			break;
			case WM_NCLBUTTONDBLCLK:		{ Win32::Utils::MaximizeWindow(Handle());			  }			return 0;



			case WM_GETMINMAXINFO:			{ OnGetMinMaxInfo(((MINMAXINFO*)lParam));			  }			return 0;
			case WM_EXITSIZEMOVE:			{ OnExitSizeMove();									  }			return 0;
			
			case WM_PAINT:					{ OnPaint();										  }			break;
			case WM_TIMER:					{ RedrawWindow();									  }			break;

			//case WM_QUIT:					{ CleanD3D();										  }			break;
		}
		return SubObject::MessageHandler(hwnd, message, wParam, lParam);
	}

	VOID Window::RedrawWindow() {
		SetWindowPos(Handle(), 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);
		SendMessage(Handle(), WM_PAINT, 0, 0);
	}

	VOID Window::OnNonClientCreate() {

		SetTimer(Handle(), 1, 100, NULL);
		SetWindowTheme(Handle(), L"", L"");
		Win32::Utils::ModifyClassStyle(Handle(), 0, CS_DROPSHADOW);

		Win32::Caption::AddCaptionButton(new CaptionButton(L"X", CB_CLOSE));
		Win32::Caption::AddCaptionButton(new CaptionButton(L"🗖", CB_MAXIMIZE));
		Win32::Caption::AddCaptionButton(new CaptionButton(L"🗕", CB_MINIMIZE));
	}

	VOID Window::OnNonClientActivate(BOOL active) {
		Active(active);
	}

	VOID Window::OnNonClientPaint(HRGN region) {

		// Start Draw
		HDC		hdc = GetDCEx(Handle(), region, DCX_WINDOW | DCX_INTERSECTRGN | DCX_USESTYLE);

		RECT rect;
		GetWindowRect(Handle(), &rect);

		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, size.cx, size.cy);
		HANDLE hOld = SelectObject(hdc, hbmMem);


		// Draw

		HBRUSH brush = CreateSolidBrush(RGB(46, 46, 46));
		
		RECT newRect = RECT{ 0, 0, size.cx, size.cy };

		FillRect(hdc, &newRect, brush);

		if (Active() && !Win32::Utils::IsWindowFullscreen(Handle())) {
			brush = CreateSolidBrush(RGB(10, 100, 150));
			FrameRect(hdc, &newRect, brush);
		}

		PaintCaption(hdc);

		DeleteObject(brush);

		// End Draw Section


		BitBlt(hdc, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);

		SelectObject(hdc, hOld);
		DeleteObject(hbmMem);

		ReleaseDC(Handle(), hdc);
	}

	VOID Window::PaintCaption(HDC hdc) {

		RECT rect;
		GetWindowRect(Handle(), &rect);

		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		if (ShowTitle()) {
			rect = RECT{ 0, 0, size.cx, 30 };

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, Active() ? RGB(255, 255, 255) : RGB(92, 92, 92));

			DrawText(hdc, m_Title.c_str(), wcslen(m_Title.c_str()), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}

		int spacing = 0;

		POINT pt;
		GetCursorPos(&pt);

		GetWindowRect(Handle(), &rect);

		pt.x -= rect.left;
		pt.y -= rect.top;

		for (auto& button : Caption::CaptionButtons()) {
			spacing += button->Width;

			button->Rect = RECT{ size.cx - spacing , 0, size.cx - spacing + button->Width , 30 };

			if (button->Rect.left < pt.x && button->Rect.right > pt.x && button->Rect.top < pt.y && button->Rect.bottom > pt.y) {

				HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
				FillRect(hdc, &button->Rect, brush);
				DeleteObject(brush);

			}

			if (button->Text.compare(L"🗖") == 0 && Win32::Utils::IsWindowFullscreen(Handle())) {
				button->Text = L"🗗";
			}
			else if (button->Text.compare(L"🗗") == 0 && !Win32::Utils::IsWindowFullscreen(Handle())) {
				button->Text = L"🗖";
			}

			DrawText(hdc, button->Text.c_str(), wcslen(button->Text.c_str()), &button->Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}

	}

	VOID Window::OnNonClientLeftMouseButtonDown() {
		POINT pt;
		GetCursorPos(&pt);

		RECT rect;
		GetWindowRect(Handle(), &rect);

		pt.x -= rect.left;
		pt.y -= rect.top;

		for (auto& button : Caption::CaptionButtons()) {

			if (button->Rect.left < pt.x && button->Rect.right > pt.x && button->Rect.top < pt.y && button->Rect.bottom > pt.y) {
				switch (button->Command) {
				case CB_CLOSE: { SendMessage(Handle(), WM_CLOSE, 0, 0); } break;
				case CB_MINIMIZE: { ShowWindow(Handle(), SW_MINIMIZE); } break;
				case CB_MAXIMIZE: { Win32::Utils::MaximizeWindow(Handle()); } break;
				}
			}
		}

	}

	VOID Window::OnGetMinMaxInfo(MINMAXINFO* minmax) {
		RECT WorkArea; SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
		minmax->ptMaxSize.x = (WorkArea.right - WorkArea.left);
		minmax->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
		minmax->ptMaxPosition.x = WorkArea.left;
		minmax->ptMaxPosition.y = WorkArea.top;
		minmax->ptMinTrackSize.x = 400;
		minmax->ptMinTrackSize.y = 300;
	}

	VOID Window::OnExitSizeMove() {
		RECT rect;
		GetWindowRect(Handle(), &rect);
		RECT WorkArea; SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
		if (rect.top < WorkArea.top + 10 && !Win32::Utils::IsWindowFullscreen(Handle()))
			Win32::Utils::MaximizeWindow(Handle());
	}

	VOID Window::OnPaint() {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(Handle(), &ps);

		RECT rc;
		GetClientRect(Handle(), &rc);

		HBRUSH brush = CreateSolidBrush(RGB(36, 36, 36));
		FillRect(hdc, &rc, brush);
		DeleteObject(brush);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		DrawText(hdc, L"Hi!", wcslen(L"Hi!"), &rc, DT_LEFT | DT_TOP);

		EndPaint(Handle(), &ps);

	}
}

// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
	// close and release all existing COM objects
	swapchain->Release();
	dev->Release();
	devcon->Release();
}
