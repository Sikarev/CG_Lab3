#pragma once
#include "Camera2D.h"
#include "Vector3D.h"
#include "Matrix.h"

class Camera3D: public Camera2D {
protected:
	Vector3D<> Ov, N, T;
	double D;
	Matrix<> WorldToView, ViewToProject, WorldToProject;

	void setOv(Vector3D<> point);
	void setN(Vector3D<> point);
	void setT(Vector3D<> point);
	void setD(double value);
public:
	Camera3D(double X0, double Y0, double px, double py, double c = 0) : Camera2D(X0, Y0, px, py, c), Ov(0, 0, 0), T(0, 1, 0), N(0, 0, 1), D(16) { UpdateCamera(); };

	void UpdateCamera();
};

void Camera3D::setOv(Vector3D<> point) {
	Ov = point;
}
void Camera3D::setN(Vector3D<> point) {
	N = point;
}
void Camera3D::setT(Vector3D<> point) {
	T = point;
}
void Camera3D::setD(double value) {
	D = value;
}
void Camera3D::UpdateCamera() {
	double T1[] = {
		1, 0,   0,      0,
		0, 1,   0,      0,
		0, 0, -(1 / D), 1
	};
	ViewToProject = Matrix<>(3, 4, T1);

	Vector3D<> k = N.normalize();
	Vector3D<> i = (T * N).normalize();
	Vector3D<> j = k * i;

	double T2[] = {
			i.x, i.y, i.z, -(Ov.scalar(i)),
			j.x, j.y, j.z, -(Ov.scalar(j)),
			k.x, k.y, k.z, -(Ov.scalar(k)),
			0, 0, 0, 1
	};
	WorldToView = Matrix<>(4, 4, T2);

	WorldToProject = ViewToProject * WorldToView;
}