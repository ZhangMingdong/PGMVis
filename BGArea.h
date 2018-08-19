#pragma once
#include <QGraphicsItem>
class BGArea :
	public QGraphicsItem
{
public:
	BGArea(int x,int y,int w,int h);
	~BGArea();

	QRectF boundingRect() const;
	void paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget);

	QPainterPath _path;
private:
	int _x;
	int _y;
	int _w;
	int _h;
};

