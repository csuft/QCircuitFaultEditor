#include "ctcpserver.h"

CTcpServer::CTcpServer(QObject *parent)
	: QTcpServer(parent)
{

}

CTcpServer::~CTcpServer()
{

}

void CTcpServer::incomingConnection(qintptr handle)
{
	m_handle = handle;
	service = new DispatchService(this, handle, m_solutionName);
	// 线程资源自动释放
	connect(service, SIGNAL(finished()), service, SLOT(deleteLater()));
	// 通知UI线程更新状态栏
	connect(service, SIGNAL(dispatchReport(int)), this, SIGNAL(dispatchReport(int)));
	emit newConnection();
	service->start();
}

void CTcpServer::refreshSolution(QString& solutionName)
{
	service->setRefreshFlag(true);
	service->setSolutionName(solutionName);
}