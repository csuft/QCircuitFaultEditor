#ifndef NEWSOLUTIONDIALOG_H
#define NEWSOLUTIONDIALOG_H

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QFormLayout>
#include <QSqlTableModel>

#include "shadowwindow.h"
#include "custompushbutton.h"
#include "msgbox.h"

class NewSolutionDialog : public ShadowWindow
{
	Q_OBJECT

public:
	explicit NewSolutionDialog(QWidget *parent = 0);
	const QString& getSolutionName() const{ return m_name;}
	const QString& getSolutionDesc() const { return m_desc; }
	const QString& getDefaultWave() const { return m_defaultWave; }
	int getNodesNum() { return m_num; }
	~NewSolutionDialog();

signals:
	void okMessageHidden(bool is_checked);
	void msgChecked(bool is_checked, bool is_ok);
	
private slots:
	void okOperate();
	void cancelOperate();

protected:
	void paintEvent(QPaintEvent *event);

private:
	QLabel* m_titleText;			// 提示框标题
	QLabel* m_titleIcon;			// 提示框图标

	QLineEdit* m_solutionName;		// 方案名称
	QTextEdit* m_solutionDesc;		// 方案描述
	QSpinBox* m_nodesNum;			// 结点个数
	QComboBox* m_waveList;			// 波形列表
	QSqlTableModel* m_waveModel;	

	CustomPushButton* m_closeBtn;   // 提示框上的关闭按钮
	QPushButton* m_okBtn;			// 确定按钮
	QPushButton* m_cancelBtn;       // 取消按钮

	QString m_defaultWave;			// 默认波形
	QString m_name;
	QString m_desc;
	int m_num;
};

#endif  //MSGBOX_H