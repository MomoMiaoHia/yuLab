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
//对每一帧，计算这一帧中每一个格子中虾的外接矩形
/*
//鼠标回调函数
bool flag = false;
Rect initRect;
bool useMouse = true;
void onMouse(int event, int x, int y, int flags, void* param);   //鼠标回调函数，用来画框


																 //对一个视频源的第一帧，用鼠标画出矩形框，表示虾活动的区域
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

//输入图像是用鼠标框好的部分，且经过预处理了
vector<Rect> getRects(const Mat& _img, int box_rows, int box_cols) {
	//确保为灰度图
	Mat img = _img.clone();
	if (_img.channels() == 3)
		cvtColor(_img, img, CV_BGR2GRAY);
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	vector<Rect> result(box_rows*box_cols);
	int kk = 0;
	int width = _img.cols / box_cols;   //每只虾的方框的宽度以及高度
	int height = _img.rows / box_rows;
	for (int i = 0; i < box_rows; ++i) {
		for (int j = 0; j < box_cols; ++j) {
			Rect temp(j*width, i*height, width, height);
			Mat roi = img(temp); //当前的框
			threshold(roi, roi, 0, 255, THRESH_BINARY + THRESH_OTSU);
			morphologyEx(roi, roi, MORPH_OPEN, ele);  //
			vector<vector<Point> >contours;         //查找轮廓
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
	//确保为灰度图
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
	//checkmode：0代表去除黑区域，1代表去除白区域；neighbormode: 0代表4领域，1代表8领域；
	int RemoveCount = 0;	//去除的个数
							//记录每个像素点检验状态的标签，0代表未检查，1代表正在检查,2代表检查不合格（需要反转颜色），3代表检查合格或不需检查
	Mat Pointlabel = Mat::zeros(src.size(), CV_8UC1);
	CheckMode = 255 * (1 - CheckMode);


	for (int i = 0;i < src.rows;++i) {
		//标记不需要去除的颜色区域
		uchar* iData = src.ptr<uchar>(i);
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0;j < src.cols;++j) {
			if (iData[j] == CheckMode) {
				iLabel[j] = 3;
			}
		}
	}

	vector<Point2i> NeihborPos;		//记录领域点位置
									//(0,0)的4邻域
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));
	if (NeighborMode == 1) {
		//8邻域
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
				//未检测
				vector<Point2i> GrowBuffer;	//栈，用于存储生长点
				GrowBuffer.push_back(Point2i(i, j));
				Pointlabel.at<uchar>(i, j) = 1;
				int CheckResult = 1;	//用于判断结果（是否超出大小），1为未超出，2为超出

				for (int z = 0;z < GrowBuffer.size();++z) {
					//对栈中的点进行邻域检查，生长出的点加入栈
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
	//CheckMode:0代表去除黑区域，1代表去除白区域；
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