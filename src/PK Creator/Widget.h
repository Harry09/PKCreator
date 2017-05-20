/*
*********************************************************************
* File          : Widget.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QVector>
#include <SFML/Graphics/Color.hpp>
#include <Common.h>

class VisualNode;
class Node;
class VisualWidget;
class InputWidget;
class OutputWidget;
class HTML5Generator;
class NodeMgr;

class Widget
{
public:
	enum ConnectionType
	{
		INPUT = 0,
		OUTPUT
	};

private:
	qint64				m_id;

	sf::Color			m_color;

	Node				*m_pParent;

	QString				m_name;

	ConnectionType		m_connType;

	DataType			m_dataType;

	QVector<qint64>		m_connected;

	friend VisualWidget;
	friend InputWidget;
	friend OutputWidget;
	friend VisualNode;
	friend HTML5Generator;
	friend NodeMgr;

	void SetDataType(DataType type);

public:
	Widget(Node *parent, const QString &name, ConnectionType connType, DataType type);
	~Widget() = default;

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

	ConnectionType GetConnType() const { return m_connType; }
	const QString &GetName() const { return m_name; }
	qint64 GetID() const { return m_id; }
	DataType GetDataType() const { return m_dataType; }
	sf::Color GetColor() const { return m_color; }
};
		