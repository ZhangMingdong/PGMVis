#pragma once

#include <QWidget>


class QSqlTableModel;
class QTableView;
class QSpinBox;

enum {
	Scooter_Id = 0,
	Scooter_Name = 1,
	Scooter_MaxSpeed = 2,
	Scooter_MaxRange = 3,
	Scooter_Weight = 4,
	Scooter_Description = 5
};


class DataWidget : public QWidget
{
	Q_OBJECT

public:
	explicit DataWidget(QWidget *parent = 0);

private:
	QSqlTableModel *model;
	QTableView *view;
	QSpinBox *m_pSpinDataId;
public:
	void SetDataId(int id);
	void SetRange(int range);

signals:
	void dataIdChanged(int id);

private:
	void createWidgets();
	void createLayout();
	void createConnections();

};

