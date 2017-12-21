#ifndef DBUTILS_H
#define DBUTILS_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>
#include <QFileInfo>
#include "../controls/msgbox.h"

class DBUtils
{
public:
	DBUtils(void);
	~DBUtils(void);
	static void createDatabase();
};

#endif


