#ifndef MSGBOX_H
#define MSGBOX_H

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "shadowwindow.h"
#include "custompushbutton.h"

class CMessageBox : public ShadowWindow
{
	Q_OBJECT

public:
	explicit CMessageBox(QWidget *parent = 0);
	~CMessageBox();
	void setInformation(const QString& title, const QString& info, QPixmap tipsIcon);

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
	QLabel* m_tipsIcon;				// 提示图片
	QLabel* m_tipsText;				// 提示信息
	CustomPushButton* m_closeBtn;   // 提示框上的关闭按钮
	QPushButton* m_okBtn;			// 确定按钮
	QPushButton* m_cancelBtn;       // 取消按钮
};

#endif  //MSGBOX_H