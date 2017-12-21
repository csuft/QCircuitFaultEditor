#include "dispatchservice.h"
extern int display_wave_id[20] =  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

DispatchService::DispatchService(QObject *parent, int fd, QString& name)
	: QThread(parent), socketfd(fd), solutionName(name), refreshFlag(false)
{

}

DispatchService::~DispatchService()
{

}

void DispatchService::run()
{
	QTcpSocket tcpSocket;
	if (!tcpSocket.setSocketDescriptor(socketfd))
	{
		// 套接字绑定出错
		return;
	}
	sleep(2);

	doDispatch(tcpSocket);

	struct package_display packageDisplay;
	packageDisplay.type = PACKAGE_TYPE_DISPLAY;
	packageDisplay.reserved = 0;
	// 循环解析命令发送显示指令
	while(tcpSocket.state() & QAbstractSocket::ConnectedState)
	{
		// 等待5秒钟，再检测条件以判断是否分发新的故障方案
		if (tcpSocket.waitForReadyRead(5000))
		{
			QByteArray block;
			QDataStream out(&block, QIODevice::ReadWrite);
			out.setVersion(QDataStream::Qt_4_0);
			block = tcpSocket.readAll();
			quint8 act = parseCommand(block);
			// 状态转化计算
			transformState(act);
			//packageDisplay.wave
			block.clear();
			// 循环发送20次
			for(int i = 1; i <= 20; ++i)
			{
				// 填充每个数据包的对应的节点ID
				packageDisplay.type = 0x55;
				packageDisplay.node = i;
				packageDisplay.wave = display_wave_id[i-1] ;
				//out.writeRawData((char*)&packageDisplay, sizeof(packageDisplay));
				//tcpSocket.write(block);
				QByteArray block2;
			    QDataStream out2(&block2, QIODevice::ReadWrite);
			    out2.setVersion(QDataStream::Qt_4_0);
				out2.writeRawData((char*)&packageDisplay, sizeof(packageDisplay));
				tcpSocket.write(block2);
				tcpSocket.flush();
				msleep(10);
			}
			block.clear();
			// 额外发送一个数据包，通知硬件数据已经发完
			// 字段名称无对应含义
			//packageDisplay.type = 0x03;
			//packageDisplay.node = 0x03;
			//packageDisplay.wave = 0x14;
			//packageDisplay.reserved = 0x07;
			////out.writeRawData((char*)&packageDisplay, sizeof(packageDisplay));
			////tcpSocket.write(block);
			//QByteArray block3;
			//QDataStream out3(&block3, QIODevice::ReadWrite);
			//out3.setVersion(QDataStream::Qt_4_0);
		    //out3.writeRawData((char*)&packageDisplay, sizeof(packageDisplay));
			//tcpSocket.write(block3);
			//tcpSocket.flush();
		}
		// 分发新的故障方案
		if (refreshFlag)
		{
			doDispatch(tcpSocket);
			// 分发完成，关闭标记，等待解析硬件指令
			refreshFlag = false;	
		}
	}

}
/**
 * 解析数据包格式，根据不同指令发送不同数据，统共有两种格式：
 * 1. 下发数据指令
 * 2. 硬件按钮指令
 * 3. 8‘hf3 + 1byte(按键信息) + 2byte(8'h00,8'h00)
 */
quint8 DispatchService::parseCommand(const QByteArray& blcok)
{
	const char* arr = blcok.data();
	return (quint8)arr[1];
}

/**
 * 状态转换，根据不同按钮的状态计算出对应的故障波形
 */
