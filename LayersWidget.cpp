#include "LayersWidget.h"
#include "LayerItemList.h"
#include <QCheckBox>
#include <QLayout>

uint LayersWidget::s_uId = 0;

LayersWidget::LayersWidget()
{
	m_uId = s_uId++;

	QVBoxLayout* layout = new QVBoxLayout();
	m_pListWidget = new LayerItemList();
	m_checkBoxVisible = new QCheckBox();
	m_checkBoxVisible->setChecked(true);
	m_checkBoxVisible->setText("Visible");
	layout->addWidget(m_checkBoxVisible);
	layout->addWidget(m_pListWidget);
	this->setLayout(layout);

	connect(m_checkBoxVisible, SIGNAL(clicked(bool)), this, SLOT(visibleChanged(bool)));

}


LayersWidget::~LayersWidget()
{
}

void LayersWidget::visibleChanged(bool checked)
{
	emit changeVisibility(this, checked);
}