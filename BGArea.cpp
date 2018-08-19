#include "BGArea.h"
#include <QPainter>


BGArea::BGArea(int x, int y, int w, int h) :_x(x), _y(y), _w(w), _h(h)
{
}


BGArea::~BGArea()
{
}
QRectF BGArea::boundingRect() const
{
	return QRectF(_x, _y, _w, _h);
}

void BGArea::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget * /* widget */)
{
	QColor color = QColor(128, 0, 0);
	QPainterPath shape;

	shape.addRect(boundingRect());

	painter->fillPath(shape, color);


}