#pragma once
#include <string>
#include <fstream>
#include "Matrix.h"

class Model3D {
private:
	Matrix<> Vertices;
	Matrix<int> Edges;
	Matrix<> ProjectedVertices;
	Matrix<int> Faces;
	Matrix<> CumulativeAT;
	Matrix<> InitialVertices;

public:
	Model3D();										// Конструктор по умолчанию
	Model3D(const Matrix<> V, const Matrix<int> E);	// Конструктор модели по заданным карте вершин и карте граней
	Model3D(string vDataPath, string eDataPath);    // Конструктор создания модели по именам файлов, в которых лежат карта вершин и карта граней


	void SetEdges();				// Создание карты рёбер по заданной карте граней
	Matrix<> GetVertices();
	Matrix<int> GetFaces();
	Matrix<int> GetEdges();
	double GetVertexX(int num);
	double GetVertexY(int num);
	double GetVertexZ(int num);
	void Apply(Matrix<> AT);		// Применение к модели аффинного преобразования
	void Project(Matrix<>);			// Проецирование модели
};