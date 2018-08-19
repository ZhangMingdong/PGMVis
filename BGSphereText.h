#pragma once
#include "BGSphere.h"

#include <QString>
class BGSphereText:public BGSphere
{
public:
	BGSphereText();
	~BGSphereText();
private:
	QString m_strLabel;
	// ����
	static int s_nSeq;
protected:
	virtual void drawGraphic(QPainter *painter);
	virtual QPointF getLinkPos(QPointF ptTarget);
};

