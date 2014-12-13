#include "ctableview.h"

CTableView::CTableView(QWidget *parent)
	: QTableView(parent)
{
	// initialize the data source
	DBUtils::createDatabase();

	QVector<int> alignCenter;
	alignCenter << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10;
	m_model = new CustomItemModel(alignCenter, QVector<int>(), this);

	m_model->setTable("Nodes");
	m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	// set table header
	m_model->setHeaderData(0, Qt::Horizontal, QStringLiteral("placeholder"));
	m_model->setHeaderData(1, Qt::Horizontal, QStringLiteral("结点编号"));
	m_model->setHeaderData(2, Qt::Horizontal, QStringLiteral("波形A"));
	m_model->setHeaderData(3, Qt::Horizontal, QStringLiteral("波形B"));
	m_model->setHeaderData(4, Qt::Horizontal, QStringLiteral("波形C"));
	m_model->setHeaderData(5, Qt::Horizontal, QStringLiteral("波形D"));
	m_model->setHeaderData(6, Qt::Horizontal, QStringLiteral("波形E"));	
	m_model->setHeaderData(7, Qt::Horizontal, QStringLiteral("波形F"));
	m_model->setHeaderData(8, Qt::Horizontal, QStringLiteral("波形G"));	
	m_model->setHeaderData(9, Qt::Horizontal, QStringLiteral("添加时间"));
	m_model->setHeaderData(10, Qt::Horizontal, QStringLiteral("所属方案"));
	m_model->select();

	// set table model
	setModel(m_model);
	setWordWrap(false);
	setMouseTracking(true);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	//setEditTriggers(QAbstractItemView::NoEditTriggers);
	setShowGrid(false);
	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setHighlightSections(false);
	horizontalHeader()->hideSection(0);  //隐藏第0列
	setItemDelegate(new NoFocusDelegate());
	//setItemDelegate(new QSqlRelationalDelegate(this));
	verticalHeader()->hide();
	setFrameShape(QFrame::NoFrame);
	setAlternatingRowColors(true);

	// set the widths of view headers
	setColumnWidth(1, 60);
	setColumnWidth(2, 60);
	setColumnWidth(3, 60);
	setColumnWidth(4, 60);
	setColumnWidth(5, 60);
	setColumnWidth(6, 60);
	setColumnWidth(7, 60);
	setColumnWidth(8, 60);
	setColumnWidth(9, 130);

	initialActions();
	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClicked(const QModelIndex&)));
}

CTableView::~CTableView()
{

}
/*
 * 增加上下文菜单
 */
void CTableView::contextMenuEvent(QContextMenuEvent* e)
{
	m_popmenu->clear();
	QPoint p = e->pos();
	QModelIndex index = this->indexAt(p);
	if (index.isValid())
	{
		m_popmenu->addAction(m_newNode);
		m_popmenu->addAction(m_delNode);
		m_popmenu->addSeparator();
		m_popmenu->addAction(m_refreshSolution);
		m_popmenu->exec(QCursor::pos());
		e->accept();  // stop the propagation of the context menu event.
	}
}

void CTableView::initialActions()
{
	m_popmenu = new QMenu(this);
	m_newNode = new QAction(QIcon(":/pics/addNode"), QStringLiteral("新增结点"), this);
	m_refreshSolution = new QAction(QStringLiteral("刷新方案"), this);
	m_delNode = new QAction(QIcon(":/pics/delNode"), QStringLiteral("删除结点"), this);
	connect(m_newNode, SIGNAL(triggered()), this, SLOT(onAddNode()));
	connect(m_delNode, SIGNAL(triggered()), this, SLOT(onDeleteNode()));
	connect(m_refreshSolution, SIGNAL(triggered()), this, SLOT(onRefreshSolution()));
}
/*
 * 加载用户当前选择的方案，显示各节点的配置
 */
void CTableView::onLoadSolution(QString solutionName)
{
	m_solutionName = solutionName;
	m_model->setFilter(QString("solutionName='%1'").arg(solutionName));
}
/*
 * 保存用户对所有结点的修改
 */
void CTableView::onSaveAllNodes()
{
	m_model->database().transaction();
	if (m_model->submitAll())
	{
		m_model->database().commit();
	}
	else
	{
		m_model->database().rollback();
		CMessageBox msgBox;
		msgBox.setInformation(QStringLiteral("严重错误"), QStringLiteral("保存失败，请稍后再试！"), QPixmap(":/pics/error"));
		msgBox.exec();
	}
}
/*
 * 撤销用户的所有修改
 */
void CTableView::onRevertAllNodes()
{
	m_model->revertAll();
	CMessageBox msgBox;
	msgBox.setInformation(QStringLiteral("操作成功"), QStringLiteral("撤销修改成功！"), QPixmap(":/pics/ok"));
	msgBox.exec();
}
/**
 * 删除结点，获取结点编号和结点所属方案编号
 */
void CTableView::onDeleteNode()
{
	QSqlQuery query;
	QModelIndexList indexList = selectionModel()->selectedRows();
	int id = m_model->data(m_model->index(indexList.first().row(), 1)).toInt();
	query.exec(QStringLiteral("DELETE FROM Nodes WHERE nodeId=%1 AND solutionName='%2'").arg(id).arg(m_solutionName));
	m_model->select();
}
/**
 * 添加新节点
 */
void CTableView::onAddNode()
{
	QSqlQuery query;
	int count = 0;
	query.exec(QStringLiteral("SELECT * FROM Nodes WHERE solutionName='%1'").arg(m_solutionName));
	while (query.next())
	{
		++count;
	}
	if (count)
	{
		if(!query.exec(QStringLiteral("INSERT INTO Nodes(nodeId, wavea, waveb, wavec, waved, wavee, wavef, waveg, desc, solutionName) VALUES(%1, 'None', 'None', 'None', 'None', 'None', 'None', 'None', '%2', '%3')").arg(count+1).arg("auto generate").arg(m_solutionName)))
		{
			CMessageBox msgBox;
			msgBox.setInformation(QStringLiteral("操作失败"), query.lastError().text(), QPixmap(":/pics/error"));
			msgBox.exec();
			return ;
		}
	}
	m_model->select();
}

void CTableView::onRefreshSolution()
{
	
}

void CTableView::onDoubleClicked(const QModelIndex& index)
{
	QString a[9];
	// 有一列隐藏，隐藏要从第2列开始计算
	for (int i = 2; i <=8; ++i)
	{
		a[i] = m_model->data(m_model->index(index.row(), i)).toString();
	}
	RecordEditor editor(a[2], a[3], a[4], a[5], a[6], a[7], a[8]);
	if (editor.exec() == QDialog::Accepted)
	{
		int i = 2;
		QList<QString> waves = editor.getWavesList();
		for (QList<QString>::const_iterator ci = waves.cbegin(); ci != waves.cend(); ++ci, ++i)
		{
			m_model->setData(m_model->index(index.row(), i), *ci);
		}
	}
}

void CTableView::onDeleteSolution(const QString&)
{
	m_model->select();
}