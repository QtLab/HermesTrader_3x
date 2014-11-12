#include "Target.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <direct.h>

/**
 *	�R���X�g���N�^
 */
Target::Target()
{
}

/**
 *	TargetRate�t�@�C���̍쐬
 */
void Target::Restruct(QString rootPath)
{
	// �ϐ��錾
	QString csvFileName;			// CSV�t�@�C����
	QString rateFileName;			// rate�t�@�C����
	QString readLine;				// text���[�_�[

	// rate�f�B���N�g������
	_mkdir(rootPath.toLocal8Bit() + "TargetRate\\");

	for (int i = 1000; i < 1000000; i++) {
		if (((i > 10000) && !((i == 998405) || (i == 998407)))) {
			continue;
		}

		// �ǂݍ��ݗp�t�@�C���I�[�v��
		csvFileName = rootPath + "\\TargetDB\\" + mTemp1.setNum(i) + ".csv";
		QFile csvFile(csvFileName);
		if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			QTextStream readCsv(&csvFile);
			readLine = "";
			mDate.clear();
			mClose.clear();

			// csv�t�@�C���̓ǂݎ��
			while (!readCsv.atEnd()) {
				readLine = readCsv.readLine();
				// �e�J�����̃f�[�^�擾
				mDate.push_back(QDate::fromString((readLine.split(",").at(0)), "M/d/yyyy"));
				mClose.push_back(readLine.split(",").at(4).toDouble());
			}

			// �������ݗp�t�@�C���I�[�v��
			rateFileName = rootPath + "TargetRate\\" + mTemp1.setNum(i) + ".rate";
			QFile rateFile(rateFileName);
			if (!rateFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int i = 2; i < (int)mClose.size(); i++) {
				QTextStream out(&rateFile);
				// �������v�Z���ʂ̏�������
				out << (mDate[i].toString("yyyy/MM/dd")) << "," << ((mClose[i] - mClose[i-1])/mClose[i-1]) << "\n";
			}

			rateFile.close();
		}

		csvFile.close();
	}
}
