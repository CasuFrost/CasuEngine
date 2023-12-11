#pragma once
#include <SDL.h>
#include "StructAndVecOp.h"

class WindowManager {
private :
	vector<Mesh> meshPool;
	vector<Object> objPool;
	vec3d vCamera = { 0,0,0 };
	SDL_Renderer* renderer;
	//window size
	int width = 1000;
	int height = 1000;
	mat4x4 matRotX, matRotZ, matRotY;
	
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
	
	
	Mesh getMesh(int i) {
		if(i<=meshPool.size())return meshPool[i];
		Mesh a;
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
		Mesh a = genMesh.creatRect(4.f, 0.05f);
		a.position = meshPool[i].position;
		a.rotation = meshPool[i].rotation;
		a.color = {0,0,255,255};
		meshPool[i] = a;
		for (int j = 0; j <8; j++) {
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
	void addMeshToPool(Mesh m) {
		meshPool.insert(meshPool.end(), m);
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
		matRotX.m[0][0] = matRotX.m[3][3] = 1;
		matRotZ.m[2][2] = matRotZ.m[3][3] = 1;
		matRotY.m[1][1] = matRotY.m[3][3] = 1;

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
	void drawVertsTriangle(triangle3d t, Color c) {
		drawLine(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, c);
		drawLine(t.p[0].x, t.p[0].y, t.p[2].x, t.p[2].y, c);
		drawLine(t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, c);

		for (int i = 0; i < 3; i++) {
			vec2d p;
			p.x = t.p[i].x;
			p.y = t.p[i].y;
			drawBigPoint(p, { 255,0,0,255 });
		}

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
	


	void drawPoint(vec2d p, Color c) {
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		SDL_RenderDrawPoint(renderer, p.x, p.y);
	}
	void drawBigPoint(vec2d point,Color c) {
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		//SDL_RenderDrawPoint(renderer, point.x, point.y);
		int size = 3;
		for (int i = -(size-1); i < size; i++) {
			SDL_RenderDrawPoint(renderer, point.x + i, point.y);
			SDL_RenderDrawPoint(renderer, point.x, point.y+i);
		}
		for (int i = -(size-2); i < size-1; i++) {
			SDL_RenderDrawPoint(renderer, point.x + i, point.y + i);
		}
		for (int i = -(size - 2); i < size - 1; i++) {
			SDL_RenderDrawPoint(renderer, point.x - i, point.y - i);
		}
		
		
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
	
	Color changeColorDarknes(Color c, float lum) {
		int r = c.r * lum / 150.f;
		int g = c.g * lum / 150.f;
		int b = c.b * lum / 150.f;
	
		return {r,g,b,c.a};
	}

	void updateRotationMatrices(float time) {
		matRotZ.m[0][0] = cosf(time);
		matRotZ.m[0][1] = sinf(time);
		matRotZ.m[1][0] = -sinf(time);
		matRotZ.m[1][1] = cosf(time);
		matRotX.m[1][1] = cosf(time);
		matRotX.m[1][2] = sinf(time);
		matRotX.m[2][1] = -sinf(time);
		matRotX.m[2][2] = cosf(time);
	}

	

	//update the triangle rotation, according to the mesh rotation
	void updateTriFixedRotation(Mesh &mesh, triangle3d &tri) {
		mat4x4 FixedMatRotX, FixedMatRotZ, FixedMatRotY;
		
		FixedMatRotX.m[1][1] = cosf(mesh.rotation.x);
		FixedMatRotX.m[1][2] = sinf(mesh.rotation.x);
		FixedMatRotX.m[2][1] = -sinf(mesh.rotation.x);
		FixedMatRotX.m[2][2] = cosf(mesh.rotation.x);
	

		FixedMatRotZ.m[0][0] = cosf(mesh.rotation.z);
		FixedMatRotZ.m[0][1] = sinf(mesh.rotation.z);
		FixedMatRotZ.m[1][0] = -sinf(mesh.rotation.z);
		FixedMatRotZ.m[1][1] = cosf(mesh.rotation.z);
	
		 
		FixedMatRotY.m[0][0] = cosf(mesh.rotation.y);
		FixedMatRotY.m[2][0] = sinf(mesh.rotation.y);
		FixedMatRotY.m[0][2] = -sinf(mesh.rotation.y);
		FixedMatRotY.m[2][2] = cosf(mesh.rotation.y);

		FixedMatRotY.m[1][1] = FixedMatRotY.m[3][3] = FixedMatRotX.m[0][0] = FixedMatRotX.m[3][3]= FixedMatRotZ.m[2][2] = FixedMatRotZ.m[3][3] = 1;

		tri=rotateTri(tri, FixedMatRotX, FixedMatRotZ, FixedMatRotY);

	} 

	void updateTriPosition(Mesh& mesh, triangle3d& tri) { 
		for (int i = 0; i < 3; i++) {
			tri.p[i].x += mesh.position.x;
			tri.p[i].y += mesh.position.y;
			tri.p[i].z += mesh.position.z;
		}
	} //update the triangle position, according to the mesh position

	void renderMesh(int wireFrame,float time) {
		vector<ColoredTriangle> renderable;

		for (auto mesh : meshPool) {

			updateRotationMatrices(time); 

			for (auto tri : mesh.tris) {
				triangle3d triProjected, triTranslated ;

				//Update Rotation
				updateTriFixedRotation(mesh, tri);

				triTranslated = tri;
				updateTriPosition(mesh, triTranslated);

				
				//Wave
				if (mesh.water) {
					float f = 1;
					if (true) {
						for (int i = 0; i < 3; i++) {
							triTranslated.p[i].x += 0.2 * sinf((time + triTranslated.p[i].z) * f);
							triTranslated.p[i].y += 0.3 * sinf((time + triTranslated.p[i].x) * f * 2);
							triTranslated.p[i].z += 0.2 * sinf((time + triTranslated.p[i].y) * f);
						}
					}
					else {
					}
				}
				

				//normals 
				vec3d normal = getNormalVector(triTranslated);
				
				


				if ((normal.x * (triTranslated.p[0].x - vCamera.x) +
					normal.y * (triTranslated.p[0].y - vCamera.y) +
					normal.z * (triTranslated.p[0].z - vCamera.z) < 0.f)) {

						vec3d light_direction = { 0.0f,0.f,-1.f };
						normalizeVec(light_direction);
						float dp = dotProdutct(normal, light_direction);

						//calc lum
						int lum = (int)(150.f * dp);
						Color litghedColor = changeColorDarknes(mesh.color, lum);
						if (wireFrame%3 != 0) {
							litghedColor = mesh.color;
						}
						

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

						renderable.insert(renderable.end(), { triProjected,litghedColor });

				}

			}
		}
		sort(renderable.begin(), renderable.end(), [](ColoredTriangle &a, ColoredTriangle& b) {
			triangle3d t1 = a.tri;
			triangle3d t2 = b.tri;
			float z1 = (t1.p[0].z+ t1.p[1].z+ t1.p[2].z) / 3.f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.f;
			return z1 > z2;
			});
		for (auto tri : renderable) {

			switch (wireFrame % 3) {
			case 0:
				drawRasterizedTriangleSdl(tri.tri, tri.c);
				break;
			case 1:
				drawTriangle(tri.tri, tri.c);
				break;
			case 2:
				drawRasterizedTriangleSdl(tri.tri, tri.c);
				drawVertsTriangle(tri.tri, { 255,255,255,255 });
				break;
			}

			
		}
		
	}



	

};
