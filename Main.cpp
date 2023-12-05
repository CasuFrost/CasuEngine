#include <iostream>
#include <vector>
#include <SDL.h>
#include "WindowManager.h"
#include "Strutture.h"
#include <math.h>
using namespace std;

int main(int argc, char* argv[]) {
	WindowManager w;
	Color c = {255,0,0,255};
	Triangle t = { {400,400}, { 450,450 }, { 400,250 } };
	//w.drawTriangle(t, c);
	


	//Main loop flag
	bool quit = false;
	//Event handler
	SDL_Event e;
	float time = 0;
	
	int x = 200;int y = 200;
	while (!quit) {
		w.drawSquare({ x,y }, 20, c);
		time +=0.01;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		x += 1;
		y += 1;

		w.endLoop();
	}


	
	
	int h;
	cin>>h;
	return 0;
}