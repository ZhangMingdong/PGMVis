#pragma once
#include <QPolygon>
#include <QGraphicsPolygonItem>

#include "IAbstractItem.h"

class BGPolygon : public QGraphicsPolygonItem,IAbstractItem
{
public:
	BGPolygon();
	~BGPolygon();
public:
	void AppendPoint(const QPointF& pt);
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
	QPolygonF calc();
	double generateRectangle(double sinOrt,double cosOrt, QPointF ptCenter, double dbAspR, QPolygonF& poly);
	double generateRectangleWithNewCenter(double sinOrt, double cosOrt, QPointF ptCenter, double dbAspR, QPolygonF& poly);
	void rayTracing(QPointF ptCenter, double sinOrt, double cosOrt, QPolygonF& poly1, QPolygonF& poly2);
	bool intersected(const QPolygonF& poly);
public:
	virtual int x() ;
	virtual int y() ;
	virtual void setX(int x) ;
	virtual void setY(int y) ;
public:
	virtual QPointF getLinkPos(QPointF ptTarget){
		return pos();
	};
	virtual QString getPortId(int index);
protected:
	virtual bool i_isSelected() ;
	virtual void i_update() ;
	virtual void updateTransform();
	virtual QPointF i_posAbs();
	virtual QPointF i_posRel();
	virtual QPointF i_posGrp();
	virtual BGGroup* i_group();
public:
	virtual void updateLink(){}
};

