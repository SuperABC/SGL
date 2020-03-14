#include "winsgl.h"
#include <string>
#include <map>

#define TRIANGLE_OBJ 3
#define QUAD_OBJ 4

using namespace geo;
using namespace std;

int graphHandle;
int WIDTH = 768, HEIGHT = 512;
int lightNum = 5;

class Eye {
public:
	Eye() {
		this->pos = Vec3f(0, 2, 15);
		this->dir = normalize(Vec3f(0, -4, -12.5));
		this->up = Vec3f(0, 1, 0);
		this->theta = float(PI / 2);
		this->phy = float(PI / 2);
		this->dist = 1.f;
		this->model = Matrix<float>(4, 4);
	}
	~Eye() {}

	vec3f pos, dir, up, level;
	float theta, phy;
	float dist;

	Matrix<float> model;

	void move(float dx, float dy) {
		pos.x -= dy * cos(theta) + dx * sin(theta);
		pos.z -= dy * sin(theta) - dx * cos(theta);
	}
	void rotate(float angx, float angy) {
		float atx = pos.x + dir.x*dist;
		float aty = pos.y + dir.y*dist;
		float atz = pos.z + dir.z*dist;
		this->theta += angx;
		this->phy -= angy;
		if (this->phy < 0.01f)this->phy = 0.01f;
		if (this->phy > float(PI - 0.01))this->phy = float(PI - 0.01);
		this->pos.x = atx + dist * sin(phy)*cos(theta);
		this->pos.y = aty + dist * cos(phy);
		this->pos.z = atz + dist * sin(phy)*sin(theta);
		this->dir.x = (atx - this->pos.x) / dist;
		this->dir.y = (aty - this->pos.y) / dist;
		this->dir.z = (atz - this->pos.z) / dist;
		this->level = cross(this->dir, this->up);
	}
	void zoom(float prop) {
		float preDist = dist;
		dist *= prop;
		if (dist < .1f)dist = .1f;
		if (dist > 256.f)dist = 256.f;

		prop = dist / preDist;
		this->pos += dir * preDist * float(1 - prop);
	}
};
Eye eye;

struct Material {
	vec3f ka = Vec3f(0.f, 0.f, 0.f), kd = Vec3f(0.f, 0.f, 0.f), ks = Vec3f(0.f, 0.f, 0.f);
	float ni = 1.f, ns = 1.f;
};
class Model {
public:
	vector<float> pos, color, norm;

	std::map<string, Material> mats;
	vector<pair<string, pair<int, int>>> corespond;

