#include "MyVisScene.h"

#include "BGArea.h"


MyVisScene::MyVisScene()// :QGraphicsScene(0, 0, 600, 500)
{
	BGArea *a = new BGArea(0,0,10,10);
	a->setPos(QPointF(10,10));
	this->addItem(a);
}


MyVisScene::~MyVisScene()
{
}
