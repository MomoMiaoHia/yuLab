#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_yuLab.h"
#include "qtlibs.h"
#include "videoTool.h"
#include "myLabel.h"
#include<opencv2\opencv.hpp>

using namespace cv;

class yuLab : public QMainWindow
{
	Q_OBJECT

public:
	yuLab(QWidget *parent = Q_NULLPTR);

	QString fileName;
	string videoName;
	videoTool *vtool;

	void createActions();   //创建动作
	void createMenus();     //创建菜单
	//void createToolbars();  //创建工具栏
	void loadFile(QString filename);
	void display(Mat mat);//在label中显示图像
	

private:
	Ui::yuLabClass ui;

	QEventLoop loop;
	//menus
	QMenu *fileMenu;
	//菜单项
	QAction *openFileAction;
	
	QImage *image;

	//leftlayout
	QVBoxLayout *LeftLayout;
	//QVideoWidget *vp;
	//QWidget *tab;
	myLabel *ImageLabel;
	QSlider *positionSlider;
	bool m_isPressed,m_cap;
	QPoint m_startPoint, m_endPoint;
	int m_cutWidth, m_cutHeight;
	//bottomlayout
	QHBoxLayout *BottomLayout;
	QPushButton *StPa;
	QPushButton *Stop;
	QPushButton *Pic_cut;
	//rightlayout
	/*QGridLayout *RightLayout;
	QLabel *TextLabel;*/
	bool mstatus;   //播放状态
	QTimer *timer;
	int maxtimeV = 1000;  //进度条最大值


	//函数
	void fitcut();

	

signals:
	void fileLoadSuccuss();
	void startPro();
	
	

protected slots:
	void ShowOpenFile();
	void togglePlayback();  //控制播放暂停
	//void onTimerOut();  //刷新进度条的值
	//void sliderClicked();
	//void sliderMove(); //进度条移动
	//void sliderRelease();  //释放滑块
	//鼠标操作
	void onCutFinished();
	//按钮控制
	void toggleStop();
	//视频处理
	void toggleCut();	//截屏
	void recoPro();
	
};
