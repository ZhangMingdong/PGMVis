#pragma once
// #include "IIdInterface.h"
// #include "IBasicGraphicItem.h"
#include "IAbstractItem.h"

#include <QGraphicsItemGroup>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class BGGroup : public IAbstractItem, public QGraphicsItemGroup
{
	Q_OBJECT
	// 	Q_PROPERTY(int fromIndex READ fromIndex WRITE setFromIndex)
	// 	Q_PROPERTY(int toIndex READ toIndex WRITE setToIndex)
	// 	Q_PROPERTY(int indexRange READ indexRange)
public:
	BGGroup();
	virtual ~BGGroup();
	// private:
	// 	void addToGroup(QGraphicsItem *item){};
public:
	void Write(QXmlStreamWriter& writer);
	void Read(QXmlStreamReader& reader);
	void InitPosition();
private:
	double m_dbX;
	double m_dbY;
	int m_nIndexFrom;
	int m_nIndexTo;
protected:
	virtual void paintSelectionOutline(QPainter *painter){}
	virtual void drawGraphic(QPainter *painter){}
	virtual bool readProperties(QXmlStreamReader& reader){ return false; }
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

public:	// from abstract item
	virtual QPointF getLinkPos(QPointF ptTarget);
	virtual QPointF getRotationCenter();
	virtual QString getPortId(int index);
protected:
	virtual bool i_isSelected();
	virtual void i_update();
	virtual void updateTransform();
	virtual QPointF i_posAbs();
	virtual QPointF i_posRel();
	virtual BGGroup* i_group();
	virtual int getChildPos(QUuid id);
public:
	void setFromIndex(int index);
	void setToIndex(int index);
	int indexRange();
	int fromIndex();
	int toIndex();
public:
	void updateLink();
public slots:
	void setLinkIndex(int from,int to);
public:
	void SetUpGroup(QList<QGraphicsItem*> items);
private:	// temp data for loading
	QString m_idFrom;
	QString m_idTo;

public:
	virtual int x();
	virtual int y();
	virtual void setX(int x);
	virtual void setY(int y);
public:
	void AddItem(QGraphicsItem* item);
private:
	QRectF outlineRect() const;
	QRectF boundingRect() const;
	int roundness(double size) const;
};

