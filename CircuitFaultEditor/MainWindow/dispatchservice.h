#ifndef DISPATCHSERVICE_H
#define DISPATCHSERVICE_H

#include <QThread>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QMessageBox>

#define PACKAGE_TYPE_DOWNLOAD 0x01
#define PACKAGE_TYPE_DISPLAY 0x55
/**
 * 12-8
 */
// 显示命令
struct package_display
{
	quint8 type;	// 数据包类型1字节
	quint8 node;	// 结点编号1字节
	quint8 wave;	// 显示波形1字节
	quint8 reserved;// 保留1字节
};

#define DOWNLOAD_PACKAGE_LOAD_SIZE 1024
// 波形数据
struct package_download
{
	quint8 type;	// 数据包类型
	quint8 node;	// 结点编号
	quint8 wave;	// 波形编号
	quint8 reserved;// 保留字节
	quint8 data[DOWNLOAD_PACKAGE_LOAD_SIZE];
};
// 包尾
struct package_end
{
	quint8 type      ;	// 数据包类型
	quint8 cmd       ;	// 结点编号
	quint8 parameter1;	// 波形编号
	quint8 parameter2;  // 保留字节
};
// 硬件发来的按钮指令
struct package_action {
	quint8 type;	// 类型
	quint8 btnInfo;	// 按键信息
	quint16 reserved; // 两字节保留
};


/*
 * 数据分发服务：将选择好的故障方案下发给各硬件结点
 * 实现为后台线程。
 */
class DispatchService : public QThread
{
	Q_OBJECT

public:
	DispatchService(QObject *parent, int fd, QString& name);
	~DispatchService();
	void setSolutionName(QString& sname) { solutionName = sname;}
	void setRefreshFlag(bool flag) { refreshFlag = flag; }
signals:
	// 该信号用于通知UI线程更新进度条
	void dispatchReport(int);

protected:
	void run();

private:
	quint8 parseCommand(const QByteArray& block);
	int transformState(quint8 act);
	void loadWaveData(const QString& fileName, struct package_download& packet);
	void doDispatch(QTcpSocket& sock);
private:
	int socketfd;
	QString& solutionName;
	bool refreshFlag;
};

#endif // DISPATCHSERVICE_H
