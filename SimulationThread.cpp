#include "SimulationThread.h"
#include "BackTestThread.h"
#include <direct.h>

/**
 *	�R���X�g���N�^
 */
SimulationThread::SimulationThread()
{
}

/**
 *	�X���b�h�������s
 */
void SimulationThread::run()
{
	Calcuration(mThresh, mMoney, mStartDate, mEndDate);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 */
void SimulationThread::ReadParam(int thresh, double money, QDate startDate, QDate endDate)
{
	mThresh			= thresh;
	mMoney			= money;
	mStartDate		= startDate;
	mEndDate		= endDate;
}

/**
 *	�v�Z�����DB�\�z
 *	�G���g���[�F�P���ړ�����(�I�l)��������臒l�𒴂��������̊�t
 *	�G�O�W�b�g�F�P���ړ�����(�I�l)���������G���g���[�̋t����0�𒴂��������̊�t
 *
 *	@param[in] thresh		臒l
 *	@param[in] money		��������
 *	@param[in] startDate	�J�n��
 *	@param[in] endDate		�I����
 */
void SimulationThread::Calcuration(int thresh, double money, QDate startDate, QDate endDate)
{
	// �ϐ��錾
	QString backTestFileName;		// �o�b�N�e�X�g�t�@�C����
	QFile backTestFile;				// �o�b�N�e�X�g�t�@�C��
	QString backTestFileLine;		// �o�b�N�e�X�g�t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <BackTest> testData;		// �o�b�N�e�X�g�f�[�^
	BackTest tempData;				// �ꎞ�ۑ��p�o�b�N�e�X�g�f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("Simulation");

	// �eID�ɂ��ď������s
	for (int i = 1000; i < 1000000; i++) {
		if (((i > 10000) && !((i == 998405) || (i == 998407)))) {
			continue;
		}

		// �ϐ�������
		testData.clear();

		// �o�b�N�e�X�g�t�@�C���I�[�v��
		backTestFileName = "BackTest\\" + temp_1.setNum(i) + ".dat";
		backTestFile.setFileName(backTestFileName);

		// �o�b�N�e�X�g�t�@�C�����Ȃ��ꍇ�͎���ID��
		if (!backTestFile.exists()) {
			continue;
		}

		if (backTestFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			QTextStream backTestFileReader(&backTestFile);
			backTestFileLine = "";

			// �o�b�N�e�X�g�f�[�^�̓ǂݎ��
			while (!backTestFileReader.atEnd()) {
				backTestFileLine = backTestFileReader.readLine();
				// �J�n������I�����܂ł̃o�b�N�e�X�g�f�[�^�擾
				if (backTestFileLine != "") {
					if ((QDate::fromString((backTestFileLine.split(",").at(0)), "yyyy/M/d").toJulianDay() > startDate.toJulianDay()) &&
						(QDate::fromString((backTestFileLine.split(",").at(0)), "yyyy/M/d").toJulianDay() < endDate.toJulianDay())) {
						tempData.mDate			= QDate::fromString((backTestFileLine.split(",").at(0)), "yyyy/M/d");
						tempData.mEMoney		= money;
						tempData.mMoney			= money;
						tempData.mBuySignal		= backTestFileLine.split(",").at(3).contains("1");
						tempData.mSellSignal	= backTestFileLine.split(",").at(4).contains("1");
						tempData.mBuyNum		= backTestFileLine.split(",").at(5).toInt();
						tempData.mSellNum		= backTestFileLine.split(",").at(6).toInt();
						tempData.mBuyPlice		= backTestFileLine.split(",").at(7).toDouble();
						tempData.mSellPlice		= backTestFileLine.split(",").at(8).toDouble();
						tempData.mOpen			= backTestFileLine.split(",").at(9).toDouble();
						tempData.mHigh			= backTestFileLine.split(",").at(10).toDouble();
						tempData.mLow			= backTestFileLine.split(",").at(11).toDouble();
						tempData.mClose			= backTestFileLine.split(",").at(12).toDouble();
						tempData.mVolume		= backTestFileLine.split(",").at(13).toDouble();
						tempData.mAClose		= backTestFileLine.split(",").at(14).toDouble();
						tempData.mSMAR_close	= backTestFileLine.split(",").at(15).toDouble();
						tempData.mTHigh			= backTestFileLine.split(",").at(16).toDouble();
						tempData.mTLow			= backTestFileLine.split(",").at(17).toDouble();
						testData.push_back(tempData);
					}
				}
			}
		}
		backTestFile.close();

		// testData���Ȃ��ꍇ�͎���ID��
		if (testData.size() == 0) {
			continue;
		}

		// �ēx�o�b�N�e�X�g���s
		for (int j = 1; j < (int)testData.size(); j++) {
			// �O���ɔ���V�O�i���_����
			if (testData[j-1].mSellSignal) {
				// �V�O�i���Ǝ��������Ǝ����̏����l�͑O���̂��̂��p��
				testData[j].mBuySignal		= testData[j-1].mBuySignal;
				testData[j].mSellSignal		= testData[j-1].mSellSignal;
				testData[j].mBuyNum			= testData[j-1].mBuyNum;
				testData[j].mSellNum		= testData[j-1].mSellNum;
				testData[j].mBuyPlice		= testData[j-1].mBuyPlice;
				testData[j].mSellPlice		= testData[j-1].mSellPlice;
				testData[j].mMoney			= testData[j-1].mMoney;

				// �]���z�͔��艿�i����O���I�l�������āA�������|�������̂��A���艿�i*�ۗL�����ɑ����āA�����ɑ���
				testData[j].mEMoney = testData[j].mMoney + ((testData[j].mSellPlice - testData[j-1].mClose) * testData[j].mSellNum) + (testData[j].mSellPlice * testData[j].mSellNum);

				// �O��SMA�����������Ȃ�n�l�ŗ��v�m�肵�ăV�O�i��������
				if (testData[j-1].mSMAR_close < 0) {
					// �����͔��艿�i*�ۗL�������瓖���n�l*�ۗL�����������āA���艿�i*�ۗL�����ɑ����āA�����ɑ���
					testData[j].mMoney += ((testData[j].mSellPlice - testData[j].mOpen) * testData[j].mSellNum) + (testData[j].mSellPlice * testData[j].mSellNum);
					// �V�O�i���������Ď��������Ǝ��������i��0�ɂ���
					testData[j].mSellNum = 0;
					testData[j].mSellPlice = 0;
					testData[j].mSellSignal = false;
					// �]���z�͎����Ɠ��l
					testData[j].mEMoney = testData[j].mMoney;
				}
			}
			// �����V�O�i���_����
			else if (testData[j-1].mBuySignal) {
				// �V�O�i���Ǝ��������Ǝ����̏����l�͑O���̂��̂��p��
				testData[j].mBuySignal		= testData[j-1].mBuySignal;
				testData[j].mSellSignal		= testData[j-1].mSellSignal;
				testData[j].mBuyNum			= testData[j-1].mBuyNum;
				testData[j].mSellNum		= testData[j-1].mSellNum;
				testData[j].mBuyPlice		= testData[j-1].mBuyPlice;
				testData[j].mSellPlice		= testData[j-1].mSellPlice;
				testData[j].mMoney			= testData[j-1].mMoney;

				// �]���z�͑O���I�l���甃�����i�������āA�������|�������̂��A�������i*�ۗL�����ɑ����āA�����ɑ���
				testData[j].mEMoney = testData[j].mMoney + ((testData[j-1].mClose - testData[j].mBuyPlice) * testData[j].mBuyNum) + (testData[j].mBuyPlice * testData[j].mBuyNum);

				// �O��SMA�����������Ȃ�n�l�ŗ��v�m�肵�ăV�O�i��������
				if (testData[j-1].mSMAR_close > 0) {
					// �����͓����n�l*�ۗL�������甃�����i*�ۗL�����������āA�������i*�ۗL�����ɑ����āA�����ɑ���
					testData[j].mMoney += ((testData[j].mOpen - testData[j].mBuyPlice) * testData[j].mBuyNum) + (testData[j].mBuyPlice * testData[j].mBuyNum);
					// �V�O�i���������Ď��������Ǝ��������i��0�ɂ���
					testData[j].mBuyNum = 0;
					testData[j].mBuyPlice = 0;
					testData[j].mBuySignal = false;
					// �]���z�͎����Ɠ��l
					testData[j].mEMoney = testData[j].mMoney;
				}
			}
			// �V�O�i���Ȃ�
			else {
				// �V�O�i���Ǝ��������Ǝ����̏����l�͑O���̂��̂��p��
				testData[j].mBuySignal = testData[j-1].mBuySignal;
				testData[j].mSellSignal = testData[j-1].mSellSignal;
				testData[j].mBuyNum = testData[j-1].mBuyNum;
				testData[j].mSellNum = testData[j-1].mSellNum;
				testData[j].mMoney = testData[j-1].mMoney;

				// �]���z�͎����Ɠ��l
				testData[j].mEMoney = testData[j].mMoney;

				// �G���g���[
				// �O��SMA��������臒l����ɒ������ꍇ�A����V�O�i�������B��t�ɔ���
				if (testData[j-1].mSMAR_close > thresh) {
					testData[j].mSellSignal = true;
					testData[j].mSellNum = (int)(testData[j].mMoney / testData[j].mOpen);
					testData[j].mSellPlice = testData[j].mOpen;
					testData[j].mMoney -= (double)testData[j].mSellNum * testData[j].mOpen;
					// �]���z�͔��艿�i���瓖���I�l�������āA�������|�������̂��A���艿�i*�ۗL�����ɑ����āA�����ɑ���
					testData[j].mEMoney = testData[j].mMoney + ((testData[j].mSellPlice - testData[j].mClose) * testData[j].mSellNum) + (testData[j].mSellPlice * testData[j].mSellNum);
				}
				// �O��SMA��������臒l�����ɒ������ꍇ�A�����V�O�i�������B��t�ɔ���
				else if (testData[j-1].mSMAR_close < -thresh) {
					testData[j].mBuySignal = true;
					testData[j].mBuyNum = (int)(testData[j].mMoney / testData[j].mOpen);
					testData[j].mBuyPlice = testData[j].mOpen;
					testData[j].mMoney -= (double)testData[j].mBuyNum * testData[j].mOpen;
					// �]���z�͓����I�l���甃�����i�������āA�������|�������̂��A�������i*�ۗL�����ɑ����āA�����ɑ���
					testData[j].mEMoney = testData[j].mMoney + ((testData[j].mClose - testData[j].mBuyPlice) * testData[j].mBuyNum) + (testData[j].mBuyPlice * testData[j].mBuyNum);
				}
			}
		}

		// �������ݗp�t�@�C���I�[�v��
		writeFileName = "Simulation\\" + temp_1.setNum(i) + ".dat";
		writeFile.setFileName(writeFileName);
		if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			return;
		}

		// �������ݏ���
		for (int j = 0; j < (int)testData.size(); j++) {
			QTextStream out(&writeFile);

			// �o�b�N�e�X�g1�f�[�^�̏�������
			out << (testData[j].mDate.toString("yyyy/M/d")) << ","
				<< (int)testData[j].mEMoney << ","						// double���ƌ덷���o��
				<< (int)testData[j].mMoney << ","						// double���ƌ덷���o��
				<< testData[j].mBuySignal << ","
				<< testData[j].mSellSignal << ","
				<< testData[j].mBuyNum << ","
				<< testData[j].mSellNum << ","
				<< testData[j].mBuyPlice << ","
				<< testData[j].mSellPlice << ","
				<< testData[j].mOpen << ","
				<< testData[j].mHigh << ","
				<< testData[j].mLow << ","
				<< testData[j].mClose << ","
				<< testData[j].mVolume << ","
				<< testData[j].mAClose << ","
				<< testData[j].mSMAR_close << ","
				<< testData[j].mTHigh << ","
				<< testData[j].mTLow << "\n";
		}

		writeFile.close();
	}
}
