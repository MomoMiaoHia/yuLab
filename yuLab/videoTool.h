#pragma once
#include "oplibs.h"
#include "XiaDetect.h"


using namespace cv;
using namespace std;

class videoTool
{
public:
	videoTool();
	videoTool(string VideoName);
	~videoTool();
	//bool isopen;
	string win = "Input";
	VideoCapture capture;
	double fps;
	bool pause;	//是否暂停
	unsigned int counts;	//当前帧数
	bool stop;
	vector<int>* status;
	int min_threshold = 1;       //超过died_min分钟连续保持死亡状态的虾，则判定为死亡
	int dead_counts;           //判定为死亡需要经过的帧数
	XiaDetect detect;	//虾死亡判定类
	Mat currentFrame;
	Mat firstFrame;
	//鼠标回调函数
	Rect initRect;
	bool flag;
	bool useMouse;
	void init(VideoCapture& capture);
	void onMouse(int event, int x, int y, int flags, void* param);
	vector<Rect> getRects(const Mat& _img);
	void RemoveSmallRegion2(Mat& src, Mat& dst, int AreaLimit, int CheckMode);
	
	
	
};/**/

