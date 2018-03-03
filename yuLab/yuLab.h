#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_yuLab.h"
#include "qtlibs.h"
#include "videoTool.h"
#include "myLabel.h"
#include "myDlg.h"
#include "smthDlg.h"
#include<opencv2\opencv.hpp>

using namespace cv;

class yuLab : public QMainWindow
{
	Q_OBJECT

public:
	yuLab(QWidget *parent = Q_NULLPTR);

	QString fileName;
	string videoPath,videoName;
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
	QAction *closeFileAction;
	QAction *saveFileAction;
	QAction *bgRemoveAction;
	QAction *smoothingAction;
	QAction *demarcateAction;

	
	QImage *image;

	//leftlayout
	QVBoxLayout *LeftLayout;
	//QVideoWidget *vp;
	//QWidget *tab;
	myLabel *ImageLabel;
	QSlider *positionSlider;	//视频进度条
	QLabel *positionV;
	bool m_isPressed,m_cap;
	QPoint m_startPoint, m_endPoint;
	int m_cutWidth, m_cutHeight;
	//bottomlayout
	QHBoxLayout *BottomLayout;
	QPushButton *StPa;
	QPushButton *Stop;
	QPushButton *Pic_cut;
	QPushButton *Start_Calculate;
	//rightlayout
	/*QGridLayout *RightLayout;
	QLabel *TextLabel;*/
	bool mstatus;   //播放状态
	bool startDetect;	//检测状态
	bool isCalculating;	//计算数值状态
	//QTimer *timer;
	int maxV = 1000;  //进度条最大值
	float selectLen;	//标定选择像素值
	float inputLen;	//输入长度

	//bgremove子窗口
	myDlg *bgRemovingDlg;

	//smooth子窗口
	smthDlg *smoothingDlg;
	bool startsmth;

	//demarcate停靠窗口
	myInputDlg *demarcateDlg;
	QDockWidget *demarcateDock;
	QVBoxLayout *input_layout;
	QLabel *tip1, *tip2, *tip3, *tip4, *tip5;
	QLineEdit *inputLenEdit;
	QPushButton *selected;
	QPushButton *lenSubmit;
	QWidget *detable, *mvtable;

	

	//函数
	void fitcut();
	void startsilder();	//启动视频进度条
	
	

signals:
	void fileLoadSuccuss();
	void startPro();
	void updateSlider();	//更新进度条
	
	

protected slots:
	void ShowOpenFile();
	void togglePlayback();  //控制播放暂停
	void toggleCalculate();	//计算数据
	void onUpdating();  //刷新进度条的值
	void sliderClicked();
	void sliderMove(); //进度条移动
	void sliderRelease();  //释放滑块
	//鼠标操作
	void onCutFinished();
	//按钮控制
	void toggleStop();
	//视频处理
	void toggleCut();	//截屏
	void recoPro();
	void createRemovingWin();
	void createDemarcatingWin();	
	void toggleDemarcate();	//标定窗口
	void toggleSelect();
	//void onSelectFinished();
	void toggleLenSubmit();
	void toggleSmooth();
	void receiveP();
	//void receiveE(int e);
	void onSmthDestroyed();
	void receiveBg();
	void saveFile();
};
