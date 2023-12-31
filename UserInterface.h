#include "WindowManager.h"


class button {
	vec2d start;
	vec2d end;
	Color c;
	Color cOnButton;
	Color activeCol;
	bool onButton=false;
	int id = -1;
	public:
	bool meshButton=true;
	button(int x1,int y1,int x2,int y2, Color cN, Color cPressed,int identificator) {
			start = { x1,y1 };
			end = { x2,y2 };
			c = cN;
			cOnButton = cPressed;
			id = identificator;
			activeCol = cN;
		}
	button() {
		start = { 10,10 };
		end = { 20,20 };
		c = COLOR_WHITE;
		cOnButton = COLOR_RED;
		activeCol = c;
	}
	int getId() {
		return id;
	}
	bool posInButton(vec2d pos) {
		return (pos.x >= start.x && pos.x <= end.x)
			&& (pos.y >= start.y && pos.y <= end.y);
	}
		void draw(WindowManager w) {
			
			
			w.drawLine(start.x, start.y,end.x,start.y, activeCol);
			w.drawLine(start.x, start.y, start.x, end.y, activeCol);
			w.drawLine(end.x, start.y, end.x, end.y, activeCol);
			w.drawLine(start.x, end.y, end.x, end.y, activeCol);
			
		}
		

		
};



class UserInterface {
private:
	void handleInput(SDL_Event e) {
		
		
	}
	public:

		vector<button> buttonPool;

		UserInterface() {
			button add = button(1, 1 , 20, 20, COLOR_WHITE, COLOR_BLUE, -2);
			add.meshButton = false;

			button redRect = button(1, 21, 20, 39, COLOR_RED, COLOR_BLUE, 0);
			redRect.meshButton = true;
			
			buttonPool.insert(buttonPool.end(),add);
			buttonPool.insert(buttonPool.end(), redRect);
			
		}
		void display(WindowManager w, int &selected) {
			w.drawLine(1, 10, 20, 10, COLOR_WHITE);
			w.drawLine(10, 1, 10, 20, COLOR_WHITE);
			for (auto but : buttonPool) {
				but.draw(w);
			}
		}

		void checkInputButton(int& selected, WindowManager &w) {
			for (auto but : buttonPool) {
				if (but.posInButton(mousePos)) {
					if (but.getId() == -2) {

						int size= w.getObjPoolSize();
						Color k;
						switch (size % 7) {
						case 0:
							k = COLOR_GREEN;
							break;
						case 1 :
							k = COLOR_BLUE;
							break;
						case 2:
							k = COLOR_GREEN;
							break;
						case 3:
							k = COLOR_PURPLE;
							break;
						case 4:
							k = COLOR_YELLOW;
							break;
						case 5:
							k = COLOR_RED;
							break;
						case 6:
							k = COLOR_CELESTE;
							break;
						}
						
						
						button newbut = button(0, 19 *(size+1)+2, 19, 19 * (size + 1)+ 21, k, k, size);
						newbut.meshButton = true;
						buttonPool.insert(buttonPool.end(), newbut);
						
						meshGenerator m;
						Mesh newMesh = m.creatCube(0.5f);
						newMesh.color = k;
						gameObject g("");
						g.mesh = newMesh;
						g.position = {0,0,3}; g.rotation = { 0,0,0 };
						w.addObjToPool(g);
						
						break;
					}

					if (but.meshButton) {
						selected = but.getId();
						break;
					}
					
				}
			}
		}
};
