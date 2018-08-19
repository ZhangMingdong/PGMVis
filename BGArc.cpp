#include "BGArc.h"
#include <QPainter>
#include <QGraphicsScene>


BGArc::BGArc() :BGRectBase()
, m_angleStart(0)
, m_angleLen(90)
{
}


BGArc::~BGArc()
{
}


QRectF BGArc::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath BGArc::shape() const
{
	QRectF rect = outlineRect();

	QPainterPath path;
	path.addRect(rect);
	return path;
}

void BGArc::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

void BGArc::drawGraphic(QPainter *painter)
{
	painter->drawArc(outlineRect(), m_angleStart*16, m_angleLen*16);
}

void BGArc::paintSelectionOutline(QPainter *painter)
{
	painter->drawEllipse(outlineRect());
}

void BGArc::setAngleStart(double angle)
{
	if (isSelected() && !qFuzzyCompare(m_angleStart, angle)) {
		m_angleStart = angle;
		emit dirty();
		scene()->update();
	}
}

void BGArc::setAngleLen(double angle)
{
	if (isSelected() && !qFuzzyCompare(m_angleLen, angle)) {
		m_angleLen = angle;
		emit dirty();
		scene()->update();
	}
}

void BGArc::writeSpecificData(QXmlStreamWriter& writer)
{
	writer.writeTextElement("angleStart", QString::number(this->m_angleStart));
	writer.writeTextElement("angleLen", QString::number(this->m_angleLen));
}

bool BGArc::readSpecificData(QXmlStreamReader& reader)
{
	if (reader.name() == "angleStart")
	{
		m_angleStart = readDoubleValue(reader);
		return true;
	}
	else if (reader.name() == "angleLen")
	{
		m_angleLen = readDoubleValue(reader);
		return true;
	}
	else return false;
}