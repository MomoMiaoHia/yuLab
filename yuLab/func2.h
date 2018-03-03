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


float getPointPath(Point2f&a, Point2f&b) {
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y,2));
}

void countDistance(vector<Point2f>&input, vector<float>&output) {
	int n = input.size();
	if (n == 0)
		return;
	if(output.size()==0){
		output.push_back(0);
		//return;
	}
	for (int i = 1; i < n; ++i) {
		output.push_back(getPointPath(input[i], input[i - 1]));
	}
}

void countSpeed(vector<float>&distance, vector<float>&speed,vector<float>& time) {
	int n = distance.size();
	if (n == 0)
		return;
	speed.push_back(0);
	for (int i = 1; i < n; ++i) {
		speed.push_back(distance[i] / (time[i]-time[i-1]));
	}
}

void coutAngle(vector<Point2f>&input,vector<float>&distance, vector<float>&output) {
	int n = input.size();
	if ( n == 0)
		return;
	vector<Point>vecs(n);
	//vector<float>angle(n);
	vecs[0].x = 0;
	vecs[0].y = 0;
	//angle[0] = 0;
	if(output.size()==0)
		output.push_back(0);
	for (int i = 1; i < n; ++i) {
		vecs[i].x = input[i-1].x-input[i].x;
		vecs[i].y = input[i - 1].y - input[i].y;
		if (distance[i] < 1e-9 || distance[i - 1] < 1e-9)
			output.push_back(0);
		else
			output.push_back(acosf((vecs[i - 1].x*vecs[i].x + vecs[i - 1].y*vecs[i].y) / (distance[i - 1] * distance[i]))*57.32);
	}
}

float ratioChange(float lenth, float ratio) {
	return lenth*ratio;
}

#endif