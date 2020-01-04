/*
* Copyright (c) 2016-2019, Super GP Individual.
* All Rights Reserved.
*
* Permission to use, copy, modify, and distribute this library for any
* purpose and without fee is hereby granted, provided that the above
* copyright notice appear in all copies and that both the copyright
* notice and this permission notice appear in supporting documentation,
* and that the name of SGL not be used in advertising or publicity
* pertaining to distribution of the software without specific,
* written prior permission.
*/



#ifndef SGL_GEOMETRY
#define SGL_GEOMETRY


//geometry const macros.

#define PI 3.1415926535897932


//geometry render macros.

#define SCAN_LINE 0
#define RAY_TRACER 1

#define LIGHT_RAY 0
#define SHADOW_RAY 1


typedef struct {
	int x, y;
}vec2i;
typedef struct {
	float x, y;
}vec2f;
typedef struct {
	int x, y, z;
}vec3i;
typedef struct {
	float x, y, z;
}vec3f;
typedef struct {
	int x, y, z, w;
}vec4i;
typedef struct {
	float x, y, z, w;
}vec4f;

vec2i Vec2i(int x, int y);
vec2f Vec2f(float x, float y);
vec3i Vec3i(int x, int y, int z);
vec3f Vec3f(float x, float y, float z);
vec4i Vec4i(int x, int y, int z, int w);
vec4f Vec4f(float x, float y, float z, float w);


#ifdef __cplusplus

#include <istream>
#include <vector>

#define VECN(Tv) (sizeof(Tv) /sizeof(Tv::x))


using std::vector;

vec3i Vec3i(vec2i v, int z);
vec3f Vec3f(vec2f v, float z);
vec4i Vec4i(vec3i v, int w);
vec4f Vec4f(vec3f v, float w);

vec2f normalize(vec2f v);
vec3f normalize(vec3f v);
vec4f normalize(vec4f v);
float length(vec2f v);
float length(vec3f v);
float length(vec4f v);

namespace geo {

class Range {
public:
	int start, end;

	Range() {}
	Range(int start, int end) :start(start), end(end) {}
	int size() { return end - start + 1; }
};

template <typename T>
class Matrix {
private:
	vector<vector<T>> data;
	int _row, _col;

	int iter = 0;
public:
	Matrix() {}
	Matrix(int row, int col) :_row(row), _col(col) {
		data = vector<vector<T>>(row, vector<T>(col));
		for (int i = 0; i < row; i++) {
			if (i >= col)break;
			operator[](i)[i] = 1;
		}
	}
	template <class Tv> Matrix(int row, int col, Tv *d) : Matrix(row, col) {
		for (int i = 0; i < row; i++)
			memcpy(data[i].data(), d + i * row,  col * sizeof(Tv));
	}
	template <class Tv> Matrix(Tv v, int l = VECN(Tv)) : Matrix(l, 1) {
		operator[](0)[0] = (&v.x)[0];
		operator[](1)[0] = (&v.x)[1];
		if (l > 2)
			if(VECN(Tv)>2)
				operator[](2)[0] = (&v.x)[2];
			else
				operator[](2)[0] = 0;
		if (l > 3)
			if (VECN(Tv) > 3)
				operator[](3)[0] = (&v.x)[3];
			else
				operator[](3)[0] = 0;
	}
	Matrix(Matrix m, Range x, Range y) {
		Matrix ret(y.size(), x.size());
		for (int i = 0; i < y.size(); i++) {
			for (int j = 0; j < x.size(); j++) {
				ret[i][j] = m[i + y.start][j + x.start];
			}
		}
	}
	~Matrix() {

	}

	inline Matrix &operator<<(const T val) {
		iter = 0;
		data[0][0] = val;
		return *this;
	}
	inline Matrix &operator,(const T val) {
		iter++;
		data[iter / _col][iter%_col] = val;
		return *this;
	}

