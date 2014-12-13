#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include <QTcpServer>
#include "dispatchservice.h"

/*
 * 自定义的TCP服务器类，当侦听到连接请求的时候，启动子线程解析发送数据。
 * 12-8
 */
class CTcpServer : public QTcpServer
{
	Q_OBJECT

public:
	CTcpServer(QObject *parent = NULL);
	~CTcpServer();
	void setSolutionName(QString solutionName) { m_solutionName = solutionName;}
	void refreshSolution(QString& solutionName);
signals:
	void dispatchReport(int index);

protected:
	void incomingConnection(qintptr handle);

private:
	QString m_solutionName;  // 将要分发的方案名称
	DispatchService* service;
	qintptr m_handle;
};

#endif // CTCPSERVER_H
