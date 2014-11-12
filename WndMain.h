#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>
#include <QDate>
#include "qwt/qwt_plot.h"
#include "qwt/qwt_plot_curve.h"
#include "qwt/qwt_scale_draw.h"
#include <QtGui>
#include "SMAThread.h"
#include "EMAThread.h"
#include "RMAThread.h"
#include "WMAThread.h"
#include "ChangeThread.h"
#include "TermHighThread.h"
#include "TermLowThread.h"
#include "IchimokuThread.h"
#include "BollingerBandThread.h"
#include "FibonacciThread.h"
#include "BackTestThread.h"
#include "SimulationThread.h"

#define			PLOT_DATA_SIZE		20

using namespace std;

namespace Ui {
	class						WndMain;
}

// IndexData�N���X
class IndexData {
public:
	QString						mID;
	QString						mMarket;
	QString						mName;
	QString						mKind;
	double						mOpen;
	double						mHigh;
	double						mLow;
	double						mClose;
	double						mVolume;
	double						mAClose;
	double						mRate;
};

// PliceData�N���X
class PliceData {
public:
	QDate						mDate;
	double						mDateJulian;
	double						mOpen;
	double						mHigh;
	double						mLow;
	double						mClose;
	double						mVolume;
	double						mAClose;
};

// FilteredScoreData�N���X
class ScoreData {
public:
	int							mTargetID;
	int							mTemplateID;
	QDate						mDate;
	double						mScore;
};

// DateScaleDraw�N���X
class DateScaleDraw: public QwtScaleDraw
{
public:
								DateScaleDraw(const QDate &base):
								baseTime(base)
								{
								}

	virtual QwtText				label(double v) const
								{
									return baseTime.fromJulianDay((int)v).toString("yyyy/MM/dd");
								}
private:
	QDate						baseTime;
};

