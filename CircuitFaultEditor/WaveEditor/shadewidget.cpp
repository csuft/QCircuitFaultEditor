#include "shadewidget.h"

ShadeWidget::ShadeWidget(QWidget *parent)
    : QWidget(parent), m_alpha_gradient(QLinearGradient(0, 0, 0, 0))
{
    setAttribute(Qt::WA_NoBackground);
    QPolygonF points;
    points << QPointF(0, sizeHint().height()) << QPointF(sizeHint().width(), 0);

    m_hoverPoints = new HoverPoints(this, HoverPoints::CircleShape);
    m_hoverPoints->setPoints(points);
    m_hoverPoints->setPointLock(0, HoverPoints::LockToLeft);
    m_hoverPoints->setPointLock(1, HoverPoints::LockToRight);
    m_hoverPoints->setSortType(HoverPoints::XSort);

    connect(m_hoverPoints, SIGNAL(pointsChanged(QPolygonF)), this, SIGNAL(colorsChanged()));
	connect(m_hoverPoints, SIGNAL(positionChanged(int)), this, SIGNAL(positionChanged(int)));
}

uint ShadeWidget::colorAt(int x)
{
    generateShade();

    QPolygonF pts = m_hoverPoints->points();
    for (int i=1; i < pts.size(); ++i) 
	{
        if (pts.at(i-1).x() <= x && pts.at(i).x() >= x)
		{
            QLineF l(pts.at(i-1), pts.at(i));
            l.setLength(l.length() * ((x - l.x1()) / l.dx()));
            return m_shade.pixel(qRound(qMin(l.x2(), (qreal(m_shade.width() - 1)))),
                                 qRound(qMin(l.y2(), qreal(m_shade.height() - 1))));
        }
    }
    return 0;
}

void ShadeWidget::setGradientStops(const QGradientStops &stops)
{
	m_alpha_gradient = QLinearGradient(0, 0, width(), 0);

	for (int i=0; i<stops.size(); ++i) 
	{
		QColor c = stops.at(i).second;
		m_alpha_gradient.setColorAt(stops.at(i).first, QColor(c.red(), c.green(), c.blue()));
	}

	m_shade = QImage();
	generateShade();
	update();
}

void ShadeWidget::paintEvent(QPaintEvent *)
{
    generateShade();

    QPainter p(this);
    p.drawImage(0, 0, m_shade);
    p.setPen(QColor(146, 146, 146));
    p.drawRect(0, 0, width() - 1, height() - 1);
}

void ShadeWidget::generateShade()
{
    if (m_shade.isNull() || m_shade.size() != size()) 
	{
       m_shade = QImage(size(), QImage::Format_RGB32);
       QLinearGradient shade(0, 0, 0, 0);
       shade.setColorAt(1, Qt::black);
       shade.setColorAt(0, QColor(90, 168, 45));
       QPainter p(&m_shade);
       p.fillRect(rect(), shade);
    }
}
