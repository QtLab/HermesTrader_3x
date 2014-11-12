#include <QtGui/QApplication>
#include "WndMain.h"
#include "Plot.h"
#include "Predict.h"
#include "Target.h"
#include "Template.h"

// �v���g�^�C�v�錾
void DeleteAll();

// �e��N���X�̃O���[�o���C���X�^���X�|�C���^
WndMain*	gpWndMain	= NULL;			// ���C���t�H�[��
Plot*		gpPlot		= NULL;			// �v���b�g�N���X
Predict*	gpPredict	= NULL;			// �\���v�Z�����N���X
Target*		gpTarget	= NULL;			// �^�[�Q�b�g�f�[�^�����N���X
Template*	gpTemplate	= NULL;			// �e���v���[�g�f�[�^�����N���X

/**
 *  ���C�����[�`��
 *
 *  @param[in] argc �R�}���h���C�������̐�
 *  @param[in] argv �R�}���h���C�������z��ւ̃|�C���^
 *
 *  @retval int �I���R�[�h
 */
int main(int argc, char *argv[])
{
	// �A�v���P�[�V�����N���X�C���X�^���X����
	QApplication* pApp = new QApplication(argc, argv);

	// �e��N���X�̃O���[�o���C���X�^���X�̐���
	gpWndMain	= new WndMain();
	gpTarget	= new Target();
	gpTemplate	= new Template();
	gpPredict	= new Predict();

	// �\��
	gpWndMain->show();

	// �A�v���P�[�V�������s
	int iResult = pApp->exec();							// ���s

	// �I������
	DeleteAll();										// �O���[�o���C���X�^���X�j��
	delete pApp;
	return iResult;										// �I��
}

/**
 *  �e��N���X�̃O���[�o���C���X�^���X�̔j��
 */
void DeleteAll()
{
	delete gpWndMain;
	delete gpPlot;
	delete gpPredict;
	delete gpTarget;
	delete gpTemplate;
}