	void pushPos(float p1, float p2, float p3) {
		pos.push_back(p1);
		pos.push_back(p2);
		pos.push_back(p3);
	}
	void pushColor(float c1, float c2, float c3) {
		color.push_back(c1);
		color.push_back(c2);
		color.push_back(c3);
	}
	void pushNorm(float n1, float n2, float n3) {
		norm.push_back(n1);
		norm.push_back(n2);
		norm.push_back(n3);
	}
	void load(const char *filename, int vertex) {
		std::ifstream fin;

		fin.open(filename);
		if (fin.is_open() == FALSE)return;

		string op;
		float num1, num2, num3, num4;

		vector<float>pos;
		vector<float>color;
		vector<float>normal;

		string mtlname = "";
		while (fin >> op) {
			if (op == "v") {
				fin >> num1 >> num2 >> num3;
				pos.push_back(num1);
				pos.push_back(num2);
				pos.push_back(num3);
			}
			else if (op == "vc") {
				fin >> num1 >> num2 >> num3;
				color.push_back(num1);
				color.push_back(num2);
				color.push_back(num3);
			}
			else if (op == "vn") {
				fin >> num1 >> num2 >> num3;
				normal.push_back(num1);
				normal.push_back(num2);
				normal.push_back(num3);
			}
			else if (op == "f") {
				if (vertex == QUAD_OBJ) {
					string v1, v2, v3, v4;
					int tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;

					fin >> v1 >> v2 >> v3 >> v4;
					num1 = (float)atoi(v1.c_str() + tmp1);
					num2 = (float)atoi(v2.c_str() + tmp2);
					num3 = (float)atoi(v3.c_str() + tmp3);
					num4 = (float)atoi(v4.c_str() + tmp4);
					this->pushPos(pos[(int)num1 * 3 - 3], pos[(int)num1 * 3 - 2], pos[(int)num1 * 3 - 1]);
					this->pushPos(pos[(int)num2 * 3 - 3], pos[(int)num2 * 3 - 2], pos[(int)num2 * 3 - 1]);
					this->pushPos(pos[(int)num3 * 3 - 3], pos[(int)num3 * 3 - 2], pos[(int)num3 * 3 - 1]);
					this->pushPos(pos[(int)num1 * 3 - 3], pos[(int)num1 * 3 - 2], pos[(int)num1 * 3 - 1]);
					this->pushPos(pos[(int)num3 * 3 - 3], pos[(int)num3 * 3 - 2], pos[(int)num3 * 3 - 1]);
					this->pushPos(pos[(int)num4 * 3 - 3], pos[(int)num4 * 3 - 2], pos[(int)num4 * 3 - 1]);

					tmp1 = v1.find_first_of('/', tmp1) + 1;
					tmp2 = v2.find_first_of('/', tmp2) + 1;
					tmp3 = v3.find_first_of('/', tmp3) + 1;
					tmp4 = v3.find_first_of('/', tmp4) + 1;
					if (color.size() == 0 || tmp1 == 0 || tmp2 == 0 || tmp3 == 0 || tmp4 == 0 ||
						v1[tmp1] == '/' || v2[tmp2] == '/' || v3[tmp3] == '/' || v4[tmp4] == '/') {
						this->pushColor(1, 1, 1);
						this->pushColor(1, 1, 1);
						this->pushColor(1, 1, 1);
						this->pushColor(1, 1, 1);
					}
					else {
						num1 = (float)atoi(v1.c_str() + tmp1);
						num2 = (float)atoi(v2.c_str() + tmp2);
						num3 = (float)atoi(v3.c_str() + tmp3);
						num4 = (float)atoi(v4.c_str() + tmp4);
						this->pushColor(color[(int)num1 * 3 - 3], color[(int)num1 * 3 - 2], color[(int)num1 * 3 - 1]);
						this->pushColor(color[(int)num2 * 3 - 3], color[(int)num2 * 3 - 2], color[(int)num2 * 3 - 1]);
						this->pushColor(color[(int)num3 * 3 - 3], color[(int)num3 * 3 - 2], color[(int)num3 * 3 - 1]);
						this->pushColor(color[(int)num1 * 3 - 3], color[(int)num1 * 3 - 2], color[(int)num1 * 3 - 1]);
						this->pushColor(color[(int)num3 * 3 - 3], color[(int)num3 * 3 - 2], color[(int)num3 * 3 - 1]);
						this->pushColor(color[(int)num4 * 3 - 3], color[(int)num4 * 3 - 2], color[(int)num4 * 3 - 1]);
					}

					tmp1 = v1.find_first_of('/', tmp1) + 1;
					tmp2 = v2.find_first_of('/', tmp2) + 1;
					tmp3 = v3.find_first_of('/', tmp3) + 1;
					tmp4 = v4.find_first_of('/', tmp4) + 1;
					if (normal.size() == 0 || tmp1 == 0 || tmp2 == 0 || tmp3 == 0 || tmp4 == 0) {
						int p1 = atoi(v1.c_str());
						int p2 = atoi(v2.c_str());
						int p3 = atoi(v3.c_str());
						int p4 = atoi(v4.c_str());
						vec3f edge1 = Vec3f(pos[p2 * 3 - 3] - pos[p1 * 3 - 3],
							pos[p2 * 3 - 2] - pos[p1 * 3 - 2],
							pos[p2 * 3 - 1] - pos[p1 * 3 - 1]);
						vec3f edge2 = Vec3f(pos[p3 * 3 - 3] - pos[p2 * 3 - 3],
							pos[p3 * 3 - 2] - pos[p2 * 3 - 2],
							pos[p3 * 3 - 1] - pos[p2 * 3 - 1]);
						vec3f norm = cross(edge1, edge2);
						this->pushNorm(norm.x, norm.y, norm.z);
						this->pushNorm(norm.x, norm.y, norm.z);
						this->pushNorm(norm.x, norm.y, norm.z);
						this->pushNorm(norm.x, norm.y, norm.z);
						this->pushNorm(norm.x, norm.y, norm.z);
						this->pushNorm(norm.x, norm.y, norm.z);
					}
					else {
						num1 = (float)atoi(v1.c_str() + tmp1);
						num2 = (float)atoi(v2.c_str() + tmp2);
						num3 = (float)atoi(v3.c_str() + tmp3);
						num4 = (float)atoi(v4.c_str() + tmp4);
						this->pushNorm(normal[(int)num1 * 3 - 3], normal[(int)num1 * 3 - 2], normal[(int)num1 * 3 - 1]);
						this->pushNorm(normal[(int)num2 * 3 - 3], normal[(int)num2 * 3 - 2], normal[(int)num2 * 3 - 1]);
						this->pushNorm(normal[(int)num3 * 3 - 3], normal[(int)num3 * 3 - 2], normal[(int)num3 * 3 - 1]);
						this->pushNorm(normal[(int)num1 * 3 - 3], normal[(int)num1 * 3 - 2], normal[(int)num1 * 3 - 1]);
						this->pushNorm(normal[(int)num3 * 3 - 3], normal[(int)num3 * 3 - 2], normal[(int)num3 * 3 - 1]);
						this->pushNorm(normal[(int)num4 * 3 - 3], normal[(int)num4 * 3 - 2], normal[(int)num4 * 3 - 1]);
					}
				}
				else if (vertex == TRIANGLE_OBJ) {
					string v1, v2, v3;
					int tmp1 = 0, tmp2 = 0, tmp3 = 0;

					fin >> v1 >> v2 >> v3;
					num1 = (float)atoi(v1.c_str() + tmp1);
					num2 = (float)atoi(v2.c_str() + tmp2);
					num3 = (float)atoi(v3.c_str() + tmp3);
					this->pushPos(pos[(int)num1 * 3 - 3], pos[(int)num1 * 3 - 2], pos[(int)num1 * 3 - 1]);
					this->pushPos(pos[(int)num2 * 3 - 3], pos[(int)num2 * 3 - 2], pos[(int)num2 * 3 - 1]);
					this->pushPos(pos[(int)num3 * 3 - 3], pos[(int)num3 * 3 - 2], pos[(int)num3 * 3 - 1]);

					tmp1 = v1.find_first_of('/', tmp1) + 1;
					tmp2 = v2.find_first_of('/', tmp2) + 1;
					tmp3 = v3.find_first_of('/', tmp3) + 1;
					if (color.size() == 0 || tmp1 == 0 || tmp2 == 0 || tmp3 == 0 ||
						v1[tmp1] == '/' || v2[tmp2] == '/' || v3[tmp3] == '/') {
						this->pushColor(1, 1, 1);
						this->pushColor(1, 1, 1);
						this->pushColor(1, 1, 1);
					}
					else {
						num1 = (float)atoi(v1.c_str() + tmp1);
						num2 = (float)atoi(v2.c_str() + tmp2);
						num3 = (float)atoi(v3.c_str() + tmp3);
						this->pushColor(color[(int)num1 * 3 - 3], color[(int)num1 * 3 - 2], color[(int)num1 * 3 - 1]);
						this->pushColor(color[(int)num2 * 3 - 3], color[(int)num2 * 3 - 2], color[(int)num2 * 3 - 1]);
						this->pushColor(color[(int)num3 * 3 - 3], color[(int)num3 * 3 - 2], color[(int)num3 * 3 - 1]);
					}

					tmp1 = v1.find_first_of('/', tmp1) + 1;
					tmp2 = v2.find_first_of('/', tmp2) + 1;
					tmp3 = v3.find_first_of('/', tmp3) + 1;
					if (normal.size() == 0 || tmp1 == 0 || tmp2 == 0 || tmp3 == 0) {
						int p1 = atoi(v1.c_str());
						int p2 = atoi(v2.c_str());
						int p3 = atoi(v3.c_str());
						vec3f edge1 = Vec3f(pos[p2 * 3 - 3] - pos[p1 * 3 - 3],
							pos[p2 * 3 - 2] - pos[p1 * 3 - 2],
							pos[p2 * 3 - 1] - pos[p1 * 3 - 1]);
						vec3f edge2 = Vec3f(pos[p3 * 3 - 3] - pos[p2 * 3 - 3],
							pos[p3 * 3 - 2] - pos[p2 * 3 - 2],
							pos[p3 * 3 - 1] - pos[p2 * 3 - 1]);
						vec3f norm = cross(edge1, edge2);
						this->pushNorm(norm.x, norm.y, norm.z);
						this->pushNorm(norm.x, norm.y, norm.z);
						this->pushNorm(norm.x, norm.y, norm.z);
					}
					else {
						num1 = (float)atoi(v1.c_str() + tmp1);
						num2 = (float)atoi(v2.c_str() + tmp2);
						num3 = (float)atoi(v3.c_str() + tmp3);
						this->pushNorm(normal[(int)num1 * 3 - 3], normal[(int)num1 * 3 - 2], normal[(int)num1 * 3 - 1]);
						this->pushNorm(normal[(int)num2 * 3 - 3], normal[(int)num2 * 3 - 2], normal[(int)num2 * 3 - 1]);
						this->pushNorm(normal[(int)num3 * 3 - 3], normal[(int)num3 * 3 - 2], normal[(int)num3 * 3 - 1]);
					}
				}
			}
			else if (op == "mtllib") {
				std::ifstream mtlin;
				string mtlfile;
				fin >> mtlfile;
				mtlin.open(mtlfile);
				if (mtlin.is_open() == FALSE)continue;

				string mtl;
				string name;
				while (mtlin >> mtl) {
					if (mtl == "newmtl") {
						mtlin >> name;
						mats[name] = Material();
					}
					else if (mtl == "Ka") {
						mtlin >> num1 >> num2 >> num3;
						mats[name].ka = Vec3f(num1, num2, num3);
					}
					else if (mtl == "Kd") {
						mtlin >> num1 >> num2 >> num3;
						mats[name].kd = Vec3f(num1, num2, num3);
					}
					else if (mtl == "Ks") {
						mtlin >> num1 >> num2 >> num3;
						mats[name].ks = Vec3f(num1, num2, num3);
					}
					else if (mtl == "Ni") {
						mtlin >> mats[name].ni;
					}
					else if (mtl == "Ns") {
						mtlin >> mats[name].ns;
					}
					else {
						char *buf = new char[256];
						mtlin.getline(buf, 256);
						delete buf;
					}
				}
			}
			else if (op == "usemtl") {
				if (mtlname != "") {
					corespond.back().second.second = this->pos.size() - corespond.back().second.first;
				}
				fin >> mtlname;
				corespond.push_back({ mtlname,{ this->pos.size(), this->pos.size() } });
			}
			else {
				char *buf = new char[256];
				fin.getline(buf, 256);
				delete buf;
			}
		}
		if (corespond.size() > 0)corespond.back().second.second = this->pos.size() - corespond.back().second.first;
	}
};
Model m, l;

