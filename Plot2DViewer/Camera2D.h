#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double X0, Y0;						// Экранные координаты начала мировой системы координат
	double px, py;						// Единицы масштаба мировой системы координат, выраженные в пикселях
	double c;							// Расстояние от начала мировой системы координат до полюсов [r1(-c,0); r2(c,0)]
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		return X0 + px * X;
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		return Y0 - px * Y;
	}
	double ScreenToWorldX(int X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		return (X - X0 + 0.5) / px;
	}
	double ScreenToWorldY(int Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		return (Y - Y0 + 0.5) / py;
	}
	double L()							// Абсцисса левой границы рабочей области окна (в мировых координатах)
	{
		return -X0 / px;
	}
	double R()							// Абсцисса правой границы рабочей области окна (в мировых координатах)
	{
		return (W - X0) / px;
	}
	double B()
	{
		return (Y0 - H) / py;
	}
	double T()
	{
		return Y0 / py;
	}
private:
	double posX, posY;				// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
	double deltaX, deltaY;
	bool isDragging;
public:
	Camera2D(double X0, double Y0, double px, double py, double c) : X0(X0), Y0(Y0), px(px), py(py), c(c), W(0), H(0), isDragging(false)
	{
	}
	void Clear(HDC dc)
	{
		Rectangle(dc, -1, -1, W, H);
	}
	void SetResolution(HDC dc)
	{
		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров X0, Y0, px, py таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		if (!(W > 0 && H > 0)) {
			W = r.right + 1;
			H = r.bottom + 1;
			return;
		}
		int W1 = r.right + 1;
		int H1 = r.bottom + 1;
		px *= double(W1) / double(W);
		py *= double(H1) / double(H);
		X0 += double(W1 - W) / 2;
		Y0 += double(H1 - H) / 2;
		W = W1;
		H = H1;
	}
	void MoveTo(double X, double Y)
	{
		// Перемещение графического курсора (posX, posY)
		// Обратите внимание, что мы действуем в мировых координатах
		posX = X;
		posY = Y;
	}
	void LineTo(HDC dc, double X, double Y)
	{
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
	}
	void Axes(HDC dc)
	{
		MoveTo(L(), 0);
		LineTo(dc, R(), 0);
		MoveTo(0, T());
		LineTo(dc, 0, B());
	}
	void StartDragging(double X, double Y)
	{
		deltaX = X - X0;
		deltaY = Y - Y0;
		isDragging = true;
	}
	void StopDragging()
	{
		isDragging = false;
	}
	void Drag(double X, double Y)
	{
		X0 = X - deltaX;
		Y0 = Y - deltaY;
	}
	bool IsDragging()
	{
		return isDragging;
	}
	void Scale(double X, double Y, double k)
	{
		X0 = X - k * (X - X0);
		Y0 = Y - k * (Y - Y0);
		px *= k;
		py *= k;
	}
};

#endif CAMERA_2D_H
