#pragma once

#include <QWidget>


class QDoubleSpinBox;


class TransformWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TransformWidget(QWidget *parent = 0);

	void setAngle(double angle);
	void setShear(double shearHorizontal, double shearVertical);

signals:
	void angleChanged(double angle);
	void shearChanged(double shearHorizontal, double shearVertical);

private slots:
	void updateShear();

private:
	void createWidgets();
	void createLayout();
	void createConnections();

	QDoubleSpinBox *angleSpinBox;
	QDoubleSpinBox *shearHorizontalSpinBox;
	QDoubleSpinBox *shearVerticalSpinBox;
};
