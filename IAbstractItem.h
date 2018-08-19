#pragma once
#include "IIdInterface.h"

#include <QSet>
#include <QObject>
#include <QPen>
#include <QBrush>


class BGLink;
class BGGroup;

class IAbstractItem : public QObject, public IIdInterface
{
	Q_OBJECT
	Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
	Q_PROPERTY(QPen pen READ pen WRITE setPen)
	Q_PROPERTY(double angle READ angle WRITE setAngle)
	Q_PROPERTY(double shearHorizontal READ shearHorizontal WRITE setShearHorizontal)
	Q_PROPERTY(double shearVertical READ shearVertical WRITE setShearVertical)
	Q_PROPERTY(int x READ x WRITE setX)
	Q_PROPERTY(int y READ y WRITE setY)

signals:
	void dirty();
public:
	IAbstractItem();
	virtual ~IAbstractItem();
protected:
	QSet<BGLink *> m_setLinks;
public:
	void addLink(BGLink *link);
	void removeLink(BGLink *link);
protected:
	QPen m_pen;
	QBrush m_brush;
	double m_angle;
	double m_shearHorizontal;
	double m_shearVertical;
public:
	QPen pen(){ return m_pen; }
	QBrush brush(){ return m_brush; }
	double angle() const { return m_angle; }
	double shearHorizontal() const { return m_shearHorizontal; }
	double shearVertical() const { return m_shearVertical; }
public slots :
	void setPen(const QPen &pen);
	void setBrush(const QBrush &brush);
	void setAngle(double angle);
	void setShearHorizontal(double shearHorizontal)
	{
		setShear(shearHorizontal, m_shearVertical);
	}
	void setShearVertical(double shearVertical)
	{
		setShear(m_shearHorizontal, shearVertical);
	}
	void setShear(double shearHorizontal, double shearVertical);
protected:
	void writePosition(QXmlStreamWriter& writer);
	void writePen(QXmlStreamWriter& writer);
	void writeBrush(QXmlStreamWriter& writer);
	void writetransform(QXmlStreamWriter& writer);
	void writeGroup(QXmlStreamWriter& writer);
	virtual void writeId(QXmlStreamWriter& writer);

public:
	virtual void setFromIndex(int index){  };
	virtual void setToIndex(int index){  };
	virtual int indexRange(){ return 0; };
	virtual int fromIndex(){ return 0; };
	virtual int toIndex(){ return 0; };
	virtual int getChildPos(QUuid id){ return 0; }
public:
	virtual int x()=0;
	virtual int y()=0;
	virtual void setX(int x) = 0;
	virtual void setY(int y) = 0;
public:
	virtual QPointF getLinkPos(QPointF ptTarget) = 0;
	virtual QPointF getItemCenter(){
		return GetGroupPos();
	}
	virtual QString getPortId(int index){ return ""; };
protected:
	virtual bool i_isSelected() = 0;
	virtual void i_update() = 0;
	virtual void updateTransform() = 0;
	virtual QPointF i_posAbs()=0;
	virtual QPointF i_posRel()=0;
	virtual BGGroup* i_group() = 0;
public slots:
	void setLocation(int x, int y);
protected:
	QUuid m_groupid;
	bool readGroup(QXmlStreamReader& reader);
public:
	QUuid GetGroupId(){ return m_groupid; }
	virtual QPointF GetGroupPos();
public:
	virtual void updateLink()=0;

	void RemoveLinks();
	void MoveLinkTo(IAbstractItem* item);
};

