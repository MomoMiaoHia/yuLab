#include "myInputDlg.h"

myInputDlg::myInputDlg(QWidget *parent)
	: QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowTitle("demarcate");
	input_layout = new QVBoxLayout(this);

	tip = new QLabel(tr("actual lenth(cm):"));
	tip->setScaledContents(true);
	inputLen = new QLineEdit();
	selected = new QPushButton(tr("confirm"));
	cacelled = new QPushButton(tr("cacell"));

	input_layout->addWidget(tip);
	input_layout->addWidget(inputLen);
	input_layout->addWidget(selected);
	input_layout->addWidget(cacelled);

	input_layout->setMargin(15);
	input_layout->setSpacing(10);
	input_layout->setSizeConstraint(QLayout::SetFixedSize);

	connect(selected, SIGNAL(clicked()), this, SLOT(onSelected()));
	connect(cacelled, SIGNAL(clicked()), this, SLOT(onCacelled()));

}

myInputDlg::~myInputDlg()
{
}

void myInputDlg::onSelected() {
	emit(sendText(inputLen->text()));
	accept();
}

void myInputDlg::onCacelled() {
	reject();
}