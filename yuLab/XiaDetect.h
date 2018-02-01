#pragma once
#ifndef XIADETECT_H_
#define XIADETECT_H_

#include<opencv2\opencv.hpp>
#include<iostream>
#include<algorithm>
#include<ctime>
#include<sstream>
#include<fstream>
#include"svm.h"

using namespace cv;
using namespace std;

class XiaDetect {
private:
	svm_model* m_model;
	const int DIMS;

public:
	XiaDetect() :DIMS(1764) {
		m_model = svm_load_model("xia-detect-2.model");
	}
	~XiaDetect() {
		svm_free_and_destroy_model(&m_model);
	}
	int judge(const Mat& _img) {     //��ϺͼƬ�����ж���0�������ģ�1������
		Mat src = _img.clone();
		if (src.channels() == 3)
			cvtColor(src, src, CV_BGR2GRAY);
		threshold(src, src, 0, 255, THRESH_BINARY + THRESH_OTSU);  //������Ƕ�ֵͼ
		resize(src, src, Size(64, 64));
		HOGDescriptor hog(Size(64, 64), Size(16, 16), Size(8, 8), Size(8, 8), 9);
		vector<float> descriptors;   //����������
		hog.compute(src, descriptors);  //DIMSά
		svm_node *testNode = new svm_node[DIMS + 1];
		for (int j = 0; j < DIMS; j++)
		{
			svm_node node;
			node.index = j + 1;
			node.value = descriptors[j];
			testNode[j] = node;    //���õ�k��
		}
		svm_node node;
		node.index = -1;
		testNode[DIMS] = node;
		double result = svm_predict(m_model, testNode);
		delete[] testNode;
		return static_cast<int>(result);
	}
};

#endif // !XIADETECT_H_
