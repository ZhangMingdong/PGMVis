#pragma once

#include <QWidget>


class QSpinBox;


class GeometryWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GeometryWidget(QWidget *parent = 0);
// 	void setX(int x);
// 	void setY(int y);
	void setW(int w);
	void setH(int h);

signals:
	void updateGeometry(/*int x, int y, */int w, int h);
private slots:
	void geometryChanged(int);

private:
	void createWidgets();
	void createLayout();
	void createConnections();
// 
// 	QSpinBox *xSpinBox;
// 	QSpinBox *ySpinBox;
	QSpinBox *widthSpinBox;
	QSpinBox *heightSpinBox;
};
