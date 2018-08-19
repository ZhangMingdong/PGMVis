#pragma once

#include <QWidget>


class QSpinBox;


class GroupWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GroupWidget(QWidget *parent = 0);
	void setFromIndex(int index);
	void setToIndex(int index);
	void setFromIndexRange(int range);
	void setToIndexRange(int range);

signals:
	void updateIndex(int from,int to);
private slots:
	void indexChanged(int);

private:
	void createWidgets();
	void createLayout();
	void createConnections();

	QSpinBox *fromSpinBox;
	QSpinBox *toSpinBox;
};
