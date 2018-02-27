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
		bi_p = 0;
		ez_p = 0;
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
			/*cout << "������ӵ�������";
			cin >> box_cols;
			cout << "������ӵ�������";
			cin >> box_rows;*/
			//status = vector<int >(box_cols*box_rows, 0);
			break;
		default:
			break;
		}
	}
}

vector<Rect> videoTool::getRects(const Mat& _img) {
	//ȷ��Ϊ�Ҷ�ͼ
	Mat img = _img.clone(),rawRoi=_img.clone(),img_t;
	if (bi_p)
		onBi(img,img_t);
	else
		img_t = img.clone();
	if (_img.channels() == 3)
		cvtColor(_img, img_t, CV_BGR2GRAY);
	//absdiff(img1, background, img);
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	vector<Rect> result;
	//int kk = 0;
	if (ez_p)
		onEz(img_t, img_t);
	else
		threshold(img_t, img_t, 0, 255, THRESH_BINARY + THRESH_OTSU);
	RemoveSmallRegion2(img_t, img_t, 100, 1);
	vector<vector<Point> >contours;
	findContours(img_t, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0;i < contours.size();++i) {
		Rect rect = boundingRect(contours[i]);
		result.push_back(rect);
	}
	//imshow(WIN2, img);
	return result;
}

void videoTool::RemoveSmallRegion2(Mat& src, Mat& dst, int AreaLimit, int CheckMode) {
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

void videoTool::onBi(Mat& src,Mat& dst) {
	bilateralFilter(src, dst, bi_p, 2 * bi_p, bi_p / 2);
}

void videoTool::onEz(Mat& src, Mat& dst) {
	if (src.channels() == 3)
		cvtColor(src, dst, CV_BGR2GRAY);
	else
		dst = src.clone();
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	threshold(dst, dst, ez_p, 255, THRESH_BINARY);
}

void videoTool::countCentroid(vector<vector<Point>>&contours, vector<Point>&Centroid) {
	vector<Moments>mu(contours.size());
	for (int i = 0; i < contours.size(); ++i) {
		mu[i] = moments(contours[i], false);
		Centroid[i] = Point(int(mu[i].m10 / mu[i].m00), int(mu[i].m01 / mu[i].m00));
	}
}