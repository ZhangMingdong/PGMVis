
#include <QtGui>

#include "BGLink.h"
#include "BGRect.h"

#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QColorDialog>
#include <QInputDialog>


BGRect::BGRect() :BGRectBase()
{
// 	myTextColor = Qt::darkGreen;
// 	myOutlineColor = Qt::darkBlue;
// 	myBackgroundColor = Qt::white;

// 	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

BGRect::~BGRect()
{
}

QRectF BGRect::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath BGRect::shape() const
{
	QRectF rect = outlineRect();

	QPainterPath path;
	path.addRect(rect);
// 	path.addRoundRect(rect, roundness(rect.width()),
// 		roundness(rect.height()));
	return path;
}

void BGRect::drawGraphic(QPainter *painter)
{
	painter->drawRect(outlineRect());
}

void BGRect::paintSelectionOutline(QPainter *painter)
{
	painter->drawRect(outlineRect());
}

void BGRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

int BGRect::roundness(double size) const
{
	const int Diameter = 12;
	return 100 * Diameter / int(size);
}

QPointF BGRect::getLinkPos(int index){
	switch (index)
	{
	case 0:
		return QPointF(this->x(), this->y() - this->height() / 2.0);
		break;
	case 1:
		return QPointF(this->x() - this->width() / 2.0, this->y());
		break;
	case 2:
		return QPointF(this->x(), this->y() + this->height() / 2.0);
		break;
	case 3:
		return QPointF(this->x() + this->width() / 2.0, this->y());
		break;
	default:
		return this->pos();
	}
}
QPointF BGRect::getLinkPos(QPointF ptTarget){
	QPointF pt[4];
	qreal len[4];
	pt[0] = QPointF(this->x(), this->y() - this->height() / 2.0);
	pt[1] = QPointF(this->x() - this->width() / 2.0, this->y());
	pt[2] = QPointF(this->x(), this->y() + this->height() / 2.0);
	pt[3] = QPointF(this->x() + this->width() / 2.0, this->y());
	len[0] = QLineF(pt[0], ptTarget).length();
	len[1] = QLineF(pt[1], ptTarget).length();
	len[2] = QLineF(pt[2], ptTarget).length();
	len[3] = QLineF(pt[3], ptTarget).length();
	int min = 0;
	for (int i = 1; i < 4;i++)
	{
		if (len[i]<len[min])
		{
			min = i;
		}
	}
	return pt[min];
}