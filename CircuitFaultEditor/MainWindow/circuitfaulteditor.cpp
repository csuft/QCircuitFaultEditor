#include "circuitfaulteditor.h"

CircuitFaultEditor::CircuitFaultEditor(QWidget *parent)
	: ShadowWindow(parent), isServing(false)
{
	setFixedSize(980, 650);

	// 波形编辑控件的初始化
    pv = new QVector<QPointF>();
    m_randomEditor = new WavesEditor(this);
    m_manualEditor = new WavesEditor(this);
    m_previewEditor = new WavesEditor(this, true);
	m_randomEditor->m_green_shade->hoverPoints()->setDisabled(true);

	m_mainLayout = new QVBoxLayout(this);
	m_bottomMainLayout = new QGridLayout(this);
	m_topLayout = new QHBoxLayout(this);
	m_bottomLayout = new QHBoxLayout(this);
	m_rightBottomLayout = new QVBoxLayout(this);
	m_rightLayout = new QVBoxLayout(this);

	// top title bar
	m_title = new QLabel(QStringLiteral("电路诊断故障控制台"), this);
	m_title->setObjectName("mainTitle");
	m_miniBtn = new CustomPushButton(this);
	m_miniBtn->setBtnBackground(":/pics/min");
	m_miniBtn->setToolTip(QStringLiteral("缩小"));
	m_closeBtn = new CustomPushButton(this);
	m_closeBtn->setBtnBackground(":/pics/close");
	m_closeBtn->setToolTip(QStringLiteral("关闭"));
	m_topLayout->addWidget(m_title, 0, Qt::AlignVCenter);
	m_topLayout->addStretch();
	m_topLayout->addWidget(m_miniBtn, 0, Qt::AlignTop);
	m_topLayout->addWidget(m_closeBtn, 0, Qt::AlignTop);
	m_topLayout->setSpacing(0);
	m_topLayout->setContentsMargins(20, 0, 0, 0);

	// bottom widget container
	QWidget* bottomWidget = new QWidget(this);
	bottomWidget->setObjectName("bottomWidget");

	// top left widget
	m_tableView = new CTableView(this);

	// top right widget
	m_builtinWaves = new QGroupBox(QStringLiteral("故障波形设置"), this);
	m_builtinWaves->setFixedWidth(300);
	m_generationType = new QComboBox(this);
	m_generationType->addItem(QStringLiteral("手动生成"), 0);
    m_generationType->addItem(QStringLiteral("随机生成"), 1);
	m_waveName = new QLineEdit(this);
	m_waveName->setPlaceholderText(QStringLiteral("请输入故障波形名称"));
	m_waveName->setObjectName("lineEdit");
	m_waveName->setFixedHeight(30);
	m_saveWaveBtn = new QPushButton(QIcon(":/pics/saveWave"), QStringLiteral("保存波形"), this);
	m_saveWaveBtn->setFixedSize(85, 30);
	m_saveWaveBtn->setObjectName("panelBtn");
	m_saveWaveBtn->setCursor(Qt::PointingHandCursor);
    m_randomGenerateBtn = new QPushButton(QStringLiteral("随机产生"), this);
    m_randomGenerateBtn->setFixedSize(85, 30);
    m_randomGenerateBtn->setObjectName("panelBtn");
	m_randomGenerateBtn->setDisabled(true);
	m_randomGenerateBtn->setCursor(Qt::PointingHandCursor);
	m_waveSettingsLayout = new QGridLayout(this);
	m_waveSettingsLayout->addWidget(new QLabel(QStringLiteral("生成类型：")), 0, 0, 1, 1);
	m_waveSettingsLayout->addWidget(m_generationType, 0, 1, 1, 2);
	m_waveSettingsLayout->addWidget(m_randomGenerateBtn, 0, 3, 1, 2);

	m_waveSettingsLayout->addWidget(new QLabel(QStringLiteral("波形名称：")), 1, 0, 1, 1);
	m_waveSettingsLayout->addWidget(m_waveName, 1, 1, 1, 2);
	m_waveSettingsLayout->addWidget(m_saveWaveBtn, 1, 3, 1, 2);

	QLabel* voltLabel = new QLabel(QStringLiteral("当前电压："), this);
	m_voltageValue = new QLabel(QStringLiteral("--"), this);
	m_voltageValue->setObjectName("voltageValueLabel");
	m_voltageValue->setFixedWidth(45);
	m_moreWavesBtn = new QPushButton(QStringLiteral("更多波形>>"), this);
	m_moreWavesBtn->setObjectName("panelBtn");
	m_moreWavesBtn->setFixedSize(80, 25);
	m_moreWavesBtn->setDisabled(true);
	m_fixedValue = new QDoubleSpinBox(this);
	m_fixedValue->setSuffix("V");
	m_fixedValue->setDecimals(1);
	m_fixedValue->setMaximum(15);
	m_fixedValue->setMinimum(-15);
	m_fixedValue->setSingleStep(0.5);
	m_fixedValue->setValue(0);
	m_fixedValue->setObjectName("spinBox");
	m_fixedValue->setFixedSize(80, 25);
	m_fixedValue->setDisabled(true);

	m_waveSettingsLayout->addWidget(voltLabel, 2, 0, 1, 1);
	m_waveSettingsLayout->addWidget(m_voltageValue, 2, 1, 1, 1);
	m_waveSettingsLayout->addWidget(m_moreWavesBtn, 2, 2, 1, 1);
	m_waveSettingsLayout->addWidget(m_fixedValue, 2, 3, 1, 2);
	m_builtinWaves->setLayout(m_waveSettingsLayout);

	m_randomWaves = new QGroupBox(QStringLiteral("随机波形"), this);
	m_randomWaves->setObjectName("GroupBox");
    QHBoxLayout* randomLayout = new QHBoxLayout(this);
    randomLayout->addWidget(m_randomEditor);
    randomLayout->setContentsMargins(0, 0, 0, 0);
    m_randomWaves->setLayout(randomLayout);
	m_manualWaves = new QGroupBox(QStringLiteral("手动编辑"), this);
	m_manualWaves->setObjectName("GroupBox");
    QHBoxLayout* manualLayout = new QHBoxLayout(this);
    manualLayout->addWidget(m_manualEditor);
	manualLayout->setContentsMargins(0, 0, 0, 0);
	m_manualWaves->setLayout(manualLayout);
	m_rightLayout->addWidget(m_builtinWaves);
	m_rightLayout->addWidget(m_randomWaves);
	m_rightLayout->addWidget(m_manualWaves);

	// bottom left widget 1
	m_faultLabel = new QLabel(QStringLiteral("选择方案:"), this);
	m_faultsList = new QComboBox(this);
	m_faultsModel = new QSqlTableModel(this);
	m_faultsModel->setTable("Solutions");
	m_faultsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_faultsModel->select();
	m_faultsList->setModel(m_faultsModel);
	m_faultsList->setModelColumn(m_faultsModel->fieldIndex("name"));
	m_faultsDesc = new QTextEdit(this);
	m_faultsDesc->setFixedHeight(90);
	m_saveEditBtn = new QPushButton(QIcon(":/pics/saveDesc"), QStringLiteral("保存描述"), this);
	m_saveEditBtn->setFixedSize(85, 30);
	m_saveEditBtn->setObjectName("panelBtn");
	m_saveEditBtn->setCursor(Qt::PointingHandCursor);
	m_loadBtn = new QPushButton(QIcon(":/pics/preview"), QStringLiteral("加载方案"), this);
	m_loadBtn->setObjectName("panelBtn");
	m_loadBtn->setFixedSize(85, 30);
	m_loadBtn->setCursor(Qt::PointingHandCursor);
	QGridLayout* faultLayout = new QGridLayout(this);
	faultLayout->setSizeConstraint(QLayout::SetFixedSize);
	faultLayout->addWidget(m_faultLabel, 0, 0, 1, 1);		// 从第零行第零列开始，占一行一列
	faultLayout->addWidget(m_faultsList, 0, 1, 1, 3);		// 从第零行第一列开始，占一行三列
	faultLayout->addWidget(m_faultsDesc, 1, 0, 3, 4);		// 从第一行第零列开始，占三行四列
	faultLayout->addWidget(m_loadBtn, 5, 0, 1, 2);			// 从第五行第零列开始，占一行两列
	faultLayout->addWidget(m_saveEditBtn, 5, 2, 1, 2, Qt::AlignRight);		// 从第五行第二列开始，占一行两列

	// bottom left widget 2
	m_newBtn = new QPushButton(QIcon(":/pics/newSolution"), QStringLiteral("新建方案"), this);
	m_newBtn->setFixedSize(85, 30);
	m_newBtn->setObjectName("panelBtn");
	m_newBtn->setCursor(Qt::PointingHandCursor);
	m_saveBtn = new QPushButton(QIcon(":/pics/saveSolution"), QStringLiteral("保存方案"), this);
	m_saveBtn->setFixedSize(85, 30);
	m_saveBtn->setObjectName("panelBtn");
	m_saveBtn->setCursor(Qt::PointingHandCursor);
	m_deleteBtn = new QPushButton(QIcon(":/pics/delete"), QStringLiteral("删除方案"), this);
	m_deleteBtn->setFixedSize(85, 30);
	m_deleteBtn->setObjectName("panelBtn");
	m_deleteBtn->setCursor(Qt::PointingHandCursor);
	m_helpBtn = new QPushButton(QIcon(":/pics/help"), QStringLiteral("操作指南"), this);
	m_helpBtn->setFixedSize(85, 30);
	m_helpBtn->setObjectName("panelBtn");
	m_helpBtn->setCursor(Qt::PointingHandCursor);
	m_revertBtn = new QPushButton(QIcon(":/pics/reset"), QStringLiteral("撤销修改"), this);
	m_revertBtn->setFixedSize(85, 30);
	m_revertBtn->setObjectName("panelBtn");
	m_revertBtn->setCursor(Qt::PointingHandCursor);
	QVBoxLayout* btnsLayout = new QVBoxLayout(this);
	btnsLayout->addWidget(m_newBtn, 0, Qt::AlignTop);
	btnsLayout->addWidget(m_revertBtn);
	btnsLayout->addWidget(m_saveBtn);
	btnsLayout->addWidget(m_deleteBtn);
	btnsLayout->addWidget(m_helpBtn, 0, Qt::AlignBottom);
	// 没用故障方案，则禁用掉按钮
	if (m_faultsList->count() == 0)
	{
		m_faultsDesc->setDisabled(true);
		m_saveBtn->setDisabled(true);
		m_deleteBtn->setDisabled(true);
		m_revertBtn->setDisabled(true);
		m_saveEditBtn->setDisabled(true);
		m_loadBtn->setDisabled(true);
	}

	// bottom left wiget 3
	m_wavesDesc = new QLabel(QStringLiteral("预览: "), this);
	m_wavesList = new QComboBox(this);
	m_wavesModel = new QSqlTableModel(this);
	m_wavesModel->setTable("Waves");
	m_wavesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_wavesList->setModel(m_wavesModel);
	m_wavesModel->select();
	m_wavesList->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_wavesList->setCurrentIndex(0);
	QHBoxLayout* waveTop = new QHBoxLayout(this);
	waveTop->addWidget(m_wavesDesc);
	waveTop->addWidget(m_wavesList, 1);
	QVBoxLayout* waveLayout = new QVBoxLayout(this);
	waveLayout->addLayout(waveTop);
    waveLayout->addWidget(m_previewEditor);

	// bottom layout
	m_bottomLayout->addLayout(faultLayout);
	m_bottomLayout->addLayout(btnsLayout);
	m_bottomLayout->addLayout(waveLayout, 1);

	// bottom right
	m_dispatchBox = new QGroupBox(QStringLiteral("数据分发"), this);
	m_dispatchBox->setFixedSize(300, 170);
	m_dispatchNote = new QLabel(QStringLiteral("当前选择的故障方案为:"), this);
	m_dispatchNote->setObjectName("dispatchNote");
	m_status = new QLabel(QStringLiteral("未分发  "), this);
	m_status->setObjectName("status");
	m_solutionName = new QLabel(QStringLiteral("未选定"), this);
	m_solutionName->setObjectName("solutionName");
	m_progressBar = new QProgressBar(this);
	m_progressBar->setObjectName("progress");
	m_progressBar->setValue(0);
	m_dispatchBtn = new QPushButton(QIcon(":/pics/power"), QStringLiteral("开启服务"), this);
	m_dispatchBtn->setFixedSize(95, 30);
	m_dispatchBtn->setCursor(Qt::PointingHandCursor);
	m_dispatchBtn->setObjectName("dispatchBtn");
	m_refreshBtn = new QPushButton(QIcon(":/pics/refresh"), QStringLiteral("更新方案"), this);
	m_refreshBtn->setFixedSize(85, 30);
	m_refreshBtn->setCursor(Qt::PointingHandCursor);
	m_refreshBtn->setObjectName("panelBtn");
	m_refreshBtn->setDisabled(true);
	QVBoxLayout* dispatchLayout = new QVBoxLayout(this);
	dispatchLayout->addWidget(m_dispatchNote, 0, Qt::AlignHCenter);
	dispatchLayout->addWidget(m_solutionName, 0, Qt::AlignHCenter);
	QHBoxLayout* progressLayout = new QHBoxLayout(this);
	progressLayout->addWidget(m_status);
	progressLayout->addWidget(m_progressBar);
	dispatchLayout->addLayout(progressLayout, 1);

	QHBoxLayout* tmpLayout = new QHBoxLayout(this);
	tmpLayout->addWidget(m_refreshBtn, 1, Qt::AlignLeft);
	tmpLayout->addWidget(m_dispatchBtn, 1);
	dispatchLayout->addLayout(tmpLayout);
	m_dispatchBox->setLayout(dispatchLayout);

	//////////////////////////////////////////////////////////////////////////
	m_bottomMainLayout->addWidget(m_tableView, 0, 0, 3, 3);
	m_bottomMainLayout->addLayout(m_rightLayout, 0, 3, 3, 1);
	m_bottomMainLayout->addLayout(m_bottomLayout, 3, 0, 1, 3);
	m_bottomMainLayout->addWidget(m_dispatchBox, 3, 3, 1, 1);
	m_bottomMainLayout->setSpacing(5);
	m_bottomMainLayout->setContentsMargins(5, 5, 5, 5);
	bottomWidget->setLayout(m_bottomMainLayout);

	m_mainLayout->addLayout(m_topLayout);
	m_mainLayout->addWidget(bottomWidget, 1);
	m_mainLayout->setSpacing(10);
	m_mainLayout->setContentsMargins(8, 8, 8, 8);

	setLayout(m_mainLayout);
	// 默认显示列表中第一个故障方案的描述
	onSolutionDisplayChanged(0);

    QTimer::singleShot(50, this, SLOT(setDefault()));
	connect(m_miniBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(close()));
	connect(m_helpBtn, SIGNAL(clicked()), this, SLOT(onHelpClicked()));
	connect(m_newBtn, SIGNAL(clicked()), this, SLOT(onAddNewSolution()));
	connect(m_saveBtn, SIGNAL(clicked()), m_tableView, SLOT(onSaveAllNodes()));
	connect(m_deleteBtn, SIGNAL(clicked()), this, SLOT(onDeleteSolutionClicked()));
	connect(m_revertBtn, SIGNAL(clicked()), m_tableView, SLOT(onRevertAllNodes()));
	connect(m_saveEditBtn, SIGNAL(clicked()), this, SLOT(onSaveDescriptionClicked()));
	connect(m_faultsList, SIGNAL(currentIndexChanged(int)), this, SLOT(onSolutionDisplayChanged(int)));
	connect(m_wavesList, SIGNAL(currentIndexChanged(int)), this, SLOT(onWavePreviewChanged(int)));
	connect(m_loadBtn, SIGNAL(clicked()), this, SLOT(onNewSolutionClicked()));
	connect(this, SIGNAL(loadNewSolution(QString)), m_tableView, SLOT(onLoadSolution(QString)));
	connect(m_dispatchBtn, SIGNAL(clicked()), this, SLOT(onDispatchBtnClicked()));
	connect(m_refreshBtn, SIGNAL(clicked()), this, SLOT(onRefreshClicked()));
	connect(m_saveWaveBtn, SIGNAL(clicked()), this, SLOT(onWaveSaveBtnClicked()));
    connect(m_randomGenerateBtn, SIGNAL(clicked()), this, SLOT(onRandomGenerate()));
	connect(this, SIGNAL(deleteSolution(const QString&)), m_tableView, SLOT(onDeleteSolution(const QString&)));
	connect(m_generationType, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onGenerationTypeChanged(const QString&)));
	connect(m_fixedValue, SIGNAL(valueChanged(double)), this, SLOT(onVoltageChanged(double)));
	connect(m_manualEditor->m_green_shade, SIGNAL(positionChanged(int)), this, SLOT(onPositionChanged(int)));
	connect(m_moreWavesBtn, SIGNAL(clicked()), this, SLOT(onMoreWavesClicked()));
}

