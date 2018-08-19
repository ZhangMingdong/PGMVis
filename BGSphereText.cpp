#include "BGSphereText.h"

#include <QPainter>

int BGSphereText::s_nSeq = 0;

BGSphereText::BGSphereText()
{
	_strLabel = 'C' + s_nSeq;
	s_nSeq++;
}


BGSphereText::~BGSphereText()
{
}

void BGSphereText::drawGraphic(QPainter *painter)
{
	QRectF rect = outlineRect();
	painter->drawEllipse(rect);
	painter->drawText(rect, Qt::AlignCenter, _strLabel);

}
