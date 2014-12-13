#ifndef CTABLEVIEW_H
#define CTABLEVIEW_H

#include <QTableView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlRelationalDelegate>

#include "customitemmodel.h"
#include "recordeditor.h"
#include "nofocusdelegate.h"
#include "Utils/DBUtils.h"
#include "msgbox.h"

class CTableView : public QTableView
{
	Q_OBJECT

public:
	CTableView(QWidget *parent);
	~CTableView();

private slots:
	void onLoadSolution(QString solutionName);
	void onSaveAllNodes();
	void onRevertAllNodes();
	void onDeleteNode();
	void onAddNode();
	void onRefreshSolution();
	void onDoubleClicked(const QModelIndex& index);
	void onDeleteSolution(const QString& name);

protected:
	void contextMenuEvent(QContextMenuEvent* e);
	void initialActions();
	void createHeaders();

private:
	QMenu* m_popmenu;
	QAction* m_newNode;
	QAction* m_refreshSolution;
	QAction* m_delNode;
	CustomItemModel* m_model;
	QString m_solutionName;
};

#endif // CTABLEVIEW_H
