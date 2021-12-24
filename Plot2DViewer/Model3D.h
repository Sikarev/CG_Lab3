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
	Model3D();										// ����������� �� ���������
	Model3D(const Matrix<> V, const Matrix<int> E);	// ����������� ������ �� �������� ����� ������ � ����� ������
	Model3D(string vDataPath, string eDataPath);    // ����������� �������� ������ �� ������ ������, � ������� ����� ����� ������ � ����� ������


	void SetEdges();				// �������� ����� ���� �� �������� ����� ������
	Matrix<> GetVertices();
	Matrix<int> GetFaces();
	Matrix<int> GetEdges();
	double GetVertexX(int num);
	double GetVertexY(int num);
	double GetVertexZ(int num);
	void Apply(Matrix<> AT);		// ���������� � ������ ��������� ��������������
	void Project(Matrix<>);			// ������������� ������
};