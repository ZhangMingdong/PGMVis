#include "SwitchWidget.h"

#include <QDoubleSpinBox>
#include <QFormLayout>

SwitchWidget::SwitchWidget(QWidget *parent)
: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
	setFixedSize(minimumSizeHint());
}


void SwitchWidget::createWidgets()
{
	m_pSpinDataId = new QSpinBox;
	m_pSpinDataId->setRange(0,5);
	m_pSpinDataId->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

}


void SwitchWidget::createLayout()
{
	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("Data Id"), m_pSpinDataId);
	setLayout(layout);
}


void SwitchWidget::createConnections()
{
	connect(m_pSpinDataId, SIGNAL(valueChanged(int)), this, SIGNAL(dataIdChanged(int)));
}

void SwitchWidget::SetDataId(int id){
	if (id != m_pSpinDataId->value())
	{
		blockSignals(true);
		m_pSpinDataId->setValue(id);
		blockSignals(false);
	}
}
void SwitchWidget::SetRange(int range){
	m_pSpinDataId->setRange(0,range);
}


