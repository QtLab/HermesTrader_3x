#ifndef ICHIMOKUTHREAD_H
#define ICHIMOKUTHREAD_H

#include <QtGui>

using namespace std;

class IchimokuThread	:	public QThread
{
public:
								IchimokuThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam();																		// �p�����[�^�ǂݍ���
	void						Calcuration();																		// �v�Z
};

#endif // ICHIMOKUTHREAD_H
