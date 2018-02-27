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


float getPointPath(pair<int, int>&a, pair<int, int>&b) {
	return sqrtf(powf(a.first - b.first, 2) + powf(a.second - b.second,2));
}

void countDistance(vector<pair<int,int>>&input, vector<float>&output) {
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

void coutAngle(vector<pair<int, int>>&input,vector<float>&distance, vector<float>&output) {
	int n = input.size();
	if (output.size() < n || n == 0)
		return;
	vector<pair<float, float>>vecs(n);
	//vector<float>angle(n);
	vecs[0].first = 0;
	vecs[0].second = 0;
	//angle[0] = 0;
	output[0] = 0;
	for (int i = 1; i < n; ++i) {
		vecs[i].first = vecs[i-1].first-vecs[i].first;
		vecs[i].second = vecs[i - 1].second - vecs[i].second;
		output[i] = acosf((vecs[i - 1].first*vecs[i].first + vecs[i - 1].second*vecs[i].second) / (distance[i - 1] + distance[i]));
	}
}

#endif