struct Prd {
	vec3f start, dir;

	vec3f result = Vec3f(0.f, 0.f, 0.f);
	vec3f radiance = Vec3f(0.f, 0.f, 0.f);
	vec3f attenuation = Vec3f(1.f, 1.f, 1.f);
	int depth = 0;
	bool done = false;
};
struct Sprd {
	bool shadow = false;
};
vec3f light_sample(int idx, vec3f origin) {
	switch (idx) {
	case 0:
		return Vec3f(-3.75, 0.f, 0.f) + 0.03 * randHemi(normalize(origin - Vec3f(-3.75, 0.f, 0.f)));
	case 1:
		return Vec3f(3.75, 0.f, 0.f) + 0.9 * randHemi(normalize(origin - Vec3f(3.75, 0.f, 0.f)));
	case 2:
		return Vec3f(1.25, 0.f, 0.f) + 0.3 * randHemi(normalize(origin - Vec3f(1.25, 0.f, 0.f)));
	case 3:
		return Vec3f(-1.25, 0.f, 0.f) + 0.1 * randHemi(normalize(origin - Vec3f(-1.25, 0.f, 0.f)));
	case 4:
		return Vec3f(10.f, 10.f, 4.f) + 0.5 * randHemi(normalize(origin - Vec3f(10.f, 10.f, 4.f)));
	}
}
vec3f light_normal(int idx, vec3f p){
	switch (idx) {
	case 0:
		return normalize(p - Vec3f(-3.75, 0.f, 0.f));
	case 1:
		return normalize(p - Vec3f(3.75, 0.f, 0.f));
	case 2:
		return normalize(p - Vec3f(1.25, 0.f, 0.f));
	case 3:
		return normalize(p - Vec3f(-1.25, 0.f, 0.f));
	case 4:
		return normalize(p - Vec3f(10.f, 10.f, 4.f));
	}
}
vec3f light_radiance(int idx) {
	switch (idx) {
	case 0:
		return Vec3f(901.8f, 901.8f, 901.8f) * 2 * PI * 0.03 * 0.03;
	case 1:
		return Vec3f(1.24f, 1.24f, 1.24f) * 2 * PI * 0.9 * 0.9;
	case 2:
		return Vec3f(11.11f, 11.11f, 11.11f) * 2 * PI * 0.3 * 0.3;
	case 3:
		return Vec3f(100.f, 100.f, 100.f) * 2 * PI * 0.1 * 0.1;
	case 4:
		return Vec3f(800.f, 800.f, 800.f) * 2 * PI * 0.5 * 0.5;
	}
}

