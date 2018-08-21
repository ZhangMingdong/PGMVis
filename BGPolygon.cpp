#include "BGPolygon.h"
#include <QPen>
#include <QGraphicsScene>
#include <QDebug>

const double PI = 3.14159265358979323846;

BGPolygon::BGPolygon()
{
	QPen pen;
	pen.setColor(Qt::black);
	QGraphicsPolygonItem::setPen(pen);
// 	QPolygon poly;
// 	poly << QPoint(0, 0) << QPoint(100, 0) << QPoint(100, 100) << QPoint(0, 100);
// 	setPolygon(poly);

	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

}


BGPolygon::~BGPolygon()
{
}

void BGPolygon::AppendPoint(const QPointF& pt){
	this->polygon() << pt;
}

void BGPolygon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	BGPolygon* newPoly = new BGPolygon;
	newPoly->setPolygon(calc());
	this->scene()->addItem(newPoly);
}
QPolygonF BGPolygon::calc()
{
	QRectF rect = this->boundingRect();
	QPolygonF thisPolygon = this->polygon();
	QPolygonF maxPolygon;
	QPolygonF currentPolygon;
	double maxArea = 0;
	int nPoints = 0;
	while (nPoints < 20)
	{
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		QPointF ptCenter = QPointF(rect.x() + rect.width()*x, rect.y()*rect.height()*y);
		if (thisPolygon.containsPoint(ptCenter, Qt::OddEvenFill))
		{
			for (int ort = -90; ort < 90; ort += 5)
			{
// 				ort = 30;
				double sinOrt = sin(ort*PI / 180.0);
				double cosOrt = cos(ort*PI / 180.0);
				for (int aspr = 1; aspr < 15; aspr++)
				{
					double newArea = generateRectangle(sinOrt,cosOrt, ptCenter, aspr, currentPolygon);
// 					return currentPolygon;
					if (newArea>maxArea)
					{
						maxArea = newArea;
						maxPolygon = currentPolygon;
					}
				}
			}
			nPoints++;
		}
	}
	// this algorithm has finished, but I want to improve it...
	QPolygonF newPoly = maxPolygon;
	// 4 direction
	while (!this->intersected(newPoly))
	{
		maxPolygon = newPoly;
		newPoly[0] += (newPoly[0] - newPoly[1]) / 100.0;
		newPoly[3] += (newPoly[3] - newPoly[2]) / 100.0;
	}
	while (!this->intersected(newPoly))
	{
		maxPolygon = newPoly;
		newPoly[1] += (newPoly[1] - newPoly[0]) / 100.0;
		newPoly[2] += (newPoly[2] - newPoly[3]) / 100.0;
	}
	while (!this->intersected(newPoly))
	{
		maxPolygon = newPoly;
		newPoly[0] += (newPoly[0] - newPoly[3]) / 100.0;
		newPoly[1] += (newPoly[1] - newPoly[2]) / 100.0;
	}
	while (!this->intersected(newPoly))
	{
		maxPolygon = newPoly;
		newPoly[3] += (newPoly[3] - newPoly[0]) / 100.0;
		newPoly[2] += (newPoly[2] - newPoly[1]) / 100.0;
	}
	return maxPolygon;

	
// 	QPolygonF myPoly = this->polygon();
// 	QPolygonF poly;
// 	if (myPoly.size()>4)
// 	{
// 		poly << myPoly[0] << myPoly[1] << myPoly[2] << myPoly[3];
// 	}
// 	
// 	return poly;
}
double BGPolygon::generateRectangle(double sinOrt, double cosOrt, QPointF ptCenter, double dbAspR, QPolygonF& poly)
{
	QPolygonF poly1, poly2;
	// 1.calculate new centers
	rayTracing(ptCenter, sinOrt, cosOrt, poly1, poly2);

// 	QLineF line1(ptCenter + QPointF(1000 * cosOrt, -1000 * sinOrt), ptCenter + QPointF(-1000 * cosOrt, 1000 * sinOrt));
// 	QLineF line2(ptCenter + QPointF(1000 * sinOrt, 1000 * cosOrt), ptCenter + QPointF(-1000 * sinOrt, -1000 * cosOrt));
// 
// 	QPolygonF myPoly = this->polygon();
// 	QPointF pt1, pt2;
// 	int nSize = myPoly.size();
// 	for (int i = 0; i < nSize - 1; i++)
// 	{
// 		QLineF seg(myPoly[i], myPoly[i + 1]);
// 		if (QLineF::BoundedIntersection == line1.intersect(seg, &pt1))
// 		{
// 			poly1 << pt1;
// 		}
// 		if (QLineF::BoundedIntersection == line2.intersect(seg, &pt2))
// 		{
// 			poly2 << pt2;
// 		}
// 	}
// 	QLineF seg(myPoly[0], myPoly[nSize - 1]);
// 	if (QLineF::BoundedIntersection == line1.intersect(seg, &pt1))
// 	{
// 		poly1 << pt1;
// 	}
// 	if (QLineF::BoundedIntersection == line2.intersect(seg, &pt2))
// 	{
// 		poly2 << pt2;
// 	}

// 	this->scene()->addPolygon(poly1);
// 	this->scene()->addPolygon(poly2);
	QPointF ptNewCenter1 = (poly1[0] + poly1[1]) / 2.0;
	QPointF ptNewCenter2 = (poly2[0] + poly2[1]) / 2.0;
	// 2.generate rectangle with new centers
	QPolygonF newPoly1, newPoly2;
	double newArea1 = generateRectangleWithNewCenter(sinOrt, cosOrt, ptNewCenter1, dbAspR, newPoly1);
	double newArea2 = generateRectangleWithNewCenter(sinOrt, cosOrt, ptNewCenter2, dbAspR, newPoly2);
	if (newArea1>newArea2)
	{
		poly = newPoly1;
		return newArea1;
	}
	else{
		poly = newPoly2;
		return newArea2;
	}
}

