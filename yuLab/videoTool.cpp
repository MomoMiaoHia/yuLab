#pragma once
#include "videoTool.h"
#include "oplibs.h"

using namespace std;
using namespace cv;

videoTool::videoTool()
{
	stop = true;
}

videoTool::videoTool(string VideoName) {
	capture.open(VideoName);
	if (!capture.isOpened())
		videoTool();
	else {
		isupdate = false;
		fps = capture.get(CV_CAP_PROP_FPS);
		pause = true;
		totalframe_n = capture.get(CV_CAP_PROP_FRAME_COUNT);
		currentframe_n = 0;
		counts = 0;
		stop = false;
		useMouse = true;
		status = new vector<int>(100, 0);
	}
}


videoTool::~videoTool()
{
}

void videoTool::init(VideoCapture& capture) {
	//void mouseWrapper(int event, int x, int y, int flags, void* param);
	capture >> firstFrame;

	if (firstFrame.cols>1000 && firstFrame.rows>900)
		resize(firstFrame, firstFrame, Size(), 0.65, 0.65);


	/*namedWindow(win);
	setMouseCallback(win, mouseWrapper,this);
	imshow(win, firstFrame);
	waitKey();*/
}

void mouseWrapper(int event, int x, int y, int flags, void* param) {
	videoTool *vt = (videoTool*)(param);
	vt->onMouse(event, x, y, flags, param);
}

void videoTool::onMouse(int event, int x, int y, int flags, void* param) {
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
			rectangle(firstFrame, initRect, Scalar(0, 0, 255), 1);
			imshow("Input", firstFrame);
			useMouse = false;
			/*cout << "输入格子的列数：";
			cin >> box_cols;
			cout << "输入格子的行数：";
			cin >> box_rows;*/
			//status = vector<int >(box_cols*box_rows, 0);
			break;
		default:
			break;
		}
	}
}

vector<Rect> videoTool::getRects(const Mat& _img) {
	//确保为灰度图
	Mat img1 = _img.clone(), img;
	if (_img.channels() == 3)
		cvtColor(_img, img, CV_BGR2GRAY);
	//absdiff(img1, background, img);
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

void videoTool::RemoveSmallRegion2(Mat& src, Mat& dst, int AreaLimit, int CheckMode) {
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

