#include "LocationWidget.h"


#include <QSpinBox>
#include <QFormLayout>


LocationWidget::LocationWidget(QWidget *parent)
: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
	setFixedSize(minimumSizeHint());
}

void LocationWidget::createWidgets()
{
	xSpinBox = new QSpinBox;
	xSpinBox->setRange(-1024, 1024);
	xSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	ySpinBox = new QSpinBox;
	ySpinBox->setRange(-1024, 1024);
	ySpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

}

void LocationWidget::createLayout()
{
	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("X:"), xSpinBox);
	layout->addRow(tr("Y:"), ySpinBox);
	setLayout(layout);
}

void LocationWidget::createConnections()
{
	connect(xSpinBox, SIGNAL(valueChanged(int)), this, SLOT(locationChanged(int)));
	connect(ySpinBox, SIGNAL(valueChanged(int)), this, SLOT(locationChanged(int)));
}

void LocationWidget::setX(int x){
	if (x != xSpinBox->value()) {
		blockSignals(true);
		xSpinBox->setValue(x);
		blockSignals(false);
	}
}

void LocationWidget::setY(int y){
	if (y != ySpinBox->value()) {
		blockSignals(true);
		ySpinBox->setValue(y);
		blockSignals(false);
	}
}

void LocationWidget::locationChanged(int){
	emit updateLocation(xSpinBox->value(), ySpinBox->value());
}