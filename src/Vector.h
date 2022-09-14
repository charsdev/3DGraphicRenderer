#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
float vec2_length(vec2_t);
float vec3_length(vec3_t);
vec3_t vec3_add(const vec3_t *a, const vec3_t *b);
vec2_t vec2_add(const vec2_t* a, const vec2_t* b);
vec3_t vec3_sub(const vec3_t* a, const vec3_t* b);
vec2_t vec2_sub(const vec2_t* a, const vec2_t* b);
vec3_t vec3_mul(const vec3_t* a, float factor);
vec2_t vec2_mul(const vec2_t* a, float factor);
vec3_t vec3_div(const vec3_t* a, float factor);
vec2_t vec2_div(const vec2_t* a, float factor);
vec3_t vec3_cross(const vec3_t* a, const vec3_t* b);
float vec3_dot(const vec3_t* a, const vec3_t* b);
float vec2_dot(const vec3_t* a, const vec3_t* b);

#endif // VECTOR_H
