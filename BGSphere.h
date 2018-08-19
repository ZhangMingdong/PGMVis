#pragma once
#include <QApplication>
#include <QColor>
#include <QSet>
#include "BGRectBase.h"

class BGSphere : public BGRectBase
{
	Q_DECLARE_TR_FUNCTIONS(BGSphere)

public:
	BGSphere();
	virtual ~BGSphere();

	QRectF boundingRect() const;
	QPainterPath shape() const;

	enum { Type = SphereItemType };

	int type() const { return Type; }
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void drawGraphic(QPainter *painter);
	void paintSelectionOutline(QPainter *painter);
};

