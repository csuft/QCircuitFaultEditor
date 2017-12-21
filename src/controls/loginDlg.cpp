#include "loginDlg.h"  

LoginDlg::LoginDlg(QWidget *parent)
	: ShadowWindow(parent)
{
	resize(421, 275);

	QVBoxLayout *main_layout = new QVBoxLayout(this);
	QHBoxLayout *title_layout = new QHBoxLayout(this);
	QHBoxLayout *bottom_layout = new QHBoxLayout(this);
	QHBoxLayout* bottomWidgetsLayout = new QHBoxLayout(this);
	QVBoxLayout* rightWidgetsLayout = new QVBoxLayout(this);

	m_closeBtn = new CustomPushButton(this);
	m_closeBtn->setBtnBackground(QString(":/pics/close"));
	m_closeBtn->setToolTip(QStringLiteral("关闭"));

	title_layout->addWidget(m_closeBtn, 0, Qt::AlignTop|Qt::AlignRight);
	title_layout->setSpacing(5);
	title_layout->setContentsMargins(10, 0, 5, 20);

	QLabel* headpic = new QLabel(this);
	headpic->setPixmap(QPixmap(":/pics/login_head"));
	m_password = new QLineEdit(this);
	m_password->setPlaceholderText(QStringLiteral("请输入登录密码"));
	m_password->setFixedSize(200, 30);
	m_password->setEchoMode(QLineEdit::Password);
	
	m_loginTip = new QLabel(QStringLiteral("*登录方可使用"), this);
	m_loginTip->setObjectName("greenText");

	m_loginBtn = new QPushButton(QIcon(":/pics/login_btn"), QStringLiteral("登录系统"), this);
	m_loginBtn->setFixedSize(100, 30);
	m_loginBtn->setObjectName("loginBtn");
	m_loginBtn->setDisabled(true);
	m_loginBtn->setCursor(Qt::PointingHandCursor);

	bottomWidgetsLayout->addWidget(m_loginBtn, 1, Qt::AlignTop);
	bottomWidgetsLayout->addWidget(m_loginTip, 1, Qt::AlignVCenter);
	bottomWidgetsLayout->setSpacing(10);

	rightWidgetsLayout->addWidget(m_password, 0, Qt::AlignBottom);
	rightWidgetsLayout->addLayout(bottomWidgetsLayout);
	rightWidgetsLayout->setContentsMargins(20, 5, 5, 5);

	bottom_layout->addWidget(headpic);
	bottom_layout->addLayout(rightWidgetsLayout);
	bottom_layout->setContentsMargins(50, 35, 50, 5);

	main_layout->addLayout(title_layout);
	main_layout->addLayout(bottom_layout);
	main_layout->setContentsMargins(5, 5, 5, 5);
	setLayout(main_layout);

	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_password, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
	connect(m_loginBtn, SIGNAL(clicked()), this, SLOT(loginCheck()));
}

LoginDlg::~LoginDlg()
{

}

void LoginDlg::onTextChanged(const QString& text)
{
	m_loginBtn->setDisabled(text.isEmpty());
}

void LoginDlg::paintEvent(QPaintEvent *event)
{
	ShadowWindow::paintEvent(event);

	int height = 185;
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
	painter.drawPixmap(QRect(5, 5, width()-10, this->height()-10), QPixmap(":/pics/login_bg"));
	painter.drawRect(QRect(5, height, width()-10, this->height()-height-5));
}

void LoginDlg::loginCheck()
{
	if (!m_password->text().compare("zzk"))
	{
		accept();
	}
	else
	{
		m_loginTip->setStyleSheet("color: red");
		m_loginTip->setText(QStringLiteral("*密码输入错误！"));
	}
}