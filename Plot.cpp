#include "Plot.h"
#include <QtGui>

/**
 *	�R���X�g���N�^
 */
Plot::Plot(QWidget *parent) :
	QwtPlot(parent)
{
	// Background
	setCanvasBackground(QColor(Qt::black));

	// Grid
	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
	grid->attach(this);
}
