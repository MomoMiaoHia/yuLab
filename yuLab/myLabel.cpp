#include "myLabel.h"

myLabel::myLabel(QWidget *parent)
	: QLabel(parent)
{
	startcut = false;
	startdm = false;
	scalling = false;
}

void myLabel::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_isMousePress = true;
		m_beginPoint = event->pos();
	}

	return QWidget::mousePressEvent(event);
}

void myLabel::paintEvent(QPaintEvent *event) {
	if (!startcut&&!startdm)
		return QLabel::paintEvent(event);
	QLabel::paintEvent(event);
	QPainter painter(this);
	painter.setPen(QPen(Qt::blue, 2));
	if (startcut)
		painter.drawRect(selectedRect);
	else
		painter.drawLine(selectedLine);
}

void myLabel::mouseReleaseEvent(QMouseEvent *event) {
	m_endPoint = event->pos();
	selectedRect = QRect::QRect(m_beginPoint, m_endPoint);
	selectedLine = QLineF::QLineF(m_beginPoint, m_endPoint);
	m_beginPoint = m_endPoint = QPoint(0, 0);
	m_isMousePress = false;
	if(startcut)
		emit(cutFinished());
	return QWidget::mouseReleaseEvent(event);
}