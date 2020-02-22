#include "winsgl.h"
#include <string>

using namespace geo;
using namespace std;

int graphHandle;
int WIDTH = 512, HEIGHT = 512;

class Eye {
public:
	Eye() {
		this->pos = Vec3f(0, 0, 30);
		this->dir = normalize(Vec3f(0, 0, -1));
		this->up = Vec3f(0, 1, 0);
		this->level = Vec3f(1, 0, 0);
		this->theta = float(PI / 2);
		this->phy = float(PI / 2);
		this->dist = Matrix<float>(pos).length();
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

class Model {
public:
	vector<float> pos, color, norm;

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
		float num1, num2, num3;

		vector<float>pos;
		vector<float>color;
		vector<float>normal;

		while (fin >> op) {
			if (op == "v") {
				fin >> num1 >> num2 >> num3;
				pos.push_back(num1);
				pos.push_back(num3);
				pos.push_back(num2);
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
				if (tmp1 == 0 || tmp2 == 0 || tmp3 == 0 ||
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
				if (tmp1 == 0 || tmp2 == 0 || tmp3 == 0) {
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
			else {
				char *buf = new char[256];
				fin.getline(buf, 256);
				delete buf;
			}
		}
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

vec3f lightAnchor = Vec3f(-20.f, 39.9f, -20.f);
vec3f lightV1 = Vec3f(40.f, 0.f, 0.f);
vec3f lightV2 = Vec3f(0.f, 0.f, 40.f);

int frame = 1;
vec3i generate(int id, vec2i index, vec2i size) {
	vec2f pixel = Vec2f(2 * float(index.x) / size.x - 1.f, 1.f - 2 * float(index.y) / size.y);
	vec3f eye = *(vec3f *)getPipelineVariable(id, "eye");
	vec3f U = *(vec3f *)getPipelineVariable(id, "U");
	vec3f V = *(vec3f *)getPipelineVariable(id, "V");
	vec3f W = *(vec3f *)getPipelineVariable(id, "W");
	U = normalize(U);
	V = normalize(cross(U, W));
	vec3f dir = normalize(pixel.x * U + pixel.y * V + W);
	Prd prd;
	prd.start = eye;
	prd.dir = dir;
	while (1) {
		rtTrace(id, 0, prd.start, prd.dir, LIGHT_RAY, .001f, INFINITY, &prd);
		prd.result += prd.radiance * prd.attenuation;
		if (random(10) >= 8)break;
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
void hit(int id, float dist, void *prd, vec3f norm) {
	Prd *perraydata = (Prd *)prd;
	perraydata->depth++;

	dist -= .001f;
	vec3f hitPoint = perraydata->start + dist * perraydata->dir;
	if (hitPoint.x < -39.999f)perraydata->attenuation *= Vec3f(1.f, 0.6f, 0.6f);
	else if (hitPoint.x > 39.999f)perraydata->attenuation *= Vec3f(0.6f, 1.f, 0.6f);
	else perraydata->attenuation *= Vec3f(.8f, .8f, .8f);
	perraydata->start = hitPoint;
	if (random(3) >= 2) {
		if (dot(perraydata->dir, norm) > 0)perraydata->dir = randHemi(-1 * norm);
		else perraydata->dir = randHemi(norm);
	}
	else if (random(2) >= 1) {
		perraydata->dir = dot(-1 * perraydata->dir, norm) * 2 * norm + perraydata->dir;
	}
	else {

	}

	perraydata->radiance = Vec3f(0.f, 0.f, 0.f);
	vec3f anchor = *(vec3f *)getPipelineVariable(id, "lightAnchor");
	vec3f v1 = *(vec3f *)getPipelineVariable(id, "lightV1");
	vec3f v2 = *(vec3f *)getPipelineVariable(id, "lightV2");
	float r1 = random(100) / 100.f, r2 = random(100) / 100.f;
	anchor += v1 * r1 + v2 * r2;
	vec3f ldir = normalize(anchor - hitPoint);
	float ldist = length(anchor - hitPoint);
	Sprd sprd;
	rtTrace(id, 0, hitPoint, ldir, SHADOW_RAY, .001f, ldist - .001f, &sprd);
	if (!sprd.shadow) {
		float ndl = abs(dot(norm, ldir));
		perraydata->radiance = length(v1) * length(v2) * ndl / (ldist * ldist) * Vec3f(1.2f, 1.2f, 1.2f);
	}
	//perraydata->done = true;
}
void emit(int id, float dist, void *prd, vec3f norm) {
	Prd *perraydata = (Prd *)prd;
	perraydata->done = true;
	perraydata->radiance = Vec3f(1.2f, 1.2f, 1.2f);
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

	setPipelineVariable(graphHandle, "lightAnchor", &lightAnchor);
	setPipelineVariable(graphHandle, "lightV1", &lightV1);
	setPipelineVariable(graphHandle, "lightV2", &lightV2);

	m.load("source/castle.obj");
	m.load("source/box.obj");
	pushObject(graphHandle, m.pos.data(), m.pos.size(), 3, intersect, hit, shadow);
	l.load("source/light.obj");
	pushObject(graphHandle, l.pos.data(), l.pos.size(), 3, intersect, emit, shadow);
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