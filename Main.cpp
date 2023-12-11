#include <iostream>

#include "UserInterface.h"


using namespace std;

int main(int argc, char* argv[]) {
	WindowManager w;
	
	Triangle t = { {400,400}, { 450,450 }, { 400,250 } };
	//w.drawTriangle(t, c);
	int wireFrame = 0;
	meshGenerator meshGen;
	Mesh rect = meshGen.creatRect(1.f,0.1f);
	Mesh car;
	car.position = {0,0,13};
	car.LoadFromObjectFile("models/VideoShip.obj");
	


	Mesh pyramid = meshGen.createPyramid(1.0f);

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
	rect.color = COLOR_BLUE;
	//w.addMeshToPool(rect);
	
	bool rotate = false;
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	cout << "Use WASD and key_up/ley_down for moving the cube in the space,\nuse the mouse wheel to rotate the cube.";
	
	car.color = COLOR_RED;
	
	int selected = 0;
	UserInterface ui;
	int xMouse = 0; int yMouse = 0;

	
	gameObject newObj("a");
	newObj.position = { 0,0,15 };
	newObj.rotation = { 0,0,0 };
	car.rotation = car.position = { 0,0,0 };
	newObj.mesh = car;
	w.addObjToPool(newObj);
	

	
	//TEST
	/*triangle3d a;
	a.p[0] = { 0,0,0 };
	a.p[1] = { 0,1,0 };
	a.p[2] = { 2,2,0 };
	adjustTris(a);*/


	while (!quit) {
		//printMousePosition();

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
					w.updateObjRotationDegrees(selected, { w.getObjRotation(selected).x - 0.1f,w.getObjRotation(selected).y, w.getObjRotation(selected).z });
					//w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x-0.1f,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z });
				}
				else if (e.wheel.y < 0) // scroll down
				{
					w.updateObjRotationDegrees(selected, { w.getObjRotation(selected).x + 0.1f,w.getObjRotation(selected).y, w.getObjRotation(selected).z });
					//w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x + 0.1f,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z });
					
				}
				if (e.wheel.x > 0) // scroll right
				{
					//w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x ,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z-0.1f });
					w.updateObjRotationDegrees(selected, { w.getObjRotation(selected).x ,w.getObjRotation(selected).y, w.getObjRotation(selected).z - 0.1f });
					
				}
				else if (e.wheel.x < 0) // scroll left
				{
					w.updateObjRotationDegrees(selected, { w.getObjRotation(selected).x ,w.getObjRotation(selected).y, w.getObjRotation(selected).z + 0.1f });
					//w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x ,w.getMeshRotation(selected).y, w.getMeshRotation(selected).z + 0.1f });
					
				}
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_y) {
					w.updateObjRotationDegrees(selected, { w.getObjRotation(selected).x ,w.getObjRotation(selected).y + 0.1f, w.getObjRotation(selected).z });
					//w.updateMeshRotationDegrees(selected, { w.getMeshRotation(selected).x ,w.getMeshRotation(selected).y + 0.1f, w.getMeshRotation(selected).z  });
				}
				if (e.key.keysym.sym == SDLK_x) {
					w.updateObjRotationDegrees(selected, { w.getObjRotation(selected).x+0.1f ,w.getObjRotation(selected).y + 0.1f, w.getObjRotation(selected).z });
				}
				if (e.key.keysym.sym == SDLK_z) {
					w.updateObjRotationDegrees(selected, { w.getObjRotation(selected).x + 0.1f ,w.getObjRotation(selected).y , w.getObjRotation(selected).z+0.1f });
				}
				if (e.key.keysym.sym == SDLK_SPACE) {
					w.subsetMesh(selected);
					
				}
				if (e.key.keysym.sym == SDLK_q) {
					wireFrame += 1;
				}
				
				if (e.key.keysym.sym == SDLK_m) {
					w.makeWater(selected);
				}
				
				if (e.key.keysym.sym == SDLK_DOWN) {
					w.updateObjPosition(selected, { w.getObjPosition(selected).x ,w.getObjPosition(selected).y, w.getObjPosition(selected).z - 0.04f });
					//w.updateMeshPosition(selected, { w.getMeshPosition(selected).x ,w.getMeshPosition(selected).y, w.getMeshPosition(selected).z - 0.04f });
				}
				
				if (e.key.keysym.sym == SDLK_UP) {
					w.updateObjPosition(selected, { w.getObjPosition(selected).x ,w.getObjPosition(selected).y, w.getObjPosition(selected).z + 0.04f });
					//w.updateMeshPosition(selected, { w.getMeshPosition(selected).x ,w.getMeshPosition(selected).y, w.getMeshPosition(selected).z + 0.04f });

				}
				if (e.key.keysym.sym == SDLK_d) {
					w.updateObjPosition(selected, { w.getObjPosition(selected).x + 0.04f ,w.getObjPosition(selected).y, w.getObjPosition(selected).z });
					// w.updateMeshPosition(selected, { w.getMeshPosition(selected).x + 0.04f ,w.getMeshPosition(selected).y, w.getMeshPosition(selected).z  });

				}
				if (e.key.keysym.sym == SDLK_a) {
					w.updateObjPosition(selected, { w.getObjPosition(selected).x - 0.04f ,w.getObjPosition(selected).y, w.getObjPosition(selected).z });

				}
				if (e.key.keysym.sym == SDLK_w) {
					w.updateObjPosition(selected, { w.getObjPosition(selected).x  ,w.getObjPosition(selected).y-0.04f, w.getObjPosition(selected).z });

				}
				if (e.key.keysym.sym == SDLK_s) {
					w.updateObjPosition(selected, { w.getObjPosition(selected).x  ,w.getObjPosition(selected).y + 0.04f, w.getObjPosition(selected).z });

				}
			}
		}
		
		


		
		w.renderMesh( wireFrame, time);
		w.renderObject(wireFrame, time);

		//SDL_Delay(10);
		ui.display(w, selected);
		
		w.endLoop();
	}
	
	
	return 0;
}