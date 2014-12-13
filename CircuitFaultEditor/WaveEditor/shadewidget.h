#ifndef SHADEWIDGET_H
#define SHADEWIDGET_H

#include <QWidget>
#include "hoverpoints.h"

class ShadeWidget : public QWidget
{
    Q_OBJECT

public:
    ShadeWidget(QWidget *parent = NULL);
    void setGradientStops(const QGradientStops &stops);
    void paintEvent(QPaintEvent *e);
    QSize sizeHint() const { return QSize(150, 40); }
	QPolygonF points() const { return m_hoverPoints->points();}
    HoverPoints *hoverPoints() const { return m_hoverPoints; }
    uint colorAt(int x);

signals:
    void colorsChanged();
	void positionChanged(int);

private:
    void generateShade();
    QImage m_shade;
    HoverPoints *m_hoverPoints;   // ¹Ø¼üµã
    QLinearGradient m_alpha_gradient;
};

#endif // SHADEWIDGET_H