void BGPolygon::rayTracing(QPointF ptCenter, double sinOrt, double cosOrt, QPolygonF& poly1, QPolygonF& poly2)
{
	// 1.calculate new centers
	QLineF line1(ptCenter + QPointF(1000 * cosOrt, -1000 * sinOrt), ptCenter + QPointF(-1000 * cosOrt, 1000 * sinOrt));
	QLineF line2(ptCenter + QPointF(1000 * sinOrt, 1000 * cosOrt), ptCenter + QPointF(-1000 * sinOrt, -1000 * cosOrt));

	QPolygonF myPoly = this->polygon();
	QPointF pt1, pt2;
	int nSize = myPoly.size();
	for (int i = 0; i < nSize - 1; i++)
	{
		QLineF seg(myPoly[i], myPoly[i + 1]);
		if (QLineF::BoundedIntersection == line1.intersect(seg, &pt1))
		{
			poly1 << pt1;
		}
		if (QLineF::BoundedIntersection == line2.intersect(seg, &pt2))
		{
			poly2 << pt2;
		}
	}
	QLineF seg(myPoly[0], myPoly[nSize - 1]);
	if (QLineF::BoundedIntersection == line1.intersect(seg, &pt1))
	{
		poly1 << pt1;
	}
	if (QLineF::BoundedIntersection == line2.intersect(seg, &pt2))
	{
		poly2 << pt2;
	}
}

