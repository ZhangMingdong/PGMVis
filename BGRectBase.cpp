#include <QtGui>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "BGRectBase.h"

BGRectBase::BGRectBase() :IBasicGraphicItem()
, m_nWidth(30), m_nHeight(30)
// , m_bFill(false)
, m_resizing(false)
{
}


BGRectBase::~BGRectBase()
{
}


int BGRectBase::width()
{
	return m_nWidth;
}

int BGRectBase::height()
{
	return m_nHeight;
}

void BGRectBase::setWidth(int width)
{
	m_nWidth = abs(width);
}

void BGRectBase::setHeight(int height)
{
	m_nHeight = abs(height);
}

void BGRectBase::setGeometry(/*int x, int y, */int w, int h){
	if (i_isSelected()){
// 		setX(x);
// 		setY(y);
		setWidth(w);
		setHeight(h);

		scene()->update();
	}
}


QRectF BGRectBase::outlineRect() const
{
	// 	const int Padding = 8;
	// 	QFontMetricsF metrics = QFontMetricsF(qApp->font());
	// 	QRectF rect = metrics.boundingRect(myText);
	// 	rect.adjust(-Padding, -Padding, +Padding, +Padding);
	// 	rect.translate(-rect.center());
	// 	return rect;

	const int Padding = 8;
	QRectF rect(-m_nWidth / 2.0, -m_nHeight / 2.0, m_nWidth, m_nHeight);
// 	rect.adjust(-Padding, -Padding, +Padding, +Padding);
	return rect;
}

void BGRectBase::Write(QXmlStreamWriter& writer)
{
	qint32 type = static_cast<qint32>(this->type());
	switch (type)
	{
	case SphereItemType:
		writer.writeStartElement("sphere");
		break;
	case RectItemType:
		writer.writeStartElement("rect");
		break;
	case ArcItemType:
		writer.writeStartElement("arc");
		break;
	case SwitchItemType:
		writer.writeStartElement("switch");
		break;
	case NodeItemType:
		writer.writeStartElement("node");
		break;
	default:
		return;
	}

	const QMetaObject *metaObject = this->metaObject();

	writeId(writer);
	writePosition(writer);
	writeBrush(writer);
	writePen(writer);
	writetransform(writer);
	writeSpecificData(writer);
	writer.writeTextElement("w", QString::number(this->width()));
	writer.writeTextElement("h", QString::number(this->height()));
	
	writer.writeEndElement();
}

void BGRectBase::Read(QXmlStreamReader& reader)
{
	reader.readNext();		// skip <sphere>
	QPen pen;
	QBrush brush;
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "w")
			{
				this->setWidth(readIntValue(reader));
			}
			else if (reader.name() == "h")
			{
				this->setHeight(readIntValue(reader));
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
			else if (reader.name() == "brush")
			{
				readBrush(reader, brush);
				// 				brush.setColor(readColorValue(reader));
			}
			else if (reader.name() == "pen")
			{
				readPen(reader, pen);
				// 				pen.setColor(readColorValue(reader));
			}
// 			else if (readSpecificData(reader))
// 			{
// 			}
// 			else if (readGroup(reader))
// 			{
// 			}
// 			else
// 			{
// 				skipUnknownElement(reader);
// 			}
			else if (!readSpecificData(reader)&&!readGroup(reader) && !readId(reader) && !readX(reader) && !readY(reader))
			{
				skipUnknownElement(reader);
			}

		}
		else {
			reader.readNext();
		}
	}
	m_pen = pen;
	m_brush = brush;
	QPointF pt = this->pos();
	updateTransform();
}

void BGRectBase::keyPressEvent(QKeyEvent *event)
{
// 	if (event->modifiers() & Qt::ShiftModifier ||
// 		event->modifiers() & Qt::ControlModifier) {
// 		bool move = event->modifiers() & Qt::ControlModifier;
// 		bool changed = true;
// 		double dx1 = 0.0;
// 		double dy1 = 0.0;
// 		double dx2 = 0.0;
// 		double dy2 = 0.0;
// 		switch (event->key()) {
// 		case Qt::Key_Left:
// 			if (move)
// 				dx1 = -1.0;
// 			dx2 = -1.0;
// 			break;
// 		case Qt::Key_Right:
// 			if (move)
// 				dx1 = 1.0;
// 			dx2 = 1.0;
// 			break;
// 		case Qt::Key_Up:
// 			if (move)
// 				dy1 = -1.0;
// 			dy2 = -1.0;
// 			break;
// 		case Qt::Key_Down:
// 			if (move)
// 				dy1 = 1.0;
// 			dy2 = 1.0;
// 			break;
// 		default:
// 			changed = false;
// 		}
// 		if (changed) {
// 			setRect(rect().adjusted(dx1, dy1, dx2, dy2));
// 			event->accept();
// 			emit dirty();
// 			return;
// 		}
// 	}
	IBasicGraphicItem::keyPressEvent(event);
}

void BGRectBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->modifiers() & Qt::ShiftModifier) {
		m_resizing = true;
		setCursor(Qt::SizeAllCursor);
	}
	else
		IBasicGraphicItem::mousePressEvent(event);
}

void BGRectBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_resizing) {

		QPointF ptCurrent = mapToScene(event->pos())-this->pos();

		setWidth(ptCurrent.x() * 2);
		setHeight(ptCurrent.y() * 2);
		scene()->update();
	}
	else
		IBasicGraphicItem::mouseMoveEvent(event);
}

void BGRectBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_resizing) {
		m_resizing = false;
		setCursor(Qt::ArrowCursor);
		emit dirty();
	}
	else
		IBasicGraphicItem::mouseReleaseEvent(event);
}

bool BGRectBase::readProperties(QXmlStreamReader& reader){
	if (reader.name() == "w")
	{
		this->setWidth(readIntValue(reader));
	}
	else if (reader.name() == "h")
	{
		this->setHeight(readIntValue(reader));
	}
	else{
		return readSpecificData(reader);
	}
	return true;

}