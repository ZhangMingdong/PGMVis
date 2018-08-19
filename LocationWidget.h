#pragma once

#include <QWidget>


class QSpinBox;


class LocationWidget : public QWidget
{
	Q_OBJECT

public:
	explicit LocationWidget(QWidget *parent = 0);
	void setX(int x);
	void setY(int y);

signals:
	void updateLocation(int x, int y);
private slots:
	void locationChanged(int);

private:
	void createWidgets();
	void createLayout();
	void createConnections();

	QSpinBox *xSpinBox;
	QSpinBox *ySpinBox;
};
