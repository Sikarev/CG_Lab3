#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

double					// Исходные значения параметров X0, Y0, px, py, c
X0 = 200,
Y0 = 200,
px = 100,
py = 50;

double scaleSpeed = 0.1;
double translationSpeed = 0.5;
double rotationSpeed = 0.05;

std::string verticesData = "dv.txt", edgesData = "de.txt", facetsData = "df.txt";