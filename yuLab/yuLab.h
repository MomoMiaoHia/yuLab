#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_yuLab.h"
#include "qtlibs.h"
#include "videoTool.h"
#include "myLabel.h"
#include "myDlg.h"
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
	QAction *closeFileAction;
	QAction *bgRemoveAction;
	QAction *smothingAction;
	QAction *demarcateAction;

	
	QImage *image;

	//leftlayout
	QVBoxLayout *LeftLayout;
	//QVideoWidget *vp;
	//QWidget *tab;
	myLabel *ImageLabel;
	QSlider *positionSlider;	//��Ƶ������
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
	//QTimer *timer;
	int maxV = 1000;  //���������ֵ
	unsigned int selectLen;	//�궨ѡ������ֵ
	unsigned int inputLen;	//���볤��

	//bgremove�Ӵ���
	myDlg *bgRemovingDlg;
	myInputDlg *demarcateDlg;
	QDockWidget *demarcateDock;
	QVBoxLayout *input_layout;
	QLabel *tip1, *tip2, *tip3, *tip4, *tip5;
	QLineEdit *inputLenEdit;
	QPushButton *selected;
	QPushButton *lenSubmit;
	QWidget *detable, *mvtable;

	//����
	void fitcut();
	void startsilder();	//������Ƶ������
	
	

signals:
	void fileLoadSuccuss();
	void startPro();
	void updateSlider();	//���½�����
	
	

protected slots:
	void ShowOpenFile();
	void togglePlayback();  //���Ʋ�����ͣ
	void onUpdating();  //ˢ�½�������ֵ
	void sliderClicked();
	void sliderMove(); //�������ƶ�
	void sliderRelease();  //�ͷŻ���
	//������
	void onCutFinished();
	//��ť����
	void toggleStop();
	//��Ƶ����
	void toggleCut();	//����
	void recoPro();
	void createRemovingWin();
	void createDemarcatingWin();	
	void toggleDemarcate();	//�궨����
	void toggleSelect();
	//void onSelectFinished();
	void toggleLenSubmit();
};
