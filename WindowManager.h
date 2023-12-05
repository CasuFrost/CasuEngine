#pragma once
#include <SDL.h>
#include "Strutture.h"
class WindowManager {
private :
	SDL_Renderer* renderer;
public:
	SDL_Window *window;
	
	WindowManager(const char *title, int x,int y,int h, int w,Uint32 flags) {
		window= SDL_CreateWindow(title, x, y, h, w, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
	}
	WindowManager() {
		window = SDL_CreateWindow("default", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);
		renderer = SDL_CreateRenderer(window, -1, 0);
	}
	void render() {
		SDL_SetRenderDrawColor(renderer,0, 0, 0, 0);
		SDL_RenderPresent(renderer);
	}
	void clear() {
		SDL_RenderClear(renderer);
	}
	void drawLine(int x1, int y1, int x2, int y2,Color c) {
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		SDL_RenderDrawLine(renderer, x1,  y1,  x2,  y2);
		
	}

	void drawTriangle(vec2 p1, vec2 p2, vec2 p3, Color c) {
		drawLine(p1.x, p1.y, p2.x, p2.y, c);
		drawLine(p1.x, p1.y, p3.x, p3.y, c);
		drawLine(p2.x, p2.y, p3.x, p3.y, c);
		
	}

	void drawTriangle(Triangle t , Color c) {
		drawLine(t.p1.x, t.p1.y, t.p2.x, t.p2.y, c);
		drawLine(t.p1.x, t.p1.y, t.p3.x, t.p3.y, c);
		drawLine(t.p2.x, t.p2.y, t.p3.x, t.p3.y, c);
		
	}
	void drawPoint(vec2 p, Color c) {
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		SDL_RenderDrawPoint(renderer, p.x, p.y);
	}
	void drawSquare(vec2 pos, int size, Color c) {
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		drawLine(pos.x - (int)size / 2, pos.y - (int)size / 2, pos.x + (int)size / 2, pos.y - (int)size / 2, c);
		drawLine(pos.x - (int)size / 2, pos.y + (int)size / 2, pos.x + (int)size / 2, pos.y + (int)size / 2, c);
		drawLine(pos.x - (int)size / 2, pos.y - (int)size / 2, pos.x - (int)size / 2, pos.y + (int)size / 2, c);
		drawLine(pos.x + (int)size / 2, pos.y - (int)size / 2, pos.x + (int)size / 2, pos.y + (int)size / 2, c);
	}
	void endLoop() {
		render();
		SDL_Delay(10);
		clear();
	}
};
