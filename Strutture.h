#include <vector>
using namespace std;
struct Color {
	int r;
	int g;
	int b;
	int a;
};
struct vec2d {
	int x;
	int y;
};
struct vec3d {
	float x,y,z;
	
};
struct Triangle {
	vec2d p1;
	vec2d p2;
	vec2d p3;
};
struct triangle3d {
	vec3d p[3];
};
struct mesh {
	vector<triangle3d> tris;
};
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
