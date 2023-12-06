#pragma once
#include "Strutture.h"
using namespace std;
static class meshGenerator {
public:
	static mesh creatCube(float size) {
		mesh meshCube;
		meshCube.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, size, 0.0f,    size, size, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    size, size, 0.0f,    size, 0.0f, 0.0f },

			// EAST                                                      
			{ size, 0.0f, 0.0f,    size, size, 0.0f,    size, size, size },
			{ size, 0.0f, 0.0f,    size, size, size,    size, 0.0f, size },
										 
			// NORTH                                                
			{ size, 0.0f, size,    size, size, size,    0.0f, size, size },
			{ size, 0.0f, size,    0.0f, size, size,    0.0f, 0.0f, size },
										 
			// WEST                                                  
			{ 0.0f, 0.0f, size,    0.0f, size, size,    0.0f, size, 0.0f },
			{ 0.0f, 0.0f, size,    0.0f, size, 0.0f,    0.0f, 0.0f, 0.0f },
										 
			// TOP                                                   
			{ 0.0f, size, 0.0f,    0.0f, size, size,    size, size, size },
			{ 0.0f, size, 0.0f,    size, size, size,    size, size, 0.0f },

			// BOTTOM                                                    
			{ size, 0.0f, size,    0.0f, 0.0f, size,    0.0f, 0.0f, 0.0f },
			{ size, 0.0f, size,    0.0f, 0.0f, 0.0f,    size, 0.0f, 0.0f },

		};
		return meshCube;
	}
};