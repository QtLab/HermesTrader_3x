#ifndef PLOT_H
#define PLOT_H

#include "qwt/qwt_plot.h"
#include "qwt/qwt_plot_grid.h"

class Plot : public QwtPlot
{
	Q_OBJECT
public:
	explicit					Plot(QWidget *parent = 0);

signals:

public slots:

private:
};

extern Plot*					gpPlot;																				// Plot�̃O���[�o���|�C���^

#endif // PLOT_H
