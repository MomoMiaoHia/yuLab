#pragma once
#include "yuLab.h"
#include "oplibs.h"
#include "qtlibs.h"
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
	
	/**左侧**/
	LeftLayout = new QVBoxLayout();
	
	// ImageLabel=new QLabel(tr("video play"));
	
	positionSlider = new QSlider(Qt::Horizontal);
	/*timer = new QTimer();
	timer->setInterval(1000); */  //1000ms刷新一次进度条
	image = new QImage();
	//tab = new QWidget();
	ImageLabel = new myLabel();
	ImageLabel->setScaledContents(true);
	ImageLabel->setMinimumSize(400, 300);
	//ImageLabel->setMaximumSize(800, 600);
	LeftLayout->addStretch();
	LeftLayout->addWidget(ImageLabel);
	//vp->setAspectRatioMode(Qt::IgnoreAspectRatio);
	mstatus = false;
	LeftLayout->addWidget(positionSlider);
	LeftLayout->addStretch();
	positionSlider->setEnabled(false);
	//LeftLayout->addWidget(ImageLabel);
	//vtool = new videoTool();


	/*下侧*/
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
	/**右侧**/
	/* RightLayout=new QGridLayout();
	TextLabel=new QLabel(tr("TextLabel"));
	RightLayout->addWidget(TextLabel);*/
	
	/*----------*/
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	//mainLayout->setMargin(15);
	mainLayout->setSpacing(10);
	mainLayout->setContentsMargins(10, 10, 10, 10);
	mainLayout->addStretch();
	mainLayout->addLayout(LeftLayout);
	//mainLayout->addLayout(RightLayout,0,1);
	mainLayout->addLayout(BottomLayout);
	mainLayout->addStretch();
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);

	/**主界面 **/

	QWidget* w = new QWidget(this);
	this->setCentralWidget(w);
	w->setLayout(mainLayout);
	createActions();
	createMenus();

}

void yuLab::createActions() {
	openFileAction = new QAction(tr("open"), this);
	closeFileAction = new QAction(tr("close"), this);
	bgRemoveAction = new QAction(tr("remove background"), this);
	smoothingAction = new QAction(tr("smooth"), this);
	smoothingAction->setDisabled(true);
	demarcateAction = new QAction(tr("demarcate"), this);
	//事件关联
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(ShowOpenFile()));
	connect(bgRemoveAction, SIGNAL(triggered()), this, SLOT(createRemovingWin()));
	//nnect(demarcateAction, SIGNAL(triggered()), this, SLOT(createDemarcatingWin()));
	connect(demarcateAction, SIGNAL(triggered()), this, SLOT(toggleDemarcate()));
	connect(smoothingAction, SIGNAL(triggered()), this, SLOT(toggleSmooth()));
	connect(StPa, SIGNAL(clicked()), this, SLOT(togglePlayback()));
	connect(this, SIGNAL(updateSlider()), this, SLOT(onUpdating()));/**/
	connect(positionSlider, SIGNAL(sliderPressed()), this, SLOT(sliderClicked()));
	connect(positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMove()));
	connect(positionSlider, SIGNAL(sliderReleased()), this, SLOT(sliderRelease()));
	connect(Stop, SIGNAL(clicked()), this, SLOT(toggleStop()));
	connect(Pic_cut, SIGNAL(clicked()), this, SLOT(toggleCut()));
	connect(ImageLabel, SIGNAL(cutFinished()), this, SLOT(onCutFinished()));
	//connect(this, SIGNAL(smthDlg::closeWin()), this, SLOT(finishSmth()));
	connect(this, SIGNAL(startPro()), this, SLOT(recoPro()));

}

void yuLab::createMenus() {
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("file"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(closeFileAction);
	menuBar()->addAction(bgRemoveAction);
	menuBar()->addAction(smoothingAction);
	startsmth = false;
	menuBar()->addAction(demarcateAction);
}

void yuLab::fitcut() {
	if (ImageLabel->selectedRect.size() == QSize(0, 0))
		return;
	ImageLabel->scalling = true;
	vtool->initRect=Rect(ImageLabel->selectedRect.x(),ImageLabel->selectedRect.y(),ImageLabel->selectedRect.width(),ImageLabel->selectedRect.height());
	//rectangle(vtool->firstFrame, vtool->initRect, Scalar(0, 0, 255), 1);
	//display(vtool->firstFrame);
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

void yuLab::onUpdating() {
	positionSlider->setValue(vtool->currentframe_n);
}/**/

void yuLab::sliderClicked() {
	//Player->setPosition(positionSlider->value()*Player->duration() / maxtimeV);
	vtool->isupdate = true;
	vtool->currentframe_n = positionSlider->value();
	vtool->capture.set(CV_CAP_PROP_POS_FRAMES, positionSlider->value());
}

void yuLab::sliderMove() {
	//timer->stop();
	//Player->setPosition(positionSlider->value()*Player->duration() / maxtimeV);
	vtool->currentframe_n = positionSlider->value();
	vtool->capture.set(CV_CAP_PROP_POS_FRAMES, positionSlider->value());
	vtool->capture >> vtool->currentFrame;
	//display(vtool->currentFrame);
}

void yuLab::sliderRelease() {
	//timer->start();
	vtool->isupdate = false;
}

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
	/*if (startsmth) {
		Mat bi_pic,ez_pic;
		cvtColor(vtool->biRoi, bi_pic, CV_BGR2RGB);
		ez_pic = vtool->ezRoi.clone();
		QImage bi_img= QImage((const uchar*)(bi_pic.data), bi_pic.cols, bi_pic.rows, bi_pic.cols*bi_pic.channels(), QImage::Format_RGB888);
		QImage ez_img = QImage((const uchar*)(ez_pic.data), ez_pic.cols, ez_pic.rows, ez_pic.cols*ez_pic.channels(), QImage::Format_Indexed8);
		smoothingDlg->updateImage(bi_img,ez_img);
	}*/
}

void yuLab::onCutFinished() {
	if(ImageLabel->startcut)
		display(vtool->firstFrame);
	startsilder();
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
	//vtool->background = vtool->background(vtool->initRect).clone();
	while (!vtool->stop) {     //一直循环
		if (!vtool->pause&&!vtool->isupdate) {     //如果不暂停也不拖动进度条
						  //long long t0 = getTickCount();
			vtool->capture >> vtool->currentFrame;         //读入一帧
			if (!vtool->currentFrame.data)
				return;
			++vtool->counts;
			++vtool->currentframe_n;emit(updateSlider());
			if (vtool->currentFrame.cols > 800 && vtool->currentFrame.rows > 600)
				cv::resize(vtool->currentFrame, vtool->currentFrame, Size(), 0.65, 0.65);   //尺寸缩小成原来一半
			//滤波
			//Mat lb = vtool->currentFrame.clone();
			//bilateralFilter(lb, vtool->currentFrame, 45, 90, 22);
			if(ImageLabel->scalling){
				vtool->currentFrame = vtool->currentFrame(vtool->initRect).clone();              //取ROI
			}

																		//vector<Rect> rects = getRects(currentFrame, box_rows, box_cols);    //各只虾的外接矩形框
			if (startsmth){
				smoothingDlg->getData(vtool->bi_p, vtool->ez_p);
				smoothingDlg->setData(vtool->currentFrame,vtool->background);
			}
			vector<Rect> rects = vtool->getRects(vtool->currentFrame);
			for (size_t i = 0; i < rects.size(); ++i) {
				if (rects[i].area() == 0)
					continue;
				//cout << currentFrame(rects[i]).channels() << endl;
				int result = vtool->detect.judge(vtool->currentFrame(rects[i]).clone());      //用SVM判定该虾的姿势（死的还是活的）
				if (result == 1) {    //活着的
					(*vtool->status)[i] = 0;
					putText(vtool->currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				else {          //判定为死亡
					(*vtool->status)[i]++;
					if ((*vtool->status)[i] > vtool->dead_counts)
						putText(vtool->currentFrame, "died", rects[i].tl(), 1, 1, Scalar(255, 255, 255));
					else
						putText(vtool->currentFrame, "alive", rects[i].tl(), 1, 1, Scalar(0, 255, 0));
				}
				rectangle(vtool->currentFrame, rects[i], Scalar(0, 0, 255), 1);   //画出虾的外接矩形框

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

void yuLab::startsilder() {
	positionSlider->setEnabled(true);
	positionSlider->setMinimum(0);
	positionSlider->setMaximum(vtool->totalframe_n);
	positionSlider->setSingleStep((int)vtool->fps);
	positionSlider->setValue(vtool->currentframe_n);
}


void yuLab::createRemovingWin() {
	bgRemovingDlg = new myDlg(this);
	bgRemovingDlg->setModal(true);
	bgRemovingDlg->setData(vtool->currentFrame);
	bgRemovingDlg->show();	
	vtool->background = vtool->currentFrame;
	connect(bgRemovingDlg, SIGNAL(sendbg()), this, SLOT(receiveBg()));
}

void yuLab::createDemarcatingWin() {
	demarcateDlg = new myInputDlg(this);
	demarcateDlg->setModal(false);
	demarcateDlg->show();
}

void yuLab::toggleDemarcate() {
	/**右悬停窗口**/
	demarcateDock = new QDockWidget(tr("demarcate"), this);
	demarcateDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	demarcateDock->setAllowedAreas(Qt::RightDockWidgetArea);
	//dock->setMaximumSize(400, 300);
	detable = new QWidget();
	input_layout = new QVBoxLayout(demarcateDock);
	tip1= new QLabel(tr("actual lenth(cm):"));
	tip1->setScaledContents(true);
	tip2 = new QLabel(tr("inputed lenth(cm):"));
	tip3 = new QLabel();
	tip3->setScaledContents(true);
	tip4 = new QLabel(tr("selected lenth(pixel):"));
	tip5 = new QLabel();
	tip3->setScaledContents(true);
	inputLenEdit = new QLineEdit();
	selected = new QPushButton(tr("select"));
	lenSubmit = new QPushButton(tr("submit"));

	input_layout->addWidget(tip1);
	input_layout->addWidget(inputLenEdit);
	input_layout->addWidget(tip2);
	input_layout->addWidget(tip3);
	input_layout->addWidget(tip4);
	input_layout->addWidget(tip5);
	input_layout->addWidget(selected);
	input_layout->addWidget(lenSubmit);

	input_layout->setMargin(15);
	input_layout->setSpacing(10);
	input_layout->setSizeConstraint(QLayout::SetFixedSize);/**/

	detable->setLayout(input_layout);
	demarcateDock->setWidget(detable);
	//demarcateDock->setLayout(input_layout);
	addDockWidget(Qt::RightDockWidgetArea, demarcateDock);

	connect(selected, SIGNAL(clicked()), this, SLOT(toggleSelect()));
	connect(lenSubmit, SIGNAL(clicked()), this, SLOT(toggleLenSubmit()));
}

void yuLab::toggleSelect() {
	if (ImageLabel->startdm) {
		selected->setText("select");
		selectLen = ImageLabel->selectedRect.width() > 1 ? ImageLabel->selectedRect.width() : 0;
		ImageLabel->selectedRect = QRect::QRect(ImageLabel->m_beginPoint, ImageLabel->m_endPoint);
		tip5->setText(QString::number(selectLen, 10));
		ImageLabel->startdm = false;
	}
	else {
		selected->setText("finish select");
		ImageLabel->selectedRect = QRect::QRect(ImageLabel->m_beginPoint, ImageLabel->m_endPoint);
		ImageLabel->startdm = true;
	}
}

void yuLab::toggleLenSubmit() {
	QString tlen = inputLenEdit->text();
	bool ok;
	if (tlen.isEmpty()) {
		inputLen = ImageLabel->width();
		tip3->setText(QString::number(inputLen,10));
	}
	else {
		inputLen = tlen.toInt(&ok);
		tip3->setText(tlen);
	}

}

void yuLab::toggleSmooth() {
	smoothingDlg = new smthDlg(this);
	smoothingDlg->setModal(false);
	startsmth = true;
	smoothingDlg->setData(vtool->currentFrame,vtool->background);
	smoothingDlg->show();
	connect(smoothingDlg, SIGNAL(sendP(int)), this, SLOT(receiveP(int)));
	connect(smoothingDlg, SIGNAL(sendE(int)), this, SLOT(receiveE(int)));
	connect(smoothingDlg, SIGNAL(destroyed()), this, SLOT(onSmthDestroyed()));
}


void yuLab::receiveP(int p) {
	vtool->bi_p = p;
}

void yuLab::receiveE(int e) {
	vtool->ez_p = e;
}

void yuLab::onSmthDestroyed() {
	startsmth = false;
}

void yuLab::receiveBg() {
	vtool->havebg = true;
	smoothingAction->setDisabled(false);
}