#ifndef TERMLOWTHREAD_H
#define TERMLOWTHREAD_H

#include <QtGui>

using namespace std;

class TermLowThread	:	public QThread
{
public:
								TermLowThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// ����
};

#endif // TERMLOWTHREAD_H
