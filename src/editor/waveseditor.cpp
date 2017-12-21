#include "waveseditor.h"

WavesEditor::WavesEditor(QWidget *parent, bool flag)
    : QWidget(parent)
{

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setSpacing(0);
    vbox->setMargin(1);

	m_plot = new QCustomPlot(this);
	m_green_shade = new ShadeWidget(this);

	m_plot->yAxis->setAutoTickStep(false);
	m_plot->yAxis->setTickStep(5);
	m_plot->yAxis->setRange(-15, 15);
	m_plot->setFixedSize(297, 125);
	m_plot->xAxis->setAutoTickStep(false);
	m_plot->xAxis->setTickStep(128);
	m_plot->xAxis->setRange(0, 1024);
    vbox->addWidget(m_green_shade);
	if (flag)
	{
		vbox->setContentsMargins(36, 14, 14, 34);
	}
	else
	{
		
		vbox->setContentsMargins(36, 14, 14, 16);
	}
	
	setLayout(vbox);
    connect(m_green_shade, SIGNAL(colorsChanged()), this, SLOT(pointsUpdated()));
}

inline static bool x_less_than(const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}

void WavesEditor::pointsUpdated()
{
    QGradientStops stops;
    QPolygonF points;
    points += m_green_shade->points();

    std::sort(points.begin(), points.end(), x_less_than);

    for (int i = 0; i < points.size(); ++i) {
        qreal x = int(points.at(i).x());
        if (i + 1 < points.size() && x == points.at(i + 1).x())
            continue;
        QColor color((0x0000ff00 & m_green_shade->colorAt(int(x))) >> 8);
    }

    emit gradientStopsChanged(stops);
}

static void set_shade_points(const QPolygonF &points, ShadeWidget *shade)
{
    shade->hoverPoints()->setPoints(points);
    shade->hoverPoints()->setPointLock(0, HoverPoints::LockToLeft);
    shade->hoverPoints()->setPointLock(points.size() - 1, HoverPoints::LockToRight);
    shade->update();
}

void WavesEditor::setGradientStops(const QGradientStops &stops)
{
    QPolygonF pts_red, pts_green, pts_blue, pts_alpha;

    qreal h_green = m_green_shade->height();

    for (int i = 0; i < stops.size(); ++i) {
        qreal pos = stops.at(i).first;
        QRgb color = stops.at(i).second.rgba();
        pts_green << QPointF(pos * m_green_shade->width(), h_green - qGreen(color) * h_green / 255.0);
    }

    set_shade_points(pts_green, m_green_shade); 
}