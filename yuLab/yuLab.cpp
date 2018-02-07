#pragma once
#include "yuLab.h"
#include "oplibs.h"
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

using namespace cv;

yuLab::yuLab(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle(tr("MainWindow"));
	
	/**���**/
	LeftLayout = new QVBoxLayout();
	
	// ImageLabel=new QLabel(tr("video play"));
	
	positionSlider = new QSlider(Qt::Horizontal);
	timer = new QTimer();
	timer->setInterval(1000);   //1000msˢ��һ�ν�����
	image = new QImage();
	//tab = new QWidget();
	ImageLabel = new myLabel();
	ImageLabel->setScaledContents(true);
	ImageLabel->setMinimumSize(400, 300);

	LeftLayout->addWidget(ImageLabel);
	//vp->setAspectRatioMode(Qt::IgnoreAspectRatio);
	mstatus = false;
	LeftLayout->addWidget(positionSlider);
	positionSlider->setEnabled(false);
	//LeftLayout->addWidget(ImageLabel);
	//vtool = new videoTool();


	/*�²�*/
	BottomLayout = new QHBoxLayout();
	StPa = new QPushButton(tr("Play"));
	Stop = new QPushButton(tr("Stop"));
	Pic_cut = new QPushButton(tr("Cut"));
	StPa->setEnabled(false);
	//Stop->setEnabled(false);
	//Pic_cut->setEnabled(false);
	BottomLayout->addStretch();
	BottomLayout->addWidget(StPa);
	BottomLayout->addWidget(Stop);
	BottomLayout->addWidget(Pic_cut);
	BottomLayout->addStretch();
	/**�Ҳ�**/
	/* RightLayout=new QGridLayout();
	TextLabel=new QLabel(tr("TextLabel"));
	RightLayout->addWidget(TextLabel);*/
	/**����ͣ����**/
	QDockWidget *dock = new QDockWidget(tr("targets info"), this);
	dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	dock->setAllowedAreas(Qt::RightDockWidgetArea);
	QTextEdit *tel = new QTextEdit();
	tel->setText(tr("window1"));
	dock->setWidget(tel);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	/*----------*/
	QGridLayout *mainLayout = new QGridLayout(this);

	mainLayout->setMargin(15);
	mainLayout->setSpacing(10);
	mainLayout->addLayout(LeftLayout, 0, 0);
	//mainLayout->addLayout(RightLayout,0,1);
	mainLayout->addLayout(BottomLayout, 1, 0, 1, 1);
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);

	/**������ **/

	QWidget* w = new QWidget(this);
	this->setCentralWidget(w);
	w->setLayout(mainLayout);
	createActions();
	createMenus();

}

void yuLab::createActions() {
	openFileAction = new QAction(tr("open"), this);
	//�¼�����
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(ShowOpenFile()));
	connect(StPa, SIGNAL(clicked()), this, SLOT(togglePlayback()));
	/*connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
	connect(positionSlider, SIGNAL(sliderPressed()), this, SLOT(sliderClicked()));
	connect(positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMove()));
	connect(positionSlider, SIGNAL(sliderReleased()), this, SLOT(sliderRelease()));*/
	connect(Stop, SIGNAL(clicked()), this, SLOT(toggleStop()));
	connect(Pic_cut, SIGNAL(clicked()), this, SLOT(toggleCut()));
	connect(ImageLabel, SIGNAL(cutFinished()), this, SLOT(onCutFinished()));
	connect(this, SIGNAL(startPro()), this, SLOT(recoPro()));
}

void yuLab::createMenus() {
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("file"));
	fileMenu->addAction(openFileAction);
}

void yuLab::fitcut() {
	if (ImageLabel->selectedRect.size() == QSize(0, 0))
		return;
	ImageLabel->scalling = true;
	vtool->initRect=Rect(ImageLabel->selectedRect.x(),ImageLabel->selectedRect.y(),ImageLabel->selectedRect.width(),ImageLabel->selectedRect.height());
	//rectangle(vtool->firstFrame, vtool->initRect, Scalar(0, 0, 255), 1);
}

void yuLab::ShowOpenFile() {
	fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()) {
		loadFile(fileName);
	}

}

void yuLab::loadFile(QString filename) {
	videoName = filename.toStdString();
	vtool = new videoTool(videoName);
	if (vtool->capture.isOpened()) {
		StPa->setEnabled(true);
		Stop->setEnabled(true);
		Pic_cut->setEnabled(true);
	}
}


