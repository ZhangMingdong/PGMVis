#pragma once
#include <QApplication>
#include <QColor>
#include <QSet>
#include "IBasicGraphicItem.h"

class BGLink;

class BGLine :
	public IBasicGraphicItem
{
	Q_DECLARE_TR_FUNCTIONS(BGLine)

public:
	BGLine();
	virtual ~BGLine();

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void SetFrom(QPointF pt){ m_ptFrom = pt; }
	void SetTo(QPointF pt){ m_ptTo = pt; }

	enum { Type = LineItemType };

	int type() const { return Type; }
	void Write(QXmlStreamWriter& writer);
	void Read(QXmlStreamReader& reader);
	void FinishedDrawing();

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void drawGraphic(QPainter *painter);
	void paintSelectionOutline(QPainter *painter);
	bool readProperties(QXmlStreamReader& reader);
private:
	QRectF outlineRect() const;
	int roundness(double size) const;

	QPointF m_ptFrom;
	QPointF m_ptTo;
};

