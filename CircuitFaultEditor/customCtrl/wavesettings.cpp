#include "wavesettings.h"

WaveSettingsDialog::WaveSettingsDialog(QWidget *parent)
	: ShadowWindow(parent)
{
	tabIndex = 0;
	resize(350, 160);
	QDesktopWidget *pDesk = QApplication::desktop();
	move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QHBoxLayout *bottomLayout = new QHBoxLayout(this);
	QHBoxLayout *title_layout = new QHBoxLayout(this);
	m_titleText = new QLabel(QStringLiteral("设置波形参数"), this);
	m_titleIcon = new QLabel(this);
	m_closeBtn = new CustomPushButton(this);

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
	title_layout->setContentsMargins(5, 0, 5, 10);

	
	m_tabWidget = new QTabWidget(this);
	m_tabWidget->setFixedHeight(160);
	m_squareWave = new SquareTab(this);
	m_sincosWave = new SinCosTab(this); 
	m_tabWidget->addTab(m_squareWave, QStringLiteral("方波波形"));
	m_tabWidget->addTab(m_sincosWave, QStringLiteral("正弦波形"));

	m_okBtn = new QPushButton(QStringLiteral("保存设置"));
	m_okBtn->setObjectName("panelBtn");
	m_okBtn->setFixedSize(80, 30);
	bottomLayout->addStretch();
	bottomLayout->addWidget(m_okBtn); 
	bottomLayout->setContentsMargins(0, 5, 5, 5);
	mainLayout->addLayout(title_layout);
	mainLayout->addWidget(m_tabWidget);
	mainLayout->addLayout(bottomLayout);
	mainLayout->setContentsMargins(5, 5, 5, 5);

	setLayout(mainLayout);
	connect(m_okBtn, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onWaveTabChanged(int)));
}

WaveSettingsDialog::~WaveSettingsDialog()
{

}


void WaveSettingsDialog::paintEvent(QPaintEvent *event)
{
	ShadowWindow::paintEvent(event);

	int height = 35;
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
	painter.drawPixmap(QRect(5, 5, width()-10, this->height()-10), QPixmap(":/pics/background"));
	painter.drawRect(QRect(5, height, width()-10, this->height()-height-5));
}

void WaveSettingsDialog::onSaveClicked()
{
	if (tabIndex == 0)  // 方波
	{
		m_squareWave->saveValues();
	}
	else	// 正弦
	{
		m_sincosWave->saveValues();
	}

	accept();
}

SquareTab::SquareTab(QWidget *parent)
{
	setFixedHeight(160);
	m_mainLayout = new QVBoxLayout(this);
	m_preview = new QLabel(this);  // 70x360
	m_topLayout = new QFormLayout(this);

	m_groupBox = new QGroupBox(QStringLiteral("波形参数"), this);	// 组合框
	m_period = new QSpinBox(this);		// 方波周期
	m_period->setObjectName("spinBox");
	m_period->setFixedHeight(25);
	m_period->setMaximum(10);
	m_period->setMinimum(1);
	m_period->setSingleStep(1);
	m_period->setValue(5);
	m_amplitude = new QSpinBox(this);	// 振幅值
	m_amplitude->setObjectName("spinBox");
	m_amplitude->setFixedHeight(25);
	m_amplitude->setSuffix("v");
	m_amplitude->setMaximum(15);
	m_amplitude->setMinimum(1);
	m_amplitude->setSingleStep(1);
	m_amplitude->setValue(5);
	m_baseValue = new QSpinBox(this);	// 基线值
	m_baseValue->setObjectName("spinBox");
	m_baseValue->setFixedHeight(25);
	m_baseValue->setSuffix("v");
	m_baseValue->setMaximum(15);
	m_baseValue->setMinimum(-15);
	m_baseValue->setSingleStep(1);
	m_baseValue->setValue(0);

	m_topLayout->addRow(QStringLiteral("方波周期："), m_period);
	m_topLayout->addRow(QStringLiteral("振幅值："), m_amplitude);
	m_topLayout->addRow(QStringLiteral("Y轴偏移值:"), m_baseValue);
	m_groupBox->setLayout(m_topLayout);
	m_mainLayout->addWidget(m_groupBox);
	m_mainLayout->addWidget(m_preview);
	m_mainLayout->setSpacing(5);

	setLayout(m_mainLayout);
}

SinCosTab::SinCosTab(QWidget *parent)
{
	setFixedHeight(160);
	m_mainLayout = new QVBoxLayout(this);	
	m_preview = new QLabel(this);
	m_topLayout = new QFormLayout(this);

	m_groupBox = new QGroupBox(QStringLiteral("波形参数"), this);	// 组合框
	m_period = new QSpinBox(this);		// 正弦周期
	m_period->setObjectName("spinBox");
	m_period->setFixedHeight(25);
	m_period->setMaximum(10);
	m_period->setMinimum(1);
	m_period->setSingleStep(1);
	m_period->setValue(5);

	m_amplitude = new QSpinBox(this);	// 振幅
	m_amplitude->setObjectName("spinBox");
	m_amplitude->setFixedHeight(25);
	m_amplitude->setSuffix("v");
	m_amplitude->setMaximum(15);
	m_amplitude->setMinimum(1);
	m_amplitude->setSingleStep(1);
	m_amplitude->setValue(5);

	m_baseValue = new QSpinBox(this);	// 基线值
	m_baseValue->setObjectName("spinBox");
	m_baseValue->setFixedHeight(25);
	m_baseValue->setSuffix("v");
	m_baseValue->setMaximum(15);
	m_baseValue->setMinimum(-15);
	m_baseValue->setSingleStep(1);
	m_baseValue->setValue(0);

	m_topLayout->addRow(QStringLiteral("正弦周期："), m_period);
	m_topLayout->addRow(QStringLiteral("振幅值："), m_amplitude);
	m_topLayout->addRow(QStringLiteral("Y轴偏移值:"), m_baseValue);
	m_groupBox->setLayout(m_topLayout);
	m_mainLayout->addWidget(m_groupBox);
	m_mainLayout->addWidget(m_preview);

	setLayout(m_mainLayout);
}

void WaveSettingsDialog::onWaveTabChanged(int i)
{
	tabIndex = i;
}

void SinCosTab::saveValues()
{
	m_periodNum = m_period->value();
	m_amplitudeNum = m_amplitude->value();
	m_baseValueNum = m_baseValue->value();	
}

void SquareTab::saveValues()
{
	m_periodNum = m_period->value();
	m_amplitudeNum = m_amplitude->value();
	m_baseValueNum = m_baseValue->value();
}