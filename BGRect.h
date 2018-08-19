#pragma once
#include <QApplication>
#include <QColor>
#include <QSet>

#include "BGRectBase.h"

class BGLink;

class BGRect : public BGRectBase 
{
	Q_DECLARE_TR_FUNCTIONS(BGRect)

public:
	BGRect();
	virtual ~BGRect();


	QRectF boundingRect() const;
	QPainterPath shape() const;

	enum { Type = RectItemType };

	int type() const { return Type; }
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void drawGraphic(QPainter *painter);
	void paintSelectionOutline(QPainter *painter);
private:
	int roundness(double size) const;
public:
	virtual QPointF getLinkPos(int index);
	virtual QPointF getLinkPos(QPointF ptTarget);



};

