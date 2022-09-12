#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000/ FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
SDL_Texture* colorBufferTexture;
extern int windowWidth;
extern int windowHeight;

bool initWindow(void);
void drawGrid(uint32_t color);
void drawRect(int x, int y, int width, int height, uint32_t color);
void clearColorBuffer(uint32_t color);
void clearColorBuffer2(uint32_t color);
void renderColorBuffer(void);
inline void drawPixel(int x, int y, uint32_t color);
void destroyWindow(void);
void drawLineDDA(int x0, int y0, int x1, int y1, uint32_t color);
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
#endif // DISPLAY_H
