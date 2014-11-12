#ifndef PREDICT_H
#define PREDICT_H

#include <QtGui>
#include <QDate>

using namespace std;

// TemplateScoreData�N���X
class TemplateScoreData {
public:
	int							mID;																				// ID(�t�@�C���ԍ�)
	double						mScore;																				// �X�R�A
	QDate						mOriginDate;																		// �t���O�����g�A�Z���u���J�n��
	QDate						mStartDate;																			// �\���J�n��(=�t���O�����g�A�Z���u���I����)
	double						mStartOpen;																			// �J�n���̎n�l
	double						mStartHigh;																			// �J�n���̍ō��l
	double						mStartLow;																			// �J�n���̍ň��l
	double						mStartClose;																		// �J�n���̏I�l
	double						mStartVolume;																		// �J�n���̃{�����e�B
	double						mStartAClose;																		// �J�n���̒�����I�l
	QDate						mEndDate;																			// �\���I����
	double						mEndOpen;																			// �I�����̎n�l
	double						mEndHigh;																			// �I�����̍ō��l
	double						mEndLow;																			// �I�����̍ň��l
	double						mEndClose;																			// �I�����̏I�l
	double						mEndVolume;																			// �I�����̃{�����e�B
	double						mEndAClose;																		// �I�����̒�����I�l
	double						mPrediction;																		// �\��������(�I�l-�O���I�l)
};

// PliceData�N���X
class CasePliceData {
public:
	QDate						mDate;
	double						mDateJulian;
	double						mOpen;
	double						mHigh;
	double						mLow;
	double						mClose;
	double						mVolume;
	double						mAClose;
	double						mStartClose;																		// �J�n���̏I�l
	double						mEndClose;																			// �I�����̏I�l
	double						mPrediction;																		// �\��������
};

// ScreeningData�N���X
class ScreeningData {
public:
	int							mID;
	double						mMaxPredictionRate;
	double						mMinPredictionRate;
	double						mCenterPredictionRate;
	double						mAveragePredictionRate;
	double						mUpProbability;
	double						mDownProbability;
	double						mSameProbability;
};

class Predict
{
public:
								Predict();
	void						CalculateScore(QString rootPath, int fragmentLength, double minScore, double minS, int start);	// �X�R�A�v�Z
	void						Prediction(QString rootPath, int targetID, int fragmentLength, int predictLength, int start);	// �\��
	void						Screening(QString rootPath, int start);															// �\�����ʂ̃X�N���[�j���O
	vector <CasePliceData>		mBestCasePliceData;																				// BestCase���i�f�[�^�z��
	vector <CasePliceData>		mWorstCasePliceData;																			// WorstCase���i�f�[�^�z��
	vector <CasePliceData>		mCenterCasePliceData;																			// CenterCase���i�f�[�^�z��

private:
	vector <QDate>				mTargetDate;
	vector <QDate>				mTemplateDate;
	vector <double>				mTargetRate;
	vector <double>				mTemplateRate;
	vector <double>				mCurrentTargetRate;
	vector <double>				mCurrentTemplateRate;
	vector <double>				mScore;
	vector <TemplateScoreData>	mTemplateScoreData;
	vector <ScreeningData>		mScreeningData;
	QString						mTemp1;
	QString						mTemp2;
	QString						mTemp3;
};

// TemplateScoreData�̗\����������r�֐�(�~��)
template <class TemplateScoreData> class comparePrediction:
		public binary_function<TemplateScoreData, TemplateScoreData, bool>
{
public:
	bool operator()(TemplateScoreData a, TemplateScoreData b)
	{
		return a.mPrediction > b.mPrediction;
	}
};

extern Predict*					gpPredict;																			// Predict�̃O���[�o���|�C���^

#endif // PREDICT_H
