#include "FibonacciThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
FibonacciThread::FibonacciThread()
{
}

/**
 *	�X���b�h�������s
 */
void FibonacciThread::run()
{
	Calcuration(mN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 */
void FibonacciThread::ReadParam(int n)
{
	mN			= n;
}

/**
 *	�v�Z�����DB�\�z
 */
void FibonacciThread::Calcuration(int n)
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <PliceData> pliceData;	// ���n��f�[�^
	double fib_recent_high;			// ���ߍ��l
	double fib_recent_low;			// ���߈��l
	QDate fib_recent_high_date;		// ���ߍ��l��
	QDate fib_recent_low_date;		// ���߈��l��
	int fib_current_condition;		// ���ݒl�̏��(���ݏI�l�����ߍ��l�̏ꍇ��0,���߈��l�̏ꍇ��1�B���ߍ��l�����߈��l������Ȃ�2,�����łȂ����3�B���ߍ��l���ƒ��߈��l����������Ȃ�4)
	double fib_push_382;			// ����(38.2%)
	double fib_push_500;			// ����(50.0%)
	double fib_push_618;			// ����(61.8%)
	double fib_pull_382;			// �߂�(38.2%)
	double fib_pull_500;			// �߂�(50.0%)
	double fib_pull_618;			// �߂�(61.8%)
	PliceData tempData;				// �ꎞ�ۑ��p���n��f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������
	double maxRec, minRec;

	// �f�B���N�g������
	_mkdir("DB\\Fibonacci_" + temp_1.setNum(n).toLocal8Bit());

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
			writeFileName = "DB\\Fibonacci_" + temp_1.setNum(n) + "\\" + temp_2.setNum(i) + ".dat";
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int j = n; j < (int)pliceData.size(); j++) {
				QTextStream out(&writeFile);

				// �T���v��n���Ԃ̒��ߍ��l�ƒ��߈��l���擾
				maxRec = 0;
				minRec = 10000000;
				for (int k = 0; k < n; k++) {
					maxRec = max(maxRec, pliceData[j-k].mHigh);
					minRec = min(minRec, pliceData[j-k].mLow);
				}
				fib_recent_high = maxRec;
				fib_recent_low = minRec;

				// �T���v��n���Ԃ̒��ߍ��l���ƒ��߈��l�����擾
				for (int k = 0; k < n; k++) {
					if (pliceData[j-k].mHigh == fib_recent_high) {
						fib_recent_high_date = pliceData[j-k].mDate;
					}
					if (pliceData[j-k].mLow == fib_recent_low) {
						fib_recent_low_date = pliceData[j-k].mDate;
					}
				}

				// ���݉��i�̏�Ԓl�擾
				// ���ݏI�l�����ߍ��l�̏ꍇ
				if (pliceData[j].mClose == fib_recent_high) {
					fib_current_condition = 0;
				}
				// ���ݏI�l�����߈��l�̏ꍇ
				else if (pliceData[j].mClose == fib_recent_low) {
					fib_current_condition = 1;
				}
				// ���ߍ��l�������߈��l�������ŋ߂̏ꍇ
				else if (fib_recent_high_date.toJulianDay() > fib_recent_low_date.toJulianDay()) {
					fib_current_condition = 2;
				}
				// ���߈��l�������ߍ��l�������ŋ߂̏ꍇ
				else if (fib_recent_high_date.toJulianDay() < fib_recent_low_date.toJulianDay()) {
					fib_current_condition = 3;
				}
				// ���ߍ��l�������߈��l���Ɠ���̏ꍇ
				else if (fib_recent_high_date == fib_recent_low_date) {
					fib_current_condition = 4;
				}
				// ����ȊO�̏ꍇ�i�z�肵�Ă��Ȃ��j
				else {
					fib_current_condition = 5;
				}

				// �����Ɩ߂��̌v�Z
				fib_push_382 = fib_recent_high - ((fib_recent_high - fib_recent_low) * 0.382);
				fib_push_500 = fib_recent_high - ((fib_recent_high - fib_recent_low) * 0.500);
				fib_push_618 = fib_recent_high - ((fib_recent_high - fib_recent_low) * 0.618);
				fib_pull_382 = fib_recent_low + ((fib_recent_high - fib_recent_low) * 0.382);
				fib_pull_500 = fib_recent_low + ((fib_recent_high - fib_recent_low) * 0.500);
				fib_pull_618 = fib_recent_low + ((fib_recent_high - fib_recent_low) * 0.618);

				// �t�B�{�i�b�`�g���[�X�����g�̏�������
				out << (pliceData[j].mDate.toString("yyyy/M/d")) << ","
					<< fib_current_condition << ","												// ���ݏ��
					<< fib_push_382 << ","														// ����(38.2%)
					<< fib_push_500 << ","														// ����(50.0%)
					<< fib_push_618 << ","														// ����(61.8%)
					<< fib_pull_382 << ","														// �߂�(38.2%)
					<< fib_pull_500 << ","														// �߂�(50.0%)
					<< fib_pull_618 << "\n";													// �߂�(61.8%)
			}

			writeFile.close();
		}

		readFile.close();
	}
}