	template <class vec> vec row(int row) {
		vec ret;
		for (int i = 0; i < VECN(vec); i++)(&ret.x)[i] = data[row][i];
		return ret;
	}
	template <class vec> vec col(int col) {
		vec ret;
		for (int i = 0; i < VECN(vec); i++)(&ret.x)[i] = data[i][col];
		return ret;
	}
	inline vector<T> &operator[](const int row) {
		return data[row];
	}
	inline Matrix rowRange(Range r) {
		Matrix ret(r.size(), _col);
		for (int i = 0; i < r.size(); i++) {
			ret[i] = data[i + r.start];
		}
		return ret;
	}
	inline Matrix colRange(Range r) {
		Matrix ret(_row, r.size());
		for (int i = 0; i < _row; i++) {
			for (int j = 0; j < r.size(); j++) {
				ret[i][j] = data[i][j + r.start];
			}
		}
		return ret;
	}
	Matrix &reshape(int row, int col);
	Matrix &diag();
	Matrix &inverse();
	template<class Tm>Matrix &mul(Tm scale);
	Matrix dot(Matrix m);
	
	inline double length() {
		T sum = 0;
		for (auto i : data)for (auto j : i)sum += j * j;
		return sqrt(sum);
	}
	inline Matrix &normalize() {
		double l = length();
		for (auto &i : data)for (auto &j : i)j /= l;
	}
	inline Matrix operator*(const Matrix &m) const {
		Matrix ret(this->_row, m._col);
		for (int i = 0; i < this->_row; i++) {
			for (int j = 0; j < m._col; j++) {
				T sum = 0;
				for (int k = 0; k < this->_col; k++)sum += data[i][k] * m.data[k][j];
				ret[i][j] = sum;
			}
		}
		return ret;
	}
	inline Matrix &operator*=(const Matrix &m) {
		*this = *this * m;
		return *this
	}

