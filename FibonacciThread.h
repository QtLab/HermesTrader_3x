#ifndef FIBONACCITHREAD_H
#define FIBONACCITHREAD_H

#include <QtGui>

using namespace std;

class FibonacciThread	:	public QThread
{
public:
								FibonacciThread();
	void						run();																				// �X���b�h�����̎��s
	void						ReadParam(int n);																	// �p�����[�^�ǂݍ���
	void						Calcuration(int n);																	// �v�Z

	// �A���S���Y���Ŏg�p����p�����[�^
	int							mN;																					// �T���v����
};

#endif // FIBONACCITHREAD_H