class WndMain : public QMainWindow
{
	Q_OBJECT

public:
	explicit					WndMain(QWidget *parent = 0);
								~WndMain();
	QDate						upTime() const;																		// �O���t�`��p���t�f�[�^�ϊ��֐�
	void						AddPliceData(QTableWidget *pTableWidget, QString date, QString open, QString high,
											 QString low, QString close, QString volume, QString aClose);			// Target���X�g/Template���X�g�Ƀf�[�^��ǉ�����֐�
	SMAThread*					mpSMAThread;																		// SMA�X���b�h
	EMAThread*					mpEMAThread;																		// EMA�X���b�h
	RMAThread*					mpRMAThread;																		// RMA�X���b�h
	WMAThread*					mpWMAThread;																		// WMA�X���b�h
	ChangeThread*				mpChangeThread;																		// �O����X���b�h
	TermHighThread*				mpTermHighThread;																	// ���ԍ��l�X���b�h
	TermLowThread*				mpTermLowThread;																	// ���Ԉ��l�X���b�h
	IchimokuThread*				mpIchimokuThread;																	// ��ڋύt�\�X���b�h
	BollingerBandThread*		mpBollingerBandThread;																// �{�����W���[�o���h�X���b�h
	FibonacciThread*			mpFibonacciThread;																	// �t�B�{�i�b�`�X���b�h
	BackTestThread*				mpBackTestThread;																	// �o�b�N�e�X�g�X���b�h
	SimulationThread*			mpSimulationThread;																	// �V�~�����[�V�����X���b�h

protected:
	void						changeEvent(QEvent *e);
	void						dragEnterEvent(QDragEnterEvent *);
	void						dropEvent(QDropEvent *);
	void						AddIndexData(QTableWidget *pTableWidget, QString id, QString market,
											 QString name, QString kind, QString open, QString high, QString low,
											 QString close, QString volume, QString aClose, QString rate);			// �������X�g�Ƀf�[�^��ǉ�����֐�
	void						AddScoreData(QTableWidget *pTableWidget, QString targetID,
											 QString templateID, QString date, QString score);						// Score���X�g�Ƀf�[�^��ǉ�����֐�
	void						AddPredictionData(QTableWidget *pTableWidget, QString day, QString bestCaseClose,
												  QString worstCaseClose, QString centerCaseClose);					// Prediction���X�g�Ƀf�[�^��ǉ�����֐�
	void						ReadIndexCSV(QString filePath);														// IndexCSV�t�@�C����ǂݍ���Ńf�[�^�z��Ƀf�[�^���i�[����֐�
	void						ReadTargetCSV(QString filePath, int fragmentLength);								// TargetCSV�t�@�C����ǂݍ���Ńf�[�^�z��Ƀf�[�^���i�[����֐�
	void						ReadTemplateCSV(QString filePath, int pltLength, QDate date);						// TemplateCSV�t�@�C����ǂݍ���Ńf�[�^�z��Ƀf�[�^���i�[����֐�
	void						ReadScore(QString filePath);														// FilteredScore�t�@�C����ǂݍ���Ńf�[�^�z��Ƀf�[�^���i�[����֐�
	void						PredictionTableMapping();															// Prediction��̃e�[�u�����f�֐�
	void						DrawTargetCSVPlot(int fragmentLength, int start);									// Target�f�[�^�O���t�`��֐�
	void						DrawTemplateCSVPlot(int pltLength, QDate date);										// Template�f�[�^�O���t�`��֐�
	void						DrawBestCasePlot();																	// BestCase�f�[�^�O���t�`��֐�
	void						DrawWorstCasePlot();																// WorstCase�f�[�^�O���t�`��֐�
	void						DrawCenterCasePlot();																// CenterCase�f�[�^�O���t�`��֐�

private slots:
	void						on_mnuOpen_triggered();																// [Open]�{�^����clicked()�V�O�i�����󂯎��X���b�g
	void						on_mnuTemplateRestruct_triggered();													// [FA]->[Template]->[Restruct]���j���[��triggered()�V�O�i�����󂯎��X���b�g
	void						on_mnuTargetRestruct_triggered();													// [FA]->[Target]->[Restruct]���j���[��triggered()�V�O�i�����󂯎��X���b�g
	void						on_mnuCalculateScore_triggered();													// [FA]->[Predict]->[CalculateScore]���j���[��triggered()�V�O�i�����󂯎��X���b�g
	void						on_btnCalculation_clicked();														// [ScoreCalculation]�{�^����clicked()�V�O�i�����󂯎��X���b�g
	void						on_btnDrawPlot_clicked();															// [DrawPlot]�{�^����clicked()�V�O�i�����󂯎��X���b�g
	void						on_btnPrediction_clicked();															// [Prediction]�{�^����clicked()�V�O�i�����󂯎��X���b�g
	void						on_tblIndexData_itemDoubleClicked(QTableWidgetItem *item);							// �������X�g��Ń_�u���N���b�N����ƃ`���[�g�_�C�A���O�\��

	void on_btnDB_clicked();
	void on_btnBackTest_clicked();
	void on_btnSimulation_clicked();

private:
	Ui::WndMain					*ui;
	QFileInfo					msOpenFile;																			// �ǂݍ��݃t�@�C��
	QString						mRootPath;																			// ���[�g�p�X
	QString						mScoreDirName;																		// �X�R�A�f�B���N�g����
	QwtPlotCurve*				mTargetPlotCurve;																	// Target�O���t�`��|�C���^
	QwtPlotCurve*				mTemplatePlotCurve;																	// Template�O���t�`��|�C���^
	QwtPlotCurve*				mBestCasePlotCurve;																	// BestCase�O���t�`��|�C���^
	QwtPlotCurve*				mWorstCasePlotCurve;																// WorstCase�O���t�`��|�C���^
	QwtPlotCurve*				mCenterCasePlotCurve;																// CenterCase�O���t�`��|�C���^
	vector <IndexData>			mIndexData;																			// Index�f�[�^�z��
	vector <PliceData>			mTargetPliceData;																	// Target���i�f�[�^�z��
	vector <PliceData>			mTemplatePliceData;																	// Template���i�f�[�^�z��
	vector <ScoreData>			mScoreData;																			// FilteredScore�z��
	QString						mTemp1;
};

extern WndMain*					gpWndMain;																			// WndMain�̃O���[�o���|�C���^

#endif // WNDMAIN_H