double BGPolygon::generateRectangleWithNewCenter(double sinOrt, double cosOrt, QPointF ptCenter, double dbAspR, QPolygonF& poly)
{
	double minLen = 0;
	// 1.calculate max len
	QPolygonF poly1, poly2;
	rayTracing(ptCenter, sinOrt, cosOrt, poly1, poly2);
	QPointF pt=poly1[0] - ptCenter;
	double maxLen = sqrt(pt.x()*pt.x() + pt.y()*pt.y());

	pt = poly1[1] - ptCenter;
	double len = sqrt(pt.x()*pt.x() + pt.y()*pt.y());
	if (len<maxLen)
	{
		maxLen = len;
	}

	pt = poly2[0] - ptCenter;
	len = sqrt(pt.x()*pt.x() + pt.y()*pt.y()) / dbAspR;
	if (len < maxLen)
	{
		maxLen = len;
	}

	pt = poly2[1] - ptCenter;
	len = sqrt(pt.x()*pt.x() + pt.y()*pt.y()) / dbAspR;
	if (len < maxLen)
	{
		maxLen = len;
	}
	// 2. generate rectangle
	len = maxLen/2.0;
	double yLen = len / dbAspR;
	double area;
	while (maxLen-minLen>1.0)
	{
		QPolygonF newPoly;
		newPoly << (ptCenter + QPointF(len*cosOrt, -len*sinOrt) + QPointF(-yLen*sinOrt, -yLen*cosOrt))
			<< (ptCenter + QPointF(len*cosOrt, -len*sinOrt) + QPointF(yLen*sinOrt, yLen*cosOrt))
			<< (ptCenter + QPointF(-len*cosOrt, len*sinOrt) + QPointF(yLen*sinOrt, yLen*cosOrt))
			<< (ptCenter + QPointF(-len*cosOrt, len*sinOrt) + QPointF(-yLen*sinOrt, -yLen*cosOrt));
		if (intersected(newPoly)){
			maxLen = len-1;
			len = (minLen + maxLen) / 2.0;
			yLen = len / dbAspR;
		}
		else
		{
			minLen = len;
			len = (minLen + maxLen) / 2.0;
			yLen = len / dbAspR;
			area = len*yLen * 4;
			poly = newPoly;
		}
	}
	return area;
}


bool BGPolygon::intersected(const QPolygonF& poly)
{
	QPointF ptTemp;
	int nSize = this->polygon().size();
	int nPolySize = poly.size();
	for (int i = 0; i < nSize; i++)
	{
		QPointF pt1 = this->polygon()[i];
		QPointF pt2 = (i == nSize - 1) ? this->polygon()[0] : this->polygon()[i + 1];
		QLineF line1(pt1, pt2);
		for (int j = 0; j < nPolySize; j++)
		{
			QPointF pt1 = poly[j];
			QPointF pt2 = (j == nPolySize - 1) ? poly[0] : poly[j + 1];
			QLineF line2(pt1, pt2);
			if (QLineF::BoundedIntersection == line1.intersect(line2, &ptTemp))
			{
				return true;
			}
		}
	}
	return false;
}

// codes to calculate the intersected line
/*
QLineF line(QPointF(-1000, ptCenter.y()), QPointF(1000, ptCenter.y()));
QPolygonF myPoly = this->polygon();
int nSize = myPoly.size();
for (int i = 0; i < nSize-1;i++)
{
QLineF seg(myPoly[i], myPoly[i + 1]);
QPointF pt;
if (QLineF::BoundedIntersection==line.intersect(seg, &pt))
{
poly << pt;
}
}
QLineF seg(myPoly[0], myPoly[nSize - 1]);
QPointF pt;
if (QLineF::BoundedIntersection == line.intersect(seg, &pt))
{
poly << pt;
}
*/

//overrided pure virtual functions
int BGPolygon::x()
{
	return this->pos().x();
}
int BGPolygon::y(){
	return this->pos().y();
}
void BGPolygon::setX(int x){}
void BGPolygon::setY(int y){}
QString BGPolygon::getPortId(int index){ return ""; }
bool BGPolygon::i_isSelected(){
	return this->isSelected();
}
void BGPolygon::i_update(){}
void BGPolygon::updateTransform(){}
QPointF BGPolygon::i_posAbs(){ return this->pos(); }
QPointF BGPolygon::i_posRel(){ return this->pos(); }
QPointF BGPolygon::i_posGrp() { return this->pos(); }
BGGroup* BGPolygon::i_group(){
	return NULL;
}