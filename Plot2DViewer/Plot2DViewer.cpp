#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene2D.h"
#include "Matrix.h"
#include "AffineTransform.h"
#include "Model2D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// �������� ���������
{
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� �������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass(&wc);								// ����������� ������ wc

	HWND hWnd = CreateWindow(						// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"MainWindowClass",					// ��� �������� ������
		(LPCSTR)"Plot2D Viewer",					// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		200,200,400,400,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

// � �������� ������ ����������� ������ ���� ���������� ���������� - �������� ������ ������ Scene2D
// ��� ���������� �������� �������������� ����������� ��������� � �������, ������������� � ���� ������
Scene2D scene(X0,Y0,px,py,c, initalModel);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
{
	switch(msg)
	{
	case WM_PAINT:
		{
			HDC dc = GetDC(hWnd);
			scene.Clear(dc);				// ����� �������������� � ������ Camera2D ������, ����������� �� ������� ������� ������� ���� hWnd
			//scene.Plot(dc, Sinusoid);		// ����� �������������� � ������ Scene2D ������, ����������� �� ��������� ������� ���������
			//scene.Plot(dc, Parabola);
			//scene.BicentricPlot(dc, Ellipse_r1, Ellipse_r2, Ellipse_t_min, Ellipse_t_max, c);
			scene.Render(dc);
			ReleaseDC(hWnd,dc);
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}

	case WM_SIZE:
		{
			HDC dc = GetDC(hWnd);
			scene.SetResolution(dc);
			ReleaseDC(hWnd,dc);
			InvalidateRect(hWnd,nullptr,false);
			return 0;
		}

	case WM_LBUTTONDOWN:
		{
			scene.StartDragging(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}

	case WM_MOUSEMOVE:
		{
			if (scene.IsDragging())
			{
				scene.Drag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				InvalidateRect(hWnd, nullptr, false);
			}
			return 0;
		}

	case WM_LBUTTONUP:
		{
			scene.StopDragging();
			return 0;
		}

	case WM_MOUSEWHEEL:
		{
		POINT mouse_pos = {
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
		};
			ScreenToClient(hWnd, &mouse_pos);
			double k = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1 + scaleSpeed : 1 - scaleSpeed;
			scene.Scale(mouse_pos.x, mouse_pos.y, k);
			InvalidateRect(hWnd, nullptr, false);
			return 0;
		}

	case WM_KEYDOWN: {
		switch (wParam) {
			case 0x41:
			case VK_LEFT: {
				scene.Model.Apply(Translation(-translationSpeed, 0));
				break;
			}
			case 0x44:
			case VK_RIGHT: {
				scene.Model.Apply(Translation(translationSpeed, 0));
				break;
			}
			case 0x57:
			case VK_UP: {
				scene.Model.Apply(Translation(0, translationSpeed));
				break;
			}
			case 0x53:
			case VK_DOWN: {
				scene.Model.Apply(Translation(0, -translationSpeed));
				break;
			}
			case 0x51: {
				scene.Model.Apply(Rotation(rotationSpeed));
				break;
			}
			case 0x45: {
				scene.Model.Apply(Rotation(-rotationSpeed));
				break;
			}
			case 0x5A: {
				scene.Model.Scale(1 + scaleSpeed, 1 + scaleSpeed);
				break;
			}
			case 0x58: {
				scene.Model.Scale(1 - scaleSpeed, 1 - scaleSpeed);
				break;
			}
			case 0x43: {
				scene.Model.Scale(1 + scaleSpeed, 1);
				break;
			}
			case 0x56: {
				scene.Model.Scale(1 - scaleSpeed, 1);
				break;
			}
			case 0x42: {
				scene.Model.Scale(1, 1 + scaleSpeed);
				break;
			}
			case 0x4e: {
				scene.Model.Scale(1, 1 - scaleSpeed);
				break;
			}
			default: {
				break;
			}
		}
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	default:
		{
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}
	}
	return 0;
}