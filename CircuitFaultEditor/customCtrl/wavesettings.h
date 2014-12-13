#ifndef WAVESETTINGS_H
#define WAVESETTINGS_H

#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QSpinBox>
#include <QGroupBox>
#include <QDebug>

#include "shadowwindow.h"
#include "custompushbutton.h"

// 方波
class SquareTab : public QWidget
{
    Q_OBJECT

public:
    explicit SquareTab(QWidget *parent = 0);
	int getPeriodValue() const { return m_periodNum; }
	int getAmplitudeValue() const { return m_amplitudeNum; }
	int getBaseValue()const {return m_baseValueNum; }
	void saveValues();

private:
	QGroupBox* m_groupBox;	// 组合框
	QSpinBox* m_period;		// 方波周期
	QSpinBox* m_amplitude;	// 振幅值
	QSpinBox* m_baseValue;	// 基线值

	QVBoxLayout* m_mainLayout;
	QLabel* m_preview;
	QFormLayout* m_topLayout;

	int m_periodNum;
	int m_amplitudeNum;
	int m_baseValueNum;	
};

// 正余弦波形
class SinCosTab : public QWidget
{
    Q_OBJECT

public:
    explicit SinCosTab(QWidget *parent = 0);
	int getPeriodValue() const { return m_periodNum; }
	int getAmplitudeValue() const { return m_amplitudeNum; }
	int getBaseValue()const {return m_baseValueNum; }
	void saveValues();
private:
	QGroupBox* m_groupBox;	// 组合框
	QSpinBox* m_period;		// 方波周期
	QSpinBox* m_amplitude;	// 振幅
	QSpinBox* m_baseValue;	// 基线值

	QVBoxLayout* m_mainLayout;	
	QLabel* m_preview;
	QFormLayout* m_topLayout;

	int m_periodNum;
	int m_amplitudeNum;
	int m_baseValueNum;	
};

class WaveSettingsDialog : public ShadowWindow
{
	Q_OBJECT 

public:
	explicit WaveSettingsDialog(QWidget *parent = 0);
	~WaveSettingsDialog();

	SquareTab* m_squareWave;		// 方波
	SinCosTab* m_sincosWave;		// 正余弦波
	int tabIndex;
	
private slots:
	void onSaveClicked();
	void onWaveTabChanged(int i);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QLabel* m_titleText;			// 提示框标题
	QLabel* m_titleIcon;			// 提示框图标
	CustomPushButton* m_closeBtn;   // 提示框上的关闭按钮
	QTabWidget* m_tabWidget;		// Tab切换
	QPushButton* m_okBtn;			// OK按钮
};



#endif  //MSGBOX_H