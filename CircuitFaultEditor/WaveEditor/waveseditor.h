#ifndef WAVESEDITOR_H
#define WAVESEDITOR_H

#include <QWidget>
#include "shadewidget.h"
#include "../3rdparty/qcustomplot.h"

class WavesEditor : public QWidget
{
    Q_OBJECT

public:
    WavesEditor(QWidget *parent = NULL, bool flag = false);
    void setGradientStops(const QGradientStops &stops);
    ShadeWidget *m_green_shade;

public slots:
    void pointsUpdated();

signals:
    void gradientStopsChanged(const QGradientStops &stops);

private:
	QCustomPlot* m_plot;
};

#endif // WAVESEDITOR_H
