#include "RMAThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
RMAThread::RMAThread()
{
}

/**
 *	�X���b�h�������s
 */
void RMAThread::run()
{
	Calcuration(mN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 *
 *	@param[in] n			�T���v����
 */
void RMAThread::ReadParam(int n)
{
	mN			= n;
}

/**
 *	�v�Z�����DB�\�z
 *
 *	@param[in] n			�T���v����
 */
void RMAThread::Calcuration(int n)
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
	double pMa_open;				// �O����MA(Open)
	double pMa_high;				// �O����MA(High)
	double pMa_low;					// �O����MA(Low)
	double pMa_close;				// �O����MA(Close)
	double pMa_volume;				// �O����MA(Volume)
	double pMa_aClose;				// �O����MA(AClose)
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("DB\\RMA_" + temp_1.setNum(n).toLocal8Bit());

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
			writeFileName = "DB\\RMA_" + temp_1.setNum(n) + "\\" + temp_2.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = n; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// RMA�v�Z
				ma_open		= 0;
				ma_high		= 0;
				ma_low		= 0;
				ma_close	= 0;
				ma_volume	= 0;
				ma_aClose	= 0;
				if (j == n) {
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
				}
				else {
					ma_open		= ((n - 1) * pMa_open + pliceData[j].mOpen) / n;
					ma_high		= ((n - 1) * pMa_high + pliceData[j].mHigh) / n;
					ma_low		= ((n - 1) * pMa_low + pliceData[j].mLow) / n;
					ma_close	= ((n - 1) * pMa_close + pliceData[j].mClose) / n;
					ma_volume	= ((n - 1) * pMa_volume + pliceData[j].mVolume) / n;
					ma_aClose	= ((n - 1) * pMa_aClose + pliceData[j].mAClose) / n;
				}

				// RMA����ј������̏�������
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
				pMa_open = ma_open;
				pMa_high = ma_high;
				pMa_low = ma_low;
				pMa_close = ma_close;
				pMa_volume = ma_volume;
				pMa_aClose = ma_aClose;
			}

			writeFile.close();
		}

		readFile.close();
	}
}
