#ifndef MOVINGAVERAGETHREAD_H
#define MOVINGAVERAGETHREAD_H

#include <QtGui>

using namespace std;

class MovingAverageThread	:	public QThread
{
public:
								MovingAverageThread();																// �R���X�g���N�^
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(QString alg, int n);													// �ړ����όv�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	QString						mAlg;																				// �A���S���Y��
	int							mN;																					// �T���v����
};

#endif // MOVINGAVERAGETHREAD_H
