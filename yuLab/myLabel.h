#pragma once

#include "qtlibs.h"

class myLabel : public QLabel
{
	Q_OBJECT

public:
	explicit myLabel(QWidget *parent = 0);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	QPoint m_beginPoint, m_endPoint;
	bool scalling;//Ëõ·Å
	QRect selectedRect;
	bool startcut;

signals:
	void cutFinished();

private:
	bool m_isMousePress;

};
