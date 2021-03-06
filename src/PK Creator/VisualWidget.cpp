/*
*********************************************************************
* File          : VisualWidget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "VisualWidget.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>
#include <NodeMgr.h>
#include <VisualNode.h>
#include <BlueprintEditor.h>
#include <Tooltip.h>
#include <Common.h>

VisualWidget::VisualWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: m_pParent(parent), m_pData(data), m_offset(offset)
{
	m_height = 20.f;

	m_shownTooltip = false;

	m_pSpace = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(parent->GetWidth() / 2 - m_verMargin, m_height)));
	m_pSpace->setFillColor(sf::Color(0x505050AA));

	m_pName = QSharedPointer<sf::Text>(new sf::Text());
	m_pName->setFont(*parent->GetNodeMgr()->GetFont());
	m_pName->setCharacterSize(16);
	m_pName->setString(m_pData->GetName().toStdString());
	m_pName->setOrigin(0.f, m_pName->getCharacterSize() / 1.5f);

	m_pPin = QSharedPointer<sf::CircleShape>(new sf::CircleShape(5.5f, 4));
	m_pPin->setRotation(135);
	m_pPin->setOutlineColor(m_pData->m_color);
	m_pPin->setOutlineThickness(1.f);
	m_pPin->setFillColor(sf::Color::Transparent);
	m_pPin->setOrigin(m_pPin->getRadius(), m_pPin->getRadius());
}


VisualWidget::~VisualWidget()
{
}

void VisualWidget::Event(sf::Event *e)
{
	if (!m_pData)
		return;

	if (e->type == sf::Event::MouseMoved)
	{
		sf::Vector2f viewOffset = m_pParent->GetNodeMgr()->GetBpEditor()->GetViewOffset();
		float scale = m_pParent->GetNodeMgr()->GetBpEditor()->GetScale();

		auto cursorPos = sf::Vector2f(e->mouseMove.x, e->mouseMove.y) * scale - viewOffset;
		auto spacePos = m_pSpace->getPosition();
		auto spaceSize = m_pSpace->getSize();

		if (cursorPos.x > spacePos.x &&
			cursorPos.y > spacePos.y &&
			cursorPos.x < spacePos.x + spaceSize.x &&
			cursorPos.y < spacePos.y + spaceSize.y)
		{
			QString str = "Var type: " + dataNames[m_pData->GetDataType()];

			Tooltip::Get()->SetText(str);
			Tooltip::Get()->SetPos(sf::Vector2f(e->mouseMove.x, e->mouseMove.y));
			Tooltip::Get()->Show();

			m_shownTooltip = true;
		}
		else
		{
			if (m_shownTooltip)
			{
				Tooltip::Get()->Hide();
				m_shownTooltip = false;
			}
		}

	}
}

void VisualWidget::SetPin(bool pin) const
{
	if (pin)
		m_pPin->setFillColor(m_pPin->getOutlineColor());
	else
		m_pPin->setFillColor(sf::Color::Transparent);
}
