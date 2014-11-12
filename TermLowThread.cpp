#include "TermLowThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
TermLowThread::TermLowThread()
{
}

/**
 *	�X���b�h�������s
 */
void TermLowThread::run()
{
	Calcuration(mN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 *
 *	@param[in] n			�T���v����
 */
void TermLowThread::ReadParam(int n)
{
	mN			= n;
}

/**
 *	�v�Z�����DB�\�z
 *
 *	@param[in] n			�T���v����
 */
void TermLowThread::Calcuration(int n)
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <PliceData> pliceData;	// ���n��f�[�^
	double tl_open;					// ���Ԉ��l(Open)
	double tl_high;					// ���Ԉ��l(High)
	double tl_low;					// ���Ԉ��l(Low)
	double tl_close;				// ���Ԉ��l(Close)
	double tl_volume;				// ���Ԉ��l(Volume)
	double tl_aClose;				// ���Ԉ��l(AClose)
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("DB\\TermLow_" + temp_1.setNum(n).toLocal8Bit());

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
			writeFileName = "DB\\TermLow_" + temp_1.setNum(n) + "\\" + temp_2.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = n; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// ���Ԉ��l�v�Z
				tl_open		= 100000000;
				tl_high		= 100000000;
				tl_low		= 100000000;
				tl_close	= 100000000;
				tl_volume	= 100000000;
				tl_aClose	= 100000000;
				for (int k = 0; k < n; k++) {
					tl_open		= min(pliceData[j-k].mOpen, tl_open);
					tl_high		= min(pliceData[j-k].mHigh, tl_high);
					tl_low		= min(pliceData[j-k].mLow, tl_low);
					tl_close	= min(pliceData[j-k].mClose, tl_close);
					tl_volume	= min(pliceData[j-k].mVolume, tl_volume);
					tl_aClose	= min(pliceData[j-k].mAClose, tl_aClose);
				}

				// ���Ԉ��l�̏�������
				out << (pliceData[j].mDate.toString("yyyy/M/d")) << ","
					<< tl_open << ","
					<< tl_high << ","
					<< tl_low << ","
					<< tl_close << ","
					<< tl_volume << ","
					<< tl_aClose << "\n";
			}

			writeFile.close();
		}

		readFile.close();
	}
}
