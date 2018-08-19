
#include <QtGui>

#include "BGLink.h"
#include "BGSphere.h"

#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QColorDialog>
#include <QInputDialog>


BGSphere::BGSphere() :BGRectBase()
{
	
}

BGSphere::~BGSphere()
{

}

QRectF BGSphere::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath BGSphere::shape() const
{
	QRectF rect = outlineRect();

	QPainterPath path;
	path.addRect(rect);
	return path;
}

void BGSphere::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

void BGSphere::drawGraphic(QPainter *painter)
{
	painter->drawEllipse(outlineRect());

// 	painter->drawArc(outlineRect(), 0, 16*90);
}

void BGSphere::paintSelectionOutline(QPainter *painter)
{
	painter->drawEllipse(outlineRect());
}