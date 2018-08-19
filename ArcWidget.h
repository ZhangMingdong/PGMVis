#pragma once

#include <QWidget>

class QDoubleSpinBox;


class ArcWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ArcWidget(QWidget *parent = 0);

	void setAngleStart(double angle);
	void setAngleLen(double angle);

signals:
	void angleStartChanged(double angle);
	void angleLenChanged(double angle);

private:
	void createWidgets();
	void createLayout();
	void createConnections();

	QDoubleSpinBox *angleStartSpinBox;
	QDoubleSpinBox *angleLenSpinBox;
};
