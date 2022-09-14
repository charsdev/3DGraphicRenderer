#include <math.h>
#include "Vector.h"

vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};
	return rotated_vector;
}

float vec2_length(vec2_t v) {
	return sqrt(v.x * v.x + v.y * v.y);
}


float vec3_length(vec3_t v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(const vec3_t* a, const vec3_t* b) {
	vec3_t v = { a->x + b->x, a->y + b->y, a->z + b->z };
	return v;
}

vec2_t vec2_add(const vec2_t* a, const vec2_t* b) {
	vec2_t v = { a->x + b->x, a->y + b->y };
	return v;
}

vec3_t vec3_sub(const vec3_t* a, const vec3_t* b) {
	vec3_t v = { a->x - b->x, a->y - b->y, a->z - b->z };
	return v;
}

vec2_t vec2_sub(const vec2_t* a, const vec2_t* b) {
	vec2_t v = { a->x - b->x, a->y - b->y };
	return v;
}

vec3_t vec3_mul(const vec3_t* a, float factor) {
	vec3_t v = { a->x * factor, a->y * factor, a->z * factor };
	return v;
}

vec2_t vec2_mul(const vec2_t* a, float factor) {
	vec2_t v = { a->x * factor, a->y * factor };
	return v;
}

vec3_t vec3_div(const vec3_t* a, float factor) {
	vec3_t v = { a->x * factor, a->y * factor, a->z * factor };
	return v;
}

vec2_t vec2_div(const vec2_t* a, float factor) {
	vec2_t v = { a->x / factor, a->y / factor };
	return v;
}

vec3_t vec3_cross(const vec3_t* a, const vec3_t* b) {
	vec3_t v = { 
		(a->y * b->z) - (a->z * b->y),
		(a->z* b->x) - (a->x * b->z), 
		(a->x* b->y) - (a->y * b->x)
	};
	return v;
}

float vec3_dot(const vec3_t* a, const vec3_t* b) {
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

float vec2_dot(const vec3_t* a, const vec3_t* b) {
	return a->x * b->x + a->y * b->y;
}
