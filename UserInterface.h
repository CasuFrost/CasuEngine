#include "WindowManager.h"
const Color COLOR_BLUE = { 0,0,255,255 };
const Color COLOR_RED = { 255,0,0,255 };
const Color COLOR_WHITE = { 255,255,255,255 };

class button {
	vec2d start;
	vec2d end;
	Color c;
	Color cOnButton;
	bool onButton=false;
	int id = -1;
	public:
	button(int x1,int y1,int x2,int y2, Color cN, Color cPressed,int identificator) {
			start = { x1,y1 };
			end = { x2,y2 };
			c = cN;
			cOnButton = cPressed;
			id = identificator;
		}
	button() {
		start = { 10,10 };
		end = { 20,20 };
		c = COLOR_WHITE;
		cOnButton = COLOR_RED;
	}
		void draw(WindowManager w) {
			Color tmp= cOnButton;
			if (!onButton) {
				tmp = c;
			}
			w.drawLine(start.x, start.y,end.x,start.y, tmp);
			w.drawLine(start.x, start.y, start.x, end.y, tmp);
			w.drawLine(end.x, start.y, end.x, end.y, tmp);
			w.drawLine(start.x, end.y, end.x, end.y, tmp);
		}
		bool checkInput(vec2d &mousePos, int& selected){
			SDL_Event e;
			
			int xMouse = 0;int yMouse=0;
			while (SDL_PollEvent(&e) != 0)
			{
				SDL_GetMouseState(&xMouse, &yMouse);
			}
			if (xMouse * yMouse != 0) {
				mousePos = { xMouse ,yMouse };
			}
			if ((mousePos.x >= start.x && mousePos.x <= end.x)
				&& (mousePos.y >= start.y && mousePos.y <= end.y)) {
				onButton = true;
			}
			else onButton = false;
			if (onButton) {
				selected = id;
			}
			return true;
		}
};
class UserInterface {
private:
	
	void handleInput(SDL_Event e) {
		
		
	}
	public:
		vec2d mousePos = { 0,0 };
		vector<button> buttonPool;
		
		UserInterface() {
			button b = button(1, 1, 55, 55, COLOR_BLUE, COLOR_BLUE, 0);
				
			button b1 = button(1, 56, 55, 111, COLOR_RED, COLOR_RED, 1);
			buttonPool.insert(buttonPool.end(), b);
			buttonPool.insert(buttonPool.end(), b1);
		}
		

		
		void display(WindowManager w, int &selected) {
			for (auto b : buttonPool) {
				b.checkInput(mousePos, selected);
			}
			
			for (auto b : buttonPool) {
				b.draw(w);
			}
		}
};
