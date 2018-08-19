#include "PGMNode.h"


PGMNode::PGMNode()
{
	PGMEvent e;
	e._strText = "NULL";
	e._dbProbability = 1.0;
	_events.push_back(e);
}


PGMNode::~PGMNode()
{
}

// add an event
void PGMNode::AddEvent(QString str){

	PGMEvent e;
	e._strText = str;
	e._dbProbability = 0.0;
	_events.push_back(e);
}
// get the events
QVector<PGMEvent> PGMNode::GetEvent(){
	return _events;
}
// reset the probability of the events
void PGMNode::ResetProbability(double *arrP){
	int i = 0;
	for each (PGMEvent e in _events)
	{
		e._dbProbability = arrP[i++];
	}
}