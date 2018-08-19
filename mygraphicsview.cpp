#include "mygraphicsview.h"

#include "BGSphere.h"
#include "BGRect.h"
#include "BGLine.h"
#include "BGRectangleText.h"
#include "IAbstractItem.h"
#include "BGPolygon.h"

#include <QMouseEvent>
#include <QAction>

MyGraphicsView::MyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
	, m_currentState(None)
	, m_bDrawing(false)
	, m_bShowGrids(false)
	, m_pDrawingPolygon(NULL)
{
}

MyGraphicsView::~MyGraphicsView()
{

}

void MyGraphicsView::mouseMoveEvent(QMouseEvent * event)
{
	if (!m_bDrawing){
		QGraphicsView::mouseMoveEvent(event);
		return;
	}

	m_ptCurrent = mapToScene(event->pos());
	switch (m_currentState)
	{
	case Draw_Sphere:
		m_pDrawingSphere->setWidth(2*abs(m_ptCurrent.x()-m_ptMousePress.x()));
		m_pDrawingSphere->setHeight(2*abs(m_ptCurrent.y() - m_ptMousePress.y()));
		break;
	case Draw_Rect:
		m_pDrawingRect->setWidth(2 * abs(m_ptCurrent.x() - m_ptMousePress.x()));
		m_pDrawingRect->setHeight(2 * abs(m_ptCurrent.y() - m_ptMousePress.y()));
		break;
	case Draw_Line:
		m_pDrawingLine->SetTo(m_ptCurrent);
		break;
	case Draw_Text:
		m_pDrawingText->setPos(m_ptCurrent);
		break;
	case Draw_Link:
		{
			QGraphicsItem* g = itemAt(event->pos());
			IAbstractItem* item = dynamic_cast<IAbstractItem*>(g);
			if (item && item!=m_pDrawingLine && item!=m_pLinkFrom)
			{
				m_pDrawingLine->SetTo(item->getItemCenter());
			}
			else{
				m_pDrawingLine->SetTo(m_ptCurrent);
			}
		}
		break;
	default:
		break;
	}
	this->scene()->update();
}

void MyGraphicsView::mousePressEvent(QMouseEvent * event)
{
// 	if (itemAt(event->pos())){
// 		QGraphicsView::mousePressEvent(event);
// 		return;
// 	}
	m_ptMousePress = mapToScene(event->pos());
	switch (m_currentState)
	{
	case Draw_Sphere:
		m_pDrawingSphere = new BGSphere();
		m_pDrawingSphere->setX(m_ptMousePress.x());
		m_pDrawingSphere->setY(m_ptMousePress.y());
		m_pDrawingSphere->setWidth(5);
		m_pDrawingSphere->setHeight(5);
		scene()->addItem(m_pDrawingSphere);
		scene()->clearSelection();
		m_pDrawingSphere->setSelected(true);
		m_bDrawing = true;
		break;
	case Draw_Rect:
		m_pDrawingRect = new BGRect();
		m_pDrawingRect->setX(m_ptMousePress.x());
		m_pDrawingRect->setY(m_ptMousePress.y());
		m_pDrawingRect->setWidth(5);
		m_pDrawingRect->setHeight(5);
		scene()->addItem(m_pDrawingRect);
		scene()->clearSelection();
		m_pDrawingRect->setSelected(true);
		m_bDrawing = true;
		break;
	case Draw_Line:
		m_pDrawingLine = new BGLine();
		m_pDrawingLine->SetFrom(m_ptMousePress);
		m_pDrawingLine->SetTo(m_ptMousePress);
		scene()->addItem(m_pDrawingLine);
		scene()->clearSelection();
		m_pDrawingLine->setSelected(true);
		m_bDrawing = true;
		break;
	case Draw_Text:
		m_pDrawingText = new BGRectangleText();
		m_pDrawingText->setPos(m_ptMousePress);
		m_pDrawingText->SetText(tr("new Text"));

		scene()->addItem(m_pDrawingText);
		scene()->clearSelection();
		m_pDrawingText->setSelected(true);
		m_bDrawing = true;
		break;
	case Draw_Link:
		{
			QGraphicsItem* g = itemAt(event->pos());
			m_pLinkFrom = dynamic_cast<IAbstractItem*>(g);
			if (m_pLinkFrom)
			{
				m_pDrawingLine = new BGLine();
				m_pDrawingLine->SetFrom(m_pLinkFrom->getItemCenter());
				m_pDrawingLine->SetTo(m_pLinkFrom->getItemCenter());
				scene()->addItem(m_pDrawingLine);
				scene()->clearSelection();
				m_pDrawingLine->setSelected(true);
				m_bDrawing = true;
			}
		}
		break;
	case Draw_Polygon:
		if (event->button()==Qt::LeftButton)
		{
			if (!m_pDrawingPolygon)
			{
				m_pDrawingPolygon = new BGPolygon();
				scene()->addItem(m_pDrawingPolygon);
				scene()->clearSelection();
				m_pDrawingPolygon->setSelected(true);

			}
			QPolygonF poly = m_pDrawingPolygon->polygon();
			poly << m_ptMousePress;
			m_pDrawingPolygon->setPolygon(poly);
		}
		else if (event->button()==Qt::RightButton)
		{
			m_pDrawingPolygon = NULL;
		}
		break;
	default:
		QGraphicsView::mousePressEvent(event);
		emit MousePress();
		break;
	}	

}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent * event)
{
	if (!m_bDrawing){
		QGraphicsView::mouseReleaseEvent(event);
		emit MouseRelease();
		return;
	}
	m_ptCurrent = mapToScene(event->pos());
	switch (m_currentState)
	{
	case Draw_Sphere:
	{
		int width = 2 * abs(m_ptCurrent.x() - m_ptMousePress.x());
		int height = 2 * abs(m_ptCurrent.y() - m_ptMousePress.y());
		m_pDrawingSphere->setWidth(width);
		m_pDrawingSphere->setHeight(height);
		if (width<5&&height<5)
		{
			scene()->removeItem(m_pDrawingSphere);
		}
		else{
			emit DrawGraphicsItem(m_pDrawingSphere);
		}
		break;

	}
	case Draw_Rect:
	{
		int width = 2 * abs(m_ptCurrent.x() - m_ptMousePress.x());
		int height = 2 * abs(m_ptCurrent.y() - m_ptMousePress.y());
		m_pDrawingRect->setWidth(width);
		m_pDrawingRect->setHeight(height);
		if (width<5&&height<5)
		{
			scene()->removeItem(m_pDrawingRect);
		}
		else{
			emit DrawGraphicsItem(m_pDrawingRect);
		}
		break;

	}
	case Draw_Line:
	{
		int width = 2 * abs(m_ptCurrent.x() - m_ptMousePress.x());
		int height = 2 * abs(m_ptCurrent.y() - m_ptMousePress.y());
		m_pDrawingLine->SetTo(m_ptCurrent);
		if (width < 5 && height < 5)
		{
			scene()->removeItem(m_pDrawingLine);
		}
		else{
			m_pDrawingLine->FinishedDrawing();
			emit DrawGraphicsItem(m_pDrawingLine);
		}
		break;
		}
	case Draw_Text:
		emit DrawGraphicsItem(m_pDrawingText);
		break;
	case Draw_Link:
		{
			QGraphicsItem* g = itemAt(event->pos());
			IAbstractItem* item = dynamic_cast<IAbstractItem*>(g);
			if (item)
			{
				emit DrawLink(m_pLinkFrom, item);
			}
			scene()->removeItem(m_pDrawingLine);
		}
		break;
	default:
		QGraphicsView::mouseReleaseEvent(event);
		break;
	}
	m_ptMousePress = QPoint(0, 0);
	m_ptCurrent = QPoint(0, 0);
	m_bDrawing = false;
}

