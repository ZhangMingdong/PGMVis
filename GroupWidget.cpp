#include "GroupWidget.h"


#include <QSpinBox>
#include <QFormLayout>

GroupWidget::GroupWidget(QWidget *parent)
: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
	setFixedSize(minimumSizeHint());
}

void GroupWidget::createWidgets()
{
	fromSpinBox = new QSpinBox;
	fromSpinBox->setRange(0, 0);
	fromSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	toSpinBox = new QSpinBox;
	toSpinBox->setRange(0, 0);
	toSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

}

void GroupWidget::createLayout()
{
	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("From:"), fromSpinBox);
	layout->addRow(tr("To:"), toSpinBox);
	setLayout(layout);
}

void GroupWidget::createConnections()
{
	connect(fromSpinBox, SIGNAL(valueChanged(int)), this, SLOT(indexChanged(int)));
	connect(toSpinBox, SIGNAL(valueChanged(int)), this, SLOT(indexChanged(int)));
}

void GroupWidget::setFromIndex(int index){
	if (index != fromSpinBox->value()) {
		blockSignals(true);
		fromSpinBox->setValue(index);
		blockSignals(false);
	}
}

void GroupWidget::setToIndex(int index){
	if (index != toSpinBox->value()) {
		blockSignals(true);
		toSpinBox->setValue(index);
		blockSignals(false);
	}
}

void GroupWidget::indexChanged(int){
	emit updateIndex(fromSpinBox->value(), toSpinBox->value());
}

void GroupWidget::setFromIndexRange(int range){

	fromSpinBox->setRange(0, range);
}

void GroupWidget::setToIndexRange(int range){

	toSpinBox->setRange(0, range);
}