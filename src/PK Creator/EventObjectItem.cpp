/*
*********************************************************************
* File          : EventObjectItem.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "EventObjectItem.h"

#include <EventObjectItemWindow.h>
#include <Node.h>

EventObjectItem::EventObjectItem(ObjectItem *parent, EventDefsMgr::Type type, QStandardItem *item)
	: m_pParent(parent), m_item(item), m_eventType(type)
{
	m_new = false;
	m_pItemWnd = nullptr;

	m_idCurrentObject = 0;
	m_idCollideWith = 0;
}

EventObjectItem::~EventObjectItem()
{
}

void EventObjectItem::Load(QDataStream *const dataStream)
{
	int n = 0;

	*dataStream >> m_idCurrentObject >> m_idCollideWith >> n;

	for (int i = 0; i < n; ++i)
	{
		auto node = QSharedPointer<Node>(new Node(QString(), sf::Vector2f(), Node::FUNCTION));
		node->Load(dataStream);

		m_nodes.push_back(node);
	}
}

void EventObjectItem::Save(QDataStream *const dataStream) const
{
	*dataStream << m_eventType << m_idCurrentObject << m_idCollideWith << m_nodes.size();

	for (auto node : m_nodes)
	{
		node->Save(dataStream);
	}
}

Node *EventObjectItem::GetNode(qint64 id) const
{
	for (auto node : m_nodes)
	{
		if (node->GetID() == id)
			return node.data();
	}

	return nullptr;
}

void EventObjectItem::Show(QWidget *parent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = QSharedPointer<EventObjectItemWindow>(new EventObjectItemWindow(parent));

		if (!m_pItemWnd->FillData(this))
		{
			m_pItemWnd.reset();
			return;
		}

		m_pItemWnd->show();
	}
}

void EventObjectItem::Close()
{
	if (m_pItemWnd)
	{
		m_pItemWnd->close();

		m_pItemWnd.reset(nullptr);
	}
}
