#pragma once
#include "Model2D.h"

class Model3D : protected Model2D {
private:
	Matrix<> ProjectedVertices;
	Matrix<int> Faces;

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