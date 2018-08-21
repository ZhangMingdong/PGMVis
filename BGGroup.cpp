#include "BGGroup.h"
#include "BGLink.h"

#include <QGraphicsScene>
#include <QMessageBox>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

#include "IBasicGraphicItem.h"

BGGroup::BGGroup() :m_nIndexFrom(0)
, m_nIndexTo(0)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
	QGraphicsItemGroup::setHandlesChildEvents(false);
}

BGGroup::~BGGroup()
{
}

void BGGroup::Write(QXmlStreamWriter& writer)
{
	IIdInterface* from = dynamic_cast<IIdInterface*>(childItems()[m_nIndexFrom]);
	IIdInterface* to = dynamic_cast<IIdInterface*>(childItems()[m_nIndexTo]);

	writer.writeStartElement("group");
	writer.writeTextElement("id", this->GetId());
	writer.writeTextElement("from", from->GetId());
	writer.writeTextElement("to", to->GetId());

// 	writer.writeTextElement("x", QString::number(QGraphicsItemGroup::x()));
// 	writer.writeTextElement("y", QString::number(QGraphicsItemGroup::y()));
	writePosition(writer);
	writetransform(writer);


	writer.writeEndElement();
}

void BGGroup::Read(QXmlStreamReader& reader){
	reader.readNext();		// skip <sphere>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "x")
			{
				m_dbX = readDoubleValue(reader);
			}
			else if (reader.name() == "y")
			{
				m_dbY = readDoubleValue(reader);
			}
			else if (reader.name() == "from")
			{
				m_idFrom = readStringValue(reader);
			}
			else if (reader.name() == "to")
			{
				m_idTo = readStringValue(reader);
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
			else if (!readId(reader)&&!readGroup(reader)){
				skipUnknownElement(reader);
			}
		}
		else {
			reader.readNext();
		}
	}
}

void BGGroup::InitPosition()
{
	int nChild = this->childItems().length();
	for (int i = 0; i < nChild;i++)
	{
		IIdInterface* idf = dynamic_cast<IIdInterface*>(this->childItems()[i]);
		if (idf->GetId() == m_idFrom)
		{
			m_nIndexFrom = i;
		}
		else if (idf->GetId()==m_idTo)
		{
			m_nIndexTo = i;
		}
	}

	QGraphicsItemGroup::setPos(m_dbX, m_dbY);
	updateTransform();
	updateLink();
}

void BGGroup::updateLink(){
	foreach(BGLink *link, m_setLinks)
		link->trackNodes();
	int n = this->childItems().size();
	for each (QGraphicsItem *child in this->childItems())
	{
		IAbstractItem* item = dynamic_cast<IAbstractItem*>(child);
		if (item)
		{
			item->updateLink();
		}
	}
}

QVariant BGGroup::itemChange(GraphicsItemChange change, const QVariant &value)
{
	QString str = this->GetId();
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
	return QGraphicsItemGroup::itemChange(change, value);
}

QPointF BGGroup::i_posAbs()
{
	QRectF rect = this->outlineRect();
	// 	QPointF ptt = this->pos();
	rect.translate(this->pos());
	return rect.center();
}

QPointF BGGroup::i_posRel()
{
	return this->pos();
}
QPointF BGGroup::i_posGrp()
{
	BGGroup* group = i_group();
	return group ? i_posRel() + group->i_posGrp() : i_posRel();
}


QPointF BGGroup::getRotationCenter(){
	return childItems()[0]->pos();
}

QString BGGroup::getPortId(int index){
	if (index>=0 && index<childItems().length())
	{
		IIdInterface* it = dynamic_cast<IIdInterface*>(childItems()[index]);
		return it->GetId();
	}
	else{
		return "";
	}
}

void BGGroup::updateTransform()
{
	QPointF pt = getRotationCenter();
	QTransform transform;
	transform.translate(pt.x(),pt.y());
	transform.shear(m_shearHorizontal, m_shearVertical);
	transform.rotate(m_angle);
	transform.translate(-pt.x(),-pt.y());
	setTransform(transform);
}

