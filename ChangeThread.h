#ifndef CHANGETHREAD_H
#define CHANGETHREAD_H

#include <QtGui>

using namespace std;

class ChangeThread	:	public QThread
{
public:
								ChangeThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam();																		// �p�����[�^�ǂݍ���
	void						Calcuration();																		// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// �T���v����
};

#endif // CHANGETHREAD_H
