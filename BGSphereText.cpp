#include "BGSphereText.h"

#include <QPainter>

int BGSphereText::s_nSeq = 0;

BGSphereText::BGSphereText()
{
	m_strLabel = 'C' + s_nSeq;
	s_nSeq++;
}


BGSphereText::~BGSphereText()
{
}

void BGSphereText::drawGraphic(QPainter *painter)
{
	QRectF rect = outlineRect();
	painter->drawEllipse(rect);
	painter->drawText(rect, Qt::AlignCenter, m_strLabel);

}
QPointF BGSphereText::getLinkPos(QPointF ptTarget){
	QPointF ptCenter = this->pos();

	QPointF dsp = ptTarget - ptCenter;
	double normDsp = sqrt(dsp.x()*dsp.x() + dsp.y()*dsp.y());
	double rate = m_nHeight / 2.0 / normDsp;
	dsp *= rate;
	return ptCenter + dsp;
}