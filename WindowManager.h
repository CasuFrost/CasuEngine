#pragma once
#include <SDL.h>
#include "Strutture.h"

class WindowManager {
private :
	vector<mesh> meshPool;
	vec3d vCamera = { 0,0,0 };
	SDL_Renderer* renderer;
	//window size
	int width = 1000;
	int height = 1000;
	mat4x4 matRotX, matRotZ;
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
	mesh getMesh(int i) {
		if(i<=meshPool.size())return meshPool[i];
		mesh a;
		return a;
	}
public:
	vec3d getMeshPosition(int i) {
		return meshPool[i].position;
	}
	vec3d getMeshRotation(int i) {
		return meshPool[i].rotation;
	}
	void makeWater(int i) {
		if (meshPool[i].water)return;
		meshGenerator genMesh;
		mesh a = genMesh.creatRect(2.f, 0.05f);
		a.position = meshPool[i].position;
		a.rotation = meshPool[i].rotation;
		a.color = {0,0,255,255};
		meshPool[i] = a;
		for (int j = 0; j < 9; j++) {
			subsetMesh(i);
		}
		meshPool[i].water = true;
	}
	void updateMeshPosition(int i, vec3d newPos) {
		if (i <= meshPool.size()) {
			meshPool[i].position = newPos;
		}
	}
	void updateMeshRotationDegrees(int i, vec3d newRot) {
		if (i <= meshPool.size()) {
			meshPool[i].rotation = newRot;
		}
	}
	void updateMeshColor(int i, Color c) {
		if (i <= meshPool.size()) {
			meshPool[i].color = c;
		}
	}
	void subsetMesh(int i) {
		if (meshPool[i].water)return;
		meshGenerator meshGen;
		 
		meshGen.subsectMesh(meshPool[i]);
		
	}
	void subsetAllMesh() {
		meshGenerator meshGen;
		for (unsigned int i = 0; i < meshPool.size(); i++)
		{
			meshGen.subsectMesh(meshPool[i]);
		}
	}
	void addMeshToPool(mesh m) {
		meshPool.insert(meshPool.end(), m);
	}
	
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
	int getMeshPoolSize() {
		return meshPool.size();
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
	void drawRasterizedTriangleSdl(triangle3d t, Color c) {
		SDL_Color c1 = { c.r, c.g, c.b, c.a };
		SDL_Color c2 = { c.r , c.g , c.b , c.a };
		SDL_Color c3 = { c.r , c.g , c.b , c.a };
		vector< SDL_Vertex > verts =
		{
			{ SDL_FPoint{ t.p[0].x, t.p[0].y}, c1, SDL_FPoint{0},},
			{ SDL_FPoint{  t.p[1].x, t.p[1].y },  c1, SDL_FPoint{ 0 }, },
			{ SDL_FPoint{  t.p[2].x, t.p[2].y },  c1, SDL_FPoint{ 0 }, },
		};
		SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
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
			clear();

	}
	void endLoop(int delay) {
		render();
		SDL_Delay(delay);
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
	void clearMeshPool() {
		meshPool.clear();
	}
	void rotateMesh(int i) {
		if (i <= meshPool.size()) {
			triangle3d triProjected, triTranslated, triRotatedZ, triRotatedX;
			for (auto tri : meshPool[i].tris) {
				for (int i = 0; i < 3; i++) {
					MultiplyMatVec(tri.p[i], triRotatedZ.p[i], matRotZ);
					MultiplyMatVec(triRotatedZ.p[i], triRotatedX.p[i], matRotX);
					triRotatedX.p[i].x += meshPool[i].position.x;
					triRotatedX.p[i].y += meshPool[i].position.y;
					triRotatedX.p[i].z += meshPool[i].position.z;
				}
			}
		}
	}
	float dotProdutct(vec3d a, vec3d b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	void normalizeVec(vec3d &normal) {
		float normalLenght = sqrtf(powf(normal.x, 2) + powf(normal.y, 2) + powf(normal.z, 2));
		normal.x /= normalLenght; normal.y /= normalLenght; normal.z /= normalLenght;
	}
	Color changeColorDarknes(Color c, float lum) {
		int r = c.r * lum / 150.f;
		int g = c.g * lum / 150.f;
		int b = c.b * lum / 150.f;
	
		return {r,g,b,c.a};
	}
	void renderMesh(bool wireFrame,float time) {
		for (auto mesh : meshPool) {
			matRotZ.m[0][0] = cosf(time);
			matRotZ.m[0][1] = sinf(time);
			matRotZ.m[1][0] = -sinf(time);
			matRotZ.m[1][1] = cosf(time);
			matRotZ.m[2][2] = matRotZ.m[3][3] = 1;


			matRotX.m[1][1] = cosf(time );
			matRotX.m[1][2] = sinf(time );
			matRotX.m[2][1] = -sinf(time);
			matRotX.m[2][2] = cosf(time);
			matRotX.m[0][0] = matRotX.m[3][3] = 1;
			/*DELETE ROT
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					matRotX.m[i][j] = 1;
					matRotZ.m[i][j] = 1;
				}
			}
			*/

			for (auto tri : mesh.tris) {
				triangle3d triProjected, triTranslated, triRotatedZ, triRotatedX,triFixedRotatedX, triFixedRotatedZ;


				for (int i = 0; i < 3; i++) {
					//MultiplyMatVec(tri.p[i], triRotatedZ.p[i], matRotZ);
					MultiplyMatVec(tri.p[i], triRotatedX.p[i], matRotX);

				}
				if (!mesh.continiusRotation) {
					triRotatedX = tri;
				}


				//Update Rotation
				mat4x4 FixedMatRotX;
				FixedMatRotX.m[1][1] = cosf(mesh.rotation.x);
				FixedMatRotX.m[1][2] = sinf(mesh.rotation.x);
				FixedMatRotX.m[2][1] =-sinf(mesh.rotation.x);
				FixedMatRotX.m[2][2] = cosf(mesh.rotation.x);
				FixedMatRotX.m[0][0] = FixedMatRotX.m[3][3] = 1;
				for(int i = 0; i < 3; i++) {
					MultiplyMatVec(triRotatedX.p[i], triFixedRotatedX.p[i], FixedMatRotX);
				}
				

				mat4x4 FixedMatRotZ;
				FixedMatRotZ.m[0][0] = cosf(mesh.rotation.z);
				FixedMatRotZ.m[0][1] = sinf(mesh.rotation.z);
				FixedMatRotZ.m[1][0] = -sinf(mesh.rotation.z);
				FixedMatRotZ.m[1][1] = cosf(mesh.rotation.z);
				FixedMatRotZ.m[2][2] = FixedMatRotZ.m[3][3] = 1;
				for (int i = 0; i < 3; i++) {
					MultiplyMatVec(triFixedRotatedX.p[i], triFixedRotatedZ.p[i], FixedMatRotZ);
				}

				
				

				for (int i = 0; i < 3; i++) {
					triFixedRotatedZ.p[i].x += mesh.position.x;
					triFixedRotatedZ.p[i].y += mesh.position.y;
					triFixedRotatedZ.p[i].z += mesh.position.z;
				}
				triTranslated = triFixedRotatedZ;


				

				//normals 
				vec3d normal, line1, line2;
				line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
				line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
				line1.z = triTranslated.p[1].z - triTranslated.p[0].z;
				line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
				line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
				line2.z = triTranslated.p[2].z - triTranslated.p[0].z;
				normal = crossProduct(line1, line2);
				normalizeVec(normal);
				
				
				
				//Wave
				if (mesh.water) {
					float f = 1;
					if (true||normal.z==0) {
						for (int i = 0; i < 3; i++) {
							triTranslated.p[i].x += 0.2 * sinf((time + triTranslated.p[i].z)*f);
							triTranslated.p[i].y += 0.3 * sinf((time + triTranslated.p[i].x)*f*2);
							//triFixedRotatedZ.p[i].z += 0.1 * sinf(time * triFixedRotatedZ.p[i].y);
						

						}
					}
					else {
					
					}
				}
				

				if ((normal.x * (triTranslated.p[0].x - vCamera.x) +
					normal.y * (triTranslated.p[0].y - vCamera.y) +
					normal.z * (triTranslated.p[0].z - vCamera.z) < 0.f)) {

						vec3d light_direction = { 1.0f,0.f,-1.f };
						normalizeVec(light_direction);
						float dp = dotProdutct(normal, light_direction);

						//calc lum
						int lum = (int)(150.f * dp);
						Color litghedColor = changeColorDarknes(mesh.color, lum);

						
						
						


						MultiplyMatVec(triTranslated.p[0], triProjected.p[0], projMatrix);//Proietta un singolo triangolo
						MultiplyMatVec(triTranslated.p[1], triProjected.p[1], projMatrix);
						MultiplyMatVec(triTranslated.p[2], triProjected.p[2], projMatrix);


						//La matrice di proiezione restituisce un risultato in uno schermo normalizzato da -1 ad +1. Va scalato !
						triProjected.p[0].x += 1.f; triProjected.p[0].y += 1.f;
						triProjected.p[1].x += 1.f; triProjected.p[1].y += 1.f;
						triProjected.p[2].x += 1.f; triProjected.p[2].y += 1.f;

						triProjected.p[0].x *= 0.5 * (float)getScreenWidth(); triProjected.p[0].y *= 0.5 * (float)getScreenWidth();
						triProjected.p[1].x *= 0.5 * (float)getScreenWidth(); triProjected.p[1].y *= 0.5 * (float)getScreenWidth();
						triProjected.p[2].x *= 0.5 * (float)getScreenWidth(); triProjected.p[2].y *= 0.5 * (float)getScreenWidth();
						if (wireFrame)drawTriangle(triProjected, litghedColor);
						else {
							drawRasterizedTriangleSdl(triProjected, litghedColor);
							//w.drawTriangle(triProjected, {255,255,255,255});
						}
				}

			}
		}
	}

};