/**
 * @brief AS_bezier
 * 绘制贝塞尔曲线
 */
void AS_bezier(QPointF p1, QPointF p2, qreal t, QPointF *presult)
{
    QPointF control1;
    QPointF control2;

    qreal distance = p2.rx() - p1.rx();

    control1.setX(p1.x() + distance / 2);
    control1.setY(p1.y());

    control2.setX(p1.x() + distance / 2);
    control2.setY(p2.y());

    qreal c1 = (1 - t) * (1 - t) * (1 - t);
    qreal c2 = t       * (1 - t) * (1 - t) * 3;
    qreal c3 = t       *      t  * (1 - t) * 3;
    qreal c4 = t       *      t  *      t;

    qreal new_x = p1.x() * c1 + control1.x() * c2 + control2.x() * c3 + p2.x() * c4;
    qreal new_y = p1.y() * c1 + control1.y() * c2 + control2.y() * c3 + p2.y() * c4;

    presult->setX(new_x);
    presult->setY(new_y);    
}

void prepare_data(QPolygonF pf, QVector<QPointF> &target)
{
	// 起始点到结束点在X轴上的距离
    qreal distance = pf[pf.size()-1].rx() - pf[0].rx();
	// 起始点到结束点的步进
    qreal step = distance / NUMBER_OF_POINTS;

    QPointF p1(0, 0);;
    QPointF p2(pf[0]);

    int i = 0;
	qreal t = 0;
	QPointF ret;
    int num_of_points_generated = 0;
    target << pf[0];
	

	// 将起始点和结束点排除在外
    while (num_of_points_generated < (NUMBER_OF_POINTS - 2)) 
    {
        while ((p2.rx() + step) > pf[i + 1].rx())
        {
			// 计算两个点之间应该插值进多少个点
            i++;
        }
        p1.setX(p2.rx());
        p2.setX(p2.rx() + step);
        t = (p2.rx() - pf[i].rx()) / (pf[i+1].rx() - pf[i].rx());
        
        AS_bezier(pf[i], pf[i+1], t, &ret);
        target << ret;
        num_of_points_generated++;
    }
    target << pf[pf.size() - 1];
}

