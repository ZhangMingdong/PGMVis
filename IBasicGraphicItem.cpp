#include "IBasicGraphicItem.h"
#include "BGLink.h"
// #include "BGLine.h"
// #include "BGRect.h"
// #include "BGSphere.h"
// #include "BGTextBase.h"
#include "BGGroup.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>


IBasicGraphicItem::IBasicGraphicItem() : QGraphicsItem()
, m_resizing(false)
, m_nDataId(-1)
{
	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

IBasicGraphicItem::~IBasicGraphicItem()
{
}

void IBasicGraphicItem::updateTransform()
{
	QTransform transform;
	transform.shear(m_shearHorizontal, m_shearVertical);
	transform.rotate(m_angle);
	setTransform(transform);
}

bool IBasicGraphicItem::i_isSelected(){
	if (this->group())
	{
		return false;
	}
	return isSelected();
}

void IBasicGraphicItem::i_update(){
	update();
}

void IBasicGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */)
{
// 	if (option->state & QStyle::State_Selected) {
// 		m_pen.setStyle(Qt::DotLine);
// 		m_pen.setWidth(2);
// 	}
// 	else{
// 		m_pen.setStyle(Qt::SolidLine);
// 		m_pen.setWidth(1);
// 	}
	painter->setPen(m_pen);
	painter->setBrush(m_brush);

	drawGraphic(painter);

	if (option->state & QStyle::State_Selected) {
		QPen pen(Qt::DashLine);
		pen.setWidth(2);
		pen.setColor(Qt::black);
		painter->setPen(pen);
		painter->setBrush(Qt::NoBrush);
		paintSelectionOutline(painter);
	}
}

void IBasicGraphicItem::updateLink(){
	foreach(BGLink *link, m_setLinks)
		link->trackNodes();
}

QVariant IBasicGraphicItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionHasChanged) {
		updateLink();
	}
	if (change == QGraphicsItem::ItemPositionChange ||
		change == QGraphicsItem::ItemPositionHasChanged ||
		change == QGraphicsItem::ItemTransformChange ||
		change == QGraphicsItem::ItemTransformHasChanged)
	{
		emit dirty();
	}
	return QGraphicsItem::itemChange(change, value);
}

bool IBasicGraphicItem::readX(QXmlStreamReader& reader){
	if (reader.name() == "x")
	{
		setX(readDoubleValue(reader));
		return true;
	}
	else return false;
}

bool IBasicGraphicItem::readY(QXmlStreamReader& reader){
	if (reader.name() == "y")
	{
		setY(readDoubleValue(reader));
		return true;
	}
	else return false;
}

void IBasicGraphicItem::Read_New(QXmlStreamReader& reader, bool bNewId){
	reader.readNext();		// skip <sphere>
	QPen pen;	
	QBrush brush;

	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}
		if (reader.isStartElement()) {
			if (reader.name() == "id")
			{
				QString strId = readStringValue(reader);
				if (!bNewId)
				{
					m_id = QUuid(strId);
				}
			}
			else if (reader.name() == "dataId")
			{
				m_nDataId = readIntValue(reader);
			}
			else if (reader.name() == "x")
			{
				this->setX(readDoubleValue(reader));
			}
			else if (reader.name() == "y")
			{
				this->setY(readDoubleValue(reader));
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
			else if (reader.name() == "brush")
			{
				readBrush(reader, brush);
				// 				brush.setColor(readColorValue(reader));
			}
			else if (reader.name() == "group")
			{
				m_groupid = QUuid(readStringValue(reader));
			}
			else if (!readProperties(reader))
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
	updateTransform();
}

QRectF IBasicGraphicItem::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QRectF IBasicGraphicItem::outlineRect() const
{
	return QRectF();
}

QPointF IBasicGraphicItem::getLinkPos(QPointF ptTarget){
	return this->pos();
}

int IBasicGraphicItem::x(){
	return QGraphicsItem::x();
}

int IBasicGraphicItem::y(){
	return QGraphicsItem::y();
}

void IBasicGraphicItem::setX(int x){
	QGraphicsItem::setX(x);
}

void IBasicGraphicItem::setY(int y){
	QGraphicsItem::setY(y);
}


void IBasicGraphicItem::writeId(QXmlStreamWriter& writer){
	IAbstractItem::writeId(writer);
	writer.writeTextElement("dataId", QString::number(m_nDataId));
}


void IBasicGraphicItem::SetDataId(int id)
{
	if (this->isSelected())
	{
		m_nDataId = id;
		update();
		emit dirty();
	}
}


BGGroup* IBasicGraphicItem::i_group(){
	return dynamic_cast<BGGroup*>(this->group());
}


QPointF IBasicGraphicItem::i_posAbs() { return this->pos(); }
QPointF IBasicGraphicItem::i_posRel() { return this->pos(); }
QPointF IBasicGraphicItem::i_posGrp() { return this->pos(); }