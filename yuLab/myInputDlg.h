#pragma once

#include <QDialog>
#include "qtlibs.h"

class myInputDlg : public QDialog
{
	Q_OBJECT

public:
	myInputDlg(QWidget *parent = 0);
	~myInputDlg();

signals:
	void sendText(QString bg);

protected slots:
	void onSelected();
	void onCacelled();

private:
	QVBoxLayout *input_layout;
	QLabel *tip;
	QLineEdit *inputLen;
	QPushButton *selected;
	QPushButton *cacelled;
;

};
