#pragma once
#include <QWidget>


class QSpinBox;

class LinkPropertyWidget : public QWidget
{
	Q_OBJECT

public:
	LinkPropertyWidget();
	~LinkPropertyWidget(); 
public:
	void SetWeight(int weight);
signals:
	void updateWeight(int weight);
private slots:
	void weightChanged(int);

private:
	void createWidgets();
	void createLayout();
	void createConnections();

	QSpinBox *weightSpinBox;
};

