#include <vector>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <strstream>
using namespace std;

struct Color {
	int r;
	int g;
	int b;
	int a;
};

const Color COLOR_BLUE = { 0,0,255,255 };
const Color COLOR_RED = { 255,0,0,255 };
const Color COLOR_GREEN = { 0,255,0,255 };
const Color COLOR_WHITE = { 255,255,255,255 };
const Color COLOR_PURPLE = { 150,0,255,255 };
const Color COLOR_YELLOW = { 255,150,0,255 };
const Color COLOR_CELESTE = { 0,255,255,255 };

struct vec2d {
	int x;
	int y;
};

struct vec3d {
	float x,y,z;
	
};

static vec2d mousePos;

struct Triangle {
	vec2d p1;
	vec2d p2;
	vec2d p3;
};

struct triangle3d {
	vec3d p[3];
};

struct ColoredTriangle {
	triangle3d tri;
	Color c;
};
struct Mesh {
	vector<triangle3d> tris;
	Color color;
	vec3d position = { 0,0,0 };
	vec3d rotation = { 0,0,0 };
	bool water = false;

	bool LoadFromObjectFile(string sFilename)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		vector<vec3d> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}

		return true;
	}

};

struct mat4x4 {
	float m[4][4] = { 0 };
};
static void printMousePosition() {
	cout << "\nx : " << mousePos.x << " y : " << mousePos.y;
}
static float fgetMax(vector<float> v) {
	float max = v[0];
	for (auto f : v) {
		if (f > max)max = f;
	}
	return max;
}

static float fgetMin(vector<float> v) {
	float min = v[0];
	for (auto f : v) {
		if (f < min)min = f;
	}
	return min;
}

static vec3d crossProduct(vec3d a, vec3d b) {
	vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
	return result;
}

static vec3d crossProduct(vec2d v1, vec2d v2) {
	vec3d a = { v1.x,v1.y,0 };
	vec3d b = { v2.x,v2.y,0 };
	vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
	return result;
}

static vec3d crossProduct(vec3d a, vec2d v2) {
	vec3d b = { v2.x,v2.y,0 };
	vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
	return result;
}

static vec3d crossProduct(vec2d v1, vec3d b) {
	vec3d a = { v1.x,v1.y,0 };
	vec3d result = { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
	return result;
}

static float edge_cross(vec2d a, vec2d b, vec2d p) { //return the z value of the cross product from 2 specified 3D vector
	vec2d ab = { b.x - a.x,b.y - a.y };
	vec2d ap = { p.x - a.x,p.y - a.y };
	return crossProduct(ab, ap).z;
}

static bool pointIsInTriangle(triangle3d tri, vec2d p, int b0, int b1, int b2) { //Check if a point is in a triangle
	vec2d v0 = { tri.p[0].x,tri.p[0].y };
	vec2d v1 = { tri.p[1].x,tri.p[1].y };
	vec2d v2 = { tri.p[2].x,tri.p[2].y };
	float w_0 = edge_cross(v1, v2, p) + b0;
	float w_1 = edge_cross(v2, v0, p) + b1;
	float w_2 = edge_cross(v0, v1, p) + b2;

	return w_0 >= 0 && w_1 >= 0 && w_2 >= 0;
}

static vec3d normalizeVec(vec3d normal) {
	float normalLenght = sqrtf(powf(normal.x, 2) + powf(normal.y, 2) + powf(normal.z, 2));
	return { normal.x /= normalLenght, normal.y /= normalLenght, normal.z /= normalLenght };
}

static void MultiplyMatVec(vec3d& in, vec3d& out, mat4x4& m) { //Moltiplica matrice e vettore
	out.x = in.x * m.m[0][0] + in.y * m.m[1][0] + in.z * m.m[2][0] + m.m[3][0];
	out.y = in.x * m.m[0][1] + in.y * m.m[1][1] + in.z * m.m[2][1] + m.m[3][1];
	out.z = in.x * m.m[0][2] + in.y * m.m[1][2] + in.z * m.m[2][2] + m.m[3][2];
	float w = in.x * m.m[0][3] + in.y * m.m[1][3] + in.z * m.m[2][3] + m.m[3][3];
	if (w != 0) {
		out.x /= w;
		out.y /= w;
		out.z /= w;
	}
}

static mat4x4 matrixMoltiplication(mat4x4 a, mat4x4 b) {
	mat4x4 c;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int sum = 0;
			for (int k = 0; k < 4; k++) {
				sum += a.m[i][k] * b.m[k][j];
			}
			c.m[i][j] = sum;
		}
	}
	return c;
}

static void multiplyMatTri(triangle3d in, triangle3d& o, mat4x4 mat) {
	for (int i = 0; i < 3; i++) {
		MultiplyMatVec(in.p[i], o.p[i], mat);
	}
}

