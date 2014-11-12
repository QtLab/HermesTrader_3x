#ifndef DLGCHART_H
#define DLGCHART_H

#include <QDialog>
#include "WndMain.h"

namespace Ui {
    class DlgChart;
}

class DlgChart : public QDialog
{
	Q_OBJECT
public:
	explicit					DlgChart(QWidget *parent = 0);														// �R���X�g���N�^
								~DlgChart();																		// �f�X�g���N�^
	void						ReadPliceCSV(QString filePath);														// ���n��CSV�t�@�C����ǂݍ���Ńf�[�^�z��Ƀf�[�^���i�[����֐�
	void						DrawPliceCSVPlot();																	// ���n��f�[�^�O���t�`��֐�

protected:
	void						changeEvent(QEvent *e);																// ChangeEvent

private:
	Ui::DlgChart				*ui;
	vector <PliceData>			mPliceData;																			// ���n��f�[�^�z��
	QwtPlotCurve*				mPlicePlotCurve;																	// ���n��O���t�`��|�C���^
};

#endif // DLGCHART_H
