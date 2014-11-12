#include "WMAThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
WMAThread::WMAThread()
{
}

/**
 *	�X���b�h�������s
 */
void WMAThread::run()
{
	Calcuration(mN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 *
 *	@param[in] n			�T���v����
 */
void WMAThread::ReadParam(int n)
{
	mN			= n;
}

/**
 *	�v�Z�����DB�\�z
 *
 *	@param[in] n			�T���v����
 */
void WMAThread::Calcuration(int n)
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
	double dMa_open;				// MA�v�Z���̕���(Open)
	double dMa_high;				// MA�v�Z���̕���(High)
	double dMa_low;					// MA�v�Z���̕���(Low)
	double dMa_close;				// MA�v�Z���̕���(Close)
	double dMa_volume;				// MA�v�Z���̕���(Volume)
	double dMa_aClose;				// MA�v�Z���̕���(AClose)
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("DB\\WMA_" + temp_1.setNum(n).toLocal8Bit());

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
			writeFileName = "DB\\WMA_" + temp_1.setNum(n) + "\\" + temp_2.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = n; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// WMA�v�Z
				ma_open		= 0;
				ma_high		= 0;
				ma_low		= 0;
				ma_close	= 0;
				ma_volume	= 0;
				ma_aClose	= 0;
				dMa_open	= 0;
				dMa_high	= 0;
				dMa_low		= 0;
				dMa_close	= 0;
				dMa_volume	= 0;
				dMa_aClose	= 0;
				for (int k = 0; k < n; k++) {
					ma_open		+= ((n - k) * pliceData[j-k].mOpen);
					ma_high		+= ((n - k) * pliceData[j-k].mHigh);
					ma_low		+= ((n - k) * pliceData[j-k].mLow);
					ma_close	+= ((n - k) * pliceData[j-k].mClose);
					ma_volume	+= ((n - k) * pliceData[j-k].mVolume);
					ma_aClose	+= ((n - k) * pliceData[j-k].mAClose);
					dMa_open	+= n;
					dMa_high	+= n;
					dMa_low		+= n;
					dMa_close	+= n;
					dMa_volume	+= n;
					dMa_aClose	+= n;
				}
				ma_open		= ma_open / dMa_open;
				ma_high		= ma_high / dMa_high;
				ma_low		= ma_low / dMa_low;
				ma_close	= ma_close / dMa_close;
				ma_volume	= ma_volume / dMa_volume;
				ma_aClose	= ma_aClose / dMa_aClose;

				// WMA����ј������̏�������
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
