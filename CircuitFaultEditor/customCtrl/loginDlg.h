#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "shadowwindow.h"
#include "custompushbutton.h"

class LoginDlg : public ShadowWindow
{
	Q_OBJECT

public:
	explicit LoginDlg(QWidget *parent = 0);
	~LoginDlg();
	
private slots:
	void loginCheck();
	void onTextChanged(const QString&);

protected:
	void paintEvent(QPaintEvent *event);

private:
	CustomPushButton* m_closeBtn;   // 提示框上的关闭按钮
	QLineEdit* m_password;			// 密码输入框
	QPushButton* m_loginBtn;		// 登录按钮
	QLabel* m_loginTip;				// 登录提示
};

#endif  //MSGBOX_H