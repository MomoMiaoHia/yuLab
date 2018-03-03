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
	float lenthRatio=1;	//ʵ�ʳ��������ر�
	bool stop;
	vector<int>* status;
	int min_threshold = 1;       //����died_min����������������״̬��Ϻ�����ж�Ϊ����
	int dead_counts;           //�ж�Ϊ������Ҫ������֡��
	float t_tick;
	vector<float> ticks;	//ʱ��
	float timestep = 500;//ʱ����
	XiaDetect detect;	//Ϻ�����ж���
	Mat currentFrame;
	Mat firstFrame;
	bool havebg;
	Mat background;	//	ǰ��
	Mat rawRoi;	//ԭʼͼ
	//�����������
	vector<Point2f> xia_centers;
	vector<float> journey;
	vector<float> speed;
	vector<float> acspeed;
	vector<float> angel;
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
	void countCentroid(vector<vector<Point>>&contours, vector<Point>&Centroid);	//��������
	void countCenter(vector<Rect>& rects, vector<Point2f>&center);	//��������
	bool judgeRect(const Rect&rect);
	void updateBg(const vector<Rect>&rects);
	void sortRect(vector<Rect>&Rects);
	
};/**/

