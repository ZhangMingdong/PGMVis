#include <QtGui>

#include "BGLink.h"
#include "XGSWitch.h"

#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QColorDialog>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

XGSWitch::XGSWitch() :BGRectBase()
{
	// 	myTextColor = Qt::darkGreen;
	// 	myOutlineColor = Qt::darkBlue;
	// 	myBackgroundColor = Qt::white;

	// 	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
	m_nHeight = 15;
	m_nWidth = 60;
}

XGSWitch::~XGSWitch()
{
}

QRectF XGSWitch::boundingRect() const
{
	const int Margin = 1;
	int nRadius = m_nWidth > m_nHeight ? m_nWidth : m_nHeight;
	QRectF rect(-nRadius / 2.0, -nRadius / 2.0, nRadius, nRadius);
	rect.adjust(-Margin, -Margin, +Margin, +Margin);
	return rect;

}

QPainterPath XGSWitch::shape() const
{
	QRectF rect = outlineRect();

	QPainterPath path;
	path.addRect(rect);
	// 	path.addRoundRect(rect, roundness(rect.width()),
	// 		roundness(rect.height()));
	return path;
}

void XGSWitch::drawGraphic(QPainter *painter)
{
	QSqlDatabase db = QSqlDatabase::database("dbTemp");
	QSqlQuery query(QString("SELECT Value FROM SCD_RTDI WHERE ID = %1").arg(m_nDataId),db);
	int switchState = 0;
	if (query.next()){
		switchState = query.value(0).toInt();
		qDebug() << switchState << "\n";
	}
	if (switchState)
	{
		painter->setBrush(Qt::black);
	}

	painter->drawRect(outlineRect());
}

void XGSWitch::paintSelectionOutline(QPainter *painter)
{
	painter->drawRect(outlineRect());
}

void XGSWitch::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_nHeight!=m_nWidth)
	{
		int nTemp = m_nHeight;
		m_nHeight = m_nWidth;
		m_nWidth = nTemp;
		this->update();
		updateLink();
		emit dirty();
	}
}
