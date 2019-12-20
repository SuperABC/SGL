#include "../../winsgl.h"

int graphHandle;

int vs(int id, float *datas[], int step[], int dataNum, vec3f position[], float *outs[]) {
	for (int i = 0; i < 3; i++) {
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
	}
	return 6;
}
vec3i fs(int id, int x, int y, float *data) {
	vec3i color;
	color.x = data[3] * 255;
	color.y = data[4] * 255;
	color.z = data[5] * 255;
	return color;
}
void sgSetup() {
	initWindow(640, 480, "View", BIT_MAP);

	graphHandle = setGraphRegion(0, 0, 640, 480);
	vertexShader(graphHandle, vs);
	fragmentShader(graphHandle, fs);
}
void sgLoop() {
	static float pos[18] = {0, 0, 0, .5f, 0, 0, 0, .5f, 0,
		.5f, 0, 0, 0, .5f, 0, .5f, .5f, 0};
	static float color[18] = { 0, 0, 0, 1, 0, 0, 0, 1, 0,
		1, 0, 0, 0, 1, 0, 1, 1, 0 };
	pushDataArray(graphHandle, pos, 3);
	pushDataArray(graphHandle, color, 3);
	clearGraphBuffer(graphHandle);
	refreshGraph(graphHandle, 2);
}