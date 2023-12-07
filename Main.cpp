#include <iostream>

#include "WindowManager.h"


using namespace std;

int main(int argc, char* argv[]) {
	WindowManager w;
	Color c = {255,0,0,255};
	Triangle t = { {400,400}, { 450,450 }, { 400,250 } };
	//w.drawTriangle(t, c);
	
	meshGenerator meshGen;
	mesh cube = meshGen.creatCube(1.0f);
	mesh pyramid = meshGen.createPyramid(1.0f);

	//Main loop flag
	bool quit = false;
	//Event handler
	SDL_Event e;
	float time = 0;
	mat4x4 matRotX, matRotZ;
	
	



	
	
	
		float dirX = 1.f;
		float dirY = 1.f;
		float dirZ = 1.f;
		float speed = 0;
	while (!quit) {
		
		time +=0.01;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_SPACE) {
					meshGen.subsectMesh(cube);
				}
			}
		}
		
		matRotZ.m[0][0] = cosf(time);
		matRotZ.m[0][1] = sinf(time);
		matRotZ.m[1][0] = -sinf(time);
		matRotZ.m[1][1] = cosf(time);
		matRotZ.m[2][2] = matRotZ.m[3][3] = 1;


		matRotX.m[1][1] = cosf(time * 0.5f);
		matRotX.m[1][2] = sinf(time * 0.5f);
		matRotX.m[2][1] = -sinf(time * 0.5f);
		matRotX.m[2][2] = cosf(time * 0.5f);
		matRotX.m[0][0] = matRotX.m[3][3] = 1;




		//Draw some triangles
		for (auto tri : cube.tris) {
			triangle3d triProjected, triTranslated,triRotatedZ,triRotatedX;

			w.MultiplyMatVec(tri.p[0], triRotatedZ.p[0], matRotZ);//Proietta un singolo triangolo
			w.MultiplyMatVec(tri.p[1], triRotatedZ.p[1], matRotZ);
			w.MultiplyMatVec(tri.p[2], triRotatedZ.p[2], matRotZ);

			w.MultiplyMatVec(triRotatedZ.p[0], triRotatedX.p[0], matRotX);//Proietta un singolo triangolo
			w.MultiplyMatVec(triRotatedZ.p[1], triRotatedX.p[1], matRotX);
			w.MultiplyMatVec(triRotatedZ.p[2], triRotatedX.p[2], matRotX);





			triTranslated = triRotatedX;
			triTranslated.p[0].z = triRotatedX.p[0].z +3.f;
			triTranslated.p[1].z = triRotatedX.p[1].z + 3.f;
			triTranslated.p[2].z = triRotatedX.p[2].z + 3.f;
			
			
			triTranslated.p[0].z += (time * speed);
			triTranslated.p[1].z += (time * speed);
			triTranslated.p[2].z += (time * speed);
			
			


			w.MultiplyMatVec(triTranslated.p[0], triProjected.p[0], w.projMatrix);//Proietta un singolo triangolo
			w.MultiplyMatVec(triTranslated.p[1], triProjected.p[1], w.projMatrix);
			w.MultiplyMatVec(triTranslated.p[2], triProjected.p[2], w.projMatrix);
			

			//La matrice di proiezione restituisce un risultato in uno schermo normalizzato da -1 ad +1. Va scalato !
			triProjected.p[0].x += 1.f; triProjected.p[0].y += 1.f;
			triProjected.p[1].x += 1.f; triProjected.p[1].y += 1.f;
			triProjected.p[2].x += 1.f; triProjected.p[2].y += 1.f;

			triProjected.p[0].x *= 0.5 * (float)w.getScreenWidth(); triProjected.p[0].y *= 0.5 * (float)w.getScreenWidth();
			triProjected.p[1].x *= 0.5 * (float)w.getScreenWidth(); triProjected.p[1].y *= 0.5 * (float)w.getScreenWidth();
			triProjected.p[2].x *= 0.5 * (float)w.getScreenWidth(); triProjected.p[2].y *= 0.5 * (float)w.getScreenWidth();

			w.drawTriangle(triProjected, c);
		}
		


		w.endLoop();
	}
	
	
	return 0;
}