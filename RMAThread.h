#ifndef RMATHREAD_H
#define RMATHREAD_H

#include <QtGui>

using namespace std;

class RMAThread	:	public QThread
{
public:
								RMAThread();																		// �R���X�g���N�^
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// �T���v����
};

#endif // RMATHREAD_H