/**
 * 保存用户编辑的波形数据
 */
void CircuitFaultEditor::onWaveSaveBtnClicked()
{
	CMessageBox msgBox;
	if (m_waveName->text().isEmpty())
	{
		msgBox.setInformation(QStringLiteral("错误"), QStringLiteral("请填写故障波形名称！"), QPixmap(":/pics/error"));
		msgBox.exec();
	}
	else
	{
		saveWaveData();
	}
}
/**
 * 后台实际保存波形数据
 */
void CircuitFaultEditor::saveWaveData()
{
	CMessageBox msgBox;
	QString text = m_waveName->text();
	QSqlQuery query;
	// 确保数据库中不存在同名的波形记录
	query.exec(QStringLiteral("SELECT id FROM Waves WHERE name='%1'").arg(text));
	if(query.next())
	{
		msgBox.setInformation(QStringLiteral("操作失败"), QStringLiteral("对不起，该波形名称已经存在！"), QPixmap(":/pics/error"));
	}
	else
	{	
        // 保存波形点描述
        QPolygonF *pp, pf;
        using namespace tinyxml2;
        tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;
        // to be reviewed and modified
        tinyxml2::XMLDeclaration *declaration = doc->NewDeclaration("xml version=\"1.0\"");
        doc->LinkEndChild(declaration);
        XMLElement *root = doc->NewElement("Wave");
        doc->LinkEndChild(root);

        int data = m_generationType->itemData(m_generationType->currentIndex()).toInt();
        if (data == 1)  // 随机生成
        {
            pp = &(m_randomEditor->m_green_shade->points());
        }
        else
        {
            pp = &(m_manualEditor->m_green_shade->points());
        }

        for (int i=0; i<pp->size(); i++)
        {
            XMLElement *point = doc->NewElement("Point");
            point->SetAttribute("x", (*pp)[i].rx());;
			point->SetAttribute("y", (*pp)[i].ry());;
            root->LinkEndChild(point);
        }
        doc->SaveFile(QString(text + "_desc.xml").toStdString().c_str());

        // 利用贝塞尔算法生成具体点
        QVector<QPointF> target;
		if (data == 1)  // 随机生成
		{
			pf = m_randomEditor->m_green_shade->hoverPoints()->points();
		}
		else
		{
			pf = m_manualEditor->m_green_shade->hoverPoints()->points();
		}
        prepare_data(pf, target);
        QFile file(text + "_detail.xml");
        file.open(QIODevice::WriteOnly);
		
        QTextStream out(&file);
        int counter = 0;
        for (int i=0; i<target.size(); i++)
        { 
			// patch for deviation
			if (target[i].ry() != 89)
			{
				target[i].ry() -= 0.349;
			}
			qDebug() << target[i].ry() << " " << (quint16)((1 - target[i].ry() / 89) * 0xffff);
            out << QString::number((quint16)((1-target[i].ry() / 89) * 0xffff));
            out << "\r\n";
            counter++;
        }
        file.close();

        // 往数据库中插入该波形记录
        query.exec(QStringLiteral("INSERT INTO Waves(name, desc) VALUES('%1', '%2')").arg(text).arg(QDateTime::currentDateTime().toString("yyyy-M-d HH:mm:ss")));
		m_wavesModel->select();
		m_waveName->clear();
		msgBox.setInformation(QStringLiteral("操作成功"), QStringLiteral("故障波形保存成功！"), QPixmap(":/pics/ok"));
	}
	msgBox.exec();
}

