#ifndef WMATHREAD_H
#define WMATHREAD_H

#include <QtGui>

using namespace std;

class WMAThread	:	public QThread
{
public:
								WMAThread();																		// �R���X�g���N�^
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// �T���v����
};

#endif // WMATHREAD_H
