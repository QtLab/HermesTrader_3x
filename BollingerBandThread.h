#ifndef BOLLINGERBANDTHREAD_H
#define BOLLINGERBANDTHREAD_H

#include <QtGui>

using namespace std;

class BollingerBandThread	:	public QThread
{
public:
								BollingerBandThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// �T���v����
};

#endif // BOLLINGERBANDTHREAD_H
