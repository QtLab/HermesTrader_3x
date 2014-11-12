#include "BackTestThread.h"
#include <direct.h>
#include "WndMain.h"

/**
 *	�R���X�g���N�^
 */
BackTestThread::BackTestThread()
{
}

/**
 *	�X���b�h�������s
 */
void BackTestThread::run()
{
	Calcuration(mThresh, mMoney, mMaN, mTN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 */
void BackTestThread::ReadParam(int thresh, double money, int maN, int tN)
{
	mThresh			= thresh;
	mMoney			= money;
	mMaN			= maN;
	mTN				= tN;
}

/**
 *	�v�Z�����DB�\�z
 *	�G���g���[�F�P���ړ�����(�I�l)��������臒l�𒴂��������̊�t
 *	�G�O�W�b�g�F�P���ړ�����(�I�l)���������G���g���[�̋t����0�𒴂��������̊�t
 *
 *	@param[in] thresh		臒l
 *	@param[in] money		��������
 *	@param[in] maN			�ړ����ς̃T���v����
 *	@param[in] tN			���ԍ��l�A���Ԉ��l�̊���
 */
void BackTestThread::Calcuration(int thresh, double money, int maN, int tN)
{
	// �ϐ��錾
	QString pliceFileName;			// ���n��t�@�C����
	QFile pliceFile;				// ���n��t�@�C��
	QString pliceFileLine;			// ���n��t�@�C���s
	QString smaFileName;			// �P���ړ����σt�@�C����
	QFile smaFile;					// �P���ړ����σt�@�C��
	QString smaFileLine;			// �P���ړ����σt�@�C���s
	QString thFileName;				// ���ԍ��l�t�@�C����
	QFile thFile;					// ���ԍ��l�t�@�C��
	QString thFileLine;				// ���ԍ��l�t�@�C���s
	QString tlFileName;				// ���Ԉ��l�t�@�C����
	QFile tlFile;					// ���Ԉ��l�t�@�C��
	QString tlFileLine;				// ���Ԉ��l�t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <BackTest> testData;		// �o�b�N�e�X�g�f�[�^
	BackTest tempData;				// �ꎞ�ۑ��p�o�b�N�e�X�g�f�[�^
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������

	// �f�B���N�g������
	_mkdir("BackTest");

	// �eID�ɂ��ď������s
	for (int i = 1000; i < 1000000; i++) {
		if (((i > 10000) && !((i == 998405) || (i == 998407)))) {
			continue;
		}

		// �ϐ�������
		testData.clear();

		// ���n��t�@�C���I�[�v��
		pliceFileName = "Yahoo\\" + temp_1.setNum(i) + ".csv";
		pliceFile.setFileName(pliceFileName);

		// ���n��t�@�C�����Ȃ��ꍇ�͎���ID��
		if (!pliceFile.exists()) {
			continue;
		}

		if (pliceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			QTextStream pliceFileReader(&pliceFile);
			pliceFileLine = "";

			// ���n��f�[�^�̓ǂݎ��
			while (!pliceFileReader.atEnd()) {
				pliceFileLine = pliceFileReader.readLine();
				// ���n��f�[�^�擾
				if (pliceFileLine != "") {
					tempData.mDate			= QDate::fromString((pliceFileLine.split(",").at(0)), "yyyy/M/d");
					tempData.mEMoney		= money;
					tempData.mMoney			= money;
					tempData.mBuySignal		= false;
					tempData.mSellSignal	= false;
					tempData.mBuyNum		= 0;
					tempData.mSellNum		= 0;
					tempData.mBuyPlice		= 0;
					tempData.mSellPlice		= 0;
					tempData.mOpen			= pliceFileLine.split(",").at(1).toDouble();
					tempData.mHigh			= pliceFileLine.split(",").at(2).toDouble();
					tempData.mLow			= pliceFileLine.split(",").at(3).toDouble();
					tempData.mClose			= pliceFileLine.split(",").at(4).toDouble();
					tempData.mVolume		= pliceFileLine.split(",").at(5).toDouble();
					tempData.mAClose		= pliceFileLine.split(",").at(6).toDouble();
					tempData.mSMAR_close	= 0;
					tempData.mTHigh			= 0;
					tempData.mTLow			= 0;
					testData.push_back(tempData);
				}
			}
		}
		pliceFile.close();

		// testData�̃f�[�^�ʂ��g�p���铝�v�f�[�^�̃T���v������2�{�ɖ����Ȃ��ꍇ�͎���ID��
		if (((int)testData.size() < mMaN * 2) || ((int)testData.size() < mTN * 2)) {
			continue;
		}

		// SMA�t�@�C���I�[�v��
		smaFileName = "DB\\SMA_" + temp_1.setNum(maN) + "\\" + temp_2.setNum(i) + ".dat";
		smaFile.setFileName(smaFileName);
		if (smaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			int j = maN;
			QTextStream smaFileReader(&smaFile);
			smaFileLine = "";

			// SMA�f�[�^�̓ǂݎ��
			while (!smaFileReader.atEnd() || j < (int)testData.size()) {
				smaFileLine = smaFileReader.readLine();
				// SMA������(�I�l)�f�[�^�擾
				if (smaFileLine != "") {
					if (testData[j].mDate == QDate::fromString((smaFileLine.split(",").at(0)), "yyyy/M/d")) {
						testData[j].mSMAR_close = smaFileLine.split(",").at(10).toDouble();
					}
					j++;
				}
			}
		}
		smaFile.close();

		// ���ԍ��l�t�@�C���I�[�v��
		thFileName = "DB\\TermHigh_" + temp_1.setNum(tN) + "\\" + temp_2.setNum(i) + ".dat";
		thFile.setFileName(thFileName);
		if (thFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			int j = tN;
			QTextStream thFileReader(&thFile);
			thFileLine = "";

			// ���ԍ��l�f�[�^�̓ǂݎ��
			while (!thFileReader.atEnd() || j < (int)testData.size()) {
				thFileLine = thFileReader.readLine();
				if (thFileLine != "") {
					if (testData[j].mDate == QDate::fromString((thFileLine.split(",").at(0)), "yyyy/M/d")) {
						testData[j].mTHigh = thFileLine.split(",").at(4).toDouble();
					}
					j++;
				}
			}
		}
		thFile.close();

		// ���Ԉ��l�t�@�C���I�[�v��
		tlFileName = "DB\\TermLow_" + temp_1.setNum(tN) + "\\" + temp_2.setNum(i) + ".dat";
		tlFile.setFileName(tlFileName);
		if (tlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			int j = tN;
			QTextStream tlFileReader(&tlFile);
			tlFileLine = "";

			// ���Ԉ��l�f�[�^�̓ǂݎ��
			while (!tlFileReader.atEnd() || j < (int)testData.size()) {
				tlFileLine = tlFileReader.readLine();
				if (tlFileLine != "") {
					if (testData[j].mDate == QDate::fromString((tlFileLine.split(",").at(0)), "yyyy/M/d")) {
						testData[j].mTLow = tlFileLine.split(",").at(4).toDouble();
					}
					j++;
				}
			}
		}
		tlFile.close();

		// �o�b�N�e�X�g���s
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
		writeFileName = "BackTest\\" + temp_1.setNum(i) + ".dat";
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
