#ifndef CUSTOMITEMMODEL_H
#define CUSTOMITEMMODEL_H

#include <QSqlTableModel>
#include <QVariant>
#include <QVector>

class CustomItemModel : public QSqlTableModel
{
	Q_OBJECT

public:
	explicit CustomItemModel(QObject *parent = NULL){}
	CustomItemModel(QVector<int>& colsAlignCenter, QVector<int>& colsAlignRight, QObject* parent);
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	~CustomItemModel(){}

private:
	QVector<int> m_columnsAlignRight;
	QVector<int> m_columnsAlignCenter;
};

#endif // CUSTOMITEMMODEL_H
