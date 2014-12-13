#include "nofocusdelegate.h"

NoFocusDelegate::NoFocusDelegate(QObject *parent)
	: QSqlRelationalDelegate(parent)
{
}

NoFocusDelegate::~NoFocusDelegate()
{

}
/*
 * Reimplemented the virtual function QStyledItemDelegate::paint()
 */
void NoFocusDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem itemOption(option);
	// remove the focus state
	if (itemOption.state & QStyle::State_HasFocus)
	{
		itemOption.state ^= QStyle::State_HasFocus;
	}
	QSqlRelationalDelegate::paint(painter, itemOption, index);
}

QWidget* NoFocusDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return 0;
}