#include "ArcWidget.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
namespace {
	const QChar Degree(0xB0);
	const QChar HorizontalArrow(0x21C6);
	const QChar VerticalArrow(0x21C5);
}

ArcWidget::ArcWidget(QWidget *parent)
: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
	setFixedSize(minimumSizeHint());
}


void ArcWidget::createWidgets()
{
	angleStartSpinBox = new QDoubleSpinBox;
	angleStartSpinBox->setRange(0.0, 360.0);
	angleStartSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	angleStartSpinBox->setDecimals(1);
	angleStartSpinBox->setSuffix(Degree);

	angleLenSpinBox = new QDoubleSpinBox;
	angleLenSpinBox->setRange(0.0, 360.0);
	angleLenSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	angleLenSpinBox->setDecimals(1);
	angleLenSpinBox->setSuffix(Degree);
}


void ArcWidget::createLayout()
{
	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("Angle Start"), angleStartSpinBox);
	layout->addRow(tr("Angle Length"), angleLenSpinBox);
	setLayout(layout);
}


void ArcWidget::createConnections()
{
	connect(angleStartSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(angleStartChanged(double)));
	connect(angleLenSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(angleLenChanged(double)));
}


void ArcWidget::setAngleStart(double angle)
{
	if (!qFuzzyCompare(angle, angleStartSpinBox->value())) {
		blockSignals(true);
		angleStartSpinBox->setValue(angle);
		blockSignals(false);
	}
}


void ArcWidget::setAngleLen(double angle)
{
	if (!qFuzzyCompare(angle, angleLenSpinBox->value())) {
		blockSignals(true);
		angleLenSpinBox->setValue(angle);
		blockSignals(false);
	}
}



