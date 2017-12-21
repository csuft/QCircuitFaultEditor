#include "DBUtils.h"

DBUtils::DBUtils()
{
}


DBUtils::~DBUtils(void)
{
}

void DBUtils::createDatabase()
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QSqlDatabase dbconn=QSqlDatabase::addDatabase("QSQLITE");
	dbconn.setDatabaseName("circuit.db");
	if(!dbconn.open()){
		CMessageBox msgBox;
		msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("数据库初始化失败！"), QPixmap(":/pics/warning"));
		msgBox.exec();
		return ;
	}
	QFileInfo file("circuit.db");
	// 如果该数据库文件不存在，创建该数据库文件，防止重复创建。
	if (!file.exists() || !file.size())
	{
		// create all tables.
		QSqlQuery query;
		query.exec("CREATE TABLE Nodes(record INTEGER PRIMARY KEY AUTOINCREMENT, nodeId int, wavea varchar(32), waveb varchar(32), wavec varchar(32), waved varchar(32), wavee varchar(32), wavef varchar(32), waveg varchar(32), desc varchar(64), solutionName varchar(64))");
		query.exec("CREATE TABLE Waves(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(32), desc varchar(128))");
		query.exec("CREATE TABLE Solutions(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(32), desc varchar(128))");
	}
}