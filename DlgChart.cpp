#include "DlgChart.h"
#include "ui_DlgChart.h"

/**
 *	�R���X�g���N�^
 */
DlgChart::DlgChart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgChart)
{
    ui->setupUi(this);

	// BestCase�v���b�g
	mPlicePlotCurve = new QwtPlotCurve("Plot");
	mPlicePlotCurve->setPen((QPen(Qt::yellow, 1)));

	// UI������
	ui->tblChartData->horizontalHeader()->setVisible(true);
}

/**
 *	�f�X�g���N�^
 */
DlgChart::~DlgChart()
{
	delete mPlicePlotCurve;
    delete ui;
}

/**
 *	changeEvent()
 *
 *	@param[in] e �󂯎�����C�x���g
 */
void DlgChart::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 *	���n��CSV�t�@�C����ǂݍ���Ńf�[�^�z��Ƀf�[�^���i�[����֐�
 */
void DlgChart::ReadPliceCSV(QString filePath)
{
	// �ϐ��錾
	//int startDate;

	// �t�@�C���ǂݍ���
	QFile readCsvFile(filePath);
	if (!readCsvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}

	// �t�@�C���s�ǂݍ��ݗp�ϐ��̐錾����єz��̏�����
	QTextStream csvFileReader(&readCsvFile);
	QString csvFileLine;
	PliceData tempData;
	mPliceData.clear();

	// �t�@�C���s����s���ǂݍ��݃f�[�^�z��Ƀf�[�^���i�[����
	while (!csvFileReader.atEnd()) {
		csvFileLine				= csvFileReader.readLine();
		if (csvFileLine != "") {
			tempData.mDate			= QDate::fromString((csvFileLine.split(",").at(0)), "yyyy/M/d");
			tempData.mDateJulian	= QDate::fromString((csvFileLine.split(",").at(0)), "yyyy/M/d").toJulianDay();
			tempData.mOpen			= csvFileLine.split(",").at(1).toDouble();
			tempData.mHigh			= csvFileLine.split(",").at(2).toDouble();
			tempData.mLow			= csvFileLine.split(",").at(3).toDouble();
			tempData.mClose			= csvFileLine.split(",").at(4).toDouble();
			tempData.mVolume		= csvFileLine.split(",").at(5).toDouble();
			tempData.mAClose		= csvFileLine.split(",").at(6).toDouble();
			mPliceData.push_back(tempData);
		}
	}

	QString temp1, temp2, temp3, temp4, temp5, temp6;
	for (int i = 0; i < (int)mPliceData.size(); i++) {
		gpWndMain->AddPliceData(ui->tblChartData, mPliceData[i].mDate.toString("yyyy/M/d"), temp1.setNum(mPliceData[i].mOpen), temp2.setNum(mPliceData[i].mHigh), temp3.setNum(mPliceData[i].mLow), temp4.setNum(mPliceData[i].mClose), temp5.setNum(mPliceData[i].mVolume), temp6.setNum(mPliceData[i].mAClose));
	}
}

/**
 *	���n��f�[�^�O���t�`��֐�
 */
void DlgChart::DrawPliceCSVPlot()
{
	if (mPliceData.size() == 0) {
		return;
	}

	// �O���t�`��p�f�[�^�z��錾�Ə�����
	vector <double> dataJulian;
	vector <double> dataClose;
	dataJulian.clear();
	dataClose.clear();

	// �ϐ��錾
	//int startDate;

	// �O���t�`��p��PliceData�z��̊e�f�[�^��ǂݍ��ށi��ԏ�̍s�̓��x��������ǂݍ��܂Ȃ��j
	for (int i = 0; i < (int)mPliceData.size(); i++) {
		dataJulian.push_back(mPliceData[i].mDateJulian);			// �����E�X��̓ǂݍ���
		dataClose.push_back(mPliceData[i].mClose);					// �I�l�̓ǂݍ���
	}

	// �O���t�`��
	mPlicePlotCurve->setSamples(&dataJulian[0], &dataClose[0], mPliceData.size());
	mPlicePlotCurve->attach(ui->qwtpltChart);

	// �O���t�G���A�̐ݒ�
	ui->qwtpltChart->setAxisTitle(QwtPlot::xBottom, "Date");
	ui->qwtpltChart->setAxisTitle(QwtPlot::yLeft, "Close");
	ui->qwtpltChart->setAxisScaleDraw(QwtPlot::xBottom, new DateScaleDraw(gpWndMain->upTime()));
	ui->qwtpltChart->setAxisLabelRotation(QwtPlot::xBottom, -50.0);
	ui->qwtpltChart->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
	ui->qwtpltChart->replot();
}
