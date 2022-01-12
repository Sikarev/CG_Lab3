#pragma once
#include "Matrix.h"

Matrix<> Translation3D(double x = 0, double y = 0, double z = 0) {
	double T[16] = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1};
	return Matrix<>(4, 4, T);
}
Matrix<> RotationX3D(double t) {
	double T[16] = {
		1, 0, 0, 0,
		0, cos(t), -sin(t), 0,
		0, sin(t), cos(t), 0,
		0, 0, 0, 1 };
	return Matrix<>(4, 4, T);
}
Matrix<> RotationY3D(double t) {
	double T[16] = {
		cos(t), 0, sin(t), 0,
		0, 1, 0, 0,
		-sin(t), 0, cos(t), 0,
		0, 0, 0, 1 };
	return Matrix<>(4, 4, T);
}
Matrix<> RotationZ3D(double t) {
	double T[16] = {
		cos(t), -sin(t), 0, 0,
		sin(t), cos(t), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	return Matrix<>(4, 4, T);
}
Matrix<> Scaling3D(double kx = 1, double ky = 1, double kz = 1) {
	double T[16] = {
		kx, 0, 0, 0,
		0, ky, 0, 0,
		0, 0, kz, 0,
		0, 0, 0, 1};
	return Matrix<>(4, 4, T);
}