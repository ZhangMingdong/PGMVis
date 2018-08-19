#pragma once
#include <QWidget>
class LayerItemList;
class QCheckBox;
class LayersWidget : public QWidget
{
	Q_OBJECT
public:
	LayersWidget();
	~LayersWidget();
public:
	LayerItemList* m_pListWidget;
	QCheckBox* m_checkBoxVisible;
private slots:
	void visibleChanged(bool checked);
signals:
	void changeVisibility(LayersWidget* panel, bool visible);
public:
	static uint s_uId;
private:
	uint m_uId;
public:
	uint GetId(){ return m_uId; }
};

