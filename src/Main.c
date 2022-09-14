#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Array.h"
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"
//#include <crtdbg.h>

#define FOVFACTOR 640

triangle_t* triangles_to_render = NULL;

vec3_t cameraPosition = { .x = 0, .y = 0, .z = 0 };

bool is_running = false;
int previousFrameTime = 0;

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

	load_cube_mesh_data();
	//load_obj_file_data("Assets/cube.obj");
	return true;
}

void processInput() {

	//const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	//if (currentKeyStates[SDL_SCANCODE_UP])
	//{
	//	cameraPosition.z += 0.1f;
	//}
	//else if (currentKeyStates[SDL_SCANCODE_DOWN])
	//{
	//	cameraPosition.z -= 0.1f;
	//}
	//else if (currentKeyStates[SDL_SCANCODE_LEFT])
	//{
	//	mesh.rotation.x -= 0.1;
	//}
	//else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	//{
	//	mesh.rotation.x += 0.1;
	//}


	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			/*if (event.key.keysym.scancode == 26) {
			}
			if (event.key.keysym.scancode == 22) {
			}*/
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

	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.02;

	//Loop all thre vertices of this current face and apply transformation
	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec3_t transformed_vertices[3];

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			//translate the vertex away from the camera
			transformed_vertex.z += 5;
			transformed_vertices[j] = transformed_vertex;
		}

		vec3_t a = transformed_vertices[0];
		vec3_t b = transformed_vertices[1];
		vec3_t c = transformed_vertices[2];

		vec3_t ba = vec3_sub(&b, &a);
		vec3_t ca = vec3_sub(&c, &a);

		vec3_t normal = vec3_cross(&ba, &ca);
		vec3_t cameraRay = vec3_sub(&cameraPosition, &a);

		float dot = vec3_dot(&normal, &cameraRay);

		// go to next iteration
		if (dot < 0) {
			continue;
		}

		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++)
		{
			//project the current vertex
			vec2_t projected_point = persepective_project(transformed_vertices[j]);

			//scale and translate the project points to the middle of the screen
			projected_point.x += (windowWidth / 2);
			projected_point.y += (windowHeight / 2);
			projected_triangle.points[j] = projected_point;
		}
		// save the projected triangle in the array of triangles to render
		array_push(triangles_to_render, projected_triangle);
	}

}

void render(void) {
	//drawGrid(0xFFFFFFF);
	int num_triangles = array_length(triangles_to_render);

	for (int i = 0; i < num_triangles; i++) {
		triangle_t triangle = triangles_to_render[i];
		drawRect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFFF00);
		drawRect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFFF00);
		drawRect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFFF00);

		drawTriangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			0xFF00FF00
		);
	}

	array_free(triangles_to_render);
	renderColorBuffer();
	clearColorBuffer2(0x000000);

	SDL_RenderPresent(renderer);
}

void freeResources(void) {
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
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
	freeResources();
	//_CrtDumpMemoryLeaks();
	return 0;
}