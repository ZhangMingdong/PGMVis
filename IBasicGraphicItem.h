#pragma once

#include "IIdInterface.h"
#include "IAbstractItem.h"
#include <QGraphicsItem>


const int LineItemType = QGraphicsItem::UserType + 1;
const int TextItemType = QGraphicsItem::UserType + 2;
const int RectItemType = QGraphicsItem::UserType + 3;
const int SphereItemType = QGraphicsItem::UserType + 4;
const int ArcItemType = QGraphicsItem::UserType + 5;
const int PolygonItemType = QGraphicsItem::UserType + 6;
const int SwitchItemType = QGraphicsItem::UserType + 7;
const int NodeItemType = QGraphicsItem::UserType + 8;

const int LinkItemType = QGraphicsItem::UserType + 10;
const int NullItemType = 0;

// class Link;

class IBasicGraphicItem :public IAbstractItem, public QGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(int DataId READ DataId WRITE SetDataId)
public:	

	IBasicGraphicItem();

	virtual ~IBasicGraphicItem();
protected:
	bool m_resizing;
public:
	virtual void Write(QXmlStreamWriter& writer) = 0;
	virtual void Read(QXmlStreamReader& reader) = 0;
	void Read_New(QXmlStreamReader& reader, bool bNewId = false);
	QRectF boundingRect() const;
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual bool readProperties(QXmlStreamReader& reader) = 0;
	virtual void drawGraphic(QPainter *painter) = 0;
	virtual void paintSelectionOutline(QPainter *painter) = 0;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	virtual void writeId(QXmlStreamWriter& writer);
private:
	QRectF outlineRect() const;
// 	int roundness(double size) const;
protected:
	bool readX(QXmlStreamReader& reader);
	bool readY(QXmlStreamReader& reader);

public:	// from abstract item
	virtual QPointF getLinkPos(QPointF ptTarget);
protected:
	virtual bool i_isSelected();
	virtual void i_update();
	virtual void updateTransform();
	virtual QPointF i_posAbs();
	virtual QPointF i_posRel();
	virtual QPointF i_posGrp();
	virtual BGGroup* i_group();
public:
	virtual int x();
	virtual int y();
	virtual void setX(int x);
	virtual void setY(int y);
public:
	void updateLink();
protected:
	int m_nDataId;
public:
	int DataId(){ return m_nDataId; }
public slots:
	void SetDataId(int id);

};

