#pragma once
#include <SDL.h>
#include "Strutture.h"

class WindowManager {
private :
	SDL_Renderer* renderer;
	//window size
	int width = 800;
	int height = 800;
	
public:
	SDL_Window *window;
	//Projection Matrix
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fov = 90.f;
	float fAspectRatio = (float)height / width;
	float radFov = fov / 180.f * 3.14159f; // conversione da gradi a radianti
	float fFovRad = 1.f/tanf(radFov *0.5f);
	mat4x4 projMatrix;
	


	int getScreenWidth() {
		return width;
	}
	int getScreenHeight() {
		return height;
	}

	void initializeMatrix() {
		projMatrix.m[0][0] = fAspectRatio * fFovRad;
		projMatrix.m[1][1] =  fFovRad;
		projMatrix.m[2][2] = fFar / (fFar - fNear);
		projMatrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		projMatrix.m[2][3] = 1.f;
		projMatrix.m[3][3] = 0.f;

		



	}
	
	
	WindowManager(const char *title, int x,int y,int h, int w,Uint32 flags) {
		width = w;
		height = h;
		window= SDL_CreateWindow(title, width, height, h, w, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		initializeMatrix();
	}
	WindowManager() {
		window = SDL_CreateWindow("default", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, NULL);
		renderer = SDL_CreateRenderer(window, -1, 0);
		initializeMatrix();
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

	void drawTriangle(vec2d p1, vec2d p2, vec2d p3, Color c) {
		drawLine(p1.x, p1.y, p2.x, p2.y, c);
		drawLine(p1.x, p1.y, p3.x, p3.y, c);
		drawLine(p2.x, p2.y, p3.x, p3.y, c);
		
	}

	void drawTriangle(triangle3d t , Color c) {
		drawLine(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, c);
		drawLine(t.p[0].x, t.p[0].y, t.p[2].x, t.p[2].y, c);
		drawLine(t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, c);
		
	}
	void drawPoint(vec2d p, Color c) {
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		SDL_RenderDrawPoint(renderer, p.x, p.y);
	}
	void drawSquare(vec2d pos, int size, Color c) {
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
	static void MultiplyMatVec(vec3d& in, vec3d& out, mat4x4& m) { //Moltiplica matrice e vettore
		out.x = in.x * m.m[0][0] + in.y * m.m[1][0] + in.z * m.m[2][0] + m.m[3][0];
		out.y = in.x * m.m[0][1] + in.y * m.m[1][1] + in.z * m.m[2][1] + m.m[3][1];
		out.z = in.x * m.m[0][2] + in.y * m.m[1][2] + in.z * m.m[2][2] + m.m[3][2];
		float w = in.x * m.m[0][3] + in.y * m.m[1][3] + in.z * m.m[2][3] + m.m[3][3];
		if (w != 0) {
			out.x /= w;
			out.y /= w;
			out.z /= w;
		}
	}

};