/**
 * 发送loadNewSolution信号通知QTableView更新数据，加载当前选择的故障方案
 */
void CircuitFaultEditor::onNewSolutionClicked()
{
	QString solutionName = m_faultsList->currentText();
	// 记录旧方案名和新方案名，确实切换过才加载
	m_oldSolutionName = m_curSolutionName;
	m_curSolutionName = solutionName;
	if(!solutionName.isEmpty() && m_oldSolutionName.compare(m_curSolutionName))
	{
		m_solutionName->setText(solutionName);
		emit loadNewSolution(solutionName);
	}
}
/**
 * 开始下发数据
 */
void CircuitFaultEditor::onDispatchBtnClicked()
{
	if (!isServing)  // 服务器尚未启动
	{
		CMessageBox msgBox;
		if (m_solutionName->text().isEmpty() || !m_solutionName->text().compare(QStringLiteral("未选定")))
		{
			msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("请先选择故障方案！"), QPixmap(":/pics/warning"));
			msgBox.exec();
			return;
		}
		// 设置需要分发的故障方案
		m_tcpServer = new CTcpServer;
		connect(m_tcpServer, SIGNAL(dispatchReport(int)), this, SLOT(onDispatchReport(int)));
		connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
		connect(m_tcpServer, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
		m_tcpServer->setSolutionName(m_solutionName->text());
		// 启动TCP服务器
		if (!m_tcpServer->listen(QHostAddress::Any, SERVER_PORT))
		{
			msgBox.setInformation(QStringLiteral("内部错误"), QStringLiteral("无法绑定端口%1，请稍后再试！").arg(SERVER_PORT), QPixmap(":/pics/error"));
			msgBox.exec();
			return;
		}
		isServing = true;
		m_dispatchBtn->setText(QStringLiteral("关闭服务"));
	}
	else  // 服务器已然启动，关闭服务
	{
		m_tcpServer->close();
		//delete m_tcpServer;
		isServing = false;
		m_dispatchBtn->setText(QStringLiteral("开启服务"));
		m_status->setText(QStringLiteral("未分发 "));
		m_progressBar->setValue(0);
		m_refreshBtn->setDisabled(true);
	}
}
/**
 * 使用手册
 */
