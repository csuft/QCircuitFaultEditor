#include "newsolutiondialog.h"

NewSolutionDialog::NewSolutionDialog(QWidget *parent)
	: ShadowWindow(parent)
{
	resize(450, 300);

	QVBoxLayout *main_layout = new QVBoxLayout(this);
	QHBoxLayout *title_layout = new QHBoxLayout(this);
	QFormLayout *center_Layout = new QFormLayout(this);		
	QHBoxLayout *bottom_layout = new QHBoxLayout(this);

	// top title layout
	m_titleText = new QLabel(QStringLiteral("添加新故障方案"), this);
	m_titleIcon = new QLabel(this);
	m_closeBtn = new CustomPushButton(this);
	m_okBtn = new QPushButton(QStringLiteral("确定"), this);
	m_okBtn->setCursor(Qt::PointingHandCursor);
	m_cancelBtn = new QPushButton(QStringLiteral("取消"), this);
	m_cancelBtn->setCursor(Qt::PointingHandCursor);
	
	QPixmap titleIcon(":/pics/app");
	m_titleIcon->setPixmap(titleIcon);
	m_titleIcon->setFixedSize(16, 16);
	m_titleIcon->setScaledContents(true);
	m_closeBtn->setBtnBackground(QString(":/pics/close"));
	m_closeBtn->setToolTip(QStringLiteral("关闭"));

	m_titleText->setFixedHeight(30);
	m_titleText->setStyleSheet("color:white;font-size:11px;");
		
	title_layout->addWidget(m_titleIcon, 0, Qt::AlignVCenter);
	title_layout->addWidget(m_titleText, 0, Qt::AlignVCenter);
	title_layout->addStretch();
	title_layout->addWidget(m_closeBtn, 0, Qt::AlignTop);
	title_layout->setSpacing(5);
	title_layout->setContentsMargins(10, 0, 5, 20);

	// center layout
	m_solutionName = new QLineEdit(this);
	m_solutionDesc = new QTextEdit(this);
	m_nodesNum = new QSpinBox(this);
	m_solutionName->setPlaceholderText(QStringLiteral("输入方案名称"));
	m_solutionName->setFixedHeight(30);
	m_solutionName->setObjectName("lineEdit");
	m_solutionDesc->setPlaceholderText(QStringLiteral("不超过30个字符"));
	m_nodesNum->setMaximum(20);
	m_nodesNum->setMinimum(0);
	m_nodesNum->setValue(20);
	m_nodesNum->setFixedHeight(20);
	center_Layout->addRow(QStringLiteral("方案名称: "), m_solutionName);
	center_Layout->addRow(QStringLiteral("方案描述: "), m_solutionDesc);

	m_waveList = new QComboBox(this);
	m_waveModel = new QSqlTableModel(this);
	m_waveModel->setTable("Waves");
	m_waveModel->select();
	m_waveList->setModel(m_waveModel);
	m_waveList->setModelColumn(m_waveModel->fieldIndex("name"));
	QLabel* wavesLabel = new QLabel(QStringLiteral("默认波形:"), this);
	QLabel* numLabel = new QLabel(QStringLiteral("硬件结点数:"), this);
	QHBoxLayout* hlayout = new QHBoxLayout(this);
	hlayout->addWidget(wavesLabel);
	hlayout->addWidget(m_waveList);
	hlayout->addWidget(numLabel);
	hlayout->addWidget(m_nodesNum);
	
	center_Layout->addRow(hlayout);
	center_Layout->setContentsMargins(10, 0, 10, 10);
	center_Layout->setSpacing(5);
	// bottom layout
	m_okBtn->setFixedSize(60, 25);
	m_cancelBtn->setFixedSize(60, 25);
	m_okBtn->setObjectName("panelBtn");
	m_cancelBtn->setObjectName("panelBtn");

	bottom_layout->addStretch();
	bottom_layout->addWidget(m_okBtn);
	bottom_layout->addWidget(m_cancelBtn);
	bottom_layout->setSpacing(10);
	bottom_layout->setContentsMargins(0, 0, 10, 10);
		
	main_layout->addLayout(title_layout);
	main_layout->addLayout(center_Layout);
	main_layout->addLayout(bottom_layout);
	main_layout->setSpacing(0);
	main_layout->setContentsMargins(5, 5, 5, 10);

	setLayout(main_layout);

	connect(m_okBtn, SIGNAL(clicked()), this, SLOT(okOperate()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(cancelOperate()));
	connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(cancelOperate()));
}

NewSolutionDialog::~NewSolutionDialog()
{

}

void NewSolutionDialog::paintEvent(QPaintEvent *event)
{
	ShadowWindow::paintEvent(event);

	int height = 35;
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
	painter.drawPixmap(QRect(5, 5, width()-10, this->height()-10), QPixmap(":/pics/background"));
	painter.drawRect(QRect(5, height, width()-10, this->height()-height-5));
}

void NewSolutionDialog::okOperate()
{
	CMessageBox msgBox;
	m_name = m_solutionName->text();
	m_desc = m_solutionDesc->toPlainText();
	m_num = m_nodesNum->value();
	m_defaultWave = m_waveList->currentText();
	if (m_defaultWave.isEmpty())
	{
		msgBox.setInformation(QStringLiteral("操作失败"), QStringLiteral("默认波形不能为空，请先选择波形！"), QPixmap(":/pics/warning"));
		msgBox.exec();
		return ;
	}
	if (m_name.isEmpty() || m_desc.isEmpty() || m_num == 0)
	{
		msgBox.setInformation(QStringLiteral("操作失败"), QStringLiteral("添加失败，请确保所有字段不为空！"), QPixmap(":/pics/warning"));
		msgBox.exec();
	}
	else
	{
		accept();
	}
}

void NewSolutionDialog::cancelOperate()
{
	reject();
}