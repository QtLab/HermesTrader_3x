#ifndef SMATHREAD_H
#define SMATHREAD_H

#include <QtGui>

using namespace std;

class SMAThread	:	public QThread
{
public:
								SMAThread();																		// �R���X�g���N�^
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// �T���v����
};

#endif // SMATHREAD_H
