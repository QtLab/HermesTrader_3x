#ifndef TERMHIGHTHREAD_H
#define TERMHIGHTHREAD_H

#include <QtGui>

using namespace std;

class TermHighThread	:	public QThread
{
public:
								TermHighThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// ����
};

#endif // TERMHIGHTHREAD_H
