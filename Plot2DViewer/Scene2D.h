#ifndef SCENE_2D_H
#define SCENE_2D_H

#include <cmath>
#include "Camera2D.h"

class Scene2D : public Camera2D
{
private:
	typedef double (*Func)(double);
public:
	Model2D Model;
	Scene2D(double X0, double Y0, double px, double py, double c, Model2D model) : Camera2D(X0, Y0, px, py, c), Model(model)
	{
	}
	void Plot(HDC dc, Func f, bool axes=true)
	{
		if (axes)
			Axes(dc);

		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);

		const double STEP = 1 / px;

		MoveTo(L(), f(L()));
		for (double x = L() + STEP; x < R(); x+= STEP)
		{
			LineTo(dc, (x), f(x));
		}
		LineTo(dc, R(), f(R()));
	}
	void BicentricPlot(HDC dc, Func f_r1, Func f_r2, double t_min, double t_max, double c, bool axes = true)
	{
		if (axes)
			Axes(dc);
		const double STEP = 0.1;
		double
			t = t_min,
			x = BicentrToCartesianX(f_r1(t), f_r2(t), c),
			y = BicentrToCartesianY(f_r1(t), f_r2(t), c);

		MoveTo(x, y);
		for (; t <= t_max; t += STEP)
		{
			x = BicentrToCartesianX(f_r1(t), f_r2(t), c),
			y = BicentrToCartesianY(f_r1(t), f_r2(t), c);
			LineTo(dc, x, y);
		}
		t = t_max;
		x = BicentrToCartesianX(f_r1(t), f_r2(t), c),
		y = BicentrToCartesianY(f_r1(t), f_r2(t), c);
		LineTo(dc, x, y);

		t = t_min;
		x = BicentrToCartesianX(f_r1(t), f_r2(t), c),
		y = -BicentrToCartesianY(f_r1(t), f_r2(t), c);

		MoveTo(x, y);
		for (; t <= t_max; t += STEP)
		{
			x = BicentrToCartesianX(f_r1(t), f_r2(t), c),
			y = -BicentrToCartesianY(f_r1(t), f_r2(t), c);
			LineTo(dc, x, y);
		}
		t = t_max;
		x = BicentrToCartesianX(f_r1(t), f_r2(t), c),
		y = -BicentrToCartesianY(f_r1(t), f_r2(t), c);
		LineTo(dc, x, y);
	}
	void Render(HDC dc, bool axes = true) 
	{
		if (axes)
			Axes(dc);

		for (int i = 1; i <= Model.DotsCount(); i++)
		{
			for (int j = 1; j <= Model.DotsCount(); j++) {
				if (Model.isBinded(i, j)) {
					MoveTo(Model.GetVertexX(i), Model.GetVertexY(i));
					LineTo(dc, Model.GetVertexX(j), Model.GetVertexY(j));
				}
			}
		}
	}
};

#endif SCENE_2D_H
