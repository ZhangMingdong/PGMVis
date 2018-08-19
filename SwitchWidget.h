#pragma once

#include <QWidget>

class QSpinBox;


class SwitchWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SwitchWidget(QWidget *parent = 0);

	void SetDataId(int id);
	void SetRange(int range);

signals:
	void dataIdChanged(int id);

private:
	void createWidgets();
	void createLayout();
	void createConnections();

	QSpinBox *m_pSpinDataId;
};
