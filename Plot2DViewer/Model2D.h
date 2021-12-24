#pragma once

#ifndef MODEL_2D_H
#define MODEL_2D_H
#include <string>
#include <fstream>
#include "Matrix.h"
#include "AffineTransform.h"
class Model2D
{
private:
	Matrix<> Vertices;
	Matrix<int> Edges;
	Matrix<> CumulativeAT;
	Matrix<> InitialVertices;
public:
	Model2D() : Vertices(), Edges(), CumulativeAT(Translation()), InitialVertices() {};
	Model2D(const Matrix<> Vertices, const Matrix<int> Edges) :
		Vertices(Vertices), Edges(Edges), CumulativeAT(Translation()), InitialVertices(Vertices) {};
	Model2D(std::string, std::string);
	Matrix<> GetVertices() { return Vertices; };
	Matrix<int> GetEdges() { return Edges; };
	double GetVertexX(int); // Абсцисса вершины (неоднородная)
	double GetVertexY(int); // Ордината вершины (неоднородная)
	int DotsCount() { return Vertices.ColsCount(); }
	bool isBinded(int v1, int v2);
	void Apply(Matrix<>);
	void Scale(double kx, double ky);
};

Model2D::Model2D(string vData, string eData) {
	int size;
	ifstream vin(vData);
	if (vin.is_open()) {
		vin >> size;
		Matrix<double> vInput(3, size);
		vin >> vInput;
		Vertices = vInput;
	}
	vin.close();
	ifstream edin(eData);
	if (edin.is_open()) {
		edin >> size;
		Matrix<int> edInput(size);
		edin >> edInput;
		Edges = edInput;
	}
	edin.close();
	InitialVertices = Matrix<>(Vertices);
	CumulativeAT = Matrix<>(Translation());
}
double Model2D::GetVertexX(int num) {
	return Vertices(1, num)/Vertices(3, num);
}
double Model2D::GetVertexY(int num) {
	return Vertices(2, num)/Vertices(3, num);
}
bool Model2D::isBinded(int v1, int v2) {
	return Edges(v1, v2) == 1;
}
void Model2D::Apply(Matrix<> AT) {
	CumulativeAT = AT * CumulativeAT;
	Vertices =  CumulativeAT * InitialVertices;
};
void Model2D::Scale(double kx, double ky) {
	// Начало локальной системы координат
	double tx = (GetVertexX(8) + GetVertexX(4)) / 2;
	double ty = (GetVertexY(8) + GetVertexY(4)) / 2;
	Apply(Matrix<>(Translation(-tx, -ty)));
	// Направление вектора, задающего локальную ось ординат фигуры
	double ax = (GetVertexX(4));
	double ay = (GetVertexY(4));
	// Применение составного АП
	Apply(Matrix<>(Translation(tx, ty)) * Matrix<>(Rotation(ax, ay)) * Matrix<>(Scaling(kx, ky)) * Matrix<>(Rotation(ax, -ay)));
}

#endif MODEL_2D_H