int DispatchService::transformState(quint8 act)
{
	/*
	switch(act)
	{
	case 0xC0:  // 对应 1 1 0 0 0 0 0 0
		wave_id = 3;
		break;
	case 0x80:  // 对应 1 0 0 0 0 0 0 0
		wave_id = 2;
		break;
	case 0x40:  // 对应 0 x 0 0 0 0 0 0
	case 0x00:
		wave_id = 1;
		break;
	default:
		Q_ASSERT(0);
		break;
	}
	return 0;//2;
	*/
	// change by ph
	//欠压指示故障-常亮故障
	//int wave_id[20] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

	// 故障说明 ： 欠压指示故障-常亮故障 结点 0,1,2,3,4,16
	// 结点0 ： A 0V B 6V   C 10V
	// 结点1 ： A 0V B 3.3V C 6V
	// 结点2 ： A 0V B 3.3V  
	// 结点3 ： A 0V B 3.3V   
	// 结点4 ： A 0V B 3.3V   
	// 结点16： A 0V B 6V   C 10V
	//----------- （方案集A） ------------------ //
	if ( !solutionName.compare(QStringLiteral("欠压指示故障-常亮故障") ) )
	{
		if ( (act&0x80) )                // 电源键未按下
		{
			display_wave_id[0]  = 1 ;
			display_wave_id[1]  = 1 ;
			display_wave_id[2]  = 1 ;
			display_wave_id[3]  = 1 ;
			display_wave_id[4]  = 1 ;
			display_wave_id[16] = 1 ;
		}
		else {                            // 电源键按下
			display_wave_id[0]  = 3 ;
			display_wave_id[16] = 3 ;
			display_wave_id[1]  = 2 ;
			display_wave_id[2]  = 1 ;
			display_wave_id[3]  = 2 ;
			display_wave_id[4]  = 1 ;
		}
	}
	/// 故障说明 ： 欠压指示故障-不亮故障 结点 0,1,2,3,4,16
	// 结点0 ： A 0V B 6V   C 10V
	// 结点1 ： A 0V B 3.3V C 6V
	// 结点2 ： A 0V B 3.3V  
	// 结点3 ： A 0V B 3.3V   
	// 结点4 ： A 0V B 3.3V   
	// 结点16： A 0V B 6V   C 10V
	if ( !solutionName.compare(QStringLiteral("欠压指示故障-不亮故障") ) )
	{
		if ( (act&0x80) )                // 电源键未按下
		{
			display_wave_id[0]  = 1 ;
			display_wave_id[1]  = 1 ;
			display_wave_id[2]  = 1 ;
			display_wave_id[3]  = 1 ;
			display_wave_id[4]  = 1 ;
			display_wave_id[16] = 1 ;
		}
		else {                            // 电源键按下
			display_wave_id[0]  = 2 ;
			display_wave_id[16] = 2 ;
			display_wave_id[1]  = 3 ;
			display_wave_id[2]  = 2 ;
			display_wave_id[3]  = 1 ;
			display_wave_id[4]  = 2 ;
		}
	}
	/// 故障说明 ： 电路收发控制故障 结点 5,6,7,8 其中方波2为方波1的反向
	// 结点5 ： A 0V B 方波1 C 方波2   
	// 结点6 ： A 0V B 方波1 C 方波2
	// 结点7 ： A 0V B 方波1 C 方波2  
	// 结点8 ： A 0V B 方波1 C 方波2 D 3.3V  
	if ( !solutionName.compare(QStringLiteral("电路收发控制故障") ) )
	{
		qDebug() << "电路收发控制故障"  ;
		if ( ((act&0x80)|(act&0x40)) )   // 电源和启动没同时按下时
		{
			display_wave_id[5]  = 1 ;
			display_wave_id[6]  = 1 ;
			display_wave_id[7]  = 1 ;
			display_wave_id[8]  = 1 ;
		}
		else {                            // 电源和启动同时按下
			display_wave_id[5]  = 1 ;
			display_wave_id[6]  = 2 ;
			display_wave_id[7]  = 2 ;
			display_wave_id[8]  = 3 ;
		}
	}
	/// 故障说明 ： 有线传输控制故障 结点 9,10,11 （是否增加新的结点）
	// 结点9  ： A 0V B 3.3V   
	// 结点10 ： A 0V B 3.3V
	// 结点11 ： A 0V B 3.3V  
	if ( !solutionName.compare(QStringLiteral("有线传输控制故障") ) )
	{
		qDebug() << "有线传输控制故障"  ;
		if ( (act&0x80) )                // 电源没按下
		{
			
			qDebug() << "有线传输控制故障.电源按下"  ;
			display_wave_id[9]  = 1 ;
			display_wave_id[10] = 1 ;
			display_wave_id[11] = 1 ;
		}
		else {                            // 电源按下
            qDebug() << "有线传输控制故障.电源没有按下"  ;
			display_wave_id[9]  = 1 ;
			display_wave_id[10] = 2 ;
			display_wave_id[11] = 1 ;
		}
	}
    /// 故障说明 ： 电源电路保险故障 结点 12,13,14,15
	// 结点12 ： A 0V B 12V   
	// 结点13 ： A 0V B 3V C 5V
	// 结点14 ： A 0V B 1V C 3.75V
	// 结点15 ： A 0V B 3V C 5V    
	if ( !solutionName.compare(QStringLiteral("电源电路保险故障") ) )
	{
		qDebug() << "电源电路保险故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "电源电路保险故障.电源按下"  ;
			display_wave_id[12] = 1 ;
			display_wave_id[13] = 1 ;
			display_wave_id[14] = 1 ;
			display_wave_id[15] = 1 ;
		}
		else {                                         // 电源按下
            qDebug() << "电源电路保险故障.电源没有按下"  ;
			display_wave_id[12] = 1 ;
			display_wave_id[13] = 3 ;
			display_wave_id[14] = 3 ;
			display_wave_id[15] = 1 ;
		}
	}
    /// 故障说明 ： 电源转换器件故障 结点 12,13,14,15
	// 结点12 ： A 0V B 12V   
	// 结点13 ： A 0V B 3V B 5V
	// 结点14 ： A 0V B 1V B 3.75V
	// 结点15 ： A 0V B 3V C 5V    
	if ( !solutionName.compare(QStringLiteral("电源转换器件故障") ) )
	{
		qDebug() << "电源转换器件故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "电源转换器件故障.电源按下"  ;
			display_wave_id[12] = 1 ;
			display_wave_id[13] = 1 ;
			display_wave_id[14] = 1 ;
			display_wave_id[15] = 1 ;
		}
		else {                                         // 电源按下
            qDebug() << "电源转换器件故障.电源没有按下"  ;
			display_wave_id[12] = 1 ;
			display_wave_id[13] = 2 ;
			display_wave_id[14] = 2 ;
			display_wave_id[15] = 2 ;
		}
	}
	/// 故障说明 ： 显示故障 结点 17,18,19   
	// 结点17 ： A 0V B 3.3V 
	// 结点18 ： A 0V B 3.3V 
	// 结点19 ： A 0V B 3.3V     
	if ( !solutionName.compare(QStringLiteral("显示故障") ) )
	{
		qDebug() << "显示故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "显示故障.电源按下"  ;
			display_wave_id[17] = 1 ;
			display_wave_id[18] = 1 ;
			display_wave_id[19] = 1 ;
		}
		else {                                         // 电源按下
            qDebug() << "显示故障.电源没有按下"  ;
			display_wave_id[17] = 2 ;
			display_wave_id[18] = 2 ;
			display_wave_id[19] = 2 ;
		}
	}
	//----------- （方案集B） ------------------ //
	/// 故障说明 ： 密码设置故障 结点 0,1,2,3,4,5    
	// 结点0 ： A 0V B 3.3V 
	// 结点1 ： A 0V B 3.3V 
	// 结点2 ： A 0V B 3.3V
	// 结点3 ： A 0V B 3.3V
	// 结点4 ： A 0V B 3.3V
	// 结点5 ： A 0V B 3.3V
	if ( !solutionName.compare(QStringLiteral("密码设置故障") ) )
	{
		qDebug() << "密码设置故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "密码设置故障.电源按下"  ;
			display_wave_id[0] = 2 ;
			display_wave_id[1] = 2 ;
			display_wave_id[2] = 2 ;
			display_wave_id[3] = 1 ;
			display_wave_id[4] = 1 ;
			display_wave_id[5] = 1 ;
		}                                                  // 电源按下
		else {                                          
            // KEY1 
			    display_wave_id[0] = 2 ;
				display_wave_id[3] = 1 ;
            // KEY2  
			if( !(act&0x10) ){
				display_wave_id[1] = 1 ;
				display_wave_id[4] = 1 ;
			}
			else {
				display_wave_id[1] = 2 ;
				display_wave_id[4] = 1 ;
			}
			// KEY3
			if( !(act&0x08) ){
				display_wave_id[2] = 1 ;
				display_wave_id[5] = 1 ;
			}
			else {
				display_wave_id[2] = 2 ;
				display_wave_id[5] = 1 ;
			}
		}
	}
	/// 故障说明 ： 同步器故障 结点 6,7,8   
	// 结点6 ： A 0V B 3.3V 
	// 结点7 ： A 0V B 3.3V 
	// 结点8 ： A 0V B 3.3V     
	if ( !solutionName.compare(QStringLiteral("同步器故障") ) )
	{
		qDebug() << "同步器故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "同步器故障.电源按下"  ;
			display_wave_id[6] = 1 ;
			display_wave_id[7] = 1 ;
			display_wave_id[8] = 1 ;
		}
		else if ( (!(act&0x80))&(act&0x40) ){          // 电源键按下、启动键未按下
			qDebug() << "同步器故障.启动键未按下"  ;
			display_wave_id[6] = 2 ;
			display_wave_id[7] = 2 ;
			display_wave_id[8] = 1 ;
		}
		else if ( (!(act&0x80))&(!(act&0x40)) ){       // 电源按下、启动键按下
            qDebug() << "同步器故障.启动键按下"  ;
			display_wave_id[6] = 2 ;
			display_wave_id[7] = 1 ;
			display_wave_id[8] = 2 ;
		}
	}
	/// 故障说明 ： 启动指示故障 结点 9,10,11   
	// 结点9  ： A 0V B 3.3V 
	// 结点10 ： A 0V B 3.3V 
	// 结点11 ： A 0V B 3.3V     
	if ( !solutionName.compare(QStringLiteral("启动指示故障") ) )
	{
		qDebug() << "启动指示故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "启动指示故障.电源按下"  ;
			display_wave_id[9]  = 1 ;
			display_wave_id[10] = 1 ;
			display_wave_id[11] = 1 ;
		}
		else if ( (!(act&0x80))&(act&0x40) ){          // 电源键按下、启动键未按下
			qDebug() << "启动指示故障.启动键未按下"  ;
			display_wave_id[9]  = 1 ;
			display_wave_id[10] = 2 ;
			display_wave_id[11] = 2 ;
		}
		else if ( (!(act&0x80))&(!(act&0x40)) ){       // 电源按下、启动键按下
            qDebug() << "启动指示故障.启动键按下"  ;
			display_wave_id[9]  = 2 ;
			display_wave_id[10] = 1 ;
			display_wave_id[11] = 1 ;
		}
	}
	/// 故障说明 ： 启动指示故障 结点 9,10,11   
	// 结点9  ： A 0V B 3.3V 
	// 结点10 ： A 0V B 3.3V 
	// 结点11 ： A 0V B 3.3V     
	if ( !solutionName.compare(QStringLiteral("启动指示故障") ) )
	{
		qDebug() << "启动指示故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "启动指示故障.电源按下"  ;
			display_wave_id[9]  = 1 ;
			display_wave_id[10] = 1 ;
			display_wave_id[11] = 1 ;
		}
		else if ( (!(act&0x80))&(act&0x40) ){          // 电源键按下、启动键未按下
			qDebug() << "启动指示故障.启动键未按下"  ;
			display_wave_id[9]  = 1 ;
			display_wave_id[10] = 2 ;
			display_wave_id[11] = 2 ;
		}
		else if ( (!(act&0x80))&(!(act&0x40)) ){       // 电源按下、启动键按下
            qDebug() << "启动指示故障.启动键按下"  ;
			display_wave_id[9]  = 2 ;
			display_wave_id[10] = 1 ;
			display_wave_id[11] = 1 ;
		}
	}
	/// 故障说明 ： 调制解调故障-发送故障 结点 12,13,14,15   
	// 结点12 ： A 0V B 方波 C 正弦 
	// 结点13 ： A 0V B 方波 C 正弦 
	// 结点14 ： A 0V B 方波 C 正弦
	// 结点15 ： A 0V B 方波 C 正弦
	if ( !solutionName.compare(QStringLiteral("调制解调故障-发送故障") ) )
	{
		qDebug() << "调制解调故障-发送故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "调制解调故障-发送故障.电源按下"  ;
			display_wave_id[12] = 1 ;
			display_wave_id[13] = 1 ;
			display_wave_id[14] = 1 ;
			display_wave_id[15] = 1 ;
		}
		else if ( (!(act&0x80))&(!(act&0x40)) ){       // 电源按下、启动键按下
            qDebug() << "调制解调故障-发送故障.启动键按下"  ;
			display_wave_id[12] = 2 ;
			display_wave_id[13] = 3 ;
			display_wave_id[14] = 1 ;
			display_wave_id[15] = 1 ;
		}
	}
	/// 故障说明 ： 调制解调故障-接收故障 结点 12,13,14,15   
	// 结点12 ： A 0V B 方波 C 正弦 
	// 结点13 ： A 0V B 方波 C 正弦 
	// 结点14 ： A 0V B 方波 C 正弦
	// 结点15 ： A 0V B 方波 C 正弦
	if ( !solutionName.compare(QStringLiteral("调制解调故障-接收故障") ) )
	{
		qDebug() << "调制解调故障-接收故障"  ;
		if ( (act&0x80) )                             // 电源没按下
		{
			
			qDebug() << "调制解调故障-接收故障.电源按下"  ;
			display_wave_id[12] = 1 ;
			display_wave_id[13] = 1 ;
			display_wave_id[14] = 1 ;
			display_wave_id[15] = 1 ;
		}
		else if ( (!(act&0x80))&(!(act&0x40)) ){       // 电源按下、启动键按下
            qDebug() << "调制解调故障-接收故障.启动键按下"  ;
			display_wave_id[12] = 1 ;
			display_wave_id[13] = 1 ;
			display_wave_id[14] = 2 ;
			display_wave_id[15] = 3 ;
		}
	}

	// change by ph
	return 0;
}
/**
 * 加载波形数据，准备下发
 */