int frame = 1;
vec3f generate(int id, vec2i index, vec2i size) {
	vec2f move = Vec2f(index.x + (float)random(100) / 100 - .5f, index.y + (float)random(100) / 100 - .5f);
	vec2f pixel = Vec2f(2 * move.x / size.x - 1.f, 1.f - 2 * move.y / size.y);
	vec3f eye = *(vec3f *)getPipelineVariable(id, "eye");
	vec3f V = *(vec3f *)getPipelineVariable(id, "V");
	vec3f W = *(vec3f *)getPipelineVariable(id, "W");
	vec3f U = normalize(cross(V, -1 * W));
	V = normalize(cross(-1 * W, U));
	pixel *= tan(28.f / 360 * PI);
	vec3f dir = normalize(pixel.x * ((float)size.x / size.y) * U + pixel.y * V + W);
	Prd prd;
	prd.start = eye;
	prd.dir = dir;
	if (linePointDist(prd.start, prd.dir, Vec3f(-3.75, 0.f, 0.f)) <= 0.03f)return light_radiance(0);
	if (linePointDist(prd.start, prd.dir, Vec3f(3.75, 0.f, 0.f)) <= 0.9f)return light_radiance(1);
	if (linePointDist(prd.start, prd.dir, Vec3f(1.25, 0.f, 0.f)) <= 0.3f)return light_radiance(2);
	if (linePointDist(prd.start, prd.dir, Vec3f(-1.25, 0.f, 0.f)) <= 0.1f)return light_radiance(3);
	while (1) {
		rtTrace(id, 0, prd.start, prd.dir, LIGHT_RAY, .001f, INFINITY, &prd);
		if (prd.radiance.x < 0 || prd.radiance.y < 0 || prd.radiance.z < 0) {
			alertInfo("", ("(" + std::to_string(index.x) + ", " + std::to_string(index.y) + ")").data(), 0, NULL);
		}
		prd.result += prd.radiance;
		if (prd.done)break;
		if (prd.depth >= 1)break;
		float pcont = max(prd.attenuation.x, max(prd.attenuation.y, prd.attenuation.z));
		if (prd.depth > 3 && random(100) > 100 * pcont)break;
		if (pcont < .001f)break;
		prd.attenuation /= pcont;
	}
	vec3f result = prd.result;
	if (isnan(result.x) || isnan(result.y) || isnan(result.z))
		alertInfo("nan", (to_string(index.x) + "," + to_string(index.y)).data(), 0, NULL);
	vec3f old = getGraphPixel(id, index.x, index.y);
	prd.result = result / frame + old * (1 - 1.f / frame);
	return prd.result;
}
void miss(int id, void *prd) {
	Prd *perraydata = (Prd *)prd;
	perraydata->done = true;
}

