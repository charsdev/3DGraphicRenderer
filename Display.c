#include "Display.h"
#include <stdio.h>


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* colorBufferTexture = NULL;
int windowWidth = 800;
int windowHeight = 600;


bool initWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	/*windowHeight = displayMode.h;
	windowWidth = displayMode.w;*/

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight, SDL_WINDOW_OPENGL
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL window. \n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer. \n");
		return false;
	}

	return true;
}

void drawGrid(uint32_t color) {

	for (int y = 0; y < windowHeight; y++) {
		for (int x = 0; x < windowWidth; x++) {
			if (y % 100 == 0 || x % 100 == 0) {
				color_buffer[(windowWidth * y) + x] = color;
			}
		}
	}
}

void drawRect(int x, int y, int width, int height, uint32_t color) {

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int current_y = y + j;
			int current_x = x + i;
            drawPixel(current_x, current_y, color);
		}
	}
}

void clearColorBuffer(uint32_t color) {
	for (int y = 0; y < windowHeight; y++) {
		for (int x = 0; x < windowWidth; x++) {
			color_buffer[(windowWidth * y) + x] = color;
		}
	}
}

void clearColorBuffer2(uint32_t color) {
	for (size_t i = 0; i < (windowWidth * windowHeight); i++)
	{
		color_buffer[i] = color;
	}
}

void renderColorBuffer(void) {
	SDL_UpdateTexture(
		colorBufferTexture,
		NULL,
		color_buffer,
		(int)(windowWidth * sizeof(uint32_t))
	);
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

extern void drawPixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < windowWidth && y >= 0 && y < windowHeight) {
		color_buffer[(windowWidth * y) + x] = color;
	}
}

void destroyWindow(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void drawLineDDA(int x0, int y0, int x1, int y1, uint32_t color) {
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;

	int side_lenght = delta_x >= delta_y ? abs(delta_x) : abs(delta_y);

	float x_increment = delta_x / (float)side_lenght;
	float y_increment = delta_y / (float)side_lenght;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= side_lenght; i++) {
		drawPixel(round(current_x), round(current_y), color);
		current_x += x_increment;
		current_y += y_increment;
	}
}

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	drawLineDDA(x0, y0, x1, y1, color);
	drawLineDDA(x1, y1, x2, y2, color);
	drawLineDDA(x2, y2, x0, y0, color);
}


