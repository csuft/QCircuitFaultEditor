#include "manual.h"

ManualDialog::ManualDialog(QWidget *parent)
	: ShadowWindow(parent)
{
	resize(400, 300);
	QDesktopWidget *pDesk = QApplication::desktop();
	move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);


	QVBoxLayout *main_layout = new QVBoxLayout(this);
	QHBoxLayout *title_layout = new QHBoxLayout(this);
	QHBoxLayout *center_layout = new QHBoxLayout(this);
	QHBoxLayout *bottom_layout = new QHBoxLayout(this);

	m_titleText = new QLabel(QStringLiteral("使用手册"), this);
	m_titleIcon = new QLabel(this);
	m_closeBtn = new CustomPushButton(this);
	m_okBtn = new QPushButton(QStringLiteral("确定"), this);
	m_okBtn->setCursor(Qt::PointingHandCursor);

	QPixmap titleIcon(":/pics/app");
	m_titleIcon->setPixmap(titleIcon);
	m_titleIcon->setFixedSize(16, 16);
	m_titleIcon->setScaledContents(true);
	m_closeBtn->setBtnBackground(QString(":/pics/close"));
	m_closeBtn->setToolTip(QStringLiteral("关闭"));

	m_titleText->setFixedHeight(30);
	m_titleText->setStyleSheet("color:white;font-size:12px;");
		
	title_layout->addWidget(m_titleIcon, 0, Qt::AlignVCenter);
	title_layout->addWidget(m_titleText, 0, Qt::AlignVCenter);
	title_layout->addStretch();
	title_layout->addWidget(m_closeBtn, 0, Qt::AlignTop);
	title_layout->setSpacing(5);
	title_layout->setContentsMargins(10, 0, 5, 20);

	m_tipsText = new QTextEdit(this);
	m_tipsText->setText(QStringLiteral("1、新建故障波形，可选择手动生成和随机生成两种模式\n2、再新建故障方案，选定默认波形和硬件节点数目，点击确定\n3、双击节点列表弹出对话框，可修改对应节点的故障波形\n4、修改节点故障波形之后，需要点击保存方案按钮，以保存修改内容\n5、如需预览故障方案内容，需在下拉框中选定故障方案，再点击加载方案按钮\n6、准备分发数据时，先点击启动服务按钮开发服务器\n7、其他问题请求技术支持"));
	m_tipsText->setDisabled(true);
	m_tipsText->setObjectName("manualEdit");
		
	center_layout->addWidget(m_tipsText);
	center_layout->setSpacing(10);
	center_layout->setContentsMargins(15, 0, 15, 0);

	m_okBtn->setFixedSize(60, 25);
	m_okBtn->setObjectName("panelBtn");

	bottom_layout->addStretch();
	bottom_layout->addWidget(m_okBtn);
	bottom_layout->setSpacing(10);
	bottom_layout->setContentsMargins(0, 0, 10, 10);
		
	main_layout->addLayout(title_layout);
	main_layout->addLayout(center_layout);
	main_layout->addStretch();
	main_layout->addLayout(bottom_layout);
	main_layout->setSpacing(0);
	main_layout->setContentsMargins(5, 5, 5, 5);

	setLayout(main_layout);

	connect(m_okBtn, SIGNAL(clicked()), this, SLOT(okOperate()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(okOperate()));
}

ManualDialog::~ManualDialog()
{

}


void ManualDialog::paintEvent(QPaintEvent *event)
{
	ShadowWindow::paintEvent(event);

	int height = 35;
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
	painter.drawPixmap(QRect(5, 5, width()-10, this->height()-10), QPixmap(":/pics/background"));
	painter.drawRect(QRect(5, height, width()-10, this->height()-height-5));
}

void ManualDialog::okOperate()
{
	accept();
}