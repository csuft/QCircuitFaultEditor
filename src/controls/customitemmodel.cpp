#include "customitemmodel.h"

CustomItemModel::CustomItemModel(QVector<int>& colsAlignCenter, QVector<int>& colsAlignRight, QObject* parent)
	: QSqlTableModel(parent), m_columnsAlignCenter(colsAlignCenter), m_columnsAlignRight(colsAlignRight)
{
	// Nothing to do

}
/*
 * Since the text alignment defaults to Qt::AlignLeft, we only need to reimplement the other
 * two alignments.
 */
QVariant CustomItemModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::TextAlignmentRole)
	{
		if (m_columnsAlignCenter.contains(index.column()))
		{
			return Qt::AlignCenter;
		}
		if (m_columnsAlignRight.contains(index.column()))
		{
			return Qt::AlignRight;
		}
	}

	return QSqlTableModel::data(index, role);
}