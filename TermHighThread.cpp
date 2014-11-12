#include "TermHighThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
TermHighThread::TermHighThread()
{
}

/**
 *	�X���b�h�������s
 */
void TermHighThread::run()
{
	Calcuration(mN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 *
 *	@param[in] n			�T���v����
 */
void TermHighThread::ReadParam(int n)
{
	mN			= n;
}

/**
 *	�v�Z�����DB�\�z
 *
 *	@param[in] n			�T���v����
 */
void TermHighThread::Calcuration(int n)
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <PliceData> pliceData;	// ���n��f�[�^
	double th_open;					// ���ԍ��l(Open)
	double th_high;					// ���ԍ��l(High)
	double th_low;					// ���ԍ��l(Low)
	double th_close;				// ���ԍ��l(Close)
	double th_volume;				// ���ԍ��l(Volume)
	double th_aClose;				// ���ԍ��l(AClose)
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("DB\\TermHigh_" + temp_1.setNum(n).toLocal8Bit());

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
			writeFileName = "DB\\TermHigh_" + temp_1.setNum(n) + "\\" + temp_2.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = n; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// ���ԍ��l�v�Z
				th_open		= 0;
				th_high		= 0;
				th_low		= 0;
				th_close	= 0;
				th_volume	= 0;
				th_aClose	= 0;
				for (int k = 0; k < n; k++) {
					th_open		= max(pliceData[j-k].mOpen, th_open);
					th_high		= max(pliceData[j-k].mHigh, th_high);
					th_low		= max(pliceData[j-k].mLow, th_low);
					th_close	= max(pliceData[j-k].mClose, th_close);
					th_volume	= max(pliceData[j-k].mVolume, th_volume);
					th_aClose	= max(pliceData[j-k].mAClose, th_aClose);
				}

				// ���ԍ��l�̏�������
				out << (pliceData[j].mDate.toString("yyyy/M/d")) << ","
					<< th_open << ","
					<< th_high << ","
					<< th_low << ","
					<< th_close << ","
					<< th_volume << ","
					<< th_aClose << "\n";
			}

			writeFile.close();
		}

		readFile.close();
	}
}
