#ifndef RECORDEDITOR_H
#define RECORDEDITOR_H

#include <QHBoxLayout>
#include <QFormLayout>
#include <QSqlTableModel>
#include <QLabel>
#include <QComboBox>

#include "shadowwindow.h"
#include "custompushbutton.h"

class RecordEditor : public ShadowWindow
{
	Q_OBJECT

public:
	RecordEditor(QString& a, QString& b, QString& c, QString& d, QString& e, QString& f, QString& g, QWidget *parent = NULL);
	~RecordEditor();

	const QList<QString> getWavesList() const { return m_list; } 
private slots:
	void onSaveClicked();

protected:
	void paintEvent(QPaintEvent *event);

private:
	//////////////////////////////////////////////////////////////////////////
	CustomPushButton* m_miniBtn;
	CustomPushButton* m_closeBtn;
	QLabel* m_title;

	//////////////////////////////////////////////////////////////////////////
	QComboBox* m_wavea;
	QComboBox* m_waveb;
	QComboBox* m_wavec;
	QComboBox* m_waved;
	QComboBox* m_wavee;
	QComboBox* m_wavef;
	QComboBox* m_waveg;

	QPushButton* m_saveBtn;
	QPushButton* m_CancelBtn;

	QSqlTableModel* m_wavesModel;
	QList<QString> m_list;
};

#endif // RECORDEDITOR_H
