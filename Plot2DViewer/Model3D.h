#pragma once
#include <string>
#include <fstream>
#include "Matrix.h"
#include "AffineTransform3D.h"

class Model3D {
private:
	Matrix<> Vertices;			// Вершины
	Matrix<int> Edges;			// Рёбра
	Matrix<int> Faces;			// Грани
	Matrix<> ProjectedVertices;
	Matrix<> CumulativeAT;
	Matrix<> InitialVertices;

public:
	Model3D() : Vertices(), Edges(), ProjectedVertices(), Faces(), CumulativeAT(Translation3D()), InitialVertices() {};										// Конструктор по умолчанию
	Model3D(const Matrix<> V, const Matrix<int> F) : Vertices(V), Faces(F), CumulativeAT(Translation3D()), InitialVertices(V) { SetEdges(); };	// Конструктор модели по заданным карте вершин и карте граней
	Model3D(string vData, string fData);    // Конструктор создания модели по именам файлов, в которых лежат карта вершин и карта граней


	void SetEdges();				// Создание карты рёбер по заданной карте граней
	Matrix<> GetVertices() { return Vertices; };
	Matrix<int> GetFaces() { return Faces; };
	Matrix<int> GetEdges() { return Edges; };
	double GetVertexX(int num);
	double GetVertexY(int num);
	double GetVertexZ(int num);
	void Apply(Matrix<> AT);		// Применение к модели аффинного преобразования
	void Project(Matrix<>);			// Проецирование модели
};

Model3D::Model3D(string vData, string fData) {
	int size;
	ifstream vin(vData);
	if (vin.is_open()) {
		vin >> size;
		Matrix<double> vInput(4, size);
		vin >> vInput;
		Vertices = vInput;
	}
	vin.close();
	ifstream fin(fData);
	if (fin.is_open()) {
		fin >> size;
		Matrix<int> fInput(size, 3);
		fin >> fInput;
		Faces = fInput;
	}
	fin.close();
	InitialVertices = Matrix<>(Vertices);
	CumulativeAT = Matrix<>(Translation3D());
}

void Model3D::SetEdges() {
	const int SIZE = Vertices.ColsCount();
	const int FACES_COLS_COUNT = Faces.ColsCount();
	const int FACES_EDGES_COUNT = Faces.RowsCount();
	Matrix<> edges(SIZE);
	for (int currentFace = 1; currentFace <= FACES_EDGES_COUNT; currentFace++) {
		for (int edge = 1; edge <= FACES_COLS_COUNT; edge++) {
			int currentEdge = Faces(currentFace, edge);
			for (int i = 1; i <= FACES_COLS_COUNT; i++) {
				if (i != edge) {
					int boundedEdge = Faces(currentFace, i);
					edges(currentEdge, boundedEdge) = 1;
				}
			}
		}
	}
}

double Model3D::GetVertexX(int num) {
	return Vertices(1, num) / Vertices(4, num);
}
double Model3D::GetVertexY(int num) {
	return Vertices(2, num) / Vertices(4, num);
}
double Model3D::GetVertexZ(int num) {
	return Vertices(3, num) / Vertices(4, num);
}

void Model3D::Apply(Matrix<> AT) {
	CumulativeAT = AT * CumulativeAT;
	Vertices = CumulativeAT * InitialVertices;
}