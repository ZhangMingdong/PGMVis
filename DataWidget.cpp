
#include "swatch.h"
#include "DataWidget.h"
#include <QComboBox>
#include <QFormLayout>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QLabel>
#include <QSpinBox>


DataWidget::DataWidget(QWidget *parent)
: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();



	setWindowTitle(tr("Data"));

}
void DataWidget::createWidgets()
{
	// 	model = new QSqlTableModel(this);
	// 	model->setTable("scooter");
	// 	model->setSort(Scooter_Name, Qt::AscendingOrder);
	// 	model->setHeaderData(Scooter_Name, Qt::Horizontal, tr("Name"));
	// 	model->setHeaderData(Scooter_MaxSpeed, Qt::Horizontal, tr("MPH"));
	// 	model->setHeaderData(Scooter_MaxRange, Qt::Horizontal, tr("Miles"));
	// 	model->setHeaderData(Scooter_Weight, Qt::Horizontal, tr("Lbs"));
	// 	model->setHeaderData(Scooter_Description, Qt::Horizontal, tr("Description"));
	// 	model->select();
	// 
	// 	view = new QTableView;
	// 	view->setModel(model);
	// 	view->setSelectionMode(QAbstractItemView::SingleSelection);
	// 	view->setSelectionBehavior(QAbstractItemView::SelectRows);
	// 	view->setColumnHidden(Scooter_Id, true);
	// 	view->resizeColumnsToContents();
	// 	view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// 
	// 	QHeaderView *header = view->horizontalHeader();
	// 	header->setStretchLastSection(true);
	// 
	// 	QHBoxLayout *mainLayout = new QHBoxLayout;
	// 	mainLayout->addWidget(view);
	// 	setLayout(mainLayout);
	// 
	// 	setWindowTitle(tr("Scooters"));

	QSqlDatabase db = QSqlDatabase::database("dbTemp");
	// 	QStringList list = db.tables();
	// 
	// 	QSqlQuery query(db);
	// 	query.exec("SELECT ID, Value FROM SCD_RTAI");
	// 	while (query.next()) {
	// 		QString title = query.value(0).toString();
	// 		int year = query.value(1).toInt();
	// 		qDebug() << qPrintable(title) << ": " << year << "\n";
	// 	}


	model = new QSqlTableModel(this, db);


	// 	QMessageBox::critical(0, QObject::tr("Database Error"), model->lastError().text());

	model->setTable(tr("SCD_RTDI"));
	model->setSort(Scooter_Id, Qt::AscendingOrder);
	model->setHeaderData(Scooter_Id, Qt::Horizontal, tr("ID"));
	model->setHeaderData(Scooter_Name, Qt::Horizontal, tr("Value"));
	// 	model->setHeaderData(Scooter_MaxRange, Qt::Horizontal, tr("Miles"));
	// 	model->setHeaderData(Scooter_Weight, Qt::Horizontal, tr("Lbs"));
	// 	model->setHeaderData(Scooter_Description, Qt::Horizontal, tr("Description"));
	model->select();
	// 	QMessageBox::critical(0, QObject::tr("Database Error"),	model->lastError().text());

	view = new QTableView;
	view->setModel(model);
	view->setSelectionMode(QAbstractItemView::SingleSelection);
	view->setSelectionBehavior(QAbstractItemView::SelectRows);
	view->setColumnHidden(Scooter_Id, true);
	view->resizeColumnsToContents();
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QHeaderView *header = view->horizontalHeader();
	header->setStretchLastSection(true);


}


void DataWidget::createLayout()
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(view);
	setLayout(mainLayout);
	QHBoxLayout *idLayout = new QHBoxLayout;

	m_pSpinDataId = new QSpinBox;
	m_pSpinDataId->setRange(-1, 5);
	m_pSpinDataId->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	QLabel* lable = new QLabel;
	lable->setText("Data Id:");
	idLayout->addWidget(lable);
	idLayout->addWidget(m_pSpinDataId);
	mainLayout->addLayout(idLayout);
}


void DataWidget::createConnections()
{
	connect(m_pSpinDataId, SIGNAL(valueChanged(int)), this, SIGNAL(dataIdChanged(int)));
}

void DataWidget::SetDataId(int id){
	if (id != m_pSpinDataId->value())
	{
		blockSignals(true);
		m_pSpinDataId->setValue(id);
		blockSignals(false);
	}
}
void DataWidget::SetRange(int range){
	m_pSpinDataId->setRange(0, range);
}