void DispatchService::loadWaveData(const QString& fileName, struct package_download& packet)
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
	QString str;
	int index = 0;

	while (!in.atEnd())
	{
		in >> str;
		if (0 == str.trimmed().compare(""))
		{
			// ignore the blank line
			continue;
		}
		*((unsigned short *)&packet.data[index]) = str.toUShort();
		// 一个值占两个字节，所以需要移动2个位置
		index += 2;
	}
}

void DispatchService::doDispatch(QTcpSocket& sock)
{
	struct package_download packetDown;
	struct package_end packetDown_end;
	int count = 0;
	QSqlQuery query;
	query.exec(QStringLiteral("SELECT nodeId, wavea, waveb, wavec, waved, wavee, wavef, waveg FROM Nodes WHERE solutionName='%1'").arg(solutionName));
	while (query.next())
	{
		packetDown.type = PACKAGE_TYPE_DOWNLOAD;
		if (!solutionName.compare(QStringLiteral("显示故障")))
		{
			packetDown.reserved = 1;
		}
		else if (!solutionName.compare(QStringLiteral("欠压灯指示故障-常亮故障")))
		{
			packetDown.reserved = 2;
		}
		else if (!solutionName.compare(QStringLiteral("启动指示故障")))
		{
			packetDown.reserved = 3;
		}
		else if (!solutionName.compare(QStringLiteral("正常模式")))
		{
			packetDown.reserved = 7;
		}
		else 
		{
			packetDown.reserved = 0;
		}

		packetDown.node = query.value("nodeId").toInt();
		++count;
		// 每个结点的7个波形
		for (int i = 1; i <= 7; ++i)
		{
			// 组包发送
			QString waveName = query.value(i).toString();
			packetDown.wave = i;//id;
			// 填充波形数据
			loadWaveData(waveName.append("_detail.xml"), packetDown);

			QByteArray block;
			QDataStream out(&block, QIODevice::WriteOnly);
			out.setVersion(QDataStream::Qt_4_0);
			out.writeRawData((char*)&packetDown, sizeof(packetDown));

			sock.write(block);
			sock.flush();

			//sleep(1);
			msleep(10);
		
		}
		emit dispatchReport(5*count); // 更新进度条
		
	}
		//if ( i == 7 ) {
				packetDown_end.type = 0x03;
				packetDown_end.cmd = 0x03;
				packetDown_end.parameter1 = 0x14;
				packetDown_end.parameter2 = 0x07;
				QByteArray block3;
				QDataStream out3(&block3, QIODevice::ReadWrite);
				out3.setVersion(QDataStream::Qt_4_0);
				out3.writeRawData((char*)&packetDown_end, sizeof(packetDown_end));
				sock.write(block3);
				sock.flush();
		//	}
}