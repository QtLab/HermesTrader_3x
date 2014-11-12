#include "SMAThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
SMAThread::SMAThread()
{
}

/**
 *	�X���b�h�������s
 */
void SMAThread::run()
{
	Calcuration(mN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 *
 *	@param[in] n			�T���v����
 */
void SMAThread::ReadParam(int n)
{
	mN			= n;
}

/**
 *	�v�Z�����DB�\�z
 *
 *	@param[in] n			�T���v����
 */
void SMAThread::Calcuration(int n)
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <PliceData> pliceData;	// ���n��f�[�^
	double ma_open;					// MA(Open)
	double ma_high;					// MA(High)
	double ma_low;					// MA(Low)
	double ma_close;				// MA(Close)
	double ma_volume;				// MA(Volume)
	double ma_aClose;				// MA(AClose)
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("DB\\SMA_" + temp_1.setNum(n).toLocal8Bit());

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
			writeFileName = "DB\\SMA_" + temp_1.setNum(n) + "\\" + temp_2.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = n; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// SMA�v�Z
				ma_open		= 0;
				ma_high		= 0;
				ma_low		= 0;
				ma_close	= 0;
				ma_volume	= 0;
				ma_aClose	= 0;
				for (int k = 0; k < n; k++) {
					ma_open		+= pliceData[j-k].mOpen;
					ma_high		+= pliceData[j-k].mHigh;
					ma_low		+= pliceData[j-k].mLow;
					ma_close	+= pliceData[j-k].mClose;
					ma_volume	+= pliceData[j-k].mVolume;
					ma_aClose	+= pliceData[j-k].mAClose;
				}
				ma_open		= ma_open / n;
				ma_high		= ma_high / n;
				ma_low		= ma_low / n;
				ma_close	= ma_close / n;
				ma_volume	= ma_volume / n;
				ma_aClose	= ma_aClose / n;

				// SMA����ј������̏�������
				out << (pliceData[j].mDate.toString("yyyy/M/d")) << ","
					<< ma_open << ","
					<< ma_high << ","
					<< ma_low << ","
					<< ma_close << ","
					<< ma_volume << ","
					<< ma_aClose << ","
					<< (pliceData[j-1].mOpen - ma_open) / ma_open * 100 << ","
					<< (pliceData[j-1].mHigh - ma_high) / ma_high * 100 << ","
					<< (pliceData[j-1].mLow - ma_low) / ma_low * 100 << ","
					<< (pliceData[j-1].mClose - ma_close) / ma_close * 100 << ","
					<< (pliceData[j-1].mVolume - ma_volume) / ma_volume * 100 << ","
					<< (pliceData[j-1].mAClose - ma_aClose) / ma_aClose * 100 << "\n";
			}

			writeFile.close();
		}

		readFile.close();
	}
}
