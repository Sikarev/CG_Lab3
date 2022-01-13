#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene3D.h"
#include "Matrix.h"
#include "AffineTransform3D.h"
#include "Model3D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создаётся окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// имя оконного класса, используемое при создании экземпляров окна
	RegisterClass(&wc);								// регистрация класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// имя оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200,200,400,400,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// В основном модуле объявляется только одна глобальная переменная - создаётся объект класса Scene2D
// Все дальнейшие действия осуществляются посредством обращения к методам, реализованным в этом классе
Scene3D scene(X0, Y0, px, py, verticesData, facetsData);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
	switch(msg)
	{
	case WM_PAINT:
		{
			HDC dc = GetDC(hWnd);
			scene.Clear(dc);				// Вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd
			//scene.Plot(dc, Sinusoid);		// Вызов реализованного в классе Scene2D метода, отвечающего за отрисовку графика синусоиды
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
				scene.Apply(Translation3D(translationSpeed, 0, 0));
				break;
			}
			case 0x44:
			case VK_RIGHT: {
				scene.Apply(Translation3D(-translationSpeed, 0, 0));
				break;
			}
			case 0x57:
			case VK_UP: {
				scene.Apply(Translation3D(0, translationSpeed, 0));
				break;
			}
			case 0x53:
			case VK_DOWN: {
				scene.Apply(Translation3D(0, -translationSpeed, 0));
				break;
			}
			case 0x31: {
				scene.Apply(RotationX3D(rotationSpeed));
				break;
			}
			case 0x32: {
				scene.Apply(RotationX3D(-rotationSpeed));
				break;
			}
			case 0x33: {
				scene.Apply(RotationY3D(rotationSpeed));
				break;
			}
			case 0x34: {
				scene.Apply(RotationY3D(-rotationSpeed));
				break;
			}
			case 0x35: {
				scene.Apply(RotationZ3D(rotationSpeed));
				break;
			}
			case 0x36: {
				scene.Apply(RotationZ3D(-rotationSpeed));
				break;
			}
			case 0x46: {
				scene.Apply(Scaling3D(-1, -1, -1));
				break;
			}
			case 0x47: {
				scene.Apply(Scaling3D(1, -1, -1));
				break;
			}
			case 0x48: {
				scene.Apply(Scaling3D(-1, 1, -1));
				break;
			}
			case 0x4A: {
				scene.Apply(Scaling3D(-1, -1, 1));
				break;
			}
			case 0x5A: {
				scene.Apply(Scaling3D(1 + scaleSpeed, 1 + scaleSpeed, 1 + scaleSpeed));
				break;
			}
			case 0x58: {
				scene.Apply(Scaling3D(1 - scaleSpeed, 1 - scaleSpeed, 1 - scaleSpeed));
				break;
			}
			case 0x43: {
				scene.Apply(Scaling3D(1 + scaleSpeed, 1, 1));
				break;
			}
			case 0x56: {
				scene.Apply(Scaling3D(1 - scaleSpeed, 1, 1));
				break;
			}
			case 0x42: {
				scene.Apply(Scaling3D(1, 1 + scaleSpeed, 1));
				break;
			}
			case 0x4e: {
				scene.Apply(Scaling3D(1, 1 - scaleSpeed, 1));
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