void CircuitFaultEditor::onHelpClicked()
{
	ManualDialog dialog;
	dialog.exec();
}
// 关于本程序
void CircuitFaultEditor::onRefreshClicked()
{
	qDebug() << "----------------------------" << isServing;
	qDebug() << m_oldSolutionName << m_curSolutionName;
	CMessageBox msgBox;
	if (!m_solutionName->text().compare(QStringLiteral("未选定")))
	{
		msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("请先选择故障方案！"), QPixmap(":/pics/warning"));
		msgBox.exec();
	}
	else if (!m_oldSolutionName.compare(m_curSolutionName))
	{	
		// 没有选择新方案，则直接返回，避免不必要的数据分发。
		return;
	}
	else 
	{
		if (!isServing)
		{
			qDebug() << "----------------------------" << isServing;
			msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("请先启动服务！"), QPixmap(":/pics/warning"));
			msgBox.exec();
		}
		else
		{
			qDebug() << "----------------------------" << isServing;
			m_refreshBtn->setDisabled(true);
			m_progressBar->setValue(0);
			m_status->setText(QStringLiteral("正在分发"));
			m_tcpServer->refreshSolution(m_solutionName->text());
		}
	}
}
/**
 * 删除故障方案
 */
void CircuitFaultEditor::onDeleteSolutionClicked()
{
	bool ret1, ret2;
	CMessageBox msgBox;
	QSqlQuery query;
	if (!m_faultsList->currentText().compare(QStringLiteral("数码管故障"))
		|| !m_faultsList->currentText().compare(QStringLiteral("欠压灯故障")))
	{
		msgBox.setInformation(QStringLiteral("操作失败"), QStringLiteral("内置故障方案不能被删除！"), QPixmap(":/pics/error"));
		msgBox.exec();
		return ;
	}
	ret2 = query.exec(QStringLiteral("DELETE FROM Nodes WHERE solutionName='%1'").arg(m_faultsList->currentText()));
	// 删除解决方案
	ret1 = query.exec(QStringLiteral("DELETE FROM Solutions WHERE name='%1'").arg(m_faultsList->currentText()));

	if(ret1 && ret2)
	{
		emit deleteSolution(m_faultsList->currentText());
		m_faultsModel->select();
		m_faultsDesc->clear();
		m_solutionName->setText(QStringLiteral("未选定"));
		msgBox.setInformation(QStringLiteral("操作成功"), QStringLiteral("您选择的方案已成功删除！"), QPixmap(":/pics/ok"));
		// 如果没有一个方案存在了，则禁用掉相关按钮
		query.exec(QStringLiteral("SELECT name FROM Solutions"));
		if (!query.next())
		{
			m_faultsDesc->setDisabled(true);
			m_saveBtn->setDisabled(true);
			m_deleteBtn->setDisabled(true);
			m_revertBtn->setDisabled(true);
			m_saveEditBtn->setDisabled(true);
			m_loadBtn->setDisabled(true);
		}
	}
	else
	{
		msgBox.setInformation(QStringLiteral("操作失败"), QStringLiteral("删除操作失败，请稍后再试！"), QPixmap(":/pics/error"));
	}
	msgBox.exec();
}
/**
 * 保存对故障方案描述的修改
 */