float intersect(int id, void *pts, void *nms, vec3f point, vec3f dir, vec3f *norm) {
	if (norm == NULL) {
		float dist;
		if ((dist = linePointDist(point, dir, Vec3f(-3.75, 0.f, 0.f))) <= 0.03f) {
			return sqrt(square(Vec3f(-3.75, 0.f, 0.f) - point) - dist * dist);
		}
		if ((dist = linePointDist(point, dir, Vec3f(3.75, 0.f, 0.f))) <= 0.9f) {
			return sqrt(square(Vec3f(3.75, 0.f, 0.f) - point) - dist * dist);
		}
		if ((dist = linePointDist(point, dir, Vec3f(1.25, 0.f, 0.f))) <= 0.3f) {
			return sqrt(square(Vec3f(1.25, 0.f, 0.f) - point) - dist * dist);
		}
		if ((dist = linePointDist(point, dir, Vec3f(-1.25, 0.f, 0.f))) <= 0.1f) {
			return sqrt(square(Vec3f(-1.25, 0.f, 0.f) - point) - dist * dist);
		}
	}

	vector<vector<vec3f>> points = *(vector<vector<vec3f>> *)pts;
	vector<vector<vec3f>> norms = *(vector<vector<vec3f>> *)nms;
	float min = INFINITY;
	int pni = 0;
	for (auto &triangle : points) {
		auto n = norms[pni++];
		float beta, gamma;
		if (float t = intersectTriangle(triangle[0], triangle[1], triangle[2], point, dir, &beta, &gamma)) {
			if (t < 0.001f)continue;
			if (t < min) {
				min = t;
				if (norm)*norm = (1 - beta - gamma) * n[0] + beta * n[1] + gamma * n[2];
			}
		}
	}
	if (min == INFINITY || min < 0.f)return 0.f;
	else return min;
}
void hit(int id, float dist, void *prd, vec3f norm, void *param) {
	Material *mat = (Material *)param;
	Prd *perraydata = (Prd *)prd;
	perraydata->depth++;

	vec3f hitPoint = perraydata->start + dist * perraydata->dir;
	perraydata->start = hitPoint;

	if (!(mat->ka == Vec3f(0.f, 0.f, 0.f))) {
		if (perraydata->depth > 1) {
			perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
			return;
		}
		perraydata->done = true;
		perraydata->radiance = mat->ka;
	}
	else {
		if (mat->ks == Vec3f(0.f, 0.f, 0.f)) {
			perraydata->attenuation *= mat->kd;
			if (dot(perraydata->dir, norm) > 0)perraydata->dir = randHemi(-1 * norm);
			else perraydata->dir = randHemi(norm);

			perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
			for (int i = 0; i < lightNum; i++) {
				vec3f anchor = light_sample(i, hitPoint);
				vec3f ldir = normalize(anchor - hitPoint);
				float ldist = length(anchor - hitPoint);
				Sprd sprd;
				rtTrace(id, 0, hitPoint, ldir, SHADOW_RAY, .001f, ldist - .001f, &sprd);
				if (!sprd.shadow) {
					float ndl = abs(dot(norm, ldir));
					float ldl = abs(dot(light_normal(i, anchor), ldir));
					perraydata->radiance += perraydata->attenuation *
						ndl * ldl / (ldist * ldist) * light_radiance(i) / PI;
				}
			}
			perraydata->attenuation *= dot(norm, perraydata->dir) * 2;
		}
		else {
			if (random(1000) < 200) {
				perraydata->attenuation *= mat->kd * 5;
				if (dot(perraydata->dir, norm) > 0)perraydata->dir = randHemi(-1 * norm);
				else perraydata->dir = randHemi(norm);

				perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
				for (int i = 0; i < lightNum; i++) {
					vec3f anchor = light_sample(i, hitPoint);
					vec3f ldir = normalize(anchor - hitPoint);
					float ldist = length(anchor - hitPoint);
					Sprd sprd;
					rtTrace(id, 0, hitPoint, ldir, SHADOW_RAY, .001f, ldist - .001f, &sprd);
					if (!sprd.shadow) {
						float ndl = abs(dot(norm, ldir));
						float ldl = abs(dot(light_normal(i, anchor), ldir));
						perraydata->radiance += perraydata->attenuation *
							ndl * ldl / (ldist * ldist) * light_radiance(i) / PI;
					}
				}
				perraydata->attenuation *= dot(norm, perraydata->dir) * 2;
			}
			else {
				perraydata->attenuation *= mat->ks/2;
				vec3f wi = -1 * perraydata->dir;
				perraydata->dir = phoneSpec(norm, wi, mat->ns);

				perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
				for (int i = 0; i < lightNum; i++) {
					vec3f anchor = light_sample(i, hitPoint);
					vec3f ldir = normalize(anchor - hitPoint);
					float ldist = length(anchor - hitPoint);
					Sprd sprd;
					rtTrace(id, 0, hitPoint, ldir, SHADOW_RAY, .001f, ldist - .001f, &sprd);
					if (!sprd.shadow) {
						float ldl = abs(dot(light_normal(i, anchor), ldir));
						perraydata->radiance += perraydata->attenuation * ldl *
							mat->ns * pow(max(dot(ldir, glassSpec(norm, wi)), 0.f), mat->ns) /
							(ldist * ldist) * light_radiance(i);
					}
				}
				perraydata->attenuation *=
					mat->ns * pow(max(dot(perraydata->dir, glassSpec(norm, wi)), 0.f), mat->ns);
			}
		}
	}
}
void shadow(int id, void *prd) {
	Sprd *perraydata = (Sprd *)prd;
	perraydata->shadow = true;
}

void sgSetup() {
	initWindow(WIDTH, HEIGHT, "View", BIT_MAP);

	graphHandle = setGraphRegion(0, 0, WIDTH, HEIGHT);
	rtGenerate(graphHandle, generate);
	rtMiss(graphHandle, miss);

	setPipelineVariable(graphHandle, "eye", &eye.pos);
	setPipelineVariable(graphHandle, "V", &eye.up);
	setPipelineVariable(graphHandle, "W", &eye.dir);

	m.load("source/mis.obj", TRIANGLE_OBJ);
	for (auto i : m.corespond) {
		pushObject(graphHandle, m.pos.data() + i.second.first, m.norm.data() + i.second.first,
			i.second.second, 3, intersect, hit, shadow, &m.mats[i.first]);
	}
}
void sgLoop() {
	refreshTracer(graphHandle);
	frame++;
}
