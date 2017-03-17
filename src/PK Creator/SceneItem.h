#pragma once

#include <Item.h>

#include <QPoint>

class SceneEditor;
class SceneItemWindow;

class ObjectItem;

namespace sf
{
	class Sprite;
}

struct SceneObject
{
	const ObjectItem	*pObj;
	sf::Sprite			*pSpr;
	QPoint				pos;
};

class SceneItem : public Item
{
protected:
	QList<SceneObject*>		m_objects;

	unsigned				m_bgColor;

	friend					SceneItemWindow;
	friend					SceneEditor;

public:
	SceneItem(QStandardItem *item, const QString &itemName);
	~SceneItem();

	void SetName(const QString &name) override;

	void Load(QDataStream *const dataStream) override;
	void Save(QDataStream *const dataStream) override;

	void Show(QWidget *wndParent) override;
	void Close() override;
};
