#pragma once
#include "yuLab.h"
#include "oplibs.h"
#include "XiaDetect.h"
#include <QtWidgets/QApplication>


using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{

	/***********界面***************/
	QApplication a(argc, argv);
	yuLab w;
	w.show();
	
	/*************视频处理****************/
	//一些全局变量
	Mat currentFrame;
	const string WIN1 = "Input";
	const string WIN2 = "Fore";
	//int box_cols = 4, box_rows = 3;   //格子的行数和列数
	vector<int> status(100, 0);  //记录每只虾已经连续被判定为死亡状态的帧数
	int min_threshold = 1;       //超过died_min分钟连续保持死亡状态的虾，则判定为死亡
	int dead_counts;           //判定为死亡需要经过的帧数
	XiaDetect detect;	//虾死亡判定类
	w.videoName = "E:\\lab\\data\\temprate\\20180301_081226_21.mp4";
	w.vtool = new videoTool(w.videoName);
	if (w.vtool->capture.isOpened() == false) {
		cerr << "Load Video Failed!\n";
		return -1;
	}
//double fps = capture.get(CV_CAP_PROP_FPS);
	w.vtool->dead_counts = (int)(min_threshold * 60 * w.vtool->fps);  //连续超过dead_counts帧如果都为死亡状态，则判定为死亡  
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
	
	while (!w.vtool->stop) {     //一直循环
		if (!w.vtool->pause) {     //如果不暂停
						  //long long t0 = getTickCount();
			(w.vtool->capture) >> currentFrame;         //读入一帧
			if (!currentFrame.data)
				break;
			++w.vtool->counts;
			if (currentFrame.cols > 1000 && currentFrame.rows > 900)
				resize(currentFrame, currentFrame, Size(), 0.65, 0.65);   //尺寸缩小成原来一半
			currentFrame = currentFrame(w.vtool->initRect).clone();              //取ROI


																		//vector<Rect> rects = getRects(currentFrame, box_rows, box_cols);    //各只虾的外接矩形框
			vector<Rect> rects = w.vtool->getRects(currentFrame);
			for (size_t i = 0; i < rects.size(); ++i) {
				if (rects[i].area() == 0)
					continue;
				//cout << currentFrame(rects[i]).channels() << endl;
				int result = detect.judge(currentFrame(rects[i]).clone());      //用SVM判定该虾的姿势（死的还是活的）
				if (result == 1) {    //活着的
					status[i] = 0;
					putText(currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				else {          //判定为死亡
					status[i]++;
					if (status[i] > dead_counts)
						putText(currentFrame, "died", rects[i].tl(), 1, 1, Scalar(255, 255, 255));
					else
						putText(currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				rectangle(currentFrame, rects[i], Scalar(0, 0, 255), 1);   //画出虾的外接矩形框

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