void yuLab::togglePlayback() {
	if (vtool->capture.isOpened()) {
		if (vtool->pause) {
			vtool->pause = false;
			StPa->setText(tr("Pause"));
		}
		else {
			vtool->pause = true;
			StPa->setText(tr("Play"));
		}
	}
}

/*void yuLab::onTimerOut() {
	positionSlider->setValue(Player->position()*maxtimeV / Player->duration());
}

void yuLab::sliderClicked() {
	Player->setPosition(positionSlider->value()*Player->duration() / maxtimeV);
}

void yuLab::sliderMove() {
	timer->stop();
	Player->setPosition(positionSlider->value()*Player->duration() / maxtimeV);
}

void yuLab::sliderRelease() {
	timer->start();
}*/

void yuLab::display(Mat mat) {
	QImage img;
	if (mat.channels() == 3) {
		cvtColor(mat, mat, CV_BGR2RGB);
		img = QImage((const uchar*)(mat.data), mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_RGB888);
	}
	else {
		img = QImage((const uchar*)(mat.data), mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_Indexed8);
	}
	ImageLabel->setPixmap(QPixmap::fromImage(img));
	ImageLabel->resize(ImageLabel->pixmap()->size());
	ImageLabel->show();
}

void yuLab::onCutFinished() {
	if(ImageLabel->startcut)
		display(vtool->firstFrame);
}

void yuLab::toggleStop() {
	if (!vtool->stop) {
			vtool->stop = true;
			Stop->setText(tr("Restart"));
	}/**/
	else {
		vtool->stop = false;
		Stop->setText(tr("Stop"));
		StPa->setText(tr("Pause"));
		emit(startPro());

	}
}

void yuLab::recoPro() {
	//if (vtool->stop)
	//	return;
	vtool->background = vtool->background(vtool->initRect).clone();
	while (!vtool->stop) {     //һֱѭ��
		if (!vtool->pause) {     //�������ͣ
						  //long long t0 = getTickCount();
			(vtool->capture) >> vtool->currentFrame;         //����һ֡
			if (!vtool->currentFrame.data)
				return;
			++vtool->counts;
			if (vtool->currentFrame.cols > 1000 && vtool->currentFrame.rows > 900)
				cv::resize(vtool->currentFrame, vtool->currentFrame, Size(), 0.65, 0.65);   //�ߴ���С��ԭ��һ��
			//�˲�
			//Mat lb = vtool->currentFrame.clone();
			//bilateralFilter(lb, vtool->currentFrame, 45, 90, 22);
			if(ImageLabel->scalling){
				vtool->currentFrame = vtool->currentFrame(vtool->initRect).clone();              //ȡROI
			}

																		//vector<Rect> rects = getRects(currentFrame, box_rows, box_cols);    //��ֻϺ����Ӿ��ο�
			vector<Rect> rects = vtool->getRects(vtool->currentFrame);
			for (size_t i = 0; i < rects.size(); ++i) {
				if (rects[i].area() == 0)
					continue;
				//cout << currentFrame(rects[i]).channels() << endl;
				int result = vtool->detect.judge(vtool->currentFrame(rects[i]).clone());      //��SVM�ж���Ϻ�����ƣ����Ļ��ǻ�ģ�
				if (result == 1) {    //���ŵ�
					(*vtool->status)[i] = 0;
					putText(vtool->currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				else {          //�ж�Ϊ����
					(*vtool->status)[i]++;
					if ((*vtool->status)[i] > vtool->dead_counts)
						putText(vtool->currentFrame, "died", rects[i].tl(), 1, 1, Scalar(255, 255, 255));
					else
						putText(vtool->currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				rectangle(vtool->currentFrame, rects[i], Scalar(0, 0, 255), 1);   //����Ϻ����Ӿ��ο�

			}
			//imshow(WIN1, currentFrame);
			display(vtool->currentFrame);/**/
		}
		//waitKey(1);
		//char ch = waitKey(1);
		//emit(startPro());
		QCoreApplication::processEvents();
	}
}

void yuLab::toggleCut() {
	if (ImageLabel->startcut) {
		ImageLabel->startcut = false;
		vtool->pause = false;
		Pic_cut->setText(tr("Cut"));
		StPa->setText(tr("Pause"));
		fitcut();
		Pic_cut->setEnabled(false);
		StPa->setEnabled(true);
		vtool->stop = false;
		emit(startPro());
	}
	else {
		ImageLabel->startcut = true;
		vtool->pause = true;
		Pic_cut->setText(tr("finish_cut"));
	}
}

