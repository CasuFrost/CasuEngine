#include <iostream>

#include "UserInterface.h"


using namespace std;

int main(int argc, char* argv[]) {
	WindowManager w;
	
	Triangle t = { {400,400}, { 450,450 }, { 400,250 } };
	//w.drawTriangle(t, c);
	bool wireFrame = true;
	meshGenerator meshGen;
	mesh rect = meshGen.creatRect(1.f,0.1f);
	mesh cube = meshGen.creatCube(0.5f);

	mesh pyramid = meshGen.createPyramid(1.0f);

	//Main loop flag
	bool quit = false;
	//Event handler
	SDL_Event e;
	float time = 0;
	mat4x4 matRotX, matRotZ;
	
	vec3d cubePosition = { 0.f,0.f,3.f };
	float dirX = 1.f;
	float dirY = 1.f;
	float dirZ = 1.f;
	float speed = 0;
	rect.color = COLOR_BLUE;
	w.addMeshToPool(rect);
	vec3d cubeRotation = { 0.f,0.f,0.f };
	bool rotate = false;
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	cube.rotation = cubeRotation;
	cube.position = cubePosition;
	cube.color = COLOR_RED;
	w.addMeshToPool(cube);
	int selected = 0;
	UserInterface ui;
	while (!quit) {
		time +=0.01;
		
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type== SDL_MOUSEWHEEL) {
				if (e.wheel.y > 0) // scroll up
				{
					cubeRotation.x -= 0.1;
				}
				else if (e.wheel.y < 0) // scroll down
				{
					cubeRotation.x += 0.1;
				}
				if (e.wheel.x > 0) // scroll right
				{
					cubeRotation.z -= 0.1;
				}
				else if (e.wheel.x < 0) // scroll left
				{
					cubeRotation.z += 0.1;
				}
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_SPACE) {
					w.subsetMesh(selected);
					
				}
				if (e.key.keysym.sym == SDLK_q) {
					wireFrame = abs(wireFrame - 1);
				}
				if (e.key.keysym.sym == SDLK_m) {
					w.makeWater(selected);
				}
				
				if (e.key.keysym.sym == SDLK_DOWN) {
					cubePosition.z -= 0.04;

				}
				
				if (e.key.keysym.sym == SDLK_UP) {
					cubePosition.z += 0.04;

				}
				if (e.key.keysym.sym == SDLK_d) {
					cubePosition.x += 0.04;

				}
				if (e.key.keysym.sym == SDLK_a) {
					cubePosition.x -= 0.04;

				}
				if (e.key.keysym.sym == SDLK_w) {
					cubePosition.y -= 0.04;

				}
				if (e.key.keysym.sym == SDLK_s) {
					cubePosition.y += 0.04;

				}
			}
		}

		vec3d vCamera = { 0,0,0 };
		w.updateMeshPosition(selected, cubePosition);
		w.updateMeshRotationDegrees(selected, cubeRotation);
		w.renderMesh( wireFrame, time);

		
		

		//SDL_Delay(10);
		ui.display(w, selected);
		
		w.endLoop();
	}
	
	
	return 0;
}