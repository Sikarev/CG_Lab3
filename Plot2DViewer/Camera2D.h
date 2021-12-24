#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double X0, Y0;						// �������� ���������� ������ ������� ������� ���������
	double px, py;						// ������� �������� ������� ������� ���������, ���������� � ��������
	double c;							// ���������� �� ������ ������� ������� ��������� �� ������� [r1(-c,0); r2(c,0)]
	int W, H;							// ���������� ������� ������� ����
	int WorldToScreenX(double X)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return X0 + px * X;
	}
	int WorldToScreenY(double Y)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return Y0 - px * Y;
	}
	double ScreenToWorldX(int X)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (X - X0 + 0.5) / px;
	}
	double ScreenToWorldY(int Y)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (Y - Y0 + 0.5) / py;
	}
	double L()							// �������� ����� ������� ������� ������� ���� (� ������� �����������)
	{
		return -X0 / px;
	}
	double R()							// �������� ������ ������� ������� ������� ���� (� ������� �����������)
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
	double posX, posY;				// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
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
		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H, � ����� ������������� �������� ���������� X0, Y0, px, py ����� �������, ����� ���������� ���������� �������� �� ������������ ����
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
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
		posX = X;
		posY = Y;
	}
	void LineTo(HDC dc, double X, double Y)
	{
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)
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
