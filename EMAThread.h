#ifndef EMATHREAD_H
#define EMATHREAD_H

#include <QtGui>

using namespace std;

class EMAThread	:	public QThread
{
public:
								EMAThread();																		// �R���X�g���N�^
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// �T���v����
};

#endif // EMATHREAD_H
