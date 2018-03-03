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
		totalframe_n = capture.get(CV_CAP_PROP_FRAME_COUNT)-1;
		currentframe_n = 0;
		counts = 0;
		bi_p = 30;
		ez_p = 0;
		stop = false;
		useMouse = true;
		status = new vector<int>(100, 0);
		havebg = false;
		haveTbg = false;
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

vector<RotatedRect> videoTool::getRects(const Mat& _img) {
	//确保为灰度图
	Mat img = _img.clone(),rawRoi=_img.clone(),img_t=_img.clone();
	if (havebg){
		absdiff(img, background, img_t);
	}
	if (bi_p)
		onBi(img_t,img_t);
	else
		img_t = img.clone();
	if (img_t.channels() == 3)
		cvtColor(img_t, img_t, CV_BGR2GRAY);
	//absdiff(img1, background, img);
	
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	vector<RotatedRect> result;
	//int kk = 0;
	if (ez_p)
		onEz(img_t, img_t);
	else
		threshold(img_t, img_t, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
	//bitwise_not(img_t, img_t);
	RemoveSmallRegion2(img_t, img_t, 50, 1);
	bitwise_not(img_t, img_t);
	vector<vector<Point> >contours;
	findContours(img_t, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0;i < contours.size();++i) {
		//Rect rect = boundingRect(contours[i]);
		RotatedRect mrect = minAreaRect(contours[i]);
		if(judgeRect(mrect))
			result.push_back(mrect);
	}
	sortRect(result);
	//if(result.size())
		//minRect=minAreaRect()
	if (result.size()>1)
	/*if(result.size()&&result[0].area()>200&&result[0].area()<1000)*/
		updateBg(result);
	//imshow(WIN2, img);
	return result;
}

void videoTool::RemoveSmallRegion2(Mat& src, Mat& dst, int AreaLimit, int CheckMode) {
	//CheckMode:0代表去除黑区域，1代表去除白区域；
	dst = src.clone();
	vector<vector<Point> >contours;
	findContours(dst.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int k = 0;k < contours.size();++k) {
		if ((int)contourArea(contours[k], false) < AreaLimit) {
			drawContours(dst, contours, k, Scalar((1 - CheckMode) * 255), FILLED);
		}
		//cout << k << ":" << contourArea(contours[k], false)<<endl;
	}
}

void videoTool::onBi(Mat& src,Mat& dst) {
	//bilateralFilter(src, dst, bi_p, 2 * bi_p, bi_p / 2);
	if (src.channels() == 3)
		cvtColor(src, dst, CV_BGR2GRAY);
	else
		dst = src.clone();
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));

	threshold(dst, dst, bi_p, 255, THRESH_TOZERO);
}

void videoTool::onEz(Mat& src, Mat& dst) {
	if (src.channels() == 3)
		cvtColor(src, dst, CV_BGR2GRAY);
	else
		dst = src.clone();
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	threshold(dst, dst, ez_p, 255, THRESH_BINARY_INV);
}

void videoTool::countCentroid(vector<vector<Point>>&contours, vector<Point>&Centroid) {
	vector<Moments>mu(contours.size());
	for (int i = 0; i < contours.size(); ++i) {
		mu[i] = moments(contours[i], false);
		Centroid[i] = Point(int(mu[i].m10 / mu[i].m00), int(mu[i].m01 / mu[i].m00));
	}
}

void videoTool::countCenter(vector<Rect>& rects, vector<Point2f>&center) {
	int n = rects.size();
	for (int i = 0; i < 1; ++i) {
		center.push_back( Point2f(rects[i].x + 0.5*rects[i].width, rects[i].y + 0.5*rects[i].height));
	}
}

bool videoTool::judgeRect(const RotatedRect&rect) {
	if (rect.size.width>20||rect.size.height>20) {
		return true;
	}
	return false;
}

void videoTool::updateBg(const vector<RotatedRect>&rects) {
	if (!havebg)
		return;
	int n = rects.size();
	Mat mask = Mat::zeros(background.size(), CV_8UC1);
	Mat t_bg = background.clone();
	//Mat temp(background.rows,background.cols,background.type());
	//if (n == 1 && rects[0].area() > 200 && rects[0].area() < 1000) {

	//}
	vector<Mat> tempImg(n);
	for (int i = 0; i < 1; ++i) {
		Rect t_rect = rects[i].boundingRect();
		correctRect(t_rect);
		mask(t_rect).setTo(255);
		background.copyTo(tempImg[i], mask);
		t_bg = currentFrame.clone();
		t_bg.setTo(0, mask);
		background = t_bg + tempImg[i];
		//t_bg(rects[i].boundingRect()).copyTo(tempImg[i]);
	}
	/*background = currentFrame.clone();
	for (int i = 0; i < 1; ++i) {
		tempImg[i].copyTo(background(rects[i].boundingRect()));
	}*/
}

void videoTool::sortRect(vector<RotatedRect>&Rects) {
	int n = Rects.size();
	for (int i = 0; i < n-1; ++i) {
		for (int j = 0; j < n-i-1; ++j) {
			if (Rects[j].size.area() < Rects[j + 1].size.area())
				swap(Rects[j], Rects[j + 1]);
		}
	}

}

/*int videoTool::maxMRect(vector<RotatedRect>& rects) {
	int t = 0, n = rects.size();
	float si=0;
	if (!n)
		return n;
	for (int i = 0; i < n; ++i) {
		if(rects[i].)
	}

	void yuLab::correctRect(Rect& r) {
	if (r.x < 0)
		r.x = 0;
	if (r.x + r.width > vtool->initRect.width)
		r.width = vtool->initRect.width - r.x;
	if (r.y < 0)
		r.y = 0;
	if (r.y + r.height > vtool->initRect.height)
		r.height = vtool->initRect.height - r.y;
}
}*/

void videoTool::correctRect(Rect& r) {
	if (r.x < 0)
		r.x = 0;
	if (r.x + r.width > initRect.width)
		r.width = initRect.width - r.x;
	if (r.y < 0)
		r.y = 0;
	if (r.y + r.height > initRect.height)
		r.height = initRect.height - r.y;
}