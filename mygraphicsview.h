#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
class BGSphere;
class BGRect;
class BGLine;
class BGRectangleText;
class IBasicGraphicItem;
class IAbstractItem;
class BGPolygon;

class MyGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	MyGraphicsView(QWidget *parent = 0);
	~MyGraphicsView();

private:
	enum State
	{
		None = 0,
		Draw_Sphere,
		Draw_Rect,
		Draw_Line,
		Draw_Text,
		Draw_Polygon,
		Draw_Link
	};
	State m_currentState;
	QPointF m_ptMousePress;
	QPointF m_ptCurrent;
	BGSphere* m_pDrawingSphere;
	BGRect* m_pDrawingRect;
	BGLine* m_pDrawingLine;
	BGRectangleText* m_pDrawingText;
	BGPolygon* m_pDrawingPolygon;
	bool m_bDrawing;
	IAbstractItem* m_pLinkFrom;
	bool m_bShowGrids;
protected:
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event); 
	virtual void wheelEvent(QWheelEvent *event);
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
signals:
	void DrawGraphicsItem(IAbstractItem* item);
	void DrawLink(IAbstractItem* from, IAbstractItem* to);
	// inform mainwindow the mouse events
	void MousePress();
	void MouseRelease();

public slots:
	void drawItem(bool b);
	void viewShowGrid(bool on);
public slots:
	void zoomIn() { scaleBy(1.1); }
	void zoomOut() { scaleBy(1.0 / 1.1); }

private:
	void scaleBy(double factor) { scale(factor, factor); }
};

#endif // MYGRAPHICSVIEW_H
