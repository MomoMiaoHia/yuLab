#pragma once
#include "qtlibs.h"
#include "oplibs.h"

using namespace cv;

class myDlg : public QDialog
{
	Q_OBJECT

public:
	myDlg(QWidget *parent = 0);
	~myDlg();
	void setData(Mat& img);

signals:
	void sendbg();

protected slots:
	void onSelected();
	void onCacelled();

private:
	QVBoxLayout *bg_layout;
	QLabel *bgLabel;
	VideoCapture *bg_cap;
	QPushButton *selected;
	QPushButton *cacelled;
	QSlider *bg_slider;
	Mat bg;
};
