#include <vector>
#include <math.h>
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
struct mat4x4 {
	float m[4][4] = { 0 };
};
static class meshGenerator {
private:
public:
	static vec3d midPointVector3D(vec3d a,vec3d b){
		vec3d mid;
		mid = { (a.x + b.x),(a.y + b.y),a.z + b.z };
		mid.x *= 0.5f;
		mid.y *= 0.5f;
		mid.z *= 0.5f;
		return mid;
	}
	static void subsectMesh(mesh &o) {
		vector<triangle3d> newTris;
		for (auto tri : o.tris) {
			vec3d mid = midPointVector3D(tri.p[0], tri.p[1]);//divide tri
			triangle3d tri1 = { tri.p[0],mid,tri.p[2] };
			triangle3d tri2 = { tri.p[1],mid,tri.p[2] };
			newTris.insert(newTris.end(), tri1);
			newTris.insert(newTris.end(), tri2);
		}
		o.tris = newTris;
	}
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
			{ 0.0f, 0.0f, size,    0.f, size, size,    0.0f, size, 0.0f },
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
	static mesh createPyramid(float size) {
		mesh meshPyr;
		meshPyr.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    size, 0.0f, 0.0f,    size, 0.f, size },
			{ 0.0f, 0.0f, 0.0f,    0.f, 0.f, size,     size, 0.f, size },

			{ 0.0f, 0.0f, 0.0f,    0.f, 0.f, size,    size*0.5f, size, size * 0.5f },

			{ 0.0f, 0.0f, 0.0f,    size, 0.f, 0.f,    size * 0.5f, size, size * 0.5f },
			{ size, 0.f, 0.f,    size, 0.f, size,    size * 0.5f, size, size * 0.5f },
			{ 0.0f, 0.0f, size,    size, 0.f, size,    size * 0.5f, size, size * 0.5f },
			

		};

		return meshPyr;
	}
	
};
