#include "LinkPropertyWidget.h"

#include <QSpinBox>
#include <QFormLayout>

LinkPropertyWidget::LinkPropertyWidget()
{
	createWidgets();
	createLayout();
	createConnections();
	setFixedSize(minimumSizeHint());
}


LinkPropertyWidget::~LinkPropertyWidget()
{
}
void LinkPropertyWidget::createWidgets()
{
	weightSpinBox = new QSpinBox;
	weightSpinBox->setRange(-1024, 1024);
}

void LinkPropertyWidget::createLayout()
{
	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("Weight:"), weightSpinBox);
	setLayout(layout);
}

void LinkPropertyWidget::createConnections()
{
	connect(weightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(weightChanged(int)));
}

void LinkPropertyWidget::SetWeight(int weight){
	if (weight != weightSpinBox->value()) {
		blockSignals(true);
		weightSpinBox->setValue(weight);
		blockSignals(false);
	}
}



void LinkPropertyWidget::weightChanged(int){
	emit updateWeight(weightSpinBox->value());
}