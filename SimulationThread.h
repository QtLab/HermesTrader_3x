#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

#include <QtGui>

using namespace std;

class SimulationThread	:	public QThread
{
public:
								SimulationThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int thresh, double money, QDate startDate, QDate endDate);				// �p�����[�^�ǂݍ���
	void						Calcuration(int thresh, double money, QDate startDate, QDate endDate);				// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mThresh;																			// 臒l
	int							mMoney;																				// ��������
	QDate						mStartDate;																			// �J�n��
	QDate						mEndDate;																			// �I����
};

#endif // SIMULATIONTHREAD_H
