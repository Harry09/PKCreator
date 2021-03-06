/*
*********************************************************************
* File          : LocalVariablesWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "LocalVariablesWindow.h"

#include <QTreeWidgetItem>
#include <QMessageBox>

#include <Var.h>

#include <VarEditWindow.h>

LocalVariablesWindow::LocalVariablesWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_pVars = nullptr;

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(m_ui.addButton, &QPushButton::clicked, this, &LocalVariablesWindow::AddVarButton_clicked);
	connect(m_ui.editButton, &QPushButton::clicked, this, &LocalVariablesWindow::EditVarButton_clicked);
	connect(m_ui.removeButton, &QPushButton::clicked, this, &LocalVariablesWindow::RemoveVarButton_clicked);
	connect(m_ui.okButton, &QPushButton::clicked, this, [this] { this->close(); });
	connect(m_ui.varsWidget, &QTreeWidget::doubleClicked, this, [this] { EditVarButton_clicked(); });
}

void LocalVariablesWindow::SetSource(QVector<QSharedPointer<Var>> *vars)
{
	if (vars)
	{
		m_pVars = vars;
	
		for (auto var : *m_pVars)
		{
			AddVarToVisualList(var.data());
		}
	}
}

void LocalVariablesWindow::AddVar(Var *var)
{
	if (!var)
		return;

	for (auto var_ : *m_pVars)
	{
		if (var_->m_name == var->m_name)
			return;
	}

	auto sharedVar = QSharedPointer<Var>(var);

	if (m_pVars)
		m_pVars->push_back(sharedVar);

	AddVarToVisualList(var);
}


void LocalVariablesWindow::AddVarToVisualList(Var *var)
{
	auto treeItem = new QTreeWidgetItem();
	treeItem->setText(0, var->m_name);
	treeItem->setText(1, dataNames[var->m_type]);

	var->m_pItem = treeItem;

	switch (var->m_type)
	{
		case DATA_INTEGER:
			treeItem->setText(2, QString::number(var->m_data.integer));
			break;
		case DATA_NUMBER:
			treeItem->setText(2, QString::number(var->m_data.number, 'f', 10));
			break;
		case DATA_STRING:
			treeItem->setText(2, *var->m_data.string);
			break;
		case DATA_BOOLEAN:
			treeItem->setText(2, var->m_data.boolean ? "true" : "false");
			break;
		default:;
	}

	m_ui.varsWidget->insertTopLevelItem(m_ui.varsWidget->topLevelItemCount(), treeItem);
}

void LocalVariablesWindow::AddVarButton_clicked()
{
	VarEditWindow varEdit(this);
	varEdit.exec();

	auto var = varEdit.GetVar();

	if (!var)
		return;

	for (auto var_ : *m_pVars)
	{
		if (var_->m_name == var->m_name)
		{
			QMessageBox::information(this, "Info", "This variable arleady exists!");
			return;
		}
	}

	AddVar(var);
}

void LocalVariablesWindow::EditVarButton_clicked()
{
	auto currItem = m_ui.varsWidget->currentItem();

	if (!currItem)
		return;

	for (auto var : *m_pVars)
	{
		if (var->m_pItem == currItem)
		{
			VarEditWindow varEdit(this);
			varEdit.SetVar(var.data());
			varEdit.exec();

			auto newVar = varEdit.GetVar();

			if (!newVar)
				return;

			if (var->m_name != newVar->m_name)
			{
				for (auto newVar_ : *m_pVars)
				{
					if (newVar_->m_name == newVar->m_name)
					{
						QMessageBox::information(this, "Info", "This variable arleady exists!");
						return;
					}
				}
			}

			var->m_name = newVar->m_name;
			var->m_type = newVar->m_type;
			var->m_data = newVar->m_data;
		
			if (var->m_pItem)
			{
				auto treeItem = var->m_pItem;

				treeItem->setText(0, var->m_name);
				treeItem->setText(1, dataNames[var->m_type]);

				var->m_pItem = treeItem;

				switch (var->m_type)
				{
				case DATA_INTEGER:
					treeItem->setText(2, QString::number(var->m_data.integer));
					break;
				case DATA_NUMBER:
					treeItem->setText(2, QString::number(var->m_data.number));
					break;
				case DATA_STRING:
					treeItem->setText(2, *var->m_data.string);
					break;
				case DATA_BOOLEAN:
					treeItem->setText(2, var->m_data.boolean ? "true" : "false");
					break;
				default:;
				}
			}
		}
	}
}

void LocalVariablesWindow::RemoveVarButton_clicked()
{
	auto currItem = m_ui.varsWidget->currentItem();

	if (!currItem)
		return;

	for (auto var : *m_pVars)
	{
		if (var->m_pItem == currItem)
		{
			delete var->m_pItem;
			m_pVars->removeOne(var);
		}
	}
}

