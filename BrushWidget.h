#pragma once

#include <QBrush>
#include <QWidget>


class QComboBox;


class BrushWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BrushWidget(QWidget *parent = 0);

	QBrush brush() const { return m_brush; }

	public slots:
	void setBrush(const QBrush &brush);

signals:
	void brushChanged(const QBrush &brush);

private slots:
	void updateColor(int index);
	void updateStyle(int index);

private:
	void createWidgets();
	void createLayout();
	void createConnections();
	void updateSwatches();

	QComboBox *colorComboBox;
	QComboBox *styleComboBox;

	QBrush m_brush;
};
