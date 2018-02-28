#pragma once
#include "qtlibs.h"
#include "oplibs.h"

using namespace cv;

class smthDlg : public QDialog
{
	Q_OBJECT

public:
	smthDlg(QWidget *parent = 0);
	~smthDlg();
	void setData(Mat& img,Mat& backg);
	void getData(int& valueb, int& valuee);
	void updateImage(QImage& bi_img, QImage& ez_img);

signals:
	void sendP(int p);
	void sendE(int e);

private slots:
	void onSetP();
	void onSetE();
	void sliderPressedp();
	void sliderMovep(); //进度条移动
	void sliderReleasep();  //释放滑块
	void sliderPressede();
	void sliderMovee(); //进度条移动
	void sliderReleasee();  //释放滑块
	void updateProssess();

private:
	QVBoxLayout *para1, *para2;
	QHBoxLayout *bg_layout,*label_layout;
	QLabel *biLabel, *ezLabel, *c1, *p1, *c2, *p2;
	VideoCapture *bg_cap;
	QPushButton *set_p;
	QPushButton *set_e;
	QSlider *bg_slider, *bi, *ez;//     滤波，二值
	int pbi, pez;
	Mat raw,bg;
	QImage bi_img, ez_img;

	void startSlider();

};

