#include <QGraphicsItemGroup>
#include "IAbstractItem.h"

#include "BGGroup.h"
#include "BGLink.h"



IAbstractItem::IAbstractItem() :QObject()
, m_angle(0.0)
, m_shearHorizontal(0.0)
, m_shearVertical(0.0)
, m_groupid("")
{
}


IAbstractItem::~IAbstractItem()
{
	foreach(BGLink *link, m_setLinks)
		delete link;
}
void IAbstractItem::addLink(BGLink *link)
{
	QString str = GetId();
	m_setLinks.insert(link);
}

void IAbstractItem::removeLink(BGLink *link)
{
	m_setLinks.remove(link);
}

void IAbstractItem::setPen(const QPen &pen_)
{
	if (i_isSelected() && pen_ != pen()) {
		m_pen = pen_;
		emit dirty();
	}
}

void IAbstractItem::setBrush(const QBrush &brush_)
{
	if (i_isSelected() && brush_ != brush()) {
		m_brush = brush_;
		emit dirty();
	}
	i_update();
}

void IAbstractItem::setShear(double shearHorizontal, double shearVertical)
{
	if (i_isSelected() &&
		(!qFuzzyCompare(m_shearHorizontal, shearHorizontal) ||
		!qFuzzyCompare(m_shearVertical, shearVertical))) {
		m_shearHorizontal = shearHorizontal;
		m_shearVertical = shearVertical;
		updateTransform();
	}
}

void IAbstractItem::setAngle(double angle)
{
	if (i_isSelected() && !qFuzzyCompare(m_angle, angle)) {
		m_angle = angle;
		updateTransform();
	}
}

void IAbstractItem::writePosition(QXmlStreamWriter& writer){
	QPointF pos = i_posAbs();
	writer.writeTextElement("x", QString::number(pos.x()));
	writer.writeTextElement("y", QString::number(pos.y()));

	writeGroup(writer);
}

void IAbstractItem::writePen(QXmlStreamWriter& writer){
	writer.writeStartElement("pen");
	writer.writeStartElement("color");
	writer.writeTextElement("r", QString::number(this->pen().color().red()));
	writer.writeTextElement("g", QString::number(this->pen().color().green()));
	writer.writeTextElement("b", QString::number(this->pen().color().blue()));
	writer.writeEndElement();
	writer.writeTextElement("width", QString::number(this->pen().width()));
	writer.writeTextElement("style", QString::number(this->pen().style()));
	writer.writeTextElement("capstyle", QString::number(this->pen().capStyle()));
	writer.writeTextElement("joinstyle", QString::number(this->pen().joinStyle()));
	writer.writeEndElement();
}

void IAbstractItem::writeBrush(QXmlStreamWriter& writer){
	writer.writeStartElement("brush");
	writer.writeStartElement("color");
	writer.writeTextElement("r", QString::number(this->brush().color().red()));
	writer.writeTextElement("g", QString::number(this->brush().color().green()));
	writer.writeTextElement("b", QString::number(this->brush().color().blue()));
	writer.writeEndElement();
	writer.writeTextElement("style", QString::number(this->brush().style()));
	writer.writeEndElement();
}

void IAbstractItem::writetransform(QXmlStreamWriter& writer){
	writer.writeTextElement("angle", QString::number(this->m_angle));
	writer.writeTextElement("shearh", QString::number(this->m_shearHorizontal));
	writer.writeTextElement("shearv", QString::number(this->m_shearVertical));
}

void IAbstractItem::writeGroup(QXmlStreamWriter& writer)
{
	QGraphicsItemGroup* group = this->i_group();
	if (group)
	{
		IIdInterface* g = dynamic_cast<IIdInterface*>(group);
		if (g)
		{
			writer.writeTextElement("group", g->GetId());
		}
	}
}
void IAbstractItem::setLocation(int x, int y){
	if (i_isSelected())
	{
		int xx = this->x();
		int yy = this->y();
		setX(x);
		setY(y);
	}
}

void IAbstractItem::writeId(QXmlStreamWriter& writer){
	writer.writeTextElement("id", this->GetId());
}

bool IAbstractItem::readGroup(QXmlStreamReader& reader)
{
	if (reader.name() == "group")
	{
		m_groupid = QUuid(readStringValue(reader));
		return true;
	}
	else return false;
}

QPointF IAbstractItem::GetGroupPos(){
	BGGroup* group = i_group();
	if (group)
	{
		return this->i_posAbs() + group->GetGroupPos();
	}
	else{
		return this->i_posAbs();
	}
}

void IAbstractItem::RemoveLinks(){
	while (!m_setLinks.empty())
	{
		delete *(m_setLinks.begin());
	}
}
void IAbstractItem::MoveLinkTo(IAbstractItem* item){
	for each (BGLink* link in m_setLinks)
	{
		if (link->fromNode()==this)
		{
			link->setFromNode(item);
			item->addLink(link);
		}
		else if (link->toNode()==this)
		{
			link->setToNode(item);
			item->addLink(link);
		}
	}
}