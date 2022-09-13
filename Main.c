#include <stdio.h>
#include <SDL2/SDL.h>
#include "Display.h"
#include "Array.h"
#include "Vector.h"
#include "Mesh.h"

#define FOVFACTOR 640

//triangle_t triangles_to_render[N_MESH_FACES];
triangle_t* triangles_to_render;

vec3_t cameraPosition = { .x = 0, .y = 0, .z = -10 };
vec3_t cubeRotation = { .x = 0, .y = 0, .z = 0 };

int previousFrameTime = 0;
bool is_running;

bool setup(void) {
	color_buffer = (uint32_t)malloc(sizeof(uint32_t) * windowHeight * windowWidth);

	if (!color_buffer) {
		fprintf(stderr, "Error creating Color Buffer\n");
		return false;
	}

	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		windowWidth,
		windowHeight
	);

	if (!colorBufferTexture) {
		fprintf(stderr, "Error creating Color Buffer Texture\n");
		return false;
	}

	int pointCount = 0;

	return true;
}

void processInput() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.scancode == 41) {
			SDL_SetWindowSize(window, 800, 600);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
		printf("press key %i", (int)event.key.keysym.scancode);

		break;
	}
}


vec2_t ortographic_project(vec3_t point) {
	vec2_t vec = {
		.x = point.x * FOVFACTOR,
		.y = point.y * FOVFACTOR
	};
	return vec;
}

vec2_t persepective_project(vec3_t point) {
	vec2_t proyectedPoint = {
		.x = point.x * FOVFACTOR / point.z,
		.y = point.y * FOVFACTOR / point.z
	};
	return proyectedPoint;
}

void update(void) {
	// Wait some time until the reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrameTime);

	// Only delay execution if we are running too fast
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	previousFrameTime = SDL_GetTicks();


	triangles_to_render = NULL;

	cubeRotation.y += 0.01f;
	cubeRotation.x += 0.01f;
	cubeRotation.z += 0.01f;

	//Loop all thre vertices of this current face and apply transformation
	for (int i = 0; i < N_MESH_FACES; i++)
	{
		face_t mesh_face = mesh_faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh_vertices[mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		for (size_t j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_x(transformed_vertex, cubeRotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cubeRotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cubeRotation.z);

			//translate the vertex away from the camera
			transformed_vertex.z -= cameraPosition.z;

			//project the current vertex
			vec2_t projected_point = persepective_project(transformed_vertex);

			//scale and translate the project points to the middle of the screen
			projected_point.x += (windowWidth / 2);
			projected_point.y += (windowHeight / 2);
			projected_triangle.points[j] = projected_point;
		}
		// save the projected triangle in the array of triangles to render
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}

}

void render(void) {
	drawGrid(0xFFFFFFF);

	for (size_t i = 0; i < array_length(triangles_to_render); i++) {
		triangle_t triangle = triangles_to_render[i];
		drawRect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFFF00);
		drawRect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFFF00);
		drawRect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFFF00);

		drawTriangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			0xFF00FF00);
	}

	array_free(triangles_to_render);
	renderColorBuffer();
	clearColorBuffer2(0x000000);
	SDL_RenderPresent(renderer);

}

int main(int argc, char* args[]) {

	is_running = initWindow();
	is_running = setup();

	while (is_running) {
		processInput();
		update();
		render();
	}

	destroyWindow();

	return 0;
}