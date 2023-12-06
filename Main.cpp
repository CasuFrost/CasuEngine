#include <iostream>

#include "WindowManager.h"

#include <math.h>
using namespace std;

int main(int argc, char* argv[]) {
	WindowManager w;
	Color c = {255,0,0,255};
	Triangle t = { {400,400}, { 450,450 }, { 400,250 } };
	//w.drawTriangle(t, c);

	meshGenerator meshGen;
	mesh cube = meshGen.creatCube(1.0f);

	//Main loop flag
	bool quit = false;
	//Event handler
	SDL_Event e;
	float time = 0;
	
	
	while (!quit) {
		
		time +=0.01;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		//Draw some triangles
		for (auto tri : cube.tris) {
			
		}
		w.drawSquare({ 44,44 }, 10, c);


		w.endLoop();
	}


	
	int h;
	cin>>h;
	return 0;
}