bool BGGroup::i_isSelected(){
	return isSelected();
}

void BGGroup::i_update(){
	update();
}

void BGGroup::setFromIndex(int index){
	m_nIndexFrom = index;
}

void BGGroup::setToIndex(int index){
	m_nIndexTo = index;
}

int BGGroup::indexRange(){
	return this->childItems().length()-1;
}

int BGGroup::fromIndex(){
	return m_nIndexFrom;
}

int BGGroup::toIndex(){
	return m_nIndexTo;
}

void BGGroup::setLinkIndex(int from, int to){
	if (i_isSelected())
	{
		setFromIndex(from);
		setToIndex(to);
		updateLink();

		emit dirty();
	}
	
}

void BGGroup::SetUpGroup(QList<QGraphicsItem*> items){
	for each (QGraphicsItem* item in items)
	{
		AddItem(item);
	}
}

int BGGroup::getChildPos(QUuid id){
	int nChild = this->childItems().length();
	for (int i = 0; i < nChild; i++)
	{
		IIdInterface* idf = dynamic_cast<IIdInterface*>(this->childItems()[i]);
		if (idf->GetId() == id.toString())
		{
			return i;
		}
	}
	return 0;
}

int BGGroup::x(){
	return QGraphicsItem::x();
}

int BGGroup::y(){
	return QGraphicsItem::y();
}

void BGGroup::setX(int x){
	QGraphicsItem::setX(x);
}

void BGGroup::setY(int y){
	QGraphicsItem::setY(y);
}

void BGGroup::AddItem(QGraphicsItem* item){
	this->addToGroup(item);
// 	if (item->zValue()>this->zValue())
// 	{
// 		setZValue(item->zValue() + 1);
// 	}
}

void BGGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
// 	qDebug() << "begin\n";
	painter->setPen(m_pen);
	painter->setBrush(m_brush);

	QRectF rect = outlineRect();
	painter->drawRoundRect(rect, roundness(rect.width()), roundness(rect.height()));

	if (option->state & QStyle::State_Selected) {
		QPen pen(Qt::DashLine);
		pen.setWidth(2);
		pen.setColor(Qt::black);
		painter->setPen(pen);
		painter->setBrush(Qt::NoBrush);
		paintSelectionOutline(painter);
	}


// 	qDebug() << "end\n";

	// 	painter->setPen(myTextColor);
	// 	painter->drawText(rect, Qt::AlignCenter, myText);
}

QRectF BGGroup::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QRectF BGGroup::outlineRect() const
{
	const int Padding = 8;
	QRectF rect = this->childItems()[0]->boundingRect();
	rect.translate(this->childItems()[0]->pos());
	for each (QGraphicsItem* item in this->childItems())
	{
		QRectF cRect = item->boundingRect();
		cRect.translate(item->pos());
		if (cRect.left() < rect.left())
		{
			rect.setLeft(cRect.left());
		}
		if (cRect.top() < rect.top())
		{
			rect.setTop(cRect.top());
		}
		if (cRect.right() > rect.right())
		{
			rect.setRight(cRect.right());
		}
		if (cRect.bottom() > rect.bottom())
		{
			rect.setBottom(cRect.bottom());
		}
	}
	rect.adjust(-Padding, -Padding, +Padding, +Padding);
	return rect;
}

int BGGroup::roundness(double size) const
{
	const int Diameter = 12;
	return 100 * Diameter / int(size);
}

QPointF BGGroup::getLinkPos(QPointF ptTarget){
	qDebug() << "getLinkPos" << endl;
	// 1.get the center of this group
	QRectF rect = this->outlineRect();
	QPointF ptCenter = i_posAbs();
	// 2.calculate bias
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

BGGroup* BGGroup::i_group(){
	return dynamic_cast<BGGroup*>(this->group());
}
