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
	 int ord = 1;
public:
	
	vector<int> getHypotenuse(triangle3d tri) { //This function get a triangle and returns de index of the 2 points of the hypotenuse
		float dist01 = sqrtf(powf((tri.p[1].x - tri.p[0].x),2)+ powf((tri.p[1].y - tri.p[0].y), 2)
			+ powf((tri.p[1].z - tri.p[0].z), 2));
		float dist02 = sqrtf(powf((tri.p[2].x - tri.p[0].x), 2) + powf((tri.p[2].y - tri.p[0].y), 2)
			+ powf((tri.p[2].z - tri.p[0].z), 2));
		float dist12 = sqrtf(powf((tri.p[2].x - tri.p[1].x), 2) + powf((tri.p[2].y - tri.p[1].y), 2)
			+ powf((tri.p[2].z - tri.p[1].z), 2));
		vector<int> v;
		if ((dist01>dist02) && (dist01 > dist12)) {
			v.push_back(0);
			v.push_back(1);
			v.push_back(2);
			ord = 1;
		}
		if ((dist12 > dist02) && (dist12 > dist01)) {
			v.push_back(1);
			v.push_back(2);
			v.push_back(0);
			ord = 2;
		}
		if ((dist02 > dist01) && (dist02 > dist12)) {
			v.push_back(0);
			v.push_back(2);
			v.push_back(1);
			ord = 3;
		}
		return v;
	}



	static vec3d midPointVector3D(vec3d a,vec3d b){
		vec3d mid;
		mid = { (a.x + b.x),(a.y + b.y),a.z + b.z };
		mid.x *= 0.5f;
		mid.y *= 0.5f;
		mid.z *= 0.5f;
		return mid;
	}

	/*
	Quando prendo un triangolo e creo i due nuovi triangoli, per selezionare il lato dell'ipotenusa come 
	dalla quale selezionare il midpoint, inverto l'ordine dei vertici del triangolo, ciò causa un problema nel momento 
	in cui necessito di calcolare il vettore normale, in quanto per i triangoli che hanno i vertici ordinati diversamente, la 
	normale risulta inversa, quindi ho bisogno di tener conto dell'ordine, ed in base a come ho selezionato i vertici, disporli diversamente.
	*/
	vector<triangle3d> getOrderedTriangle(triangle3d tri, vec3d mid,vector<int> hypVertices) {
		triangle3d tri1;
		triangle3d tri2;
		switch (ord)
		{
		case 1:
			tri1 = { tri.p[hypVertices[2]],tri.p[hypVertices[0]],mid };
			tri2 = { mid,tri.p[hypVertices[1]],tri.p[hypVertices[2]] };
			break;
		case 2:
			tri1 = { mid,tri.p[hypVertices[1]],tri.p[hypVertices[2]] };
			tri2 = { tri.p[hypVertices[2]],tri.p[hypVertices[0]] ,mid };
			break;
		case 3:
			tri1 = { mid,tri.p[hypVertices[0]],tri.p[hypVertices[2]] };
			tri2 = { tri.p[hypVertices[2]],tri.p[hypVertices[1]] ,mid };
			break;
		default:
			break;
		}
		return { tri1,tri2 };
	}

	 void subsectMesh(mesh &o) {
		vector<triangle3d> newTris;
		for (auto tri : o.tris) {
			vector<int> hypVertices = getHypotenuse(tri);
			//Divido il triangolo creado il "midPoint" nel lato dell'ipotenusa

			vec3d mid = midPointVector3D(tri.p[hypVertices[0]], tri.p[hypVertices[1]]);//divide tri
			vector<triangle3d> newTriangles= getOrderedTriangle(tri, mid, hypVertices);
			
			
			
			newTris.insert(newTris.end(), newTriangles[0]);
			newTris.insert(newTris.end(), newTriangles[1]);
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
