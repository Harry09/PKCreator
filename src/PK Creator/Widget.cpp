/*
*********************************************************************
* File          : Widget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Widget.h"

#include <Common.h>

#include <QDataStream>

Widget::Widget(Node *parent, const QString &name, ConnectionType connType, DataType dataType)
	: m_pParent(parent), m_name(name), m_connType(connType), m_dataType(dataType)
{
	m_id = qint64(this);

	// colors source: https://material.io/guidelines/style/color.html
	switch (m_dataType)
	{
		case DATA_OBJECTID:
			m_color = sf::Color(0xF44336FF);
			break;
		case DATA_BOOLEAN:
			m_color = sf::Color(0xE0E0E0FF);
			break;
		case DATA_INTEGER:
			m_color = sf::Color(0x2196F3FF);
			break;
		case DATA_NUMBER:
			m_color = sf::Color(0x0D47A1FF);
			break;
		case DATA_STRING:
			m_color = sf::Color(0xFF9800FF);
			break;
		case DATA_VECTOR2:
			m_color = sf::Color(0x9575CDFF);
			break;
	}
	
}

Widget::~Widget()
{
}

void Widget::Load(QDataStream *const dataStream)
{
	int connType = 0;
	int dataType = 0;
	unsigned color = 0;

	*dataStream >> m_id >> m_name >> connType >> m_connected >> dataType >> color;

	m_connType = static_cast<ConnectionType>(connType);
	m_dataType = static_cast<DataType>(dataType);
	m_color = sf::Color(color);
}

void Widget::Save(QDataStream *const dataStream)
{
	*dataStream << m_id << m_name << m_connType << m_connected << m_dataType << m_color.toInteger();
}
