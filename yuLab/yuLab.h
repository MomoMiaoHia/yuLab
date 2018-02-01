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

	void createActions();   //��������
	void createMenus();     //�����˵�
	//void createToolbars();  //����������
	void loadFile(QString filename);
	void display(Mat mat);//��label����ʾͼ��
	

private:
	Ui::yuLabClass ui;

	QEventLoop loop;
	//menus
	QMenu *fileMenu;
	//�˵���
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
	bool mstatus;   //����״̬
	QTimer *timer;
	int maxtimeV = 1000;  //���������ֵ


	//����
	void fitcut();

	

signals:
	void fileLoadSuccuss();
	void startPro();
	
	

protected slots:
	void ShowOpenFile();
	void togglePlayback();  //���Ʋ�����ͣ
	//void onTimerOut();  //ˢ�½�������ֵ
	//void sliderClicked();
	//void sliderMove(); //�������ƶ�
	//void sliderRelease();  //�ͷŻ���
	//������
	void onCutFinished();
	//��ť����
	void toggleStop();
	//��Ƶ����
	void toggleCut();	//����
	void recoPro();
	
};
