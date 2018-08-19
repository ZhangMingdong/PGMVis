#pragma once
#include <QApplication>
#include <QColor>
#include <QSet>
#include "IBasicGraphicItem.h"

class BGLink;

class BGTextBase :public IBasicGraphicItem
{
	Q_DECLARE_TR_FUNCTIONS(BGTextBase)

public:
	BGTextBase();
	virtual ~BGTextBase();

	void setText(const QString &text);
	QString text() const;

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void SetText(QString strText){ myText = strText; }

	enum { Type = TextItemType };

	int type() const { return Type; }
	void Write(QXmlStreamWriter& writer);
	void Read(QXmlStreamReader& reader);

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void drawGraphic(QPainter *painter);
	void paintSelectionOutline(QPainter *painter);
	bool readProperties(QXmlStreamReader& reader);
private:
	QRectF outlineRect() const;
	int roundness(double size) const;

	QString myText; 
public:
	virtual QPointF getLinkPos(QPointF ptTarget);

};

