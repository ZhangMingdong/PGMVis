#pragma once
/*
	This class are designed to represent the PGM node.
	I forget why BGTextBase is designed for, but decide to follow this hierarchy.
	Date: 2017/01/31
	Mingdong
*/

#include "BGTextBase.h"

// struct for a pgm event
struct PGMEvent{
	QString _strText;
	double _dbProbability;
};

class PGMNode : public BGTextBase
{
public:
	PGMNode();
	~PGMNode();
protected:
	// events list
	QVector<PGMEvent> _events;
public:
	// add an event
	void AddEvent(QString str);
	// get the events
	QVector<PGMEvent> GetEvent();
	// reset the probability of the events
	void ResetProbability(double *arrP);

};

