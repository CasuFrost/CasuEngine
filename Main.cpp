#include <iostream>

#include "UserInterface.h"


using namespace std;

int main(int argc, char* argv[]) {
	WindowManager w;
	
	Triangle t = { {400,400}, { 450,450 }, { 400,250 } };
	//w.drawTriangle(t, c);
	bool wireFrame = false;
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
	//w.addMeshToPool(rect);
	vec3d cubeRotation = { 0.f,0.f,0.f };
	bool rotate = false;
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	cout << "Use WASD and key_up/ley_down for moving the cube in the space,\nuse the mouse wheel to rotate the cube.";
	cube.rotation = cubeRotation;
	cube.position = cubePosition;
	cube.color = COLOR_RED;
	w.addMeshToPool(cube);
	int selected = 0;
	UserInterface ui;
	int xMouse = 0; int yMouse = 0;
	while (!quit) {
		time +=0.01;	
		while (SDL_PollEvent(&e) != 0)
		{
			
			
			SDL_GetMouseState(&xMouse, &yMouse);
			if (xMouse * yMouse != 0) {
				mousePos = { xMouse ,yMouse };
			}
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				ui.checkInputButton(selected,w);
			}
			if (e.type== SDL_MOUSEWHEEL) {
				if (e.wheel.y > 0) // scroll up
				{
					w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x-0.1f,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z });
					cubeRotation.x -= 0.1;
				}
				else if (e.wheel.y < 0) // scroll down
				{
					w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x + 0.1f,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z });
					cubeRotation.x += 0.1;
				}
				if (e.wheel.x > 0) // scroll right
				{
					w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x ,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z-0.1f });
					cubeRotation.z -= 0.1;
				}
				else if (e.wheel.x < 0) // scroll left
				{
					w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x ,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z + 0.1f });
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
					w.updateMeshPosition(selected, { w.getMeshPosition(selected).x ,w.getMeshPosition(selected).y, w.getMeshPosition(selected).z - 0.04f });
				}
				
				if (e.key.keysym.sym == SDLK_UP) {
					w.updateMeshPosition(selected, { w.getMeshPosition(selected).x ,w.getMeshPosition(selected).y, w.getMeshPosition(selected).z + 0.04f });

				}
				if (e.key.keysym.sym == SDLK_d) {
					w.updateMeshPosition(selected, { w.getMeshPosition(selected).x + 0.04f ,w.getMeshPosition(selected).y, w.getMeshPosition(selected).z  });

				}
				if (e.key.keysym.sym == SDLK_a) {
					w.updateMeshPosition(selected, { w.getMeshPosition(selected).x - 0.04f ,w.getMeshPosition(selected).y, w.getMeshPosition(selected).z });

				}
				if (e.key.keysym.sym == SDLK_w) {
					w.updateMeshPosition(selected, { w.getMeshPosition(selected).x  ,w.getMeshPosition(selected).y-0.04f, w.getMeshPosition(selected).z });

				}
				if (e.key.keysym.sym == SDLK_s) {
					w.updateMeshPosition(selected, { w.getMeshPosition(selected).x  ,w.getMeshPosition(selected).y + 0.04f, w.getMeshPosition(selected).z });

				}
			}
		}
		
		vec3d vCamera = { 0,0,0 };
		//w.updateMeshPosition(selected, cubePosition);
		//w.updateMeshRotationDegrees(selected, cubeRotation);
		w.renderMesh( wireFrame, time);

		
		

		//SDL_Delay(10);
		ui.display(w, selected);
		
		w.endLoop();
	}
	
	
	return 0;
}