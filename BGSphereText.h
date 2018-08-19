#pragma once
#include "BGSphere.h"

#include <QString>
/*
	Sphere with text
*/
class BGSphereText:public BGSphere
{
public:
	BGSphereText();
	~BGSphereText();
private:
	QString _strLabel;
	// ¼ÆÊý
	static int s_nSeq;
protected:
	virtual void drawGraphic(QPainter *painter);
	virtual QPointF getLinkPos(QPointF ptTarget);
};

