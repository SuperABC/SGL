#include "winsgl.h"
#include <string>
#include <map>

#define CBOX
//#define DINNINGROOM
//#define MIS

using namespace geo;
using namespace std;

int graphHandle;
#ifdef CBOX
int WIDTH = 512, HEIGHT = 512;
#endif

class Eye {
public:
	Eye() {
#ifdef CBOX
		this->pos = Vec3f(278, 273, -800);
		this->dir = normalize(Vec3f(0, 0, 1));
		this->theta = float(3 * PI / 2);
#endif
		this->up = Vec3f(0, 1, 0);
		this->level = Vec3f(1, 0, 0);
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
	void load(const char *filename) {
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
				if (norm.size() == 0 || tmp1 == 0 || tmp2 == 0 || tmp3 == 0 || tmp4 == 0) {
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
				corespond.push_back({ mtlname, { this->pos.size(), this->pos.size() } });
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
vec3f light_cbox() {
	vec3f anchor = Vec3f(213.f, 547.9f, 227.f);
	vec3f v1 = Vec3f(130.f, 0.f, 0.f);
	vec3f v2 = Vec3f(0.f, 0.f, 105.f);
	float r1 = random(100) / 100.f, r2 = random(100) / 100.f;
	anchor += v1 * r1 + v2 * r2;
	return anchor;
}

int frame = 1;
vec3i generate(int id, vec2i index, vec2i size) {
	vec2f move = Vec2f(index.x + (float)random(100) / 100 - .5f, index.y + (float)random(100) / 100 - .5f);
	vec2f pixel = Vec2f(2 * move.x / size.x - 1.f, 1.f - 2 * move.y / size.y);
	vec3f eye = *(vec3f *)getPipelineVariable(id, "eye");
	vec3f U = *(vec3f *)getPipelineVariable(id, "U");
	vec3f V = *(vec3f *)getPipelineVariable(id, "V");
	vec3f W = *(vec3f *)getPipelineVariable(id, "W");
	U = normalize(U);
	V = normalize(cross(W, U));
#ifdef CBOX
	pixel *= tan(39.3077 / 360 * PI);
#endif 
	vec3f dir = normalize(-pixel.x * U + pixel.y * V + W);
	Prd prd;
	prd.start = eye;
	prd.dir = dir;
	while (1) {
		rtTrace(id, 0, prd.start, prd.dir, LIGHT_RAY, .001f, INFINITY, &prd);
		prd.result += prd.radiance;// *prd.attenuation;
		if (prd.depth > 10 || random(100) > 100 * max(prd.attenuation.x, max(prd.attenuation.y,prd.attenuation.z)))break;
		if (prd.done)break;
	}
	rgb tmp = getPixel(index.x, index.y);
	vec3f old = Vec3f(tmp.r / 255.f, tmp.g / 255.f, tmp.b / 255.f);
	if (prd.result.x > 1.f)prd.result.x = 1.f;
	if (prd.result.y > 1.f)prd.result.y = 1.f;
	if (prd.result.z > 1.f)prd.result.z = 1.f;
	prd.result = prd.result / frame + old * (1 - 1.f / frame);
	return Vec3i(prd.result.x * 255, prd.result.y * 255, prd.result.z * 255);
}
float intersect(int id, void *pts, vec3f point, vec3f dir, vec3f *norm) {
	vector<vector<vec3f>> points = *(vector<vector<vec3f>> *)pts;
	float min = INFINITY;
	for (auto &triangle : points) {
		if (float t = intersectTriangle(triangle[0], triangle[1], triangle[2], point, dir)) {
			if (t < 0.001f)continue;
			if (t < min) {
				min = t;
				if(norm)*norm = normalize(cross(triangle[1] - triangle[0], triangle[0] - triangle[2]));
			}
		}
	}
	if(min == INFINITY || min < 0.f)return 0.f;
	else return min;
}
void hit(int id, float dist, void *prd, vec3f norm, void *param) {
	Material *mat = (Material *)param;
	Prd *perraydata = (Prd *)prd;
	perraydata->depth++;

	dist -= .001f;
	vec3f hitPoint = perraydata->start + dist * perraydata->dir;
	perraydata->start = hitPoint;

	if (mat->ni == 1.f) {
		if (mat->ks == Vec3f(0.f, 0.f, 0.f)) {
			perraydata->attenuation *= mat->kd;
			if (dot(perraydata->dir, norm) > 0)perraydata->dir = randHemi(-1 * norm);
			else perraydata->dir = randHemi(norm);

			perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
			vec3f anchor = light_cbox();
			vec3f ldir = normalize(anchor - hitPoint);
			float ldist = length(anchor - hitPoint);
			Sprd sprd;
			rtTrace(id, 0, hitPoint, ldir, SHADOW_RAY, .001f, ldist - .001f, &sprd);
			if (!sprd.shadow) {
				float ndl = abs(dot(norm, ldir));
				perraydata->radiance = 6000 * perraydata->attenuation * ndl / (ldist * ldist) * Vec3f(10.f, 10.f, 10.f);
			}
		}
		else {
			float prob = (length(mat->kd) / (length(mat->kd + mat->ks)));
			if (random(1000) < (prob * 1000)) {
				perraydata->attenuation *= mat->kd;
				if (dot(perraydata->dir, norm) > 0)perraydata->dir = randHemi(-1 * norm);
				else perraydata->dir = randHemi(norm);

				perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
				vec3f anchor = light_cbox();
				vec3f ldir = normalize(anchor - hitPoint);
				float ldist = length(anchor - hitPoint);
				Sprd sprd;
				rtTrace(id, 0, hitPoint, ldir, SHADOW_RAY, .001f, ldist - .001f, &sprd);
				if (!sprd.shadow) {
					float ndl = abs(dot(norm, ldir));
					perraydata->radiance = 6000 / prob * perraydata->attenuation * ndl / (ldist * ldist) * Vec3f(10.f, 10.f, 10.f);
				}
			}
			else {
				vec3f spec = dot(-1 * perraydata->dir, norm) * 2 * norm + perraydata->dir;

				perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
				vec3f anchor = light_cbox();
				vec3f ldir = normalize(anchor - hitPoint);
				float ldist = length(anchor - hitPoint);
				Sprd sprd;
				rtTrace(id, 0, hitPoint, ldir, SHADOW_RAY, .001f, ldist - .001f, &sprd);
				if (!sprd.shadow) {
					perraydata->radiance = 1000 / (1 - prob) * perraydata->attenuation *mat->ks *
						(mat->ns + 2) / (mat->ns + 1) * 4 * max(dot(ldir, (-1 * perraydata->dir + ldir) / 2), 0.0) / 
						(ldist * ldist) * Vec3f(10.f, 10.f, 10.f);
				}

				vec3f wi = -1 * perraydata->dir;
				perraydata->dir = phoneSpec(norm, wi, mat->ns);
				perraydata->attenuation *= mat->ks *
					(mat->ns + 2) / (mat->ns + 1) * 4 * max(dot(perraydata->dir, (perraydata->dir + wi) / 2), 0.0);
			}
		}
	}
	else {
		perraydata->start += perraydata->dir * .002f;
		perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
	}
}
void emit(int id, float dist, void *prd, vec3f norm, void *param) {
	Prd *perraydata = (Prd *)prd;
	perraydata->done = true;
	perraydata->radiance = Vec3f(10.f, 10.f, 10.f);
}
void shadow(int id, void *prd) {
	Sprd *perraydata = (Sprd *)prd;
	perraydata->shadow = true;
}
void miss(int id, void *prd) {
	Prd *perraydata = (Prd *)prd;
	perraydata->done = true;
}

void sgSetup() {
	initWindow(WIDTH, HEIGHT, "View", BIT_MAP);

	graphHandle = setGraphRegion(0, 0, WIDTH, HEIGHT);
	rtGenerate(graphHandle, generate);
	rtMiss(graphHandle, miss);

	setPipelineVariable(graphHandle, "eye", &eye.pos);
	setPipelineVariable(graphHandle, "U", &eye.level);
	setPipelineVariable(graphHandle, "V", &eye.up);
	setPipelineVariable(graphHandle, "W", &eye.dir);

	m.load("source/cbox.obj");
	for (auto i : m.corespond) {
		pushObject(graphHandle, m.pos.data() + i.second.first, i.second.second, 3, intersect, hit, shadow, &m.mats[i.first]);
	}
	l.load("source/light.obj");
	pushObject(graphHandle, l.pos.data(), l.pos.size(), 3, intersect, emit, shadow, NULL);
}
void sgLoop() {
	refreshTracer(graphHandle);
	frame++;

	static int drag = 0;
	static int prex, prey;
	if (biosMouse(1).z) {
		vec3i m = biosMouse(0);
		if (m.z == (SG_LEFT_BUTTON | SG_BUTTON_DOWN)) {
			drag = 1;
			prex = m.x;
			prey = m.y;
		}
		if (m.z == (SG_LEFT_BUTTON | SG_BUTTON_UP)) {
			drag = 0;
		}
		if (m.z == SG_MIDDLE_BUTTON_UP) {
			eye.zoom(0.8f);
			frame = 1;
		}
		if (m.z == SG_MIDDLE_BUTTON_DOWN) {
			eye.zoom(1.25f);
			frame = 1;
		}
	}
	if (drag) {
		vec2i m = mousePos();
		eye.rotate(0.01f * (m.x - prex), 0.01f * (m.y - prey));
		prex = m.x;
		prey = m.y;
		frame = 1;
	}
}
