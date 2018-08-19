#ifndef LINK_H
#define LINK_H

#include <QGraphicsPathItem>
#include <QXmlStreamWriter>

class IAbstractItem;

class BGLink : public QObject, public QGraphicsPathItem
{
	Q_OBJECT
	Q_PROPERTY(int fromIndex READ fromIndex WRITE setFromIndex)
	Q_PROPERTY(int toIndex READ toIndex WRITE setToIndex)
	Q_PROPERTY(int fromRange READ fromRange)
	Q_PROPERTY(int toRange READ toRange)
	Q_PROPERTY(int weight READ weight WRITE setWeight)


public:
	enum LinkStyle{
		LS_Line,
		LS_Curve
	};
public:
	BGLink(IAbstractItem *fromNode, IAbstractItem *toNode);
	~BGLink();

	IAbstractItem *fromNode() const;
	IAbstractItem *toNode() const;

	void setFromNode(IAbstractItem* node);
	void setToNode(IAbstractItem* node);

	void setColor(const QColor &color);
	QColor color() const;

	void trackNodes();
	void Write(QXmlStreamWriter& writer);

private:
	IAbstractItem *m_pNodeFrom;
	IAbstractItem *m_pNodeTo;
	int m_nIndexFrom;
	int m_nIndexTo;
private:	// proverty
	void setFromIndex(int index);
	void setToIndex(int index);
	int fromRange();
	int toRange();
	int fromIndex();
	int toIndex();


public slots:
	void setLinkIndex(int from, int to);
	void setWeight(int weight);

public:
	void InitPorts(QUuid fromPort, QUuid toPort);
	bool ChangeStyle(LinkStyle style);
	LinkStyle GetStyle(){ return m_style; }
	int weight(){ return m_nWeight; }
	void ChangeWeight(int weight);
private:
	LinkStyle m_style;
	int m_nWeight;

};

#endif
