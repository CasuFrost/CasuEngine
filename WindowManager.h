#pragma once
#include <SDL.h>
#include "Strutture.h"

class WindowManager {
private :
	SDL_Renderer* renderer;
	//window size
	int width = 1000;
	int height = 1000;

	bool is_top_left(vec2d a, vec2d b) {
		vec2d edge = { b.x - a.x,b.y - a.y };
		bool is_top_edge = edge.y == 0 && edge.x > 0;
		bool is_left_edge = edge.y < 0;
		return is_top_edge || is_left_edge;
	}
	bool is_top_left(vec3d v1, vec3d v2) {
		vec2d a = { v1.x,v1.y };
		vec2d b = { v2.x,v2.y };
		vec2d edge = { b.x - a.x,b.y - a.y };
		bool is_top_edge = edge.y == 0 && edge.x > 0;
		bool is_left_edge = edge.y < 0;
		return is_top_edge || is_left_edge;
	}
	float edge_cross(vec2d a, vec2d b, vec2d p) {
		vec2d ab = { b.x - a.x,b.y - a.y };
		vec2d ap = { p.x - a.x,p.y - a.y };
		return crossProduct(ab,ap).z;
	}
	bool pointIsInTriangle(triangle3d tri, vec2d p,int b0,int b1, int b2) {
		vec2d v0 = { tri.p[0].x,tri.p[0].y };
		vec2d v1 = { tri.p[1].x,tri.p[1].y };
		vec2d v2 = { tri.p[2].x,tri.p[2].y };
		float w_0 = edge_cross(v1,v2,p)+b0;
		float w_1 = edge_cross(v2, v0, p)+b1;
		float w_2 = edge_cross(v0, v1, p)+b2;
		
		return w_0 >= 0 && w_1 >= 0 && w_2 >= 0;
	}
public:
	vec3d crossProduct(vec3d a, vec3d b) {
		vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
		return result;
	}
	vec3d crossProduct(vec2d v1, vec2d v2) {
		vec3d a = { v1.x,v1.y,0 };
		vec3d b = { v2.x,v2.y,0 };
		vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
		return result;
	}
	vec3d crossProduct(vec3d a, vec2d v2) {
		vec3d b = { v2.x,v2.y,0 };
		vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
		return result;
	}
	vec3d crossProduct(vec2d v1, vec3d b) {
		vec3d a = { v1.x,v1.y,0 };
		vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
		return result;
	}
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

	void drawRasterizedTriangle(triangle3d t, Color c) {
		vector<float> xCords;
		vector<float> yCords;
		xCords.insert(xCords.end(),t.p[0].x ); xCords.insert(xCords.end(), t.p[1].x); xCords.insert(xCords.end(), t.p[2].x);
		yCords.insert(yCords.end(), t.p[0].y); yCords.insert(yCords.end(), t.p[1].y); yCords.insert(yCords.end(), t.p[2].y);
		
		int xMin = fgetMin(xCords);
		int xMax = fgetMax(xCords);
		int yMin = fgetMin(yCords);
		int yMax = fgetMax(yCords);

		int bias0 = is_top_left(t.p[1],t.p[2]) ? 0 : -1;
		int bias1 = is_top_left(t.p[2], t.p[0]) ? 0 : -1;
		int bias2 = is_top_left(t.p[0], t.p[1]) ? 0 : -1;


		for (int y = yMin; y <= yMax; y++) {
			for (int x = xMin; x <= xMax; x++) {
				vec2d point = { x,y };
				if (pointIsInTriangle(t,point, bias0, bias1, bias2)) {
					SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
					SDL_RenderDrawPoint(renderer, point.x, point.y);
				}
			}
		}
	}

	float fgetMax(vector<float> v) {
		float max = v[0];
		for (auto f : v) {
			if (f > max)max = f;
		}
		return max;
	}
	float fgetMin(vector<float> v) {
		float min = v[0];
		for (auto f : v) {
			if (f < min)min = f;
		}
		return min;
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
		//SDL_Delay(10);
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
