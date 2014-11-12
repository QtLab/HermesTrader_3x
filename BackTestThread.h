#ifndef BACKTESTTHREAD_H
#define BACKTESTTHREAD_H

#include <QtGui>

using namespace std;

// BackTest1�N���X
class BackTest {
public:
	QDate						mDate;																				// ���t
	double						mEMoney;																			// �]���z
	double						mMoney;																				// ����
	bool						mBuySignal;																			// �����V�O�i��
	bool						mSellSignal;																		// ����V�O�i��
	int							mBuyNum;																			// ��������
	int							mSellNum;																			// ���芔��
	double						mBuyPlice;																			// �������i
	double						mSellPlice;																			// ���艿�i
	double						mOpen;																				// �n�l
	double						mHigh;																				// ���l
	double						mLow;																				// ���l
	double						mClose;																				// �I�l
	double						mVolume;																			// �o����
	double						mAClose;																			// ������I�l
	double						mSMAR_close;																		// SMA(�I�l)������
	double						mTHigh;																				// ���ԍ��l
	double						mTLow;																				// ���Ԉ��l
};

class BackTestThread	:	public QThread
{
public:
								BackTestThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int thresh, double money, int maN, int tN);								// �p�����[�^�ǂݍ���
	void						Calcuration(int thresh, double money, int maN, int tN);								// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mThresh;																			// 臒l
	int							mMoney;																				// ��������
	int							mMaN;																				// �ړ����ς̃T���v����
	int							mTN;																				// ���ԍ��l�A���Ԉ��l�̊���
};

#endif // BACKTESTTHREAD_H
