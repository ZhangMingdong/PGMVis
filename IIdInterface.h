#pragma once
#include <QUuid>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
class IIdInterface
{
public:
	IIdInterface();
	virtual ~IIdInterface();
public:
	QString GetId(){ return m_id.toString(); }
	bool CheckId(QString strId){ return QUuid(strId) == m_id; }
	bool CheckId(QUuid id){ 
		return id == m_id;
	}
// 	void SetId(QString strId){ m_id = QUuid(strId); }		// only used while loading
	bool readId(QXmlStreamReader& reader);
	void GenerateNewId();
protected:
	QUuid m_id;
public:
	static void readPen(QXmlStreamReader& reader, QPen& pen);
	static void readBrush(QXmlStreamReader& reader, QBrush& brush);
	static QColor readColorValue(QXmlStreamReader& reader);
	static int readIntValue(QXmlStreamReader& reader);
	static double readDoubleValue(QXmlStreamReader& reader);
	static void skipUnknownElement(QXmlStreamReader& reader);
	static QString readStringValue(QXmlStreamReader& reader);
};

