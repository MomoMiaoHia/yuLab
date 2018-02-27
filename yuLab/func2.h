#pragma once
#ifndef FUNC2_H_
#define FUNC2_H_

#include<opencv2\opencv.hpp>
#include<iostream>
#include<algorithm>
#include<ctime>
#include <vector>
#include<sstream>
#include<fstream>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;


float getPointPath(Point&a, Point&b) {
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y,2));
}

void countDistance(vector<Point>&input, vector<float>&output) {
	int n = input.size();
	if (output.size() < n || n == 0)
		return;
	output[0] = 0;
	for (int i = 1; i < n; ++i) {
		output[i] = getPointPath(input[i], input[i - 1]);
	}
}

void countSpeed(vector<float>&distance, vector<float>&speed,float time) {
	int n = distance.size();
	if (speed.size() < n || n == 0)
		return;
	for (int i = 0; i < n; ++i) {
		speed[i] = distance[i] / time;
	}
}

void coutAngle(vector<Point>&input,vector<float>&distance, vector<float>&output) {
	int n = input.size();
	if (output.size() < n || n == 0)
		return;
	vector<Point>vecs(n);
	//vector<float>angle(n);
	vecs[0].x = 0;
	vecs[0].y = 0;
	//angle[0] = 0;
	output[0] = 0;
	for (int i = 1; i < n; ++i) {
		vecs[i].x = input[i-1].x-input[i].x;
		vecs[i].y = input[i - 1].y - input[i].y;
		output[i] = acosf((vecs[i - 1].x*vecs[i].x + vecs[i - 1].y*vecs[i].y) / (distance[i - 1] + distance[i]));
	}
}

#endif