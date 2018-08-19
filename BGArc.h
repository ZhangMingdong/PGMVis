#pragma once
#include <QApplication>
#include "BGRectBase.h"

class BGArc : public BGRectBase
{
// 	Q_DECLARE_TR_FUNCTIONS(BGArc)

	Q_OBJECT
	Q_PROPERTY(double angleStart READ angleStart WRITE setAngleStart)
	Q_PROPERTY(double angleLen READ angleLen WRITE setAngleLen)


public:
	BGArc();
	virtual ~BGArc();	


	QRectF boundingRect() const;
	QPainterPath shape() const;

	enum { Type = ArcItemType };

	int type() const { return Type; }
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void drawGraphic(QPainter *painter);
	void paintSelectionOutline(QPainter *painter);
	void writeSpecificData(QXmlStreamWriter& writer);
	bool readSpecificData(QXmlStreamReader& reader);

	double m_angleStart;
	double m_angleLen;
public:
	double angleStart() const { return m_angleStart; }
	double angleLen() const { return m_angleLen; }
public slots :
	void setAngleStart(double angle);
	void setAngleLen(double angle);
};

