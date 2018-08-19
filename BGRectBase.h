#pragma once

#include "IBasicGraphicItem.h"

class BGRectBase: public IBasicGraphicItem
{
	Q_OBJECT
	Q_PROPERTY(int width READ width WRITE setWidth)
	Q_PROPERTY(int height READ height WRITE setHeight)
public:
	BGRectBase();
	virtual ~BGRectBase();
protected:

	int m_nWidth;
	int m_nHeight;

	bool m_resizing;

protected:

	QRectF outlineRect() const;
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	virtual void writeSpecificData(QXmlStreamWriter& writer){}
	virtual bool readSpecificData(QXmlStreamReader& reader){ return false; };
	bool readProperties(QXmlStreamReader& reader);
public slots:

	void setGeometry(/*int x, int y, */int w, int h);
public:
// 	int x();
// 	int y();
// 	int setX();
// 	int setY();
	int width();
	int height();
	void setWidth(int width);
	void setHeight(int height);
	void Write(QXmlStreamWriter& writer);
	void Read(QXmlStreamReader& reader);
};

