#include "IIdInterface.h"
#include <QBrush>
#include <QPen>

IIdInterface::IIdInterface()
{
	m_id = QUuid::createUuid();

}


IIdInterface::~IIdInterface()
{
}
void IIdInterface::GenerateNewId()
{
	m_id = QUuid::createUuid();

}
void IIdInterface::readPen(QXmlStreamReader& reader, QPen& pen)
{
	reader.readNext();		// skip <sphere>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "color")
			{
				pen.setColor(readColorValue(reader));
			}
			else if (reader.name() == "width")
			{
				pen.setWidth(readIntValue(reader));
			}
			else if (reader.name() == "style")
			{
				pen.setStyle(Qt::PenStyle(readIntValue(reader)));
			}
			else if (reader.name() == "capstyle")
			{
				pen.setCapStyle(Qt::PenCapStyle(readIntValue(reader)));
			}
			else if (reader.name() == "joinstyle")
			{
				pen.setJoinStyle(Qt::PenJoinStyle(readIntValue(reader)));
			}
			else {
				skipUnknownElement(reader);
			}
		}
		else {
			reader.readNext();
		}
	}
}

void IIdInterface::readBrush(QXmlStreamReader& reader, QBrush& brush)
{
	reader.readNext();		// skip <sphere>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "color")
			{
				brush.setColor(readColorValue(reader));
			}
			else if (reader.name() == "style")
			{
				brush.setStyle(Qt::BrushStyle(readIntValue(reader)));
			}
			else {
				skipUnknownElement(reader);
			}
		}
		else {
			reader.readNext();
		}
	}
}

QColor IIdInterface::readColorValue(QXmlStreamReader& reader)
{
	int r, g, b;
	reader.readNext();		// skip <sphere>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "r")
			{
				r = readIntValue(reader);
			}
			else if (reader.name() == "g")
			{
				g = readIntValue(reader);
			}
			else if (reader.name() == "b")
			{
				b = readIntValue(reader);
			}
			else {
				skipUnknownElement(reader);
			}
		}
		else {
			reader.readNext();
		}
	}
	return QColor::fromRgb(r, g, b);
}

int IIdInterface::readIntValue(QXmlStreamReader& reader)
{
	QString str = reader.readElementText();

	if (reader.isEndElement())
		reader.readNext();

	return str.toInt();
}

double IIdInterface::readDoubleValue(QXmlStreamReader& reader)
{
	QString str = reader.readElementText();

	if (reader.isEndElement())
		reader.readNext();

	return str.toDouble();
}

QString IIdInterface::readStringValue(QXmlStreamReader& reader)
{
	QString str = reader.readElementText();

	if (reader.isEndElement())
		reader.readNext();

	return str;
}

void IIdInterface::skipUnknownElement(QXmlStreamReader& reader)
{
	reader.readNext();
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			skipUnknownElement(reader);
		}
		else {
			reader.readNext();
		}
	}
}

bool IIdInterface::readId(QXmlStreamReader& reader){
	if (reader.name() == "id")
	{
		m_id = QUuid(readStringValue(reader));
		return true;
	}
	else return false;
}