void CircuitFaultEditor::onSaveDescriptionClicked()
{
	CMessageBox msgBox;
	QString desc = m_faultsDesc->toPlainText();
	QSqlQuery query;
	bool result = query.exec(QStringLiteral("UPDATE Solutions SET desc='%1' WHERE name='%2'").arg(desc).arg(m_faultsList->currentText()));
	if (result)
	{
		msgBox.setInformation(QStringLiteral("操作成功"), QStringLiteral("修改故障方案描述成功！"), QPixmap(":/pics/ok"));
	}
	else
	{
		msgBox.setInformation(QStringLiteral("操作失败"), QStringLiteral("修改操作失败，请稍后再试！"), QPixmap(":/pics/error"));
	}
	msgBox.exec();
}

/**
 * 绘制主窗口背景图
 */
void CircuitFaultEditor::paintEvent(QPaintEvent* event)
{
	ShadowWindow::paintEvent(event);

	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawPixmap(5, 5, width()-10, height()-10, QPixmap(":/pics/background"));
}
/**
 * 处理关闭事件，确保数据分发的时候不中断。
 */
void CircuitFaultEditor::closeEvent(QCloseEvent* event)
{
	if (isServing)
	{
		CMessageBox msgBox;
		msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("服务器正在运行，确定关闭吗？"), QPixmap(":/pics/question"));
		if( msgBox.exec() == QDialog::Accepted )
		{
			m_tcpServer->close();
			delete m_tcpServer;
			event->accept();
		}
		else
		{
			event->ignore();
		}
	} 
	else
	{
		event->accept();
	}
}


CircuitFaultEditor::~CircuitFaultEditor()
{
    delete pv;
}
/**
 * 切换故障方案的描述
 */
void CircuitFaultEditor::onSolutionDisplayChanged(int index)
{
	QSqlQuery query;
	query.exec(QStringLiteral("SELECT * FROM Solutions WHERE name='%1'").arg(m_faultsList->itemText(index)));
	if (query.next())
	{
		m_faultsDesc->setText(query.value(2).toString());
	}
}

/**
 * 波形预览的当前选择有变化，显示对应的波形
 */
