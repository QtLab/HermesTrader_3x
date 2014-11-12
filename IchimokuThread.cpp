#include "IchimokuThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
IchimokuThread::IchimokuThread()
{
}

/**
 *	�X���b�h�������s
 */
void IchimokuThread::run()
{
	Calcuration();
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 */
void IchimokuThread::ReadParam()
{
}

/**
 *	�v�Z�����DB�\�z
 */
void IchimokuThread::Calcuration()
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <PliceData> pliceData;	// ���n��f�[�^
	double ichimoku_base;			// ���
	double ichimoku_conversion;		// �]����
	double ichimoku_early_1[27];	// ��s�X�p��1
	double ichimoku_early_2[27];	// ��s�X�p��2
	double ichimoku_later;			// �x�s�X�p��
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	double max26, min26, max9, min9, max52, min52;

	// �f�B���N�g������
	_mkdir("DB\\Ichimoku");

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
			writeFileName = "DB\\Ichimoku\\" + temp_1.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = 1; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// ����v�Z(�������܂߁A�ߋ�26���Ԃ̍��l�ƈ��l�̒��l)
				if (j > 26) {
					max26 = 0;
					min26 = 10000000;
					for (int k = 0; k < 26; k++) {
						max26 = max(max26, pliceData[j-k].mHigh);		// �ߋ�26���Ԃ̍��l
						min26 = min(min26, pliceData[j-k].mLow);		// �ߋ�26���Ԃ̈��l
					}
					ichimoku_base = (max26 + min26) / 2;
				}
				else {
					ichimoku_base = 0;
				}

				// �]�����v�Z(���̓����܂߁A�ߋ�9���Ԃ̍��l�ƈ��l�̒��l)
				if (j > 9) {
					max9 = 0;
					min9 = 10000000;
					for (int k = 0; k < 9; k++) {
						max9 = max(max9, pliceData[j-k].mHigh);			// �ߋ�9���Ԃ̍��l
						min9 = min(min9, pliceData[j-k].mLow);			// �ߋ�9���Ԃ̈��l
					}
					ichimoku_conversion = (max9 + min9) / 2;
				}
				else {
					ichimoku_conversion = 0;
				}

				// ��s�X�p��1�v�Z(����Ɠ]�����̒��l��26����s�����l)
				ichimoku_early_1[0] = (ichimoku_base + ichimoku_conversion) / 2;
				for (int k = 0; k < 26; k++) {
					ichimoku_early_1[k+1] = ichimoku_early_1[k];
				}

				// ��s�X�p��2�v�Z(�������܂߉ߋ�52���Ԃ̍��l�ƈ��l�̒��l��26����s�����l)
				if (j > 52) {
					max52 = 0;
					min52 = 10000000;
					for (int k = 0; k < 52; k++) {
						max52 = max(max52, pliceData[j-k].mHigh);		// �ߋ�52���Ԃ̍��l
						min52 = min(min52, pliceData[j-k].mLow);		// �ߋ�52���Ԃ̈��l
					}
					ichimoku_early_2[0] = (max52 + min52) / 2;
				}
				else {
					ichimoku_early_2[0] = 0;
				}
				for (int k = 0; k < 26; k++) {
					ichimoku_early_2[k+1] = ichimoku_early_2[k];
				}


				// �x�s�X�p���v�Z(�����̏I�l��26���k�����l)
				if (j < (int)pliceData.size() - 26) {
					ichimoku_later = pliceData[j+26].mClose;
				}
				else {
					ichimoku_later = 0;
				}

				// ��ڋύt�\�̏�������
				out << (pliceData[j].mDate.toString("yyyy/M/d")) << ","
					<< ichimoku_base << ","
					<< ichimoku_conversion << ","
					<< ichimoku_early_1[26] << ","
					<< ichimoku_early_2[26] << ","
					<< ichimoku_later << "\n";
			}

			writeFile.close();
		}

		readFile.close();
	}
}
