#include <QtGui>

#include "BGLink.h"
#include "IAbstractItem.h"

#define SQRT30	1.7320508
#define HALF_SQRT30	0.8660254

BGLink::BGLink(IAbstractItem *fromNode, IAbstractItem *toNode) :m_nIndexFrom(1)
, m_nIndexTo(0)
, m_pNodeFrom(fromNode)
, m_pNodeTo(toNode)
, m_style(LS_Curve)
, m_nWeight(0)
{

	m_pNodeFrom->addLink(this);
	m_pNodeTo->addLink(this);

	setFlags(QGraphicsItem::ItemIsSelectable);
	setZValue(-1);

	setColor(Qt::darkRed);
	trackNodes();
}

BGLink::~BGLink()
{
	m_pNodeFrom->removeLink(this);
	m_pNodeTo->removeLink(this);
}

IAbstractItem *BGLink::fromNode() const
{
	return m_pNodeFrom;
}

IAbstractItem *BGLink::toNode() const
{
	return m_pNodeTo;
}

void BGLink::setFromNode(IAbstractItem* node){
	m_pNodeFrom = node;
}

void BGLink::setToNode(IAbstractItem* node){
	m_pNodeTo = node;
}

void BGLink::setColor(const QColor &color)
{
	setPen(QPen(color, 1.0));
	setBrush(color);
}

QColor BGLink::color() const
{
	return pen().color();
}

void BGLink::trackNodes()
{
	bool bDrawArrow = true;
	QPainterPath path;

	// get the points of the line

	QPointF ptStartCenter = m_pNodeFrom->getItemCenter();
	QPointF ptEndCenter = m_pNodeTo->getItemCenter();

	QPointF ptStart = m_pNodeFrom->getLinkPos(ptEndCenter);
	QPointF ptEnd = m_pNodeTo->getLinkPos(ptStartCenter);
	qDebug() << ptStartCenter << ptEndCenter;
	qDebug() << ptStart << ptEnd;


	if (m_style==LS_Curve)
	{
		// draw line
		path.moveTo(ptStart);
		path.lineTo(ptEnd);

		if (bDrawArrow)
		{
			qreal arrowSize = 20;
			QLineF line = QLineF(ptEnd,ptStart);
			double angle = std::atan2(-line.dy(), line.dx());

			QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
				cos(angle + M_PI / 3) * arrowSize);
			QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
				cos(angle + M_PI - M_PI / 3) * arrowSize);

			QPolygonF arrowHead;
			arrowHead << line.p1() << arrowP1 << arrowP2;
			path.addPolygon(arrowHead);

			/*
			// calculate arrow
			QPointF pt = ptStart - ptEnd;
			QPointF pt1(pt.x()*HALF_SQRT30 + pt.y()*0.5, -pt.x()*0.5 + pt.y()*HALF_SQRT30);
			QPointF pt2(pt.x()*HALF_SQRT30 - pt.y()*0.5, pt.x()*0.5 + pt.y()*HALF_SQRT30);
			double scale = sqrt(pt.x()*pt.x() + pt.y() * pt.y()) / 20.0;
			pt1 /= scale;
			pt2 /= scale;
			pt1 += ptEnd;
			pt2 += ptEnd;

			qreal arrowSize = 20;

			// draw arrow
			path.lineTo(pt1);
			path.moveTo(ptEnd);
			path.lineTo(pt2);
			*/
		}
	}
	else{
		// orthogonal layout
		// calculate base
		QPointF ptStartDir = ptStart - ptStartCenter;
		QPointF ptEndDir = ptEnd - ptEndCenter;
		QPointF ptStartBase, ptEndBase;
		if (ptStartDir.x()==0){
			if (ptEndDir.x()==0)
			{
				double y = (ptEnd.y() + ptStart.y()) / 2.0;
				ptStartBase = QPointF(ptStart.x(), y);
				ptEndBase = QPointF(ptEnd.x(), y);
			}
			else{
				ptStartBase = ptEndBase = QPointF(ptStart.x(), ptEnd.y());
			}
		}
		else{
			if (ptEndDir.x() == 0)
			{
				ptStartBase = ptEndBase = QPointF(ptEnd.x(), ptStart.y());
			}
			else{
				double x = (ptEnd.x() + ptStart.x()) / 2.0;
				ptStartBase = QPointF(x, ptStart.y());
				ptEndBase = QPointF(x, ptEnd.y());
			}
		}


		if (bDrawArrow)
		{
			// calculate arrow
			QPointF pt = ptEndBase - ptEnd;
			QPointF pt1(pt.x()*HALF_SQRT30 + pt.y()*0.5, -pt.x()*0.5 + pt.y()*HALF_SQRT30);
			QPointF pt2(pt.x()*HALF_SQRT30 - pt.y()*0.5, pt.x()*0.5 + pt.y()*HALF_SQRT30);
			double scale = sqrt(pt.x()*pt.x() + pt.y() * pt.y()) / 20.0;
			pt1 /= scale;
			pt2 /= scale;
			pt1 += ptEnd;
			pt2 += ptEnd;
			// draw line
			path.moveTo(ptStart);
			path.lineTo(ptStartBase);
			path.lineTo(ptEndBase);
			path.lineTo(ptEnd);

			// draw arrow
			path.lineTo(pt1);
			path.moveTo(ptEnd);
			path.lineTo(pt2);
		}
	}
	//path.addText((ptStart + ptEnd) / 2.0, QFont(), QString::number(m_nWeight));
	setPath(path);
}

void BGLink::Write(QXmlStreamWriter& writer){
	writer.writeStartElement("link");

	writer.writeTextElement("from", this->fromNode()->GetId());
	writer.writeTextElement("to", this->toNode()->GetId());

	writer.writeTextElement("fromPort", QString::number(m_nIndexFrom));
	writer.writeTextElement("toPort", QString::number(m_nIndexTo));

	writer.writeTextElement("style", QString::number(m_style));
	writer.writeTextElement("weight", QString::number(m_nWeight));

	writer.writeEndElement();
}

void BGLink::setFromIndex(int index){
	m_nIndexFrom = index;
	// 	if (m_pNodeFrom)
	// 	{
	// 		m_pNodeFrom->setFromIndex(index);
	// 	}
}

void BGLink::setToIndex(int index){
	m_nIndexTo = index;
	// 	if (m_pNodeTo)
	// 	{
	// 		m_pNodeTo->setToIndex(index);
	// 	}
}

int BGLink::fromRange(){
	if (m_pNodeFrom)
	{
		return m_pNodeFrom->indexRange();
	}
	return 0;
}

int BGLink::toRange(){
	if (m_pNodeTo)
	{
		return m_pNodeTo->indexRange();
	}
	return 0;
}

int BGLink::fromIndex(){
	return m_nIndexFrom;
	// 	if (m_pNodeFrom)
	// 	{
	// 		return m_pNodeFrom->fromIndex();
	// 	}
	// 	return 0;
}

int BGLink::toIndex(){
	return m_nIndexTo;
	// 	if (m_pNodeTo)
	// 	{
	// 		return m_pNodeTo->toIndex();
	// 	}
	// 	return 0;
}

void BGLink::setLinkIndex(int from, int to){
	if (this->isSelected())
	{
		m_nIndexFrom = from;
		m_nIndexTo = to;
		trackNodes();
	}
}

void BGLink::InitPorts(QUuid fromPort, QUuid toPort){
	// 	this->m_nIndexFrom = m_pNodeFrom->getChildPos(fromPort);
	// 	this->m_nIndexTo = m_pNodeTo->getChildPos(toPort);
	trackNodes();
}

bool BGLink::ChangeStyle(LinkStyle style){
	if (style == m_style)
		return false;

	m_style = style;
	trackNodes();

	return true;
}


void BGLink::ChangeWeight(int weight)
{
	m_nWeight = weight;
	trackNodes();
}

void BGLink::setWeight(int weight)
{ 
	if (this->isSelected()&& weight!=this->m_nWeight)
	{
		m_nWeight = weight;
		trackNodes();
// 		emit dirty();
	}
}