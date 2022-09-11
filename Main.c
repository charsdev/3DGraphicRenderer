#include <stdio.h>
#include "Display.h"
#include "Vector.h"

#define NPOINTS (9*9*9)
#define FOVFACTOR 640

vec3_t cube_points[NPOINTS];
vec2_t projectedPoints[NPOINTS];
vec3_t cameraPosition = { .x = 0, .y = 0, .z = -5 };
vec3_t cubeRotation = {.x = 0, .y = 0, .z = 0};

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
	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t point = {.x = x, .y = y, .z = z };
				cube_points[pointCount++] = point;
			}
		}
	}

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
	/*if (!SDL_TICKS_PASSED(SDL_GetTicks(), previousFrameTime + FRAME_TARGET_TIME))
		return;*/

	//previousFrameTime = SDL_GetTicks();

	int waitTime = SDL_GetTicks() - previousFrameTime - FRAME_TARGET_TIME;

	if (waitTime >= 0 && waitTime <= FRAME_TARGET_TIME) {
		SDL_Delay(waitTime);
	}

	cubeRotation.y += 0.001f;
	cubeRotation.x += 0.001f;
	cubeRotation.z += 0.001f;

	for (int i =  0; i < NPOINTS; i++) {
		vec3_t point = cube_points[i];
		vec3_t transformed_point = vec3_rotate_x(point, cubeRotation.x);
		transformed_point = vec3_rotate_y(transformed_point, cubeRotation.y);
		transformed_point = vec3_rotate_z(transformed_point, cubeRotation.z);

		transformed_point.z -= cameraPosition.z;
		vec2_t projectedPoint = persepective_project(transformed_point);
		projectedPoints[i] = projectedPoint;
	}
}

void render(void) {
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderClear(renderer);

	drawGrid(0xFFFFFFF);
	
	for (size_t i = 0; i < NPOINTS; i++)	{
		vec2_t projected_point = projectedPoints[i];
		drawRect(
			projected_point.x + (windowWidth / 2),
			projected_point.y + (windowHeight / 2), 
			4,
			4,
			0xFFFFFF00);
	}
	
	//drawRect(windowWidth/2, windowHeight / 2, 100, 100, 0xFFFFFFF);

	renderColorBuffer();
	//clearColorBuffer(0x0000FF);
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