void CircuitFaultEditor::onWavePreviewChanged(int index)
{
    using namespace tinyxml2;
	QString waveName = m_wavesList->itemText(index);
	if (waveName.isEmpty()) return ;
    QFileInfo file(waveName.append("_desc.xml"));
	if (!file.exists())
    {
        CMessageBox msgBox;
        msgBox.setInformation(QStringLiteral("内部错误"), QStringLiteral("对应波形数据文件不存在！"), QPixmap(":/pics/error"));
        msgBox.exec();
        return;
    }
    tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;
    doc->LoadFile(waveName.toStdString().c_str());
    QPolygonF pts;

    XMLElement *e = doc->RootElement();
    e = e->FirstChildElement();

    const XMLAttribute *pattr_x = NULL;
    const XMLAttribute *pattr_y = NULL;
    while (e)
    {
        pattr_x = e->FirstAttribute();
        pattr_y = pattr_x->Next();
        pts << QPointF(pattr_x->FloatValue(), pattr_y->FloatValue());
        e = e->NextSiblingElement();
    }

    m_previewEditor->m_green_shade->hoverPoints()->setPoints(pts);
    m_previewEditor->m_green_shade->hoverPoints()->setPointLock(0, HoverPoints::LockToLeft);
    m_previewEditor->m_green_shade->hoverPoints()->setPointLock(pts.size() - 1, HoverPoints::LockToRight);
    m_previewEditor->m_green_shade->update();
	
    delete doc;
}

/**
 * 报告数据分发进度，更新进度条
 */
void CircuitFaultEditor::onDispatchReport(int index)
{
	m_progressBar->setValue(index);
	if (index == 100)
	{
		m_refreshBtn->setEnabled(true);
		m_status->setText(QStringLiteral("分发完成"));
		CMessageBox msgBox;
		msgBox.setInformation(QStringLiteral("操作成功"), QStringLiteral("波形数据分发成功！"), QPixmap(":/pics/ok"));
		msgBox.exec();
	}
}
/**
 * 服务器收到客户端连接，开始更新进度条
 */
void CircuitFaultEditor::onNewConnection()
{
	m_status->setText(QStringLiteral("正在分发"));
}
/**
 * 客户端断开连接
 */
void CircuitFaultEditor::onClientDisconnected()
{
	m_refreshBtn->setDisabled(true);
	CMessageBox msgBox;
	msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("客户端已经断开连接！"), QPixmap(":/pics/warning"));
	msgBox.exec();
}

void CircuitFaultEditor::setDefault()
{
    QGradientStops stops;
    stops << QGradientStop(0.00, QColor::fromRgba(0xff000000));
    stops << QGradientStop(1.00, QColor::fromRgba(0xffff00ff));

    m_randomEditor->setGradientStops(stops);
    m_manualEditor->setGradientStops(stops);
	m_previewEditor->setGradientStops(stops);
}

/**
 * 随机产生波形点数据
 */
void CircuitFaultEditor::onRandomGenerate()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec() + time.second()*1000);

    int num_of_points = qrand() % RANDOM_STOPS_MAX_POINTS_EXCEPT_END_POINTS;

    QGradientStops stops;
    QColor c;
    qreal x = -1;

    // end points
    c.setGreen(0xff/2);  // 纵坐标从0开始，取值范围在0到255，最底端为0， 最顶端为255
    stops << QGradientStop(0.00, c);	// 横坐标范围从0到1
    stops << QGradientStop(1.00, c);

    for (int i = 0; i < num_of_points; i++)
	{
        c.setGreen(qrand() % 0xff);	   // c 是0到0XFF的一个随机值，表示点在纵坐标上的值
        x = qrand() % 1000 / 1000.0;   // x 是点在横轴上的坐标，随机生成的。
        stops << QGradientStop(x, c);
    }
    m_randomEditor->setGradientStops(stops);
    m_randomEditor->m_green_shade->hoverPoints()->firePointChange();
}

/**
 *
 * 插入新的故障方案及结点配置
 */
void CircuitFaultEditor::onAddNewSolution()
{
	CMessageBox msgBox;
	NewSolutionDialog* dlg = new NewSolutionDialog();
	if (dlg->exec() == QDialog::Accepted)
	{
		QSqlQuery query;
		query.exec(QStringLiteral("INSERT INTO Solutions(name, desc) VALUES('%1', '%2')").arg(dlg->getSolutionName()).arg(dlg->getSolutionDesc()));

		QVariantList ids, descs;
		const QString& defaultWave = dlg->getDefaultWave();
		
		QString str = QString("INSERT INTO Nodes(nodeId, wavea, waveb, wavec, waved, wavee, wavef, waveg, desc, solutionName) VALUES(?, '%1', '%1', '%1', '%1', '%1', '%1', '%1', ?, '%2')").arg(defaultWave).arg(dlg->getSolutionName());
		for (int i = 1; i <= dlg->getNodesNum(); ++i)
		{
			ids << i;
			descs << QDateTime::currentDateTime().toString("yyyy-M-d HH:mm:ss");
		}
		query.prepare(str);
		query.addBindValue(ids);
		query.addBindValue(descs);
		
		if (!query.execBatch())
		{
			qDebug() << query.lastQuery();
			msgBox.setInformation(QStringLiteral("内部错误"), QStringLiteral("批量插入结点数据失败!"), QPixmap(":/pics/error"));
		}
		else
		{
			msgBox.setInformation(QStringLiteral("提示"), QStringLiteral("故障方案已成功生成!"), QPixmap(":/pics/ok"));
			m_faultsDesc->setEnabled(true);
			m_saveBtn->setEnabled(true);
			m_deleteBtn->setEnabled(true);
			m_revertBtn->setEnabled(true);
			m_saveEditBtn->setEnabled(true);
			m_loadBtn->setEnabled(true);
		}
		
		m_faultsModel->select();
		m_faultsList->setCurrentIndex(0);
		msgBox.exec();
	}
}