static triangle3d rotateTri(triangle3d& tri, mat4x4 FixedMatRotX, mat4x4 FixedMatRotY, mat4x4 FixedMatRotZ) {
	triangle3d triFixedRotatedZ, triFixedRotatedX, triFixedRotatedY;
	multiplyMatTri(tri, triFixedRotatedX, FixedMatRotX);
	multiplyMatTri(triFixedRotatedX, triFixedRotatedZ, FixedMatRotZ);
	multiplyMatTri(triFixedRotatedZ, triFixedRotatedY, FixedMatRotY);
	return triFixedRotatedY;
}

static vec3d getNormalVector(triangle3d tri) {
	vec3d normal, line1, line2;
	line1.x = tri.p[1].x - tri.p[0].x;
	line1.y = tri.p[1].y - tri.p[0].y;
	line1.z = tri.p[1].z - tri.p[0].z;
	line2.x = tri.p[2].x - tri.p[0].x;
	line2.y = tri.p[2].y - tri.p[0].y;
	line2.z = tri.p[2].z - tri.p[0].z;
	normal = crossProduct(line1, line2);
	return normalizeVec(normal);
}

class Object {
	public:
		string name;
		vec3d position;
		vec3d rotation;
		Mesh mesh;
		Object();
		Object(string n) {
			name = n;
		}
		bool physics;
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
			return v;
		}
		if ((dist12 > dist02) && (dist12 > dist01)) {
			v.push_back(1);
			v.push_back(2);
			v.push_back(0);
			ord = 2;
			return v;
		}
		if ((dist02 > dist01) && (dist02 > dist12)) {
			v.push_back(0);
			v.push_back(2);
			v.push_back(1);
			ord = 3;
			return v;
		}
		v.push_back(0);
		v.push_back(1);
		v.push_back(2);
		ord = 1;
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

	 void subsectMesh(Mesh &o) {
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
	 static Mesh creatRect(float w,float h) {
		 Mesh meshRect;
		 float start = w * -0.5f;
		 float start2 = h * -0.5f;
		 meshRect.tris = {

			 // SOUTH
			 { start, start2, start,		start, h, start,    w, h, start },
			{ start, start2, start,		w, h, start,		 w, start2,start },

			// EAST                                                      
			{ w, start2, start,    w, h, start,    w, h, w },
			{ w, start2, start,    w, h, w,     w, start2, w },

			// NORTH                                                
			{ w, start2, w,    w, h, w,    start, h, w },
			{ w, start2, w,    start, h, w,    start, start2, w },

			// WEST                                                  
			{ start, start2, w,    start, h, w,		   start, h, start },
			{ start, start2, w,    start, h, start,    start, start2, start },

			// TOP                                                   
			{ start, h, start,    start, h, w,    w, h, w },
			{ start, h, start,    w, h, w,     w, h, start },

			// BOTTOM                                                    
			{ w, start2, w,    start, start2, w,    start, start2, start },
			{ w, start2, w,    start, start2, start,    w, start2, start },

		 };
		 meshRect.position = { 0,0,3 };
		 return meshRect;
	 }
	static Mesh creatCube(float size) {
		Mesh meshCube;
		float start =size * -0.5f;
		meshCube.tris = {
			                                                                                                                                                                                                          
			// SOUTH
			{ start, start, start,    start, size, start,    size, size, start },
			{ start, start, start,    size, size, start,    size, start,start },

			// EAST                                                      
			{ size, start, start,    size, size, start,    size, size, size },
			{ size, start, start,    size, size, size,    size, start, size },

			// NORTH                                                
			{ size, start, size,    size, size, size,    start, size, size },
			{ size, start, size,    start, size, size,    start, start, size },

			// WEST                                                  
			{ start, start, size,    start, size, size,    start, size, start },
			{ start, start, size,    start, size, start,    start, start, start },

			// TOP                                                   
			{ start, size, start,    start, size, size,    size, size, size },
			{ start, size, start,    size, size, size,    size, size, start },

			// BOTTOM                                                    
			{ size, start, size,    start, start, size,    start, start, start },
			{ size, start, size,    start, start, start,    size, start, start },

		};
		meshCube.position = { 0,0,3 };
		return meshCube;
	}
	static Mesh createPyramid(float size) {
		Mesh meshPyr;
		meshPyr.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    size, 0.0f, 0.0f,    size, 0.f, size },
			{ 0.0f, 0.0f, 0.0f,    0.f, 0.f, size,     size, 0.f, size },

			{ 0.0f, 0.0f, 0.0f,    0.f, 0.f, size,    size*0.5f, size, size * 0.5f },

			{ 0.0f, 0.0f, 0.0f,    size, 0.f, 0.f,    size * 0.5f, size, size * 0.5f },
			{ size, 0.f, 0.f,    size, 0.f, size,    size * 0.5f, size, size * 0.5f },
			{ 0.0f, 0.0f, size,    size, 0.f, size,    size * 0.5f, size, size * 0.5f },
			

		};
		meshPyr.position = { 0,0,3 };
		return meshPyr;
	}
	
};