	template<class Ttx, class Tty, class Ttz>
	static Matrix translate(const Ttx x, const Tty y, const Ttz z) {
		return Matrix(4, 4) << (T)1, (T)0, (T)0, (T)x, (T)0, (T)1, (T)0, (T)y, (T)0, (T)0, (T)1, (T)z, (T)0, (T)0, (T)0, (T)1;
	}
	template<class Tsx, class Tsy, class Tsz>
	static Matrix scale(const Tsx x, const Tsy y, const Tsz z) {
		return Matrix(4, 4) << (T)x, (T)0, (T)0, (T)0, (T)0, (T)y, (T)0, (T)0, (T)0, (T)0, (T)z, (T)0, (T)0, (T)0, (T)0, (T)1;
	}
	template<class Trx, class Try, class Trz, class Ta>
	static Matrix rotate(const Trx x, const Try y, const Trz z, const Ta rad) {
		double l = Matrix<float>(Vec3f(x, y, z)).length();
		return Matrix(4, 4) << (T)((x*x + (y*y + z * z)*cos(rad)) / (l*l)),
			(T)((x * y * (1 - cos(rad)) - z * l * sin(rad)) / (l*l)),
			(T)((x * z * (1 - cos(rad)) + y * l * sin(rad)) / (l*l)), (T)0,
			(T)((x * y * (1 - cos(rad)) + z * l * sin(rad)) / (l*l)),
			(T)((y*y + (x*x + z * z)*cos(rad)) / (l*l)),
			(T)((y * z * (1 - cos(rad)) - x * l * sin(rad)) / (l*l)), (T)0,
			(T)((x * z * (1 - cos(rad)) - y * l * sin(rad)) / (l*l)),
			(T)((y * z * (1 - cos(rad)) + x * l * sin(rad)) / (l*l)),
			(T)((z * z + (x*x + y * y)*cos(rad)) / (l*l)), (T)0,
			(T)0, (T)0, (T)0, (T)1;
	}
	static Matrix view(const vec3f eye, const vec3f tar, const vec3f up) {
		vec3f W = eye - tar;
		W = ::normalize(W);
		vec3f U = cross(up, W);
		U = ::normalize(U);
		vec3f V = cross(W, U);
		V = ::normalize(V);
		Matrix rot(4, 4), trans(4, 4);
		rot << U.x, U.y, U.z, 0, V.x, V.y, V.z, 0, W.x, W.y, W.z, 0, 0, 0, 0, 1;
		trans << 1, 0, 0, -eye.x, 0, 1, 0, -eye.y, 0, 0, 1, -eye.z, 0, 0, 0, 1;
		Matrix tmp = rot * trans * Vec4f(0, 0, 0, 1);
		return rot * trans;
	}
	static Matrix perspect(const float fov, const float aspect,
		const float zNear, const float zFar) {
		const float zRange = zNear - zFar;
		const float tanFov = (float)tan(fov / 2.0f * PI / 180);
		return (Matrix(4, 4) << 1.0f / (2 * aspect * tanFov), 0, 0, 0,
			0, 1.0f / (2 * tanFov), 0, 0,
			0, 0, (zNear + zFar) / zRange, 2.0f * zFar*zNear / zRange,
			0, 0, -1, 0);
	}
};

template<class T> T clamp(T left, T right, T value) {
	if (value < left)return left;
	if (value > right)return right;
	return value;
}

template<class vec> vec operator+(const vec v1, const vec v2) {
	vec ret;
	(&ret.x)[0] = (&v1.x)[0] + (&v2.x)[0];
	(&ret.x)[1] = (&v1.x)[1] + (&v2.x)[1];
	if (VECN(vec) > 2)(&ret.x)[2] = (&v1.x)[2] + (&v2.x)[2];
	if (VECN(vec) > 3)(&ret.x)[3] = (&v1.x)[3] + (&v2.x)[3];
	return ret;
}
template<class vec> vec &operator+=(vec &v1, const vec v2) {
	(&v1.x)[0] += (&v2.x)[0];
	(&v1.x)[1] += (&v2.x)[1];
	if (VECN(vec) > 2)(&v1.x)[2] +=(&v2.x)[2];
	if (VECN(vec) > 3)(&v1.x)[3] += (&v2.x)[3];
	return v1;
}
template<class vec> vec operator-(const vec v1, const vec v2) {
	vec ret;
	(&ret.x)[0] = (&v1.x)[0] - (&v2.x)[0];
	(&ret.x)[1] = (&v1.x)[1] - (&v2.x)[1];
	if (VECN(vec) > 2)(&ret.x)[2] = (&v1.x)[2] - (&v2.x)[2];
	if (VECN(vec) > 3)(&ret.x)[3] = (&v1.x)[3] - (&v2.x)[3];
	return ret;
}
template<class vec> vec &operator-=(vec &v1, const vec v2) {
	(&v1.x)[0] -= (&v2.x)[0];
	(&v1.x)[1] -= (&v2.x)[1];
	if (VECN(vec) > 2)(&v1.x)[2] -= (&v2.x)[2];
	if (VECN(vec) > 3)(&v1.x)[3] -= (&v2.x)[3];
	return v1;
}
template<class vec> vec operator*(const vec v, const float s) {
	vec ret;
	(&ret.x)[0] = (&v.x)[0] * s;
	(&ret.x)[1] = (&v.x)[1] * s;
	if (VECN(vec) > 2)(&ret.x)[2] = (&v.x)[2] * s;
	if (VECN(vec) > 3)(&ret.x)[3] = (&v.x)[3] * s;
	return ret;
}
template<class vec> vec operator*(const float s, const vec v) {
	vec ret;
	(&ret.x)[0] = (&v.x)[0] * s;
	(&ret.x)[1] = (&v.x)[1] * s;
	if (VECN(vec) > 2)(&ret.x)[2] = (&v.x)[2] * s;
	if (VECN(vec) > 3)(&ret.x)[3] = (&v.x)[3] * s;
	return ret;
}
template<class vec> vec &operator*=(vec &v, const float s) {
	(&v.x)[0] *= s;
	(&v.x)[1] *= s;
	if (VECN(vec) > 2)(&v.x)[2] *= s;
	if (VECN(vec) > 3)(&v.x)[3] *= s;
	return v;
}
template<class vec> vec operator*(const vec v1, const vec v2) {
	vec ret;
	(&ret.x)[0] = (&v1.x)[0] * (&v2.x)[0];
	(&ret.x)[1] = (&v1.x)[1] * (&v2.x)[1];
	if (VECN(vec) > 2)(&ret.x)[2] = (&v1.x)[2] * (&v2.x)[2];
	if (VECN(vec) > 3)(&ret.x)[3] = (&v1.x)[3] * (&v2.x)[3];
	return ret;
}
template<class vec> vec &operator*=(vec &v1, const vec v2) {
	(&v1.x)[0] *= (&v2.x)[0];
	(&v1.x)[1] *= (&v2.x)[1];
	if (VECN(vec) > 2)(&v1.x)[2] *= (&v2.x)[2];
	if (VECN(vec) > 3)(&v1.x)[3] *= (&v2.x)[3];
	return v1;
}
template<class vec> vec operator/(const vec v, const float s) {
	vec ret;
	(&ret.x)[0] = (&v.x)[0] / s;
	(&ret.x)[1] = (&v.x)[1] / s;
	if (VECN(vec) > 2)(&ret.x)[2] = (&v.x)[2] / s;
	if (VECN(vec) > 3)(&ret.x)[3] = (&v.x)[3] / s;
	return ret;
}
template<class vec> vec &operator/=(vec &v, const float s) {
	(&v.x)[0] /= s;
	(&v.x)[1] /= s;
	if (VECN(vec) > 2)(&v.x)[2] /= s;
	if (VECN(vec) > 3)(&v.x)[3] /= s;
	return v;
}
template<class vec> bool operator<(const vec v1, const vec v2) {
	if ((&v1.x)[0] > (&v2.x)[0])return false;
	if ((&v1.x)[1] > (&v2.x)[1])return false;
	if (VECN(vec) > 2)if ((&v1.x)[2] > (&v2.x)[2])return false;
	if (VECN(vec) > 3)if ((&v1.x)[3] > (&v2.x)[3])return false;
	return true;
}
template<class vec> bool operator>(const vec v1, const vec v2) {
	if ((&v1.x)[0] < (&v2.x)[0])return false;
	if ((&v1.x)[1] < (&v2.x)[1])return false;
	if (VECN(vec) > 2)if ((&v1.x)[2] < (&v2.x)[2])return false;
	if (VECN(vec) > 3)if ((&v1.x)[3] < (&v2.x)[3])return false;
	return true;
}
template<class vec> bool operator==(const vec v1, const vec v2) {
	if ((&v1.x)[0] != (&v2.x)[0])return false;
	if ((&v1.x)[1] != (&v2.x)[1])return false;
	if (VECN(vec) > 2)if ((&v1.x)[2] != (&v2.x)[2])return false;
	if (VECN(vec) > 3)if ((&v1.x)[3] != (&v2.x)[3])return false;
	return true;
}

template<class vec> float dot(const vec v1, const vec v2) {
	float ret = 0.f;
	ret += (&v1.x)[0] * (&v2.x)[0];
	ret += (&v1.x)[1] * (&v2.x)[1];
	if (VECN(vec) > 2)ret += (&v1.x)[2] * (&v2.x)[2];
	if (VECN(vec) > 3)ret += (&v1.x)[3] * (&v2.x)[3];
	return ret;
}
template<class vec> vec cross(const vec v1, const vec v2) {
	vec ret;
	ret.x = v1.y * v2.z - v2.y * v1.z;
	ret.y = v2.x * v1.z - v1.x * v2.z;
	ret.z = v1.x * v2.y - v2.x * v1.y;
	return ret;
}

float intersectTriangle(const vec3f p0, const vec3f p1, const vec3f p2,
	const vec3f line, const vec3f dir);

}
#endif

#endif