#include "myDlg.h"
#include <QDebug>

myDlg::myDlg(QWidget *parent)
	: QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowTitle("removing backgroud");
	bg_layout = new QVBoxLayout(this);

	bgLabel = new QLabel();
	bgLabel->setScaledContents(true);
	bgLabel->setMinimumSize(400, 300);


	selected = new QPushButton(tr("selected"));
	cacelled = new QPushButton(tr("cacell"));
	bg_slider = new QSlider(Qt::Horizontal);

	bg = new Mat();

	bg_layout->addWidget(bgLabel);
	bg_layout->addWidget(bg_slider);
	bg_layout->addWidget(selected);
	bg_layout->addWidget(cacelled);

	bg_layout->setMargin(15);
	bg_layout->setSpacing(10);
	bg_layout->setSizeConstraint(QLayout::SetFixedSize);

	connect(selected, SIGNAL(clicked()), this, SLOT(onSelected()));
	connect(cacelled, SIGNAL(clicked()), this, SLOT(onCacelled()));


}

myDlg::~myDlg()
{
}

void myDlg::onSelected() {
	emit(sendData(*bg));
	accept();
}

void myDlg::onCacelled() {
	reject();
}