/**
 * 生成类型切换，禁用掉相关组件
 */
void CircuitFaultEditor::onGenerationTypeChanged(const QString& str)
{
	if (!str.compare(QStringLiteral("随机生成")))
	{
		m_fixedValue->setEnabled(true);
		m_randomGenerateBtn->setEnabled(true);
		m_manualEditor->m_green_shade->hoverPoints()->setDisabled(true);
		m_randomEditor->m_green_shade->hoverPoints()->setEnabled(true);
		m_voltageValue->setText(QStringLiteral("--"));
		m_moreWavesBtn->setEnabled(true);
	}
	else
	{
		m_randomGenerateBtn->setDisabled(true);
		m_fixedValue->setDisabled(true);
		m_manualEditor->m_green_shade->hoverPoints()->setEnabled(true);
		m_randomEditor->m_green_shade->hoverPoints()->setDisabled(true);
		m_moreWavesBtn->setDisabled(true);
	}
}

/**
 * 精确控制波形电压及周期
 */
void CircuitFaultEditor::onVoltageChanged(double i)
{
	QGradientStops stops;
	QColor start_y, end_y;
	qreal voltage_value = 8.5*i;

	// 纵坐标从0开始，取值范围在0到255，最底端为0， 最顶端为255
	start_y.setGreen(0xFF/2 + (int)voltage_value);
	end_y.setGreen(0xFF/2 + (int)voltage_value);  
	// 横坐标范围从0到1
	stops << QGradientStop(0.00, start_y);	
	stops << QGradientStop(1.00, end_y);

	m_randomEditor->setGradientStops(stops);
	m_randomEditor->m_green_shade->hoverPoints()->firePointChange();
}

void CircuitFaultEditor::onPositionChanged(int i)
{
	if (i >= 0 && i <= 90)
	{
		m_voltageValue->setText(QStringLiteral("%1V").arg(15 - (int)(i/3)));  // 比值转换成电压值
	}
}
/** 
 * 生成方波和正弦波形
 */
void CircuitFaultEditor::onMoreWavesClicked()
{
	WaveSettingsDialog dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		QGradientStops stops;
		int amplitude, period, baseValue;
		if (dlg.tabIndex == 0)  // 方波
		{
			amplitude = dlg.m_squareWave->getAmplitudeValue();  // 振幅
			period = dlg.m_squareWave->getPeriodValue();		// 周期
			baseValue = dlg.m_squareWave->getBaseValue();		// 基线值

			QColor topPointColor, bottomPointColor;
			double singleStep = 1.0/(2*period);		// X值步进
			double start_x = 0.0;
			baseValue *= 8.5;
			baseValue += 0xFF/2;
			int topPoint = baseValue + amplitude*8.5;    // 方波峰值关键点
			if (topPoint > 0xFF)  // 超过范围重置为最大值
			{
				topPoint = 0xFF;
			}
			int bottomPoint = baseValue - amplitude*8.5; // 方波谷值关键点
			if (bottomPoint < 0) // 同上
			{
				bottomPoint = 0;
			}
			topPointColor.setGreen(topPoint);
			bottomPointColor.setGreen(bottomPoint);
			// 添加周期中所有关键点
			for (int i = 0; i < period; ++i)
			{
				stops << QGradientStop(start_x, topPointColor);  // 峰值关键点1
				start_x += singleStep;
				stops << QGradientStop(start_x, topPointColor);	 // 峰值关键点2
				stops << QGradientStop(start_x, bottomPointColor); // 谷值关键点1
				start_x += singleStep;
				stops << QGradientStop(start_x, bottomPointColor); // 谷值关键点2
			}
		}
		else		// 正弦 
		{
			amplitude = dlg.m_sincosWave->getAmplitudeValue();  // 振幅
			period = dlg.m_sincosWave->getPeriodValue();		// 周期
			baseValue = dlg.m_sincosWave->getBaseValue();		// 基线值

			QColor topColor, middleColor, bottomColor;
			double singleStep = 1.0/(2*period);
			baseValue *= 8.5;
			baseValue += 0xFF/2;
			double start_x = 0.0;
			int middlePoint = baseValue;
			int topPoint = baseValue + amplitude*8.5;
			int bottomPoint = baseValue - amplitude*8.5; 
			if (topPoint > 0xFF)
			{
				topPoint = 0xFF;
			}
			if (bottomPoint < 0)
			{
				bottomPoint = 0;	
			}
			topColor.setGreen(topPoint);
			middleColor.setGreen(middlePoint);
			bottomColor.setGreen(bottomPoint);

			for (int i = 0; i < period; ++i)
			{
				stops << QGradientStop(start_x, bottomColor); // 正弦曲线用三个关键点组成
				start_x += singleStep;
				stops << QGradientStop(start_x, topColor);
				start_x += singleStep;
				stops << QGradientStop(start_x, bottomColor);
			}
		}
		m_randomEditor->setGradientStops(stops);
		m_randomEditor->m_green_shade->hoverPoints()->firePointChange();
	}
}