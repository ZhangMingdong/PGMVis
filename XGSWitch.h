#pragma once
#include <QApplication>
#include <QColor>
#include <QSet>

#include "BGRectBase.h"

class BGLink;

class XGSWitch : public BGRectBase
{
	Q_DECLARE_TR_FUNCTIONS(XGSWitch)

public:
	XGSWitch();
	virtual ~XGSWitch();

	QRectF boundingRect() const;
	QPainterPath shape() const;

	enum { Type = SwitchItemType };

	int type() const { return Type; }
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void drawGraphic(QPainter *painter);
	void paintSelectionOutline(QPainter *painter);
public:
	virtual int indexRange(){ return 2; };
};

