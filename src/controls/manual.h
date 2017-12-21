#ifndef MANUAL_H
#define MANUAL_H

#include <QTextEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>

#include "shadowwindow.h"
#include "custompushbutton.h"

class ManualDialog : public ShadowWindow
{
	Q_OBJECT

public:
	explicit ManualDialog(QWidget *parent = 0);
	~ManualDialog();
	
private slots:
	void okOperate();

protected:
	void paintEvent(QPaintEvent *event);

private:
	QLabel* m_titleText;			// 提示框标题
	QLabel* m_titleIcon;			// 提示框图标
	QTextEdit* m_tipsText;			// 提示信息
	CustomPushButton* m_closeBtn;   // 提示框上的关闭按钮
	QPushButton* m_okBtn;			// 确定按钮
};

#endif  //MSGBOX_H