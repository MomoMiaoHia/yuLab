#include "smthDlg.h"
#include <QDebug>


smthDlg::smthDlg(QWidget *parent)
	: QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowTitle("smooth");
	bg_layout = new QHBoxLayout(this);
	label_layout = new QHBoxLayout();
	para1 = new QVBoxLayout();
	para2 = new QVBoxLayout();

	biLabel = new QLabel();
	ezLabel = new QLabel();
	c1 = new QLabel(tr("Parameter1"));
	p1 = new QLabel(tr("0"));
	c2 = new QLabel(tr("Parameter2"));
	p2 = new QLabel(tr("0"));
	biLabel->setScaledContents(true);
	biLabel->setMinimumSize(400, 300);
	ezLabel->setScaledContents(true);
	ezLabel->setMinimumSize(400, 300);


	set_p = new QPushButton(tr("set"));
	set_e = new QPushButton(tr("set"));
	bg_slider = new QSlider(Qt::Horizontal);
	bi = new QSlider(Qt::Horizontal);
	ez = new QSlider(Qt::Horizontal);
	pbi = 0;
	pez = 0;

	//raw = new Mat();

	para1->addWidget(biLabel);
	//bg_layout->addWidget(bg_slider);
	para1->addWidget(c1);
	para1->addWidget(p1);
	para1->addWidget(bi);
	para1->addWidget(set_p);
	bg_layout->addLayout(para1);
	
	para2->addWidget(ezLabel);
	para2->addWidget(c2);
	para2->addWidget(p2);
	para2->addWidget(ez);
	para2->addWidget(set_e);
	bg_layout->addLayout(para2);
	
	//bg_layout->addWidget(selected);
	//bg_layout->addWidget(cacelled);

	bg_layout->setMargin(15);
	bg_layout->setSpacing(10);
	bg_layout->setSizeConstraint(QLayout::SetFixedSize);

	startSlider();

	connect(set_p, SIGNAL(clicked()), this, SLOT(onSetP()));
	connect(set_e, SIGNAL(clicked()), this, SLOT(onSetE()));

}


smthDlg::~smthDlg()
{
}

void smthDlg::onSetP() {
	//emit(sendData(*bg));
	//accept();
	emit sendP(pbi);
}

void smthDlg::onSetE() {
	//reject();
	emit sendE(pez);
}

void smthDlg::startSlider() {
	bi->setMaximum(50);
	bi->setSingleStep(1);
	p1->setText(QString::number(pbi, 10));
	bi->setTickPosition(QSlider::TicksAbove);
	connect(bi, SIGNAL(sliderPressed()), this, SLOT(sliderPressedp()));
	connect(bi, SIGNAL(sliderMoved()), this, SLOT(sliderMovep()));
	connect(bi, SIGNAL(sliderReleased()), this, SLOT(sliderReleasep()));
	ez->setMaximum(255);
	ez->setSingleStep(1);
	p2->setText(QString::number(pez, 10));
	ez->setTickPosition(QSlider::TicksAbove);
	connect(ez, SIGNAL(sliderPressed()), this, SLOT(sliderPressede()));
	connect(ez, SIGNAL(sliderMoved()), this, SLOT(sliderMovee()));
	connect(ez, SIGNAL(sliderReleased()), this, SLOT(sliderReleasee()));
}

void smthDlg::sliderPressedp() {
	pbi = bi->value();
	p1->setText(QString::number(pbi, 10));
	updateProssess();
}

void smthDlg::sliderMovep() {
	pbi = bi->value();
	p1->setText(QString::number(pbi, 10));
	updateProssess();
}

void smthDlg::sliderReleasep() {
	pbi = bi->value();
	p1->setText(QString::number(pbi, 10));
	updateProssess();
}

void smthDlg::sliderPressede() {
	pez = ez->value();
	p2->setText(QString::number(pez, 10));
	updateProssess();
}

void smthDlg::sliderMovee() {
	pez = ez->value();
	p2->setText(QString::number(pez, 10));
	updateProssess();
}

void smthDlg::sliderReleasee() {
	pez = ez->value();
	p2->setText(QString::number(pez, 10));
	updateProssess();
}

void smthDlg::setData(Mat& img) {
	raw = img.clone();
	updateProssess();
}


void smthDlg::getData(int& valueb, int& valuee) {
	valueb = pbi;
	valuee = pez;
}

void smthDlg::updateImage(QImage& bi_img, QImage& ez_img) {
	biLabel->setPixmap(QPixmap::fromImage(bi_img));
	biLabel->resize(biLabel->pixmap()->size());
	biLabel->show();
	ezLabel->setPixmap(QPixmap::fromImage(ez_img));
	ezLabel->resize(biLabel->pixmap()->size());
	ezLabel->show();
}

void smthDlg::updateProssess() {
	Mat t_bi,bi_pic, ez_pic;
	cvtColor(raw, t_bi, CV_BGR2RGB);
	bilateralFilter(t_bi, bi_pic, pbi, 2 * pbi, pbi / 2);
	ez_pic = bi_pic.clone();
	if (ez_pic.channels() == 3)
		cvtColor(ez_pic, ez_pic, CV_BGR2GRAY);
	Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	threshold(ez_pic, ez_pic, pez, 255, THRESH_BINARY);
	QImage bi_img = QImage((const uchar*)(bi_pic.data), bi_pic.cols, bi_pic.rows, bi_pic.cols*bi_pic.channels(), QImage::Format_RGB888);
	QImage ez_img = QImage((const uchar*)(ez_pic.data), ez_pic.cols, ez_pic.rows, ez_pic.cols*ez_pic.channels(), QImage::Format_Indexed8);
	updateImage(bi_img, ez_img);
}