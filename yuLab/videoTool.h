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
	bool isupdate;	//�Ƿ��ڵ���������
	string win = "Input";
	VideoCapture capture;
	double fps;
	bool pause;	//�Ƿ���ͣ
	unsigned int totalframe_n;
	unsigned int currentframe_n;
	unsigned int counts;	//��ǰ֡��
	int bi_p;	//�˲�����
	int ez_p;	//��ֵ����
	bool stop;
	vector<int>* status;
	int min_threshold = 1;       //����died_min����������������״̬��Ϻ�����ж�Ϊ����
	int dead_counts;           //�ж�Ϊ������Ҫ������֡��
	XiaDetect detect;	//Ϻ�����ж���
	Mat currentFrame;
	Mat firstFrame;
	Mat background;	//	ǰ��
	Mat rawRoi;	//ԭʼͼ
	//Mat ezRoi;	//��ֵͼ
	//���ص�����
	Rect initRect;
	bool flag;
	bool useMouse;
	void init(VideoCapture& capture);
	void onMouse(int event, int x, int y, int flags, void* param);
	vector<Rect> getRects(const Mat& _img);
	void RemoveSmallRegion2(Mat& src, Mat& dst, int AreaLimit, int CheckMode);
	void onBi(Mat& src, Mat& dst);
	void onEz(Mat& src, Mat& dst);
	
};/**/

