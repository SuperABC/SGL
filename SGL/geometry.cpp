#include "winsgl.h"

vec2i Vec2i(int x, int y) {
	vec2i ret;
	ret.x = x;
	ret.y = y;
	return ret;
}
vec2f Vec2f(float x, float y) {
	vec2f ret;
	ret.x = x;
	ret.y = y;
	return ret;
}
vec3i Vec3i(int x, int y, int z) {
	vec3i ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}
vec3f Vec3f(float x, float y, float z) {
	vec3f ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}
vec4i Vec4i(int x, int y, int z, int w) {
	vec4i ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	ret.w = w;
	return ret;
}
vec4f Vec4f(float x, float y, float z, float w) {
	vec4f ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	ret.w = w;
	return ret;
}

vec3i Vec3i(vec2i v, int z) {
	vec3i ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = z;
	return ret;
}
vec3f Vec3f(vec2f v, float z) {
	vec3f ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = z;
	return ret;
}
vec4i Vec4i(vec3i v, int w) {
	vec4i ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	ret.w = w;
	return ret;
}
vec4f Vec4f(vec3f v, float w) {
	vec4f ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	ret.w = w;
	return ret;
}

vec2f normalize(vec2f v) {
	float length = sqrt(v.x * v.x + v.y * v.y);
	return Vec2f(v.x / length, v.y / length);
}
vec3f normalize(vec3f v) {
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return Vec3f(v.x / length, v.y / length, v.z / length);
}
vec4f normalize(vec4f v) {
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	return Vec4f(v.x / length, v.y / length, v.z / length, v.w / length);
}
float square(vec2f v) {
	return v.x * v.x + v.y * v.y;
}
float square(vec3f v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
float square(vec4f v) {
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}
float length(vec2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}
float length(vec3f v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
float length(vec4f v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

namespace geo {

float intersectTriangle(const vec3f p0, const vec3f p1, const vec3f p2,
	const vec3f line, const vec3f dir, float *beta, float *gamma) {
	vec3f e0 = p1 - p0;
	vec3f e1 = p0 - p2;
	vec3f n = cross(e1, e0);
	vec3f e2 = (1.f / dot(n, dir) * (p0 - line));
	vec3f i = cross(dir, e2);

	*beta = dot(i, e1), *gamma = dot(i, e0);
	float t = dot(n, e2);

	if (*beta >= 0.f && *gamma >= 0.f &&*beta + *gamma <= 1.f)
		return t;
	else return 0.f;
}
vec3f TriangleInterp(vector<vec3f> shape, vec3f hit, vector<vec3f> data) {
	vec3f ret;
	if ((shape[1].y - shape[0].y) * (shape[2].x - shape[0].x) != (shape[2].y - shape[0].y) * (shape[1].x - shape[0].x)) {
		float u = -((shape[0].x - hit.x)*(shape[2].y - shape[0].y) - (shape[0].y - hit.y)*(shape[2].x - shape[0].x)) /
			((shape[1].x - shape[0].x)*(shape[2].y - shape[0].y) - (shape[1].y - shape[0].y)*(shape[2].x - shape[0].x));
		float v = -((shape[0].x - hit.x)*(shape[1].y - shape[0].y) - (shape[0].y - hit.y)*(shape[1].x - shape[0].x)) /
			((shape[2].x - shape[0].x)*(shape[1].y - shape[0].y) - (shape[2].y - shape[0].y)*(shape[1].x - shape[0].x));
		ret = (1 - u - v)*data[0] + u * data[1] + v * data[2];
	}
	else if((shape[1].z - shape[0].z) * (shape[2].y - shape[0].y) != (shape[2].z - shape[0].z) * (shape[1].y - shape[0].y)) {
		float u = -((shape[0].y - hit.y)*(shape[2].z - shape[0].z) - (shape[0].z - hit.z)*(shape[2].y - shape[0].y)) /
			((shape[1].y - shape[0].y)*(shape[2].z - shape[0].z) - (shape[1].z - shape[0].z)*(shape[2].y - shape[0].y));
		float v = -((shape[0].y - hit.y)*(shape[1].z - shape[0].z) - (shape[0].z - hit.z)*(shape[1].y - shape[0].y)) /
			((shape[2].y - shape[0].y)*(shape[1].z - shape[0].z) - (shape[2].z - shape[0].z)*(shape[1].y - shape[0].y));
		ret = (1 - u - v)*data[0] + u * data[1] + v * data[2];
	}
	else {
		float u = -((shape[0].z - hit.z)*(shape[2].x - shape[0].x) - (shape[0].x - hit.x)*(shape[2].z - shape[0].z)) /
			((shape[1].z - shape[0].z)*(shape[2].x - shape[0].x) - (shape[1].x - shape[0].x)*(shape[2].z - shape[0].z));
		float v = -((shape[0].z - hit.z)*(shape[1].x - shape[0].x) - (shape[0].x - hit.x)*(shape[1].z - shape[0].z)) /
			((shape[2].z - shape[0].z)*(shape[1].x - shape[0].x) - (shape[2].x - shape[0].x)*(shape[1].z - shape[0].z));
		ret = (1 - u - v)*data[0] + u * data[1] + v * data[2];
	}
	return ret;
}
float linePointDist(vec3f start, vec3f dir, vec3f point) {
	vec3f d = point - start;
	dir = normalize(dir);
	return length(cross(d, dir));
}

}