void MyGraphicsView::drawItem(bool b)
{
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;
	QObject *item = 0;
	int type = action->data().toInt();

	switch (type)
	{
	case TextItemType:
		m_currentState = Draw_Text;
		break;
	case LineItemType:
		m_currentState = Draw_Line;
		break;
	case RectItemType:
		m_currentState = Draw_Rect;
		break;
	case SphereItemType:
		m_currentState = Draw_Sphere;
		break;
	case PolygonItemType:
		m_currentState = Draw_Polygon;
		break;
	case LinkItemType:
		m_currentState = Draw_Link;
		break;
	default:
		m_currentState = None;
		return;
	}
	// 	if (type == BoxItemType)
	// 		item = new BoxItem(QRect(position(), QSize(90, 30)), scene);
	// 	else if (type == SmileyItemType)
	// 		item = new SmileyItem(position(), scene);
	// 	else if (type == TextItemType) {
	// 		TextItemDialog dialog(0, position(), scene, this);
	// 		if (dialog.exec())
	// 			item = dialog.textItem();
	// 	}
	// 	if (item) {
	// 		connectItem(item);
	// 		setDirty(true);
	// 	}
}

void MyGraphicsView::viewShowGrid(bool on){
	m_bShowGrids = on;
	this->scene()->update();
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
	scaleBy(std::pow(4.0 / 3.0, (-event->delta() / 240.0)));
}

void MyGraphicsView::drawBackground(QPainter *painter, const QRectF &rect){
	// draw grids
	if (m_bShowGrids)
	{
		const int GridSize = 40;
		QPen pen(QColor(175, 175, 175, 127));
		const int MaxX = static_cast<int>(std::ceil(this->scene()->width()) / GridSize) * GridSize;
		const int MaxY = static_cast<int>(std::ceil(scene()->height()) / GridSize) * GridSize;
		painter->setPen(pen);
		for (int x = 0; x <= MaxX; x += GridSize) {
			painter->drawLine(x, 0, x, MaxY);
		}
		for (int y = 0; y <= MaxY; y += GridSize) {
			painter->drawLine(0, y, MaxX, y);
		}
	}
// 	else{
// 		QGraphicsView::drawBackground(painter, rect);
// 	}
}