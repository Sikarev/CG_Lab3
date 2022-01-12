#pragma once

template <typename T = double>
class Vector3D {
public:
	T x, y, z;

	Vector3D(T xValue = 0, T yValue = 0, T zValue = 0) : x(xValue), y(yValue), z(zValue) {};
	Vector3D(const Vector3D& src) : x(src.x), y(src.y), z(src.z) {};

	double length();
	double scalar(const Vector3D&);

	Vector3D operator = (const Vector3D&);
	Vector3D operator + (const Vector3D&);
	Vector3D operator - (const Vector3D&);
	Vector3D operator * (const Vector3D&);
	Vector3D operator * (double);
};

template <typename T>
double Vector3D<T>::length() {
	return sqrt(x * x + y * y + z * z);
}

template <typename T>
double Vector3D<T>::scalar(const Vector3D& V) {
	return x * V.x + y * V.y + z * V.z;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator=(const Vector3D& V) {
	Vector3D res;
	res.x = V.x;
	res.y = V.y;
	res.z = V.z;
	return res;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator+(const Vector3D& V) {
	Vector3D res;
	res.x = x + V.x;
	res.y = y + V.y;
	res.z = z + V.z;
	return res;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator-(const Vector3D& V) {
	Vector3D res;
	res.x = x - V.x;
	res.y = y - V.y;
	res.z = z - V.z;
	return res;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator*(const Vector3D& V) {
	Vector3D res;
	res.x = z * V.y - y * V.z;
	res.y = x * V.z - z * V.x;
	res.z = y * V.x - x * V.y;
	return res;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator*(double k) {
	Vector3D res;
	res.x = x * k;
	res.y = y * k;
	res.z = z * k;
	return res;
}