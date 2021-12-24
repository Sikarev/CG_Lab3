#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include "Model2D.h"

double					// Исходные значения параметров X0, Y0, px, py, c
X0 = 200,
Y0 = 200,
px = 100,
py = 50;

double scaleSpeed = 0.1;
double translationSpeed = 0.5;
double rotationSpeed = 0.05;

double a = 4;
double c = 3;

double Parabola(double x)
{
	return x*x-2;
}

double Sinusoid(double x)
{
	return sin(x);
}

double Ellipse_r1(double t)
{
	return t;
}

double Ellipse_r2(double t)
{
	return 2 * a - t;
}

double Ellipse_t_min = a - c;

double Ellipse_t_max = a + c;

double BicentrToCartesianX(double r1, double r2, double c) {
	return (r1 * r1 - r2 * r2) / (4.0 * c);
}

double BicentrToCartesianY(double r1, double r2, double c) {
	return sqrt(16 * c * c * r1 * r1 - pow(r1 * r1 - r2 * r2 + 4.0 * c * c, 2)) / (4.0 * c);
}

string verticesData = "dv.txt", edgesData = "de.txt";
Model2D initalModel(verticesData, edgesData);