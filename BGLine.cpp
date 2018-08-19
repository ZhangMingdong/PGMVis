
#include <QtGui>

#include "BGLink.h"
#include "BGLine.h"

#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QColorDialog>
#include <QInputDialog>


BGLine::BGLine() :IBasicGraphicItem()
{
// 	myTextColor = Qt::darkGreen;
// 	myOutlineColor = Qt::darkBlue;
// 	myBackgroundColor = Qt::white;

// 	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

BGLine::~BGLine()
{
}

// void BGLine::setText(const QString &text)
// {
// 	prepareGeometryChange();
// 	myText = text;
// 	update();
// }
// 
// QString BGLine::text() const
// {
// 	return myText;
// }
// 
// void BGLine::setTextColor(const QColor &color)
// {
// 	myTextColor = color;
// 	update();
// }
// 
// QColor BGLine::textColor() const
// {
// 	return myTextColor;
// }
// 
// void BGLine::setOutlineColor(const QColor &color)
// {
// 	myOutlineColor = color;
// 	update();
// }
// 
// QColor BGLine::outlineColor() const
// {
// 	return myOutlineColor;
// }
// 
// void BGLine::setBackgroundColor(const QColor &color)
// {
// 	myBackgroundColor = color;
// 	update();
// }
// 
// QColor BGLine::backgroundColor() const
// {
// 	return myBackgroundColor;
// }


QRectF BGLine::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath BGLine::shape() const
{
	QRectF rect = outlineRect();

	QPainterPath path;
	path.addRoundRect(rect, roundness(rect.width()),
		roundness(rect.height()));
	return path;

// 	QPainterPath path;
// 	path.moveTo(m_ptFrom);
// 	path.lineTo(m_ptTo);
// 	return path;
}

void BGLine::drawGraphic(QPainter *painter)
{
// 	QPen pen(myOutlineColor);
// 	if (option->state & QStyle::State_Selected) {
// 		pen.setStyle(Qt::DotLine);
// 		pen.setWidth(2);
// 	}
// 	painter->setPen(pen);
// 	painter->setBrush(myBackgroundColor);

	painter->drawLine(m_ptFrom, m_ptTo);
// 	painter->drawRect(outlineRect());

// 	painter->setPen(myTextColor);
// 	painter->drawText(rect, Qt::AlignCenter, myText);
}

void BGLine::paintSelectionOutline(QPainter *painter)
{
	painter->drawLine(m_ptFrom, m_ptTo);
}
void BGLine::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
// 	QString text = QInputDialog::getText(event->widget(),
// 		tr("Edit Text"), tr("Enter new text:"),
// 		QLineEdit::Normal, myText);
// 	if (!text.isEmpty())
// 		setText(text);
}


QRectF BGLine::outlineRect() const
{
	const int Padding = 8;

	int x = m_ptFrom.x() < m_ptTo.x() ? m_ptFrom.x() : m_ptTo.x();
	int y = m_ptFrom.y() < m_ptTo.y() ? m_ptFrom.y() : m_ptTo.y();
	QRectF rect(x, y, abs(m_ptFrom.x() - m_ptTo.x()), abs(m_ptFrom.y() - m_ptTo.y()));
	rect.adjust(-Padding, -Padding, +Padding, +Padding);
// 	rect.translate(-rect.center());
	return rect;
}

int BGLine::roundness(double size) const
{
	const int Diameter = 12;
	return 100 * Diameter / int(size);
}

void BGLine::Write(QXmlStreamWriter& writer)
{
	writer.writeStartElement("line");

	writeId(writer);
	writePosition(writer);
	writePen(writer);
	writetransform(writer);
	writer.writeTextElement("fx", QString::number(this->m_ptFrom.x()));
	writer.writeTextElement("fy", QString::number(this->m_ptFrom.y()));
	writer.writeTextElement("tx", QString::number(this->m_ptTo.x()));
	writer.writeTextElement("ty", QString::number(this->m_ptTo.y()));

	writer.writeEndElement();
}

void BGLine::Read(QXmlStreamReader& reader)
{
	reader.readNext();		// skip <sphere>
	QPen pen;
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "fx")
			{
				m_ptFrom.setX(readDoubleValue(reader));
			}
			else if (reader.name() == "fy")
			{
				m_ptFrom.setY(readDoubleValue(reader));
			}
			else if (reader.name() == "tx")
			{
				m_ptTo.setX(readDoubleValue(reader));
			}
			else if (reader.name() == "ty")
			{
				m_ptTo.setY(readDoubleValue(reader));
			}
			else if (reader.name() == "angle")
			{
				this->m_angle = readDoubleValue(reader);
			}
			else if (reader.name() == "shearh")
			{
				this->m_shearHorizontal = readDoubleValue(reader);
			}
			else if (reader.name() == "shearv")
			{
				this->m_shearVertical = readDoubleValue(reader);
			}
			else if (reader.name() == "pen")
			{
				readPen(reader, pen);
				// 				pen.setColor(readColorValue(reader));
			}
// 			else if (readGroup(reader))
// 			{
// 			}
// 			else {
// 				skipUnknownElement(reader);
// 			}
			else if (!readGroup(reader) && !readId(reader) && !readX(reader) && !readY(reader))
			{
				skipUnknownElement(reader);
			}

		}
		else {
			reader.readNext();
		}
	}
	m_pen = pen;
	updateTransform();
}

void BGLine::FinishedDrawing()
{
	QPointF ptCenter = (m_ptFrom + m_ptTo) / 2;
	setPos(ptCenter);
	m_ptFrom -= ptCenter;
	m_ptTo -= ptCenter;
}

bool BGLine::readProperties(QXmlStreamReader& reader){
	if (reader.name() == "fx")
	{
		m_ptFrom.setX(readDoubleValue(reader));
	}
	else if (reader.name() == "fy")
	{
		m_ptFrom.setY(readDoubleValue(reader));
	}
	else if (reader.name() == "tx")
	{
		m_ptTo.setX(readDoubleValue(reader));
	}
	else if (reader.name() == "ty")
	{
		m_ptTo.setY(readDoubleValue(reader));
	}
	else{
		return false;
	}
	return true;

}