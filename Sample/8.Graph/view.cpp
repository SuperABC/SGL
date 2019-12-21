#include "winsgl.h"
#include <string>

using namespace geo;
using namespace std;

int graphHandle;

class Eye {
public:
	Eye() {
		this->pos = Vec3f(0, 0, 100);
		this->dir = Vec3f(0, 0, -1);
		this->up = Vec3f(0, 1, 0);
		this->theta = float(PI / 2);
		this->phy = float(PI / 2);
		this->dist = 100.f;
		this->model = Matrix<float>(4, 4);
	}
	~Eye() {}

	vec3f pos, dir, up;
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
Model m;

Matrix<float> transform(4, 4);
vec3f light;
int vs(int id, float *datas[], int step[], int dataNum, vec3f position[], float *outs[]) {
	Matrix<float> trans(4, 1);
	for (int i = 0; i < 3; i++) {
		trans << datas[0][i * step[0]], datas[0][i * step[0] + 1], datas[0][i * step[0] + 2], 1.f;
		trans = *(Matrix<float> *)getPipelineVariable(id, "transform") * trans;
		position[i].x = trans[0][0] / trans[3][0];
		position[i].y = trans[1][0] / trans[3][0];
		position[i].z = trans[2][0] / trans[3][0];

		outs[i] = (float *)malloc(9 * sizeof(float));
		outs[i][0] = datas[0][i * step[0]];
		outs[i][1] = datas[0][i * step[0] + 1];
		outs[i][2] = datas[0][i * step[0] + 2];
		outs[i][3] = datas[1][i * step[1]];
		outs[i][4] = datas[1][i * step[1] + 1];
		outs[i][5] = datas[1][i * step[1] + 2];
		outs[i][6] = datas[2][i * step[2]];
		outs[i][7] = datas[2][i * step[2] + 1];
		outs[i][8] = datas[2][i * step[2] + 2];
	}
	return 9;
}
vec3i fs(int id, int x, int y, float *data) {
	vec3i color;
	vec3f pos, norm;
	pos.x = data[0];
	pos.y = data[1];
	pos.z = data[2];
	norm.x = data[6];
	norm.y = data[7];
	norm.z = data[8];
	color.x = data[3] * 255;
	color.y = data[4] * 255;
	color.z = data[5] * 255;
	vec3f light = *(vec3f*)getPipelineVariable(id, "light");

	float ratio = dot(normalize(norm), normalize(light - pos));
	if (ratio < 0)return Vec3i(16, 16, 16);
	else return color * ratio + Vec3i(16, 16, 16);
}
void sgSetup() {
	initWindow(640, 480, "View", BIT_MAP);

	graphHandle = setGraphRegion(0, 0, 640, 480);
	vertexShader(graphHandle, vs);
	fragmentShader(graphHandle, fs);

	transform = Matrix<float>(4, 4);
	transform = Matrix<float>::view(eye.pos, eye.pos + eye.dir, eye.up) * transform;
	transform = Matrix<float>::perspect(90, 1.f, 1.f, 1000.f) * transform;
	light = Vec3f(100, -100, 100);

	setPipelineVariable(graphHandle, "transform", &transform);
	setPipelineVariable(graphHandle, "light", &light);

	m.load("source/castle.obj");
}
void sgLoop() {
	pushDataArray(graphHandle, m.pos.data(), 3);
	pushDataArray(graphHandle, m.color.data(), 3);
	pushDataArray(graphHandle, m.norm.data(), 3);
	clearGraphBuffer(graphHandle);
	refreshGraph(graphHandle, m.pos.size()/9);

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
			transform = Matrix<float>(4, 4);
			transform = Matrix<float>::view(eye.pos, eye.pos + eye.dir, eye.up) * transform;
			transform = Matrix<float>::perspect(90, 1.f, 1.f, 1000.f) * transform;
		}
		if (m.z == SG_MIDDLE_BUTTON_DOWN) {
			eye.zoom(1.25f);
			transform = Matrix<float>(4, 4);
			transform = Matrix<float>::view(eye.pos, eye.pos + eye.dir, eye.up) * transform;
			transform = Matrix<float>::perspect(90, 1.f, 1.f, 1000.f) * transform;
		}
	}
	if (drag) {
		vec2i m = mousePos();
		eye.rotate(0.01f * (m.x - prex), 0.01f * (m.y - prey));
		prex = m.x;
		prey = m.y;
		transform = Matrix<float>(4, 4);
		transform = Matrix<float>::view(eye.pos, eye.pos + eye.dir, eye.up) * transform;
		transform = Matrix<float>::perspect(90, 1.f, 1.f, 1000.f) * transform;
	}
}