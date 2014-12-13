#ifndef CIRCUITFAULTEDITOR_H
#define CIRCUITFAULTEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QtSql/QSqlTableModel>
#include <QLabel>
#include <QTableView>
#include <QDateTime>
#include <QHeaderView>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QMessageBox>
#include <QDoubleSpinBox>

#include "../customCtrl/custompushbutton.h"
#include "../customCtrl/nofocusdelegate.h"
#include "../customCtrl/shadowwindow.h"
#include "../customCtrl/ctableview.h"
#include "../customCtrl/msgbox.h"
#include "../customCtrl/manual.h"
#include "../customCtrl/newsolutiondialog.h"
#include "../customCtrl/wavesettings.h "
#include "../WaveEditor/waveseditor.h"
#include "../WaveEditor/tinyxml2.h"
#include "../WaveEditor/shadewidget.h"
#include "../WaveEditor/hoverpoints.h"
#include "ctcpserver.h"

#define SERVER_PORT 1000 //8888  // SERVER PORT
const UINT32 NUMBER_OF_POINTS = 512;
const UINT32 RANDOM_STOPS_MAX_POINTS_EXCEPT_END_POINTS = 10;

class CircuitFaultEditor : public ShadowWindow
{
	Q_OBJECT

public:
	CircuitFaultEditor(QWidget *parent = 0);
	~CircuitFaultEditor();

signals:
	void saveAllNodes();		// 保存所有结点
	void resetAllNodes();		// 重置所有结点
	void loadNewSolution(QString);		// 加载新的故障方案
	void deleteSolution(const QString&);

private slots:
	void onNewSolutionClicked();
	void onDispatchBtnClicked();
	void onRefreshClicked();
	void onHelpClicked();
	void onDeleteSolutionClicked();
	void onSaveDescriptionClicked();
	void onSolutionDisplayChanged(int index);
	void onWavePreviewChanged(int index);
	void onWaveSaveBtnClicked();
	void onDispatchReport(int index);
	void onNewConnection();
	void onAddNewSolution();
	void onGenerationTypeChanged(const QString&);
	void onVoltageChanged(double i);
	void onPositionChanged(int);
	void onClientDisconnected();
	// wave editor
	void setDefault();
	void onRandomGenerate();
	void onMoreWavesClicked();

protected:
	void paintEvent(QPaintEvent* event);
	void closeEvent(QCloseEvent *event);

private:
	void saveWaveData();

private:
	WavesEditor* m_randomEditor;
	WavesEditor* m_manualEditor;
	WavesEditor* m_previewEditor;
	QVector<QPointF> *pv;  // points
	// Widget layout
	QHBoxLayout* m_topLayout;
	QGridLayout* m_bottomMainLayout;
	QVBoxLayout* m_rightLayout;
	QHBoxLayout* m_bottomLayout;
	QVBoxLayout* m_rightBottomLayout;
	QVBoxLayout* m_mainLayout;

	//////////////////////////////////////////////////////////////////////////
	CustomPushButton* m_miniBtn;
	CustomPushButton* m_closeBtn;
	QLabel* m_title;

	//////////////////////////////////////////////////////////////////////////
	CTableView* m_tableView;	// 节点列表
	QPushButton* m_newBtn;
	QPushButton* m_saveBtn;
	QPushButton* m_deleteBtn;	// 删除故障方案
	QPushButton* m_helpBtn;
	QPushButton* m_revertBtn;	// 撤销修改

	//////////////////////////////////////////////////////////////////////////
	QLabel* m_faultLabel;
	QComboBox* m_faultsList;	// 故障方案列表
	QSqlTableModel* m_faultsModel;  
	QTextEdit* m_faultsDesc;	// 故障描述
	QPushButton* m_saveEditBtn;	// 修改故障描述按钮
	QPushButton* m_loadBtn;

	//////////////////////////////////////////////////////////////////////////
	QLabel* m_wavesDesc;		// 说明标签
	QComboBox* m_wavesList;		// 波形列表
	QSqlTableModel* m_wavesModel;

	//////////////////////////////////////////////////////////////////////////
	QGroupBox* m_builtinWaves;	// 故障波形配置
	QComboBox* m_generationType;// 生成类型
	QLineEdit* m_waveName;		// 波形名称
	QPushButton* m_saveWaveBtn;	// 保存按钮
	QPushButton* m_randomGenerateBtn; // 随机按钮
	QGridLayout* m_waveSettingsLayout;
	//////////////////////////////////////////////////////////////////////////
	QGroupBox* m_randomWaves;	// 随机生成
	QGroupBox* m_manualWaves;	// 手动生成
	QLabel* m_voltageValue;		// 当前电压值
	QDoubleSpinBox*  m_fixedValue;	// 定值电压
	QPushButton* m_moreWavesBtn;	// 设置更多波形按钮

	//////////////////////////////////////////////////////////////////////////
	QGroupBox* m_dispatchBox;	// 数据分发
	QLabel* m_dispatchNote;		// 方案说明
	QLabel* m_status;			// 进度说明
	QLabel* m_solutionName;		// 方案名称
	QPushButton* m_dispatchBtn; // 数据分发按钮
	QProgressBar* m_progressBar;// 分发进度
	QPushButton* m_refreshBtn;	// 关于按钮

	//////////////////////////////////////////////////////////////////////////
	// 数据分发相关
	bool isServing;
	CTcpServer* m_tcpServer;
	QString m_oldSolutionName;		// 旧的故障方案
	QString m_curSolutionName;		// 新的故障方案
};

#endif // CIRCUITFAULTEDITOR_H
