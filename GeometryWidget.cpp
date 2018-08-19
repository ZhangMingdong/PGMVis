#include "GeometryWidget.h"


#include <QSpinBox>
#include <QFormLayout>





GeometryWidget::GeometryWidget(QWidget *parent)
: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
	setFixedSize(minimumSizeHint());
}


void GeometryWidget::createWidgets()
{
// 	xSpinBox = new QSpinBox;
// 	xSpinBox->setRange(0, 1024);
// 	xSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
// 
// 	ySpinBox = new QSpinBox;
// 	ySpinBox->setRange(0, 1024);
// 	ySpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	widthSpinBox = new QSpinBox;
	widthSpinBox->setRange(0, 100);
	widthSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	heightSpinBox = new QSpinBox;
	heightSpinBox->setRange(0, 100);
	heightSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
}


void GeometryWidget::createLayout()
{
	QFormLayout *layout = new QFormLayout;
// 	layout->addRow(tr("X:"), xSpinBox);
// 	layout->addRow(tr("Y:"), ySpinBox);
	layout->addRow(tr("Width:"), widthSpinBox);
	layout->addRow(tr("Height:"), heightSpinBox);
	setLayout(layout);
}


void GeometryWidget::createConnections()
{
// 	connect(xSpinBox, SIGNAL(valueChanged(int)), this, SLOT(geometryChanged(int)));
// 	connect(ySpinBox, SIGNAL(valueChanged(int)), this, SLOT(geometryChanged(int)));
	connect(widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(geometryChanged(int)));
	connect(heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(geometryChanged(int)));
}

// void GeometryWidget::setX(int x){
// 	if (x != xSpinBox->value()) {
// 		blockSignals(true);
// 		xSpinBox->setValue(x);
// 		blockSignals(false);
// 	}
// }
// void GeometryWidget::setY(int y){
// 	if (y != ySpinBox->value()) {
// 		blockSignals(true);
// 		ySpinBox->setValue(y);
// 		blockSignals(false);
// 	}
// }
void GeometryWidget::setW(int w){
	if (w != widthSpinBox->value()) {
		blockSignals(true);
		widthSpinBox->setValue(w);
		blockSignals(false);
	}
}
void GeometryWidget::setH(int h){
	if (h != heightSpinBox->value()) {
		blockSignals(true);
		heightSpinBox->setValue(h);
		blockSignals(false);
	}
}
void GeometryWidget::geometryChanged(int){
	emit updateGeometry(/*xSpinBox->value(), ySpinBox->value(),*/ widthSpinBox->value(), heightSpinBox->value());
}