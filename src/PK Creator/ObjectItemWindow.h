#pragma once

#include <ui_ObjectWindow.h>

#include <ItemWindow.h>

#include <QVector>

class SpriteItem;
class ObjectItem;
class QStandardItemModel;

class ObjectItemWindow : public ItemWindow
{
	struct ComboBoxItem
	{
		int index;
		SpriteItem *pSpr;
	};

	Q_OBJECT

private:
	Ui::ObjectWindow		m_ui;

	ObjectItem				*m_pItemParent;

	QStandardItemModel		*m_pModel;

	QMenu					*m_pContextMenu;
	QList<QAction*>			m_actionList;

	QVector<ComboBoxItem*>	m_sprites;

	bool					m_inited;

private:
	void CreateContextMenu();
	void RefreshSpriteBox();

protected:
	void changeEvent(QEvent *e) override;
	void closeEvent(QCloseEvent *event) override;

public:
	explicit ObjectItemWindow(QWidget *parent);
	virtual ~ObjectItemWindow();

	bool FillData(Item *item) override;

private slots:
	void OkButton_clicked();
	void AddEventButton_clicked() const;
	void RemoveEventButton_clicked();
	void EditButton_clicked();
	void AddEventAction_triggered(int type);
	void AddSprButton_clicked();
	void EditSprButton_clicked() const;
	void SpriteBox_activated(int index);

};
