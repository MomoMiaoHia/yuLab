#pragma once
#include "yuLab.h"
#include "oplibs.h"
#include "XiaDetect.h"
#include <QtWidgets/QApplication>


using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{

	/***********����***************/
	QApplication a(argc, argv);
	yuLab w;
	w.show();
	
	/*************��Ƶ����****************/
	//һЩȫ�ֱ���
	Mat currentFrame;
	const string WIN1 = "Input";
	const string WIN2 = "Fore";
	//int box_cols = 4, box_rows = 3;   //���ӵ�����������
	vector<int> status(100, 0);  //��¼ÿֻϺ�Ѿ��������ж�Ϊ����״̬��֡��
	int min_threshold = 1;       //����died_min����������������״̬��Ϻ�����ж�Ϊ����
	int dead_counts;           //�ж�Ϊ������Ҫ������֡��
	XiaDetect detect;	//Ϻ�����ж���
	w.videoName = "E:\\lab\\data\\temprate\\20180301_081226_21.mp4";
	w.vtool = new videoTool(w.videoName);
	if (w.vtool->capture.isOpened() == false) {
		cerr << "Load Video Failed!\n";
		return -1;
	}
//double fps = capture.get(CV_CAP_PROP_FPS);
	w.vtool->dead_counts = (int)(min_threshold * 60 * w.vtool->fps);  //��������dead_counts֡�����Ϊ����״̬�����ж�Ϊ����  
	//capture.set(CV_CAP_PROP_POS_FRAMES, 6700);
	/*w.vtool->capture.set(CV_CAP_PROP_POS_FRAMES, 2 * 60 * w.vtool->fps);
	w.vtool->capture >> w.vtool->background;
	resize(w.vtool->background, w.vtool->background, Size(), 0.65, 0.65);
	cvtColor(w.vtool->background, w.vtool->background, CV_BGR2GRAY);*/
	//w.vtool->capture.set(CV_CAP_PROP_POS_FRAMES, 3 * 60 * w.vtool->fps);
	w.vtool->currentframe_n = 3 * 60 * w.vtool->fps;
	w.vtool->init((w.vtool->capture));
	
	w.display(w.vtool->firstFrame);


	//double time = 0;
	/*w.recoPro();
	
	while (!w.vtool->stop) {     //һֱѭ��
		if (!w.vtool->pause) {     //�������ͣ
						  //long long t0 = getTickCount();
			(w.vtool->capture) >> currentFrame;         //����һ֡
			if (!currentFrame.data)
				break;
			++w.vtool->counts;
			if (currentFrame.cols > 1000 && currentFrame.rows > 900)
				resize(currentFrame, currentFrame, Size(), 0.65, 0.65);   //�ߴ���С��ԭ��һ��
			currentFrame = currentFrame(w.vtool->initRect).clone();              //ȡROI


																		//vector<Rect> rects = getRects(currentFrame, box_rows, box_cols);    //��ֻϺ����Ӿ��ο�
			vector<Rect> rects = w.vtool->getRects(currentFrame);
			for (size_t i = 0; i < rects.size(); ++i) {
				if (rects[i].area() == 0)
					continue;
				//cout << currentFrame(rects[i]).channels() << endl;
				int result = detect.judge(currentFrame(rects[i]).clone());      //��SVM�ж���Ϻ�����ƣ����Ļ��ǻ�ģ�
				if (result == 1) {    //���ŵ�
					status[i] = 0;
					putText(currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				else {          //�ж�Ϊ����
					status[i]++;
					if (status[i] > dead_counts)
						putText(currentFrame, "died", rects[i].tl(), 1, 1, Scalar(255, 255, 255));
					else
						putText(currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				rectangle(currentFrame, rects[i], Scalar(0, 0, 255), 1);   //����Ϻ����Ӿ��ο�

			}
			//imshow(WIN1, currentFrame);
			w.display(currentFrame);
		}
		char ch = waitKey(1);
	}*/
	//cout << "Video Over!\n";
	/*	
	destroyAllWindows();*/
	int ret = a.exec();
	return ret;
}



