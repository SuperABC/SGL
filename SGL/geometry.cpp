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
	const vec3f line, const vec3f dir) {
		vec3f e0 = p1 - p0;
		vec3f e1 = p0 - p2;
		vec3f n = cross(e1, e0);
		vec3f e2 = (1.f / dot(n, dir) * (p0 - line));
		vec3f i = cross(dir, e2);

		float beta = dot(i, e1), gamma = dot(i, e0);
		float t = dot(n, e2);

		if (beta >= 0.f && gamma >= 0.f &&beta + gamma <= 1.f)
			return t;
		else return 0.f;
	}

}

