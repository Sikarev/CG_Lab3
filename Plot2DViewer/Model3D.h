#pragma once
#include <string>
#include <fstream>
#include "Matrix.h"
#include "AffineTransform3D.h"

class Model3D {
private:
	Matrix<> Vertices;			// Вершины
	Matrix<int> Edges;			// Рёбра
	Matrix<int> Facets;			// Грани
	Matrix<> ProjectedVertices;
	Matrix<> CumulativeAT;
	Matrix<> InitialVertices;

public:
	Model3D() : Vertices(), Edges(), ProjectedVertices(), Facets(),			// Конструктор по умолчанию
		CumulativeAT(Translation3D()), InitialVertices() {};				
	Model3D(const Matrix<> V, const Matrix<int> F) :						// Конструктор модели по заданным карте вершин и карте граней
		Vertices(V), Facets(F), ProjectedVertices(),
		CumulativeAT(Translation3D()), InitialVertices(V) { SetEdges(); };	
	Model3D(string vData, string fData);									// Конструктор создания модели по именам файлов, в которых лежат карта вершин и карта граней


	void SetEdges();				// Создание карты рёбер по заданной карте граней
	Matrix<> GetVertices() { return Vertices; };
	Matrix<int> GetFacets() { return Facets; };
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
		Facets = fInput;
	}
	fin.close();
	InitialVertices = Matrix<>(Vertices);
	CumulativeAT = Matrix<>(Translation3D());
}

void Model3D::SetEdges() {
	const int SIZE = Vertices.ColsCount();
	const int FACETS_COLS_COUNT = Facets.ColsCount();
	const int FACETS_EDGES_COUNT = Facets.RowsCount();
	Matrix<> edges(SIZE);
	for (int currentFace = 1; currentFace <= FACETS_EDGES_COUNT; currentFace++) {
		for (int edge = 1; edge <= FACETS_COLS_COUNT; edge++) {
			int currentEdge = Facets(currentFace, edge);
			for (int i = 1; i <= FACETS_COLS_COUNT; i++) {
				if (i != edge) {
					int boundedEdge = Facets(currentFace, i);
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

void Model3D::Project(Matrix<> P) {
	ProjectedVertices = P * Vertices;
}