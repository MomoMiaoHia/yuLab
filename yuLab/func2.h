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

using namespace cv;
using namespace std;



//XiaDetect detect;
//��ÿһ֡��������һ֡��ÿһ��������Ϻ����Ӿ���
/*
//���ص�����
bool flag = false;
Rect initRect;
bool useMouse = true;
void onMouse(int event, int x, int y, int flags, void* param);   //���ص���������������


																 //��һ����ƵԴ�ĵ�һ֡������껭�����ο򣬱�ʾϺ�������
void init(VideoCapture& capture) {
	capture >> currentFrame;
	if (currentFrame.cols>1000 && currentFrame.rows>900)
		resize(currentFrame, currentFrame, Size(), 0.65, 0.65);
	namedWindow(WIN1);
	setMouseCallback(WIN1, onMouse);
	imshow(WIN1, currentFrame);
	waitKey();
}

void onMouse(int event, int x, int y, int flags, void* param) {
	if (useMouse) {
		switch (event) {
		case EVENT_LBUTTONDOWN:
			flag = true;
			initRect = Rect(x, y, 0, 0);
			break;
		case EVENT_MOUSEMOVE:
			if (flag) {
				initRect.width = x - initRect.x;
				initRect.height = y - initRect.y;
			}
			break;
		case EVENT_LBUTTONUP:
			flag = false;
			if (initRect.width < 0) {
				initRect.x += initRect.width;
				initRect.width *= -1;
			}
			if (initRect.height < 0) {
				initRect.y += initRect.height;
				initRect.height *= -1;
			}
			rectangle(currentFrame, initRect, Scalar(0, 0, 255), 1);
			imshow(WIN1, currentFrame);
			useMouse = false;
			//status = vector<int >(box_cols*box_rows, 0);
			break;
		default:
			break;
		}
	}
}
*/
/*
bool cmp(const Rect& _lhs, const Rect& _rhs) {
	return _lhs.area() > _rhs.area();
}

//����ͼ����������õĲ��֣��Ҿ���Ԥ������
vector<Rect> getRects(const Mat& _img, int box_rows, int box_cols) {
	//ȷ��Ϊ�Ҷ�ͼ
	Mat img = _img.clone();
	if (_img.channels() == 3)
		cvtColor(_img, img, CV_BGR2GRAY);
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	vector<Rect> result(box_rows*box_cols);
	int kk = 0;
	int width = _img.cols / box_cols;   //ÿֻϺ�ķ���Ŀ���Լ��߶�
	int height = _img.rows / box_rows;
	for (int i = 0; i < box_rows; ++i) {
		for (int j = 0; j < box_cols; ++j) {
			Rect temp(j*width, i*height, width, height);
			Mat roi = img(temp); //��ǰ�Ŀ�
			threshold(roi, roi, 0, 255, THRESH_BINARY + THRESH_OTSU);
			morphologyEx(roi, roi, MORPH_OPEN, ele);  //
			vector<vector<Point> >contours;         //��������
			findContours(roi.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			vector<Rect> goodRects;
			for (size_t k = 0; k < contours.size(); ++k) {
				Rect rect = boundingRect(contours[k]);
				if (valid(rect, width, height)) {
					rect.x += temp.x;
					rect.y += temp.y;
					goodRects.push_back(rect);
				}
			}
			if (goodRects.size() != 0) {
				sort(goodRects.begin(), goodRects.end(), cmp);
				result[kk] = goodRects[0];
			}
			kk++;
		}
	}
	//imshow(WIN2, img);
	return result;
}
*/
/*void RemoveSmallRegion2(Mat& src, Mat& dst, int AreaLimit, int CheckMode);

vector<Rect> getRects(const Mat& _img) {
	//ȷ��Ϊ�Ҷ�ͼ
	Mat img = _img.clone();
	if (_img.channels() == 3)
		cvtColor(_img, img, CV_BGR2GRAY);
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	vector<Rect> result;
	//int kk = 0;
	threshold(img, img, 0, 255, THRESH_BINARY + THRESH_OTSU);
	RemoveSmallRegion2(img, img, 100, 1);
	vector<vector<Point> >contours;
	findContours(img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0;i < contours.size();++i) {
		Rect rect = boundingRect(contours[i]);
		result.push_back(rect);
	}
	//imshow(WIN2, img);
	return result;
}
*/

/*
void RemoveSmallRegion(Mat& src, Mat& dst, int Arealimit, int CheckMode, int NeighborMode) {
	//checkmode��0����ȥ��������1����ȥ��������neighbormode: 0����4����1����8����
	int RemoveCount = 0;	//ȥ���ĸ���
							//��¼ÿ�����ص����״̬�ı�ǩ��0����δ��飬1�������ڼ��,2�����鲻�ϸ���Ҫ��ת��ɫ����3������ϸ������
	Mat Pointlabel = Mat::zeros(src.size(), CV_8UC1);
	CheckMode = 255 * (1 - CheckMode);


	for (int i = 0;i < src.rows;++i) {
		//��ǲ���Ҫȥ������ɫ����
		uchar* iData = src.ptr<uchar>(i);
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0;j < src.cols;++j) {
			if (iData[j] == CheckMode) {
				iLabel[j] = 3;
			}
		}
	}

	vector<Point2i> NeihborPos;		//��¼�����λ��
									//(0,0)��4����
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));
	if (NeighborMode == 1) {
		//8����
		NeihborPos.push_back(Point2i(-1, -1));
		NeihborPos.push_back(Point2i(-1, 1));
		NeihborPos.push_back(Point2i(1, -1));
		NeihborPos.push_back(Point2i(1, 1));
	}

	int NeighborCount = 4 + 4 * NeighborMode;
	int CurX = 0, CurY = 0;

	for (int i = 0; i < src.rows;++i) {
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0;j < src.cols;++j) {
			if (iLabel[j] == 0) {
				//δ���
				vector<Point2i> GrowBuffer;	//ջ�����ڴ洢������
				GrowBuffer.push_back(Point2i(i, j));
				Pointlabel.at<uchar>(i, j) = 1;
				int CheckResult = 1;	//�����жϽ�����Ƿ񳬳���С����1Ϊδ������2Ϊ����

				for (int z = 0;z < GrowBuffer.size();++z) {
					//��ջ�еĵ���������飬�������ĵ����ջ
					for (int q = 0;q < NeighborCount;++q) {
						CurX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurX >= 0 && CurX < src.rows&&CurY >= 0 && CurY < src.cols) {
							if (Pointlabel.at<uchar>(CurX, CurY) == 0) {
								GrowBuffer.push_back(Point2i(CurX, CurY));
								Pointlabel.at<uchar>(CurX, CurY) = 1;
							}
						}
					}
				}
				if (GrowBuffer.size() > Arealimit) {
					CheckResult = 2;
				}
				else
				{
					CheckResult = 1;
					++RemoveCount;
				}
				for (int z = 0;z < GrowBuffer.size();++z) {
					CurX = GrowBuffer.at(z).x;
					CurY = GrowBuffer.at(z).y;
					Pointlabel.at<uchar>(CurX, CurY) += CheckResult;
				}

			}
		}
	}


	for (int i = 0;i < src.rows;++i) {
		uchar* iData = src.ptr<uchar>(i);
		uchar* iDstData = dst.ptr<uchar>(i);
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0;j < src.cols;++j) {
			if (iLabel[j] == 2) {
				iDstData[j] = CheckMode;
			}
			else if (iLabel[j] == 3) {
				iDstData[j] = iData[j];
			}
		}
	}
	cout << "Removed:" << RemoveCount << endl;
}
*/
/*
void RemoveSmallRegion2(Mat& src, Mat& dst, int AreaLimit, int CheckMode) {
	//CheckMode:0����ȥ��������1����ȥ��������
	dst = src.clone();
	vector<vector<Point> >contours(100);
	findContours(dst.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int k = 0;k < contours.size();++k) {
		if ((int)contourArea(contours[k], false) < AreaLimit) {
			drawContours(dst, contours, k, Scalar((1 - CheckMode) * 255), FILLED);
		}
		//cout << k << ":" << contourArea(contours[k], false)<<endl;
	}
}
*/

#endif