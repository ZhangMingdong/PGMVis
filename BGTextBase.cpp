
#include <QtGui>

#include "BGLink.h"
#include "BGTextBase.h"

#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QColorDialog>
#include <QInputDialog>

#include "BGGroup.h"


BGTextBase::BGTextBase() :IBasicGraphicItem()
{
// 	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

BGTextBase::~BGTextBase()
{
}

void BGTextBase::setText(const QString &text)
{
	prepareGeometryChange();
	myText = text;
	update();
}

QString BGTextBase::text() const
{
	return myText;
}

QRectF BGTextBase::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath BGTextBase::shape() const
{
	QRectF rect = outlineRect();

	QPainterPath path;
	path.addRoundRect(rect, roundness(rect.width()),
		roundness(rect.height()));
	return path;
}

void BGTextBase::drawGraphic(QPainter *painter)
{
// 	qDebug() << "begin\n";
// 	QPen pen(myOutlineColor);
// 	if (option->state & QStyle::State_Selected) {
// 		pen.setStyle(Qt::DotLine);
// 		pen.setWidth(2);
// 	}
// 	painter->setPen(pen);
// 	painter->setBrush(myBackgroundColor);

	QRectF rect = outlineRect();
	painter->drawRoundRect(rect, roundness(rect.width()),
		roundness(rect.height()));

// 	painter->setPen(myTextColor);
	painter->drawText(rect, Qt::AlignCenter, myText);
// 	qDebug() << "end\n";
}

void BGTextBase::paintSelectionOutline(QPainter *painter)
{
	QRectF rect = outlineRect();
	painter->drawRoundRect(rect, roundness(rect.width()),
	 		roundness(rect.height()));

}

void BGTextBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QString text = QInputDialog::getText(event->widget(),
		tr("Edit Text"), tr("Enter new text:"),
		QLineEdit::Normal, myText);
	if (!text.isEmpty())
		setText(text);
}

QRectF BGTextBase::outlineRect() const
{
	const int Padding = 8;
	QFontMetricsF metrics = QFontMetricsF(qApp->font());
	QRectF rect = metrics.boundingRect(myText);
	rect.adjust(-Padding, -Padding, +Padding, +Padding);
	rect.translate(-rect.center());
	return rect;
}

int BGTextBase::roundness(double size) const
{
	const int Diameter = 12;
	return 100 * Diameter / int(size);
}

void BGTextBase::Write(QXmlStreamWriter& writer)
{
	writer.writeStartElement("text");

	writeId(writer);
	writePosition(writer);
	writePen(writer);
	writeBrush(writer);
	writetransform(writer);
	writer.writeTextElement("text", myText);

	writer.writeEndElement();

}

void BGTextBase::Read(QXmlStreamReader& reader)
{
	reader.readNext();		// skip <sphere>
	QPen pen;
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "text")
			{
				myText = readStringValue(reader);
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
			}
			else if (!readGroup(reader)&&!readId(reader)&&!readX(reader)&&!readY(reader))
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

bool BGTextBase::readProperties(QXmlStreamReader& reader){
	if (reader.name() == "text")
	{
		myText = readStringValue(reader);
	}
	else{
		return false;
	}
	return true;

}

QPointF BGTextBase::getLinkPos(QPointF ptTarget){
	QRectF rect = this->outlineRect();
	QPointF ptCenter = this->getItemCenter();
	QPointF pt[4];
	qreal len[4];
	pt[0] = QPointF(ptCenter.x(), ptCenter.y() - rect.height() / 2.0);
	pt[1] = QPointF(ptCenter.x() - rect.width() / 2.0, ptCenter.y());
	pt[2] = QPointF(ptCenter.x(), ptCenter.y() + rect.height() / 2.0);
	pt[3] = QPointF(ptCenter.x() + rect.width() / 2.0, ptCenter.y());
	len[0] = QLineF(pt[0], ptTarget).length();
	len[1] = QLineF(pt[1], ptTarget).length();
	len[2] = QLineF(pt[2], ptTarget).length();
	len[3] = QLineF(pt[3], ptTarget).length();
	int min = 0;
	for (int i = 1; i < 4; i++)
	{
		if (len[i] < len[min])
		{
			min = i;
		}
	}
	
	return pt[min];
}