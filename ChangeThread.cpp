#include "ChangeThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
ChangeThread::ChangeThread()
{
}

/**
 *	�X���b�h�������s
 */
void ChangeThread::run()
{
	Calcuration();
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 */
void ChangeThread::ReadParam()
{
}

/**
 *	�v�Z�����DB�\�z
 */
void ChangeThread::Calcuration()
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <PliceData> pliceData;	// ���n��f�[�^
	double change_open;				// �O����(Open)
	double change_high;				// �O����(High)
	double change_low;				// �O����(Low)
	double change_close;			// �O����(Close)
	double change_volume;			// �O����(Volume)
	double change_aClose;			// �O����(AClose)
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("DB\\Change");

	// �eID�ɂ��ď������s
	for (int i = 1000; i < 1000000; i++) {
		if (((i > 10000) && !((i == 998405) || (i == 998407)))) {
			continue;
		}

		// �ϐ�������
		pliceData.clear();

		// �ǂݍ��ݗp�t�@�C���I�[�v��
		readFileName = "Yahoo\\" + temp_1.setNum(i) + ".csv";
		readFile.setFileName(readFileName);
		if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			QTextStream readFileReader(&readFile);
			readFileLine = "";

			// ���n��f�[�^�̓ǂݎ��
			while (!readFileReader.atEnd()) {
				readFileLine = readFileReader.readLine();
				// �f�[�^�擾
				if (readFileLine != "") {
					tempData.mDate		= QDate::fromString((readFileLine.split(",").at(0)), "yyyy/M/d");
					tempData.mOpen		= readFileLine.split(",").at(1).toDouble();
					tempData.mHigh		= readFileLine.split(",").at(2).toDouble();
					tempData.mLow		= readFileLine.split(",").at(3).toDouble();
					tempData.mClose		= readFileLine.split(",").at(4).toDouble();
					tempData.mVolume	= readFileLine.split(",").at(5).toDouble();
					tempData.mAClose	= readFileLine.split(",").at(6).toDouble();
					pliceData.push_back(tempData);
				}
			}

			// �������ݗp�t�@�C���I�[�v��
			writeFileName = "DB\\Change\\" + temp_1.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = 1; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// �O����v�Z
				change_open		= (pliceData[j].mOpen - pliceData[j-1].mOpen) / pliceData[j-1].mOpen;
				change_high		= (pliceData[j].mHigh - pliceData[j-1].mHigh) / pliceData[j-1].mHigh;
				change_low		= (pliceData[j].mLow - pliceData[j-1].mLow) / pliceData[j-1].mLow;
				change_close	= (pliceData[j].mClose - pliceData[j-1].mClose) / pliceData[j-1].mClose;
				change_volume	= (pliceData[j].mVolume - pliceData[j-1].mVolume) / pliceData[j-1].mVolume;
				change_aClose	= (pliceData[j].mAClose - pliceData[j-1].mAClose) / pliceData[j-1].mAClose;

				// �O����̏�������
				out << (pliceData[j].mDate.toString("yyyy/M/d")) << ","
					<< change_open << ","
					<< change_high << ","
					<< change_low << ","
					<< change_close << ","
					<< change_volume << ","
					<< change_aClose << "\n";
			}

			writeFile.close();
		}

		readFile.close();
	}
}
