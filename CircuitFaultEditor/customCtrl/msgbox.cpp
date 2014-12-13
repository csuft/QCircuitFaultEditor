#include "msgbox.h"  

CMessageBox::CMessageBox(QWidget *parent)
	: ShadowWindow(parent)
{
	resize(280, 160);
	move(475, 300);

	QVBoxLayout *main_layout = new QVBoxLayout(this);
	QHBoxLayout *title_layout = new QHBoxLayout(this);
	QHBoxLayout *center_layout = new QHBoxLayout(this);
	QHBoxLayout *bottom_layout = new QHBoxLayout(this);

	m_titleText = new QLabel(QStringLiteral("提示"), this);
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

	m_tipsIcon = new QLabel(this);
	m_tipsIcon->setFixedSize(45, 45);
	m_tipsIcon->setScaledContents(true);

	m_tipsText = new QLabel(this);
	m_tipsText->setWordWrap(true);
	m_tipsText->setStyleSheet("color: rgb(164, 190, 156); font-size: 12px;");
		
	center_layout->addWidget(m_tipsIcon);
	center_layout->addWidget(m_tipsText);
	center_layout->setSpacing(10);
	center_layout->setContentsMargins(15, 0, 15, 0);

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
	main_layout->addLayout(center_layout);
	main_layout->addStretch();
	main_layout->addLayout(bottom_layout);
	main_layout->setSpacing(0);
	main_layout->setContentsMargins(5, 5, 5, 5);

	setLayout(main_layout);

	connect(m_okBtn, SIGNAL(clicked()), this, SLOT(okOperate()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(cancelOperate()));
	connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(cancelOperate()));
}

CMessageBox::~CMessageBox()
{

}

void CMessageBox::setInformation(const QString& title, const QString& info, QPixmap tipsIcon)
{
	m_titleText->setText(title);
	m_tipsText->setText(info);
	m_tipsIcon->setPixmap(tipsIcon);
}

void CMessageBox::paintEvent(QPaintEvent *event)
{
	ShadowWindow::paintEvent(event);

	int height = 35;
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
	painter.drawPixmap(QRect(5, 5, width()-10, this->height()-10), QPixmap(":/pics/background"));
	painter.drawRect(QRect(5, height, width()-10, this->height()-height-5));
}

void CMessageBox::okOperate()
{
	accept();
}

void CMessageBox::cancelOperate()
{
	reject();
}