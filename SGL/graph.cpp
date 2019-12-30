#define _CRT_SECURE_NO_WARNINGS

#include "winsgl.h"
#include "geometry.h"
#include <amp.h>
#include <vector>
#include <map>
#include <list>

#define TRIANGLE_VERTEX 3


using std::string;
using std::pair;
using std::vector;
using std::map;
using std::list;
using namespace geo;


extern bitMap *canvasCache;

typedef struct tagEDGE {
	float xi;
	float dx;
	float zi;
	float dzy;
	float dzx;
	int ymax;
	float xedge;
	int pid;
 }EDGE;
void UpdateAetEdgeInfo(EDGE& e) {
	e.xi += e.dx;
	e.zi += e.dzy + e.dzx * e.dx;
	if (e.dx > 0 && e.xi > e.xedge)e.xi = e.xedge;
	if (e.dx < 0 && e.xi < e.xedge)e.xi = e.xedge;
}
bool EdgeXiComparator(EDGE& e1, EDGE& e2) {
	return (e1.xi == e2.xi) ? false : (e1.xi < e2.xi);
}
bool IsEdgeOutOfActive(EDGE e, int y) { 
	return (e.ymax == y);
}
int inline Round(float f) {
	if ((int)(f - .5f) == (int)f)return (int)f + 1;
	else return (int)f;
}

int vsDefault(int id, float *datas[], int step[], int dataNum, vec3f position[], float *outs[]) {
	for (int i = 0; i < TRIANGLE_VERTEX; i++) {
		position[i].x = datas[0][i * step[0]];
		position[i].y = datas[0][i * step[0] + 1];
		position[i].z = datas[0][i * step[0] + 2];

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
vec3i fsDefault(int id, int x, int y, float *data) {
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
	return color;
}
vec3i generateDefault(int id, vec2i index, vec2i size) {
	vec2f pixel = Vec2f(2 * float(index.x) / size.x - 1.f, 1.f - 2 * float(index.y) / size.y);
	vec3f eye = *(vec3f *)getPipelineVariable(id, "eye");
	vec3f U = *(vec3f *)getPipelineVariable(id, "U");
	vec3f V = *(vec3f *)getPipelineVariable(id, "V");
	vec3f W = *(vec3f *)getPipelineVariable(id, "W");
	U = normalize(U);
	V = normalize(cross(U, W));
	vec3f dir = pixel.x * U + pixel.y * V + W;
	vec3f radiance = Vec3f(0, 0, 0);
	rtTrace(id, 0, eye, dir, LIGHT_RAY, .001f, INFINITY, &radiance);
	return Vec3i(radiance.x * 255, radiance.y * 255, radiance.z * 255);
}

extern int period;

class Graph {

//Members for both scan line and ray tracing.
private:
	int id;
	int left, top, right, bottom;
	int window;

	bitMap *tmpCanvas;
	map<string, void *>vars;
public:
	Graph(int id, int left, int top, int right, int bottom) :
		id(id), left(left), top(top), right(right), bottom(bottom) {
		window = _currentWindow;
		vs = vsDefault;
		fs = fsDefault;
		generate = generateDefault;
	}
	~Graph() {

	}

	int getWidth() { return right - left; }
	int getHeight() { return bottom - top; }
	void clearBuffer() {
		setColor(0, 0, 0);
		putQuad(left, top, right, bottom, SOLID_FILL);
	}
	void setPipelineVariable(string name, void *var) {
		vars[name] = var;
	}
	void *getPipelineVariable(string name) {
		return vars[name];
	}
	void refresh(int eleNum, int method = SCAN_LINE) {
		if (window != _currentWindow)return;
		tmpCanvas = canvasCache;
		if (method == SCAN_LINE) {
			vector<float *>datas(this->datas.size());
			vector<int> steps(this->datas.size());
			vector<vector<vec3f>> positions(eleNum, vector<vec3f>(TRIANGLE_VERTEX));
			vector<vector<float *>> outs(eleNum, vector<float *>(TRIANGLE_VERTEX));
			int dataLength = 0;
			for (int i = 0; i < eleNum; i++) {
				for (unsigned int j = 0; j < this->datas.size(); j++) {
					datas[j] = this->datas[j].first + i * TRIANGLE_VERTEX * this->datas[j].second;
					steps[j] = this->datas[j].second;
				}
				dataLength = vs(id, &datas[0], &steps[0], this->datas.size(), positions[i].data(), outs[i].data());
			}
			scan(positions, outs, dataLength, TRIANGLE_VERTEX);
			for (auto o : outs)for (auto d : o)free(d);
			this->datas.clear();
		}
		else if (method == RAY_TRACER) {
			vec2i size = Vec2i(right - left, bottom - top);
			for (int j = 0; j < size.y; j++) {
				for (int i = 0; i < size.x; i++) {
					vec3i radiance = generate(id, Vec2i(i, j), size);
					int p = ((j + left) * tmpCanvas->sizeX + (i + top)) * 3;
					tmpCanvas->data[p] = clamp(0, 255, radiance.z);
					tmpCanvas->data[p + 1] = clamp(0, 255, radiance.y);
					tmpCanvas->data[p + 2] = clamp(0, 255, radiance.x);
				}
			}
		}
	}

//Members for just scan line.
private:
	vector<pair<float *, int>>datas;

	int(*vs)(int id, float *datas[], int step[], int dataNum, vec3f position[], float *outs[]);
	vec3i(*fs)(int id, int x, int y, float *data);

	void InitScanLineNewEdgeTable(std::vector< std::list<EDGE> >& slNet,
		vector<vector<vec3f>>points, int vertexNum, int ymin, int ymax) {
		EDGE e;
		for (unsigned int j = 0; j < points.size(); j++) {
			for (int i = 0; i < vertexNum; i++) {
				const vec3f ps = points[j][i];
				const vec3f pe = points[j][(i + 1) % vertexNum];
				const vec3f pss = points[j][(i - 1 + vertexNum) % vertexNum];
				const vec3f pee = points[j][(i + 2) % vertexNum];

				vec3f normal = cross(pe - ps, pee - pe);
				if (normal.z == 0)break;

				e.dzx = -normal.x / normal.z;
				e.dzy = -normal.y / normal.z;
				if (Round(pe.y) != Round(ps.y)) {
					e.dx = float(pe.x - ps.x) / float(pe.y - ps.y);
					//e.dzy = float(pe.z - ps.z) / float(pe.y - ps.y);
					if (pe.y > ps.y) {
						e.xi = ps.x;
						e.zi = ps.z;
						if (Round(pee.y) > Round(pe.y))
							e.ymax = Round(pe.y) - 1;
						else
							e.ymax = Round(pe.y);
						e.xedge = pe.x;
						e.pid = j;
						slNet[Round(ps.y) - ymin].push_front(e);
					}
					else {
						e.xi = pe.x;
						e.zi = pe.z;
						if (Round(pss.y) > Round(ps.y))
							e.ymax = Round(ps.y) - 1;
						else
							e.ymax = Round(ps.y);
						e.xedge = ps.x;
						e.pid = j;
						slNet[Round(pe.y) - ymin].push_front(e);
					}
				}
			}
		}
	}
	void InsertNetListToAet(std::list<EDGE> &src, std::list<EDGE> &dst) {
		for (auto &e : src) {
			dst.push_back(e);
		}
	}
	float *TriangleInterp(vector<vec3f> points, int x, int y, vector<float *> datas, int dataLength) {
		float u = -((points[0].x - x)*(points[2].y - points[0].y) - (points[0].y - y)*(points[2].x - points[0].x)) /
			((points[1].x - points[0].x)*(points[2].y - points[0].y) - (points[1].y - points[0].y)*(points[2].x - points[0].x));
		float v = -((points[0].x - x)*(points[1].y - points[0].y) - (points[0].y - y)*(points[1].x - points[0].x)) /
			((points[2].x - points[0].x)*(points[1].y - points[0].y) - (points[2].y - points[0].y)*(points[1].x - points[0].x));
		float *ret = (float *)malloc(dataLength * sizeof(float));
		for (int i = 0; i < dataLength; i++) {
			ret[i] = (1 - u - v)*datas[0][i] + u * datas[1][i] + v * datas[2][i];
		}
		return ret;
	}
	bool AboutEqual(float f1, float f2, float delta) {
		return f1 - f2 < delta &&f1 - f2 > -delta;
	}
	void FillAetScanLine(vector<vector<vec3f>>&points, vector<vector<float *>> outs,
		int dataLength, std::list<EDGE>aet, int y) {
		vector<EDGE> start;
		vector<float> zBuffer(right - left + 1, INFINITY);

		if (aet.empty())return;

		for (auto &e : aet) {
			int end = -1, idx = 0;
			for (auto tmp : start) {
				if (tmp.pid == e.pid) {
					end = e.pid;

					if (e.xi >= left) {
						int l = left, r = right - 1;
						if (tmp.xi > l)l = Round(tmp.xi);
						if (e.xi < r)r = Round(e.xi) - 1;
						for (int i = l; i <= r; i++) {
							if (tmp.zi + tmp.dzx * (i - tmp.xi) > zBuffer[i])continue;
							zBuffer[i] = tmp.zi + tmp.dzx * (i - tmp.xi);
							float *interp = TriangleInterp(points[tmp.pid], i, y, outs[tmp.pid], dataLength);
							vec3i color = fs(id, i, y, interp);
							free(interp);
							int p = (y * tmpCanvas->sizeX + i) * 3;
							tmpCanvas->data[p] = clamp(0, 255, color.z);
							tmpCanvas->data[p + 1] = clamp(0, 255, color.y);
							tmpCanvas->data[p + 2] = clamp(0, 255, color.x);
						}
					}
					break;
				}
				idx++;
			}
			if (end != -1) {
				start[idx] = start.back();
				start.pop_back();
			}
			else {
				start.push_back(e);
			}
			if (e.xi > right - 1)break;
		}
		return;
	}
	void FillAetPeriodScanLine(vector<vector<vec3f>>&points, vector<vector<float *>> outs,
		int dataLength, std::list<EDGE>aet, int y) {
		vector<EDGE> start;

		if (aet.empty())return;

		EDGE prev;
		for (auto &e : aet) {
			int end = -1, idx = 0;
			for (auto tmp : start) {
				if (tmp.pid == e.pid) {
					end = e.pid;
					break;
				}
				idx++;
			}
			if (e.xi >= left && !start.empty()) {
				EDGE top = start.front();

				for (auto &tmp : start) {
					if (tmp.zi + tmp.dzx * (e.xi - tmp.xi) / 2 < top.zi + top.dzx * (e.xi - top.xi) / 2) {
						top = tmp;
					}
				}
				int l = left, r = right - 1;
				if (prev.xi > l)l = Round(prev.xi);
				if (e.xi < r)r = Round(e.xi) - 1;
				for (int i = l; i <= r; i++) {
					float *interp = TriangleInterp(points[top.pid], i, y, outs[top.pid], dataLength);
					//float *interp = outs[top.pid][0];
					vec3i color = fs(id, i, y, interp);
					free(interp);
					int p = (y * tmpCanvas->sizeX + i) * 3;
					tmpCanvas->data[p] = clamp(0, 255, color.z);
					tmpCanvas->data[p + 1] = clamp(0, 255, color.y);
					tmpCanvas->data[p + 2] = clamp(0, 255, color.x);
				}
			}
			if (end != -1) {
				start[idx] = start.back();
				start.pop_back();
			}
			else {
				start.push_back(e);
			}
			if (e.xi > right - 1)break;
			prev = e;
		}
		return;
	}
	void RemoveNonActiveEdgeFromAet(std::list<EDGE>& aet, int y) {
		aet.remove_if(std::bind2nd(std::ptr_fun(IsEdgeOutOfActive), y));
	}
	void ProcessScanLineFill(vector<vector<vec3f>>&points, vector<vector<float *>> outs,
		int dataLength, std::vector< std::list<EDGE> >&slNet, int ymin, int ymax) {
		std::list<EDGE> aet;
		for (int y = ymin; y <= ymax; y++) {
			InsertNetListToAet(slNet[y - ymin], aet);
			aet.sort(EdgeXiComparator);
			if(y >= top && y < bottom)
				if(period)FillAetPeriodScanLine(points, outs, dataLength, aet, y);
				else FillAetScanLine(points, outs, dataLength, aet, y);
			RemoveNonActiveEdgeFromAet(aet, y);
			for_each(aet.begin(), aet.end(), UpdateAetEdgeInfo);
		}
	}
	void scan(vector<vector<vec3f>>&points, vector<vector<float *>> &outs, int dataLength, int vertexNum) {
		int minX = right, maxX = left - 1, minY = bottom, maxY = top - 1;
		int objs = points.size();
		for (int obj = 0; obj < objs; obj++) {
			vector<vec3f> &ps = points[obj];
			for (int i = 0; i < vertexNum; i++) {
				ps[i] = (geo::Matrix<float>::translate((right - left) / 2, (bottom - top) / 2, 0) *
					geo::Matrix<float>::scale(400, -400, 1) * Vec4f(ps[i], 1)).col<vec3f>(0);
				if (Round(ps[i].x) > maxX)maxX = Round(ps[i].x);
				if (Round(ps[i].x) < minX)minX = Round(ps[i].x);
				if (Round(ps[i].y) > maxY)maxY = Round(ps[i].y);
				if (Round(ps[i].y) < minY)minY = Round(ps[i].y);
			}
			for (int i = 0; i < vertexNum; i++) {
				if (ps[i].z < -1 || ps[i].z > 1)
					ps[i] = ps[(i + vertexNum - 1) % vertexNum];
			}
		}
		if (minX > right - 1 || maxX < left || minY > bottom - 1 || maxY < top)return;

		vector<list<EDGE>> slNet(maxY - minY + 1);
		InitScanLineNewEdgeTable(slNet, points, vertexNum, minY, maxY);
		ProcessScanLineFill(points, outs, dataLength, slNet, minY, maxY);
	}
public:
	int pushDataArray(float *data, int step) {
		datas.push_back(pair<float *, int>(data, step));
		return datas.size();
	}
	void vertexShader(int(*vs)(int id, float *datas[], int step[], int dataNum,
		vec3f position[], float *outs[])) {
		this->vs = vs;
	}
	void fragmentShader(vec3i(*fs)(int id, int x, int y, float *data)) {
		this->fs = fs;
	}

//Members for just ray tracing.
private:
	vec3i(*generate)(int id, vec2i index, vec2i size);
	void(*miss)(int id, void *prd);

	class Aabb {
	private:
		vec3f min, max;
	public:
		Aabb(vector<vector<vec3f>> p) {
			min = Vec3f(INFINITY, INFINITY, INFINITY);
			max = Vec3f(-INFINITY, -INFINITY, -INFINITY);
			for (auto points : p) {
				for (auto point : points) {
					if (point.x > max.x)max.x = point.x;
					if (point.x < min.x)min.x = point.x;
					if (point.y > max.y)max.y = point.y;
					if (point.y < min.y)min.y = point.y;
					if (point.z > max.z)max.z = point.z;
					if (point.z < min.z)min.z = point.z;
				}
			}
		}

		bool intersect(vec3f o, vec3f d) {
			float t;
			vec3f hitPoint;
			if (o > min && o < max)return true;
			if (d.x != 0.f) {
				if (d.x > 0)t = (min.x - o.x) / d.x;
				else t = (max.x - o.x) / d.x;
				if (t > 0.f) {
					hitPoint = o + t * d;
					if (min.y < hitPoint.y && hitPoint.y < max.y && min.z < hitPoint.z && hitPoint.z < max.z)return true;
				}
			}
			if (d.y != 0.f) {
				if (d.y > 0)t = (min.y - o.y) / d.y;
				else t = (max.y - o.y) / d.y;
				if (t > 0.f) {
					hitPoint = o + t * d;
					if (min.z < hitPoint.z && hitPoint.z < max.z && min.x < hitPoint.x && hitPoint.x < max.x)return true;
				}
			}
			if (d.z != 0.f) {
				if (d.z > 0)t = (min.z - o.z) / d.z;
				else t = (max.z - o.z) / d.z;
				if (t > 0.f) {
					hitPoint = o + t * d;
					if (min.x < hitPoint.x && hitPoint.x < max.x && min.y < hitPoint.y && hitPoint.y < max.y)return true;
				}
			}
			return false;
		}
	};
	class Object {
	private:
		map<string, void *>vars;
	public:
		Aabb aabb;

		vector<vector<vec3f>> points;
		float(*intersect)(int id, void *points, vec3f point, vec3f dir, vec3f *norm);
		void(*hit)(int id, float dist, void *prd, vec3f norm);
		void(*shadow)(int id, void *prd);

		Object(vector<vector<vec3f>> p,
			float(*intersect)(int id, void *points, vec3f point, vec3f dir, vec3f *norm),
			void(*hit)(int id, float dist, void *prd, vec3f norm), void(*shadow)(int id, void *prd)) :
			points(p), intersect(intersect), hit(hit), shadow(shadow), aabb(p) {}
	};
	vector<Object *> objs;
public:
	int pushObject(float *data, int length, int vertices,
		float(*intersect)(int id, void *points, vec3f point, vec3f dir, vec3f *norm),
		void(*hit)(int id, float dist, void *prd, vec3f norm), void(*shadow)(int id, void *prd)) {
		vector<vector<vec3f>> p;
		for (int i = 0; i < length; i+=3) {
			if ((i/3)%vertices == 0)p.push_back(vector<vec3f>());
			p.back().push_back(Vec3f(data[i], data[i + 1], data[i + 2]));
		}
		objs.push_back(new Object(p, intersect, hit, shadow));
		return objs.size() - 1;
	}
	void rtTrace(int obj, vec3f light, vec3f dir, int type, float tmin, float tmax, void *prd) {
		if (type == LIGHT_RAY) {
			float min = INFINITY;
			Object *tmp = NULL;
			vec3f norm, niter;
			for (auto &obj : objs) {
				if (!obj->aabb.intersect(light, dir))continue;
				if (float t = obj->intersect(id, &obj->points, light, dir, &niter)) {
					if (t < tmin || t > tmax)continue;
					if (t < min) {
						min = t;
						tmp = obj;
						norm = niter;
					}
				}
			}
			if (tmp == NULL)miss(id, prd);
			else tmp->hit(id, min, prd, norm);
		}
		else if (type == SHADOW_RAY) {
			for (auto &obj : objs) {
				if (!obj->aabb.intersect(light, dir))continue;
				if (float t = obj->intersect(id, &obj->points, light, dir, NULL)) {
					if (t <= tmin || t >= tmax)continue;
					obj->shadow(id, prd);
				}
			}
		}
	}
	void rtGenerate(vec3i(*generate)(int id, vec2i index, vec2i size)) {
		this->generate = generate;
	}
	void rtMiss(void(*miss)(int id, void *prd)) {
		this->miss = miss;
	}
};
vector<Graph *> graphs;
extern int _currentWindow;

SGint setGraphRegion(int left, int top, int right, int bottom) {
	for (unsigned int i = 0; i < graphs.size(); i++) {
		if (graphs[i] == NULL)graphs[i] = new Graph(i, left, top, right, bottom);
		return i;
	}
	graphs.push_back(new Graph(graphs.size(), left, top, right, bottom));
	return graphs.size() - 1;
}
SGint deleteGraphRegion(int id) {
	if (graphs[id] == NULL)
		return SG_OBJECT_NOT_FOUND;
	else {
		delete graphs[id];
		graphs[id] = NULL;
		return SG_NO_ERORR;
	}
}
SGvoid clearGraphBuffer(int id) {
	graphs[id]->clearBuffer();
}
SGvoid setPipelineVariable(int id, const char * name, void *var) {
	if (id < 0 || id >= graphs.size())return;
	graphs[id]->setPipelineVariable(name, var);
}
SGvoid *getPipelineVariable(int id, const char *name) {
	if (id < 0 || id >= graphs.size())return NULL;
	return graphs[id]->getPipelineVariable(name);
}
SGvoid refreshGraph(int id, int elementNum) {
	return graphs[id]->refresh(elementNum);
}
SGvoid refreshTracer(int id) {
	return graphs[id]->refresh(0, RAY_TRACER);
}

SGint pushDataArray(int id, float *data, int step) {
	return graphs[id]->pushDataArray(data, step);
}
SGvoid vertexShader(int id, int(*vs)(int id, float *datas[], int step[], int dataNum, vec3f position[], float *outs[])) {
	graphs[id]->vertexShader(vs);
}
SGvoid fragmentShader(int id, vec3i(*fs)(int id, int x, int y, float *data)) {
	graphs[id]->fragmentShader(fs);
}

vec3f randHemi(vec3f normal) {
	vec3f binormal, tangent;
	if (fabs(normal.x) > fabs(normal.z)) {
		binormal.x = -normal.y;
		binormal.y = normal.x;
		binormal.z = 0;
	}
	else {
		binormal.x = 0;
		binormal.y = -normal.z;
		binormal.z = normal.y;
	}
	binormal = normalize(binormal);
	tangent = cross(binormal, normal);

	float p = random(1000) / 1000.f;
	float phi = acos(1 - p);
	float theta = random(1000) / 1000.f * 2 * PI;
	float x = sin(phi) * cos(theta);
	float y = sin(phi) * sin(theta);
	float z = cos(phi);

	return x * tangent + y * binormal + z * normal;
}
SGint pushObject(int id, float *data, int length, int vertices,
	float(*intersect)(int id, void *points, vec3f point, vec3f dir, vec3f *norm),
	void(*hit)(int id, float dist, void *prd, vec3f norm), void(*shadow)(int id, void *prd)) {
	return graphs[id]->pushObject(data, length, vertices, intersect, hit, shadow);
}
SGvoid rtTrace(int id, int obj, vec3f light, vec3f dir, int type, float tmin, float tmax, void *prd) {
	graphs[id]->rtTrace(obj, light, dir, type, tmin, tmax, prd);
}
SGvoid rtGenerate(int id, vec3i(*generate)(int id, vec2i index, vec2i size)) {
	graphs[id]->rtGenerate(generate);
}
SGvoid rtMiss(int id, void(*miss)(int id, void *prd)) {
	graphs[id]->rtMiss(miss);
}


