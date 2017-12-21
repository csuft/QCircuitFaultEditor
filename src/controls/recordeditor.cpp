#include "recordeditor.h"

RecordEditor::RecordEditor(QString& a, QString& b, QString& c, QString& d, QString& e, QString& f, QString& g, QWidget *parent)
	: ShadowWindow(parent)
{
	setFixedSize(450, 290);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* bodyLayout = new QHBoxLayout(this);
	QHBoxLayout* topLayout = new QHBoxLayout(this);
	QFormLayout* comboBoxLayout = new QFormLayout(this);
	QHBoxLayout* btnsLayout = new QHBoxLayout(this);
	
	// 窗口头部
	m_title = new QLabel(QStringLiteral("修改结点波形"), this);
	m_title->setObjectName("mainTitle");
	m_closeBtn = new CustomPushButton(this);
	m_closeBtn->setBtnBackground(":/pics/close");
	m_closeBtn->setToolTip(QStringLiteral("关闭"));
	topLayout->addWidget(m_title, 0, Qt::AlignVCenter);
	topLayout->addStretch();
	topLayout->addWidget(m_closeBtn, 0, Qt::AlignTop);
	topLayout->setSpacing(0);
	topLayout->setContentsMargins(15, 5, 5, 5);

	// 模型
	m_wavesModel = new QSqlTableModel(this);
	m_wavesModel->setTable("Waves");
	m_wavesModel->select();

	m_wavea = new QComboBox(this);
	m_wavea->setModel(m_wavesModel);
	m_wavea->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_wavea->setCurrentText(a);
	m_wavea->setFixedHeight(25);
	m_waveb = new QComboBox(this);
	m_waveb->setModel(m_wavesModel);
	m_waveb->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_waveb->setFixedHeight(25);
	m_waveb->setCurrentText(b);
	m_wavec = new QComboBox(this);
	m_wavec->setModel(m_wavesModel);
	m_wavec->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_wavec->setFixedHeight(25);
	m_wavec->setCurrentText(c);
	m_waved = new QComboBox(this);
	m_waved->setModel(m_wavesModel);
	m_waved->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_waved->setFixedHeight(25);
	m_waved->setCurrentText(d);
	m_wavee = new QComboBox(this);
	m_wavee->setModel(m_wavesModel);
	m_wavee->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_wavee->setFixedHeight(25);
	m_wavee->setCurrentText(e);
	m_wavef = new QComboBox(this);
	m_wavef->setModel(m_wavesModel);
	m_wavef->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_wavef->setFixedHeight(25);
	m_wavef->setCurrentText(f);
	m_waveg = new QComboBox(this);
	m_waveg->setModel(m_wavesModel);
	m_waveg->setModelColumn(m_wavesModel->fieldIndex("name"));
	m_waveg->setFixedHeight(25);
	m_waveg->setCurrentText(g);

	// 表单布局
	comboBoxLayout->addRow(QStringLiteral("波形A:"), m_wavea);
	comboBoxLayout->addRow(QStringLiteral("波形B:"), m_waveb);
	comboBoxLayout->addRow(QStringLiteral("波形C:"), m_wavec);
	comboBoxLayout->addRow(QStringLiteral("波形D:"), m_waved);
	comboBoxLayout->addRow(QStringLiteral("波形E:"), m_wavee);
	comboBoxLayout->addRow(QStringLiteral("波形F:"), m_wavef);
	comboBoxLayout->addRow(QStringLiteral("波形G:"), m_waveg);

	QFrame* verticalLine = new QFrame(this);
	verticalLine->setFrameShape(QFrame::VLine);

	QLabel* pic = new QLabel(this);
	QPixmap flag(":/pics/flag"); 
	pic->setPixmap(flag);
	pic->setFixedSize(flag.width(), flag.height());

	bodyLayout = new QHBoxLayout(this);
	bodyLayout->addLayout(comboBoxLayout);
	bodyLayout->addWidget(verticalLine);
	bodyLayout->addWidget(pic);
	bodyLayout->setContentsMargins(10, 5, 10, 5);

	m_saveBtn = new QPushButton(QStringLiteral("确定修改"), this);
	m_saveBtn->setObjectName("panelBtn");
	m_saveBtn->setFixedSize(85, 30);
	m_saveBtn->setCursor(Qt::PointingHandCursor);
	m_CancelBtn = new QPushButton(QStringLiteral("放弃修改"), this);
	m_CancelBtn->setObjectName("panelBtn");
	m_CancelBtn->setFixedSize(85, 30);
	m_CancelBtn->setCursor(Qt::PointingHandCursor);
	btnsLayout->addStretch();
	btnsLayout->addWidget(m_saveBtn, 0, Qt::AlignRight);
	btnsLayout->addWidget(m_CancelBtn, 0, Qt::AlignRight);
	btnsLayout->setContentsMargins(0, 0, 10, 10);

	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bodyLayout);
	mainLayout->addLayout(btnsLayout);
	mainLayout->setContentsMargins(5, 5, 5, 5);

	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_CancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_saveBtn, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
}

RecordEditor::~RecordEditor()
{

}

void RecordEditor::onSaveClicked()
{
	m_list.append(m_wavea->currentText());
	m_list.append(m_waveb->currentText());
	m_list.append(m_wavec->currentText());
	m_list.append(m_waved->currentText());
	m_list.append(m_wavee->currentText());
	m_list.append(m_wavef->currentText());
	m_list.append(m_waveg->currentText());
	accept();
}

void RecordEditor::paintEvent(QPaintEvent *event)
{
	ShadowWindow::paintEvent(event);

	int height = 35;
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
	painter.drawPixmap(QRect(5, 5, width()-10, this->height()-10), QPixmap(":/pics/background"));
	painter.drawRect(QRect(5, height, width()-10, this->height()-height-5));
}
