#include "Predict.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <direct.h>
#include <math.h>
#include <algorithm>

/**
 *	�R���X�g���N�^
 */
Predict::Predict()
{
}

/**
 *	�X�R�A�v�Z
 */
void Predict::CalculateScore(QString rootPath, int fragmentLength, double minScore, double minS, int start)
{
	// �ϐ��錾
	QString targetRateFileName;					// TargetRate�t�@�C����
	QString templateRateFileName;				// TemplateRate�t�@�C����
	QString scoreFileName;						// Score�t�@�C����
	QString readTargetLine;						// TargetText���[�_�[
	QString readTemplateLine;					// TemplateText���[�_�[
	double	sumTargetRate, averageTargetRate, sumPowerTargetRate, squareSumPowerTargetRate,
			sumTemplateRate, averageTemplateRate, sumPowerTemplateRate, squareSumPowerTemplateRate,
			numerator, denominate, score, targetS, templateS;
	vector <double> remainderTargetRate, powerTargetRate, remainderTemplateRate, powerTemplateRate;
	bool flag;

	// Score�f�B���N�g������
	_mkdir(rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Score\\");

	// TargetRate�������[�v
	for (int targetRateFileNum = 1000; targetRateFileNum < 1000000; targetRateFileNum++) {
		if (((targetRateFileNum > 10000) && !((targetRateFileNum == 998405) || (targetRateFileNum == 998407)))) {
			continue;
		}

		// �ǂݍ��ݗp�t�@�C���I�[�v��
		targetRateFileName = rootPath + "\\TargetRate\\" + mTemp1.setNum(targetRateFileNum) + ".rate";
		QFile targetRateFile(targetRateFileName);
		if (targetRateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �������ݗp�t�@�C���I�[�v��
			scoreFileName = scoreFileName = rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Score\\" + mTemp2.setNum(targetRateFileNum) + ".score";
			QFile scoreFile(scoreFileName);
			if (!scoreFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �錾&������
			QTextStream readTargetRate(&targetRateFile);
			readTargetLine = "";
			mTargetDate.clear();
			mTargetRate.clear();

			// rate�t�@�C���̓ǂݎ��
			while (!readTargetRate.atEnd()) {
				readTargetLine = readTargetRate.readLine();
				// �e�J�����̃f�[�^�擾
				mTargetDate.push_back(QDate::fromString((readTargetLine.split(",").at(0)), "yyyy/MM/dd"));
				mTargetRate.push_back(readTargetLine.split(",").at(1).toDouble());
			}

			// �v�Z�p�ϐ�������
			sumTargetRate = 0;
			averageTargetRate = 0;
			remainderTargetRate.clear();
			powerTargetRate.clear();
			sumPowerTargetRate = 0;
			squareSumPowerTargetRate = 0;

			// CurrentTargetRate���x�N�^�[�ɍĊi�[�i�v�Z����TemplateRate�Ƃ̐�������ۂ��߁j
			mCurrentTargetRate.clear();
			for (int TargetRateItemCount = mTargetRate.size()-fragmentLength-start; TargetRateItemCount < (int)mTargetRate.size()-start; TargetRateItemCount++) {
				mCurrentTargetRate.push_back(mTargetRate[TargetRateItemCount]);
			}

			// TargetRate�̃t���O�����g�����̘a���v�Z
			for (int targetRateItemCount = 0; targetRateItemCount < fragmentLength; targetRateItemCount++) {
				sumTargetRate += mCurrentTargetRate[targetRateItemCount];
			}

			// TargetRate�̃t���O�����g�����̑�������
			averageTargetRate = sumTargetRate / fragmentLength;

			// TargetRate�̑������ςƂ̍����Z�o���ē�悷��
			for (int targetRateItemCount = 0; targetRateItemCount < fragmentLength; targetRateItemCount++) {
				remainderTargetRate.push_back(mCurrentTargetRate[targetRateItemCount] - averageTargetRate);						// TargetRate�̑������ςƂ̍�
				powerTargetRate.push_back(remainderTargetRate[targetRateItemCount] * remainderTargetRate[targetRateItemCount]);	// TargetRate��Remainder�̓��
			}

			// TargetRate��Power�̘a
			for (int targetRateItemCount = 0; targetRateItemCount < fragmentLength; targetRateItemCount++) {
				sumPowerTargetRate += powerTargetRate[targetRateItemCount];
			}

			// TargetRate��SumPower�̕�����
			squareSumPowerTargetRate = sqrt(sumPowerTargetRate);

			// TemplateRate�������[�v
			for (int templateRateFileNum = 1000; templateRateFileNum < 1000000; templateRateFileNum++) {
				if (((templateRateFileNum > 10000) && !((templateRateFileNum == 998405) || (templateRateFileNum == 998407)))) {
					continue;
				}

				// �ǂݍ��ݗp�t�@�C���I�[�v��
				templateRateFileName = rootPath + "\\TemplateRate\\" + mTemp1.setNum(templateRateFileNum) + ".rate";
				QFile templateRateFile(templateRateFileName);

				if (templateRateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
					// �錾&������
					QTextStream readTemplateRate(&templateRateFile);
					readTemplateLine = "";
					mTemplateDate.clear();
					mTemplateRate.clear();

					// rate�t�@�C���̓ǂݎ��
					while (!readTemplateRate.atEnd()) {
						readTemplateLine = readTemplateRate.readLine();
						// �e�J�����̃f�[�^�擾
						mTemplateDate.push_back(QDate::fromString((readTemplateLine.split(",").at(0)), "yyyy/MM/dd"));
						mTemplateRate.push_back(readTemplateLine.split(",").at(1).toDouble());
					}

					// TemplateRate�t�@�C���̊J�n�s����s���ύX
					for (int TemplateRateItemCount = 0; TemplateRateItemCount < (int)mTemplateRate.size() - fragmentLength + 1; TemplateRateItemCount++) {
						// �v�Z�p�ϐ�������
						sumTemplateRate = 0;
						averageTemplateRate = 0;
						remainderTemplateRate.clear();
						powerTemplateRate.clear();
						sumPowerTemplateRate = 0;
						squareSumPowerTemplateRate = 0;
						numerator = 0;
						denominate = 0;
						score = 0;

						// CurrentTemplateRate���x�N�^�[�ɍĊi�[�i�v�Z����TargetRate�Ƃ̐�������ۂ��߁j
						mCurrentTemplateRate.clear();
						for (int currentTemplateRateItemCount = TemplateRateItemCount; currentTemplateRateItemCount < TemplateRateItemCount + fragmentLength; currentTemplateRateItemCount++) {
							mCurrentTemplateRate.push_back(mTemplateRate[currentTemplateRateItemCount]);
						}

						// TargetRate��TemplateRate���r���āA����臒l�ȉ��łȂ��ꍇ�͑����Ɏ��̃f�[�^�ɉf��
						flag = false;
						for (int fACount = 0; fACount < fragmentLength; fACount++) {
							if (abs(mCurrentTemplateRate[fACount] - mCurrentTargetRate[fACount]) < 0.001) {
								flag = true;
								break;
							}
						}
						if (!flag) {
							continue;
						}

						// TemplateRate�̘a���v�Z
						for (int fACount = 0; fACount < fragmentLength; fACount++) {
							sumTemplateRate += mCurrentTemplateRate[fACount];
						}

						// TemplateRate�̑������ς��v�Z
						averageTemplateRate = sumTemplateRate / fragmentLength;

						// TemplateRate�̑������ςƂ̍����v�Z���ē��
						for (int fACount = 0; fACount < fragmentLength; fACount++) {
							remainderTemplateRate.push_back(mCurrentTemplateRate[fACount] - averageTemplateRate);				// TemplateRate�̑������ςƂ̍����v�Z
							powerTemplateRate.push_back(remainderTemplateRate[fACount] * remainderTemplateRate[fACount]);		// TemplateRate��Remainder�̓��
						}

						// TemplateRate��Power�̘a
						for (int fACount = 0; fACount < fragmentLength; fACount++) {
							sumPowerTemplateRate += powerTemplateRate[fACount];
							numerator += (mCurrentTargetRate[fACount] - averageTargetRate) * (mCurrentTemplateRate[fACount] - averageTemplateRate); // ���֌W���̕��q�̌v�Z
						}

						// TemplateRate��SumPower�̕�����
						squareSumPowerTemplateRate = sqrt(sumPowerTemplateRate);

						// ���֌W���̕���̌v�Z
						denominate = squareSumPowerTargetRate * squareSumPowerTemplateRate;

						// ���֌W���̌v�Z
						score = numerator / denominate;

						// Template,Target�e�X�̊J�n���i�ƏI�����i�̓��������v�Z
						targetS = 10000;
						templateS = 10000;
						for (int fACount = 0; fACount < fragmentLength; fACount++) {
							targetS = targetS + (targetS * mCurrentTargetRate[fACount]);
							templateS = templateS + (templateS * mCurrentTemplateRate[fACount]);
						}

						// ���֌W����minScore�����傫����Ώo��
						if (score >= minScore) {
							// �J�n���i�ƏI�����i�̓���������臒l�����Ȃ�o��
							if (abs(targetS - templateS) < minS) {
								// �������ݗp�X�g���[���쐬
								QTextStream out(&scoreFile);
								// Target��Template��Rate�t�@�C������������
								out << targetRateFileNum << ",";
								out << templateRateFileNum << ",";
								// TemplateRate���̌��ݓ��t��������
								out << mTemplateDate[TemplateRateItemCount].toString("yyyy/MM/dd") << ",";
								// ���֌W���̏�������
								out << "Score" << "=" << score << "\n";
							}
						}
					}
				}
				templateRateFile.close();
			}

			scoreFile.close();
		}

		targetRateFile.close();
	}
}

/**
 *	Prediction()
 *
 *	@param[in] filteredScoreFilePath		���o����X�R�A�̍ŏ��l
 *	@param[in] predictLength				�������s��Score�f�B���N�g�����́i�^�[�Q�b�g�j�f�B���N�g���ԍ�
 */
void Predict::Prediction(QString rootPath, int targetID, int fragmentLength, int predictLength, int start)
{
	// �ϐ��錾
	TemplateScoreData tempData;
	QString scoreFilePath;
	QString readScoreLine;
	QString templateFilePath;
	QString readTemplateFileLine;
	QString predictionFilePath;
	QFile scoreFile;
	QFile templateFile;
	QFile predictionFile;
	int rowFromOrigin;
	int rowOfStartDate;
	vector <TemplateScoreData>	templateScoreData;
	double upCount, downCount, sameCount;
	double sumPrediction;
	CasePliceData tempPlice;

	// �ϐ�������
	mTemplateScoreData.clear();
	templateScoreData.clear();
	mBestCasePliceData.clear();
	mWorstCasePliceData.clear();
	mCenterCasePliceData.clear();

	// Prediction�f�B���N�g������
	_mkdir(rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Prediction\\");

	// ScoreFile���擾
	scoreFilePath = rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Score\\" + mTemp2.setNum(targetID) + ".score";

	// ScoreFile���J��
	scoreFile.setFileName(scoreFilePath);
	if (!scoreFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	QTextStream readScore(&scoreFile);

	// �t�@�C�����̃f�[�^�擾
	while (!readScore.atEnd()) {
		readScoreLine = readScore.readLine();

		// TemplateScoreData�z��̍쐬
		if (readScoreLine != "") {
			tempData.mID			= readScoreLine.split(",").at(1).toInt();
			tempData.mScore			= readScoreLine.split(",").at(3).split("=").at(1).toDouble();
			tempData.mOriginDate	= QDate::fromString(readScoreLine.split(",").at(2), "yyyy/MM/dd");
			tempData.mStartDate		= QDate::fromString(readScoreLine.split(",").at(2), "yyyy/MM/dd");
			tempData.mStartOpen		= 0;
			tempData.mStartHigh		= 0;
			tempData.mStartLow		= 0;
			tempData.mStartClose	= 0;
			tempData.mStartVolume	= 0;
			tempData.mStartAClose	= 0;
			tempData.mEndDate		= QDate::fromString(readScoreLine.split(",").at(2), "yyyy/MM/dd");
			tempData.mEndOpen		= 0;
			tempData.mEndHigh		= 0;
			tempData.mEndLow		= 0;
			tempData.mEndClose		= 0;
			tempData.mEndVolume		= 0;
			tempData.mEndAClose		= 0;
			mTemplateScoreData.push_back(tempData);
		}
	}

	// �f�[�^���Ȃ��ꍇ��return
	if (mTemplateScoreData.size() == 0) {
		return;
	}

	// �eTemplateScoreData�̏ڍ׃f�[�^���擾
	for (int i = 0; i < (int)mTemplateScoreData.size(); i++) {
		// �o�ߍs��0�ɏ�����
		rowFromOrigin = 0;
		rowOfStartDate = 0;

		// Template�t�@�C�����擾
		templateFilePath = rootPath + "TemplateDB\\" + mTemp1.setNum(mTemplateScoreData[i].mID) + ".csv";

		// Template�t�@�C�����J��
		templateFile.setFileName(templateFilePath);
		if (templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream readTemplateFile(&templateFile);

			// �t�@�C�����̃f�[�^�擾
			while (!readTemplateFile.atEnd()) {
				readTemplateFileLine = readTemplateFile.readLine();

				// Template�t�@�C���̃t���O�����g�A�Z���u���J�n�����擾
				if (QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy") == mTemplateScoreData[i].mOriginDate) {
					// �t���O�����g�A�Z���u���J�n���̌o�ߍs���擾
					rowOfStartDate = rowFromOrigin;
				}

				// �o�ߍs���C���N�������g
				rowFromOrigin++;

				if (rowOfStartDate != 0) {
					// �o�ߍs���\���J�n��(=�t���O�����g�A�Z���u���J�n��+�t���O�����g��)�ɂȂ�����Template�t�@�C���̊J�n���f�[�^���擾
					if (rowFromOrigin == rowOfStartDate + fragmentLength) {
						// TemplateScoreData�z��̏C��
						mTemplateScoreData[i].mStartDate	= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy");
						mTemplateScoreData[i].mStartOpen	= readTemplateFileLine.split(",").at(1).toDouble();
						mTemplateScoreData[i].mStartHigh	= readTemplateFileLine.split(",").at(2).toDouble();
						mTemplateScoreData[i].mStartLow		= readTemplateFileLine.split(",").at(3).toDouble();
						mTemplateScoreData[i].mStartClose	= readTemplateFileLine.split(",").at(4).toDouble();
						mTemplateScoreData[i].mStartVolume	= readTemplateFileLine.split(",").at(5).toDouble();
						mTemplateScoreData[i].mStartAClose	= readTemplateFileLine.split(",").at(6).toDouble();
					}

					// �o�ߍs���\���I�����ɂȂ�����Template�t�@�C���̏I�����f�[�^���擾
					if (rowFromOrigin == rowOfStartDate + fragmentLength + predictLength) {
						// TemplateScoreData�z��̏C��
						mTemplateScoreData[i].mEndDate		= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy");
						mTemplateScoreData[i].mEndOpen		= readTemplateFileLine.split(",").at(1).toDouble();
						mTemplateScoreData[i].mEndHigh		= readTemplateFileLine.split(",").at(2).toDouble();
						mTemplateScoreData[i].mEndLow		= readTemplateFileLine.split(",").at(3).toDouble();
						mTemplateScoreData[i].mEndClose		= readTemplateFileLine.split(",").at(4).toDouble();
						mTemplateScoreData[i].mEndVolume	= readTemplateFileLine.split(",").at(5).toDouble();
						mTemplateScoreData[i].mEndAClose	= readTemplateFileLine.split(",").at(6).toDouble();
						mTemplateScoreData[i].mPrediction	= ((mTemplateScoreData[i].mEndClose - mTemplateScoreData[i].mStartClose) / mTemplateScoreData[i].mStartClose);		// �I�l-�O�I�l
						//mTemplateScoreData[i].mPrediction2	= ((mTemplateScoreData[i].mEndClose - mTemplateScoreData[i].mEndOpen) / mTemplateScoreData[i].mStartClose);			// �I�l-�n�l
					}
				}
			}
		}

		templateFile.close();

		// EndDate��StartDate�ȑO�ɂ���f�[�^�͗L���f�[�^�O�̉\�����������ߏ��O����
		if (mTemplateScoreData[i].mEndDate.toJulianDay() <= mTemplateScoreData[i].mStartDate.toJulianDay()) {
			templateScoreData = mTemplateScoreData;
			mTemplateScoreData.clear();
			for (int j = 0; j < i; j++) {
				mTemplateScoreData.push_back(templateScoreData[j]);
			}
			for (int j = i + 1; j < (int)templateScoreData.size(); j++) {
				mTemplateScoreData.push_back(templateScoreData[j]);
			}
			i = i - 1;
		}
	}

	scoreFile.close();

	// mTemplateScoreData�z���\�����������Ƀ\�[�g
	sort(mTemplateScoreData.begin(), mTemplateScoreData.end(), comparePrediction<TemplateScoreData>());

	//-------------------------
	// Prediction1
	//-------------------------
	// �\�z�������̏㏸���Ɖ������ƈ������Z�o����
	upCount = 0;
	downCount = 0;
	sameCount = 0;
	sumPrediction = 0;
	for (int i = 0; i < (int)mTemplateScoreData.size(); i++) {
		if (mTemplateScoreData[i].mPrediction > 0) {
			upCount++;
		}
		else if (mTemplateScoreData[i].mPrediction < 0) {
			downCount++;
		}
		else {
			sameCount++;
		}

		// �\�����������ϒl�̌v�Z
		sumPrediction += mTemplateScoreData[i].mPrediction;
	}

	// �\�����������ϒl�̌v�Z
	sumPrediction = sumPrediction / (upCount + downCount + sameCount);

	// �������ݗp�t�@�C���I�[�v��
	predictionFilePath = rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Prediction\\" + mTemp2.setNum(targetID) + ".prediction";
	predictionFile.setFileName(predictionFilePath);
	if (!predictionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}

	// �������ݏ���
	QTextStream out(&predictionFile);
	out << "Max Prediction Rate:" << mTemplateScoreData[0].mPrediction << "\n";											// �ő�\��������
	out << "Min Prediction Rate:" << mTemplateScoreData[mTemplateScoreData.size()-1].mPrediction << "\n";				// �ŏ��\��������
	out << "Center Prediction Rate:" << mTemplateScoreData[(mTemplateScoreData.size()-1)/2].mPrediction << "\n";		// �����\��������
	out << "Average Prediction Rate:" << sumPrediction << "\n";															// ���ϗ\��������
	out << "Up Probability:" << (upCount / (upCount + downCount + sameCount)) << "\n";									// �㏸�m��
	out << "Down Probability:" << (downCount / (upCount + downCount + sameCount)) << "\n";								// �����m��
	out << "Same Probability:" << (sameCount / (upCount + downCount + sameCount)) << "\n";								// �s�ϊm��
	for (int i = 0; i < (int)mTemplateScoreData.size(); i++) {
		// Prediction�f�[�^�̏�������
		out << (mTemplateScoreData[i].mID) << ","
			<< (mTemplateScoreData[i].mScore) << ","
			<< (mTemplateScoreData[i].mStartDate.toString("yyyy/MM/dd")) << ","
			<< (mTemplateScoreData[i].mStartClose) << ","
			<< (mTemplateScoreData[i].mEndDate.toString("yyyy/MM/dd")) << ","
			<< (mTemplateScoreData[i].mEndClose) << ","
			<< (mTemplateScoreData[i].mPrediction) << "\n";
	}

	predictionFile.close();

	// BestCase�̃O���t�p�p�����[�^
	// Template�t�@�C�����擾
	templateFilePath = rootPath + "TemplateDB\\" + mTemp1.setNum(mTemplateScoreData[0].mID) + ".csv";
	// Template�t�@�C�����J��
	templateFile.setFileName(templateFilePath);
	if (templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream readTemplateFile(&templateFile);

		// �t�@�C�����̃f�[�^�擾
		while (!readTemplateFile.atEnd()) {
			readTemplateFileLine = readTemplateFile.readLine();

			// �t���O�����g�A�Z���u���J�n������\���I�����܂ł̃f�[�^���擾
			if ((QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay() >= mTemplateScoreData[0].mOriginDate.toJulianDay()) &&
				(QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay() <= mTemplateScoreData[0].mEndDate.toJulianDay())) {
				tempPlice.mDate			= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy");
				tempPlice.mDateJulian	= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay();
				tempPlice.mOpen			= readTemplateFileLine.split(",").at(1).toDouble();
				tempPlice.mHigh			= readTemplateFileLine.split(",").at(2).toDouble();
				tempPlice.mLow			= readTemplateFileLine.split(",").at(3).toDouble();
				tempPlice.mClose		= readTemplateFileLine.split(",").at(4).toDouble();
				tempPlice.mVolume		= readTemplateFileLine.split(",").at(5).toDouble();
				tempPlice.mAClose		= readTemplateFileLine.split(",").at(6).toDouble();
				tempPlice.mStartClose	= mTemplateScoreData[0].mStartClose;
				tempPlice.mEndClose		= mTemplateScoreData[0].mEndClose;
				tempPlice.mPrediction	= mTemplateScoreData[0].mPrediction;
				mBestCasePliceData.push_back(tempPlice);
			}
		}
	}

	templateFile.close();

	// WorstCase�̃O���t�p�p�����[�^
	// Template�t�@�C�����擾
	templateFilePath = rootPath + "TemplateDB\\" + mTemp1.setNum(mTemplateScoreData[mTemplateScoreData.size()-1].mID) + ".csv";
	// Template�t�@�C�����J��
	templateFile.setFileName(templateFilePath);
	if (templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream readTemplateFile(&templateFile);

		// �t�@�C�����̃f�[�^�擾
		while (!readTemplateFile.atEnd()) {
			readTemplateFileLine = readTemplateFile.readLine();

			// �t���O�����g�A�Z���u���J�n������\���I�����܂ł̃f�[�^���擾
			if ((QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay() >= mTemplateScoreData[mTemplateScoreData.size()-1].mOriginDate.toJulianDay()) &&
				(QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay() <= mTemplateScoreData[mTemplateScoreData.size()-1].mEndDate.toJulianDay())) {
				tempPlice.mDate			= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy");
				tempPlice.mDateJulian	= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay();
				tempPlice.mOpen			= readTemplateFileLine.split(",").at(1).toDouble();
				tempPlice.mHigh			= readTemplateFileLine.split(",").at(2).toDouble();
				tempPlice.mLow			= readTemplateFileLine.split(",").at(3).toDouble();
				tempPlice.mClose		= readTemplateFileLine.split(",").at(4).toDouble();
				tempPlice.mVolume		= readTemplateFileLine.split(",").at(5).toDouble();
				tempPlice.mAClose		= readTemplateFileLine.split(",").at(6).toDouble();
				tempPlice.mStartClose	= mTemplateScoreData[mTemplateScoreData.size()-1].mStartClose;
				tempPlice.mEndClose		= mTemplateScoreData[mTemplateScoreData.size()-1].mEndClose;
				tempPlice.mPrediction	= mTemplateScoreData[mTemplateScoreData.size()-1].mPrediction;
				mWorstCasePliceData.push_back(tempPlice);
			}
		}
	}

	templateFile.close();

	// CenterCase�̃O���t�p�p�����[�^
	// Template�t�@�C�����擾
	templateFilePath = rootPath + "TemplateDB\\" + mTemp1.setNum(mTemplateScoreData[(mTemplateScoreData.size()-1)/2].mID) + ".csv";
	// Template�t�@�C�����J��
	templateFile.setFileName(templateFilePath);
	if (templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream readTemplateFile(&templateFile);

		// �t�@�C�����̃f�[�^�擾
		while (!readTemplateFile.atEnd()) {
			readTemplateFileLine = readTemplateFile.readLine();

			// �t���O�����g�A�Z���u���J�n������\���I�����܂ł̃f�[�^���擾
			if ((QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay() >= mTemplateScoreData[(mTemplateScoreData.size()-1)/2].mOriginDate.toJulianDay()) &&
				(QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay() <= mTemplateScoreData[(mTemplateScoreData.size()-1)/2].mEndDate.toJulianDay())) {
				tempPlice.mDate			= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy");
				tempPlice.mDateJulian	= QDate::fromString(readTemplateFileLine.split(",").at(0), "M/d/yyyy").toJulianDay();
				tempPlice.mOpen			= readTemplateFileLine.split(",").at(1).toDouble();
				tempPlice.mHigh			= readTemplateFileLine.split(",").at(2).toDouble();
				tempPlice.mLow			= readTemplateFileLine.split(",").at(3).toDouble();
				tempPlice.mClose		= readTemplateFileLine.split(",").at(4).toDouble();
				tempPlice.mVolume		= readTemplateFileLine.split(",").at(5).toDouble();
				tempPlice.mAClose		= readTemplateFileLine.split(",").at(6).toDouble();
				tempPlice.mStartClose	= mTemplateScoreData[(mTemplateScoreData.size()-1)/2].mStartClose;
				tempPlice.mEndClose		= mTemplateScoreData[(mTemplateScoreData.size()-1)/2].mEndClose;
				tempPlice.mPrediction	= mTemplateScoreData[(mTemplateScoreData.size()-1)/2].mPrediction;
				mCenterCasePliceData.push_back(tempPlice);
			}
		}
	}

	templateFile.close();
}

/**
 *	Screening()
 *
 *	@param[in] filteredScoreFilePath		���o����X�R�A�̍ŏ��l
 *	@param[in] predictLength				�������s��Score�f�B���N�g�����́i�^�[�Q�b�g�j�f�B���N�g���ԍ�
 */
void Predict::Screening(QString rootPath, int start)
{
	// �ϐ��錾
	ScreeningData tempData;
	QString readPredictionLine;
	QString predictionFilePath;
	QFile predictionFile;
	QString screeningFilePath;
	QFile screeningFile;

	// �ϐ�������
	mScreeningData.clear();

	// Screening�f�B���N�g������
	_mkdir(rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Screening\\");

	for (int i = 1000; i < 1000000; i++) {
		if (((i > 10000) && !((i == 998405) || (i == 998407)))) {
			continue;
		}

		// PredictionFile���擾
		predictionFilePath = rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Prediction\\" + mTemp2.setNum(i) + ".prediction";

		// PredictionFile���J��
		predictionFile.setFileName(predictionFilePath);
		if (predictionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream readPrediction(&predictionFile);

			// �t�@�C�����̃f�[�^�擾
			while (!readPrediction.atEnd()) {
				readPredictionLine = readPrediction.readLine();

				// ���v�f�[�^�̎擾
				tempData.mID = i;
				if (readPredictionLine.split(":").at(0) == "Max Prediction Rate") {
					tempData.mMaxPredictionRate = readPredictionLine.split(":").at(1).toDouble();
				}
				else if (readPredictionLine.split(":").at(0) == "Min Prediction Rate") {
					tempData.mMinPredictionRate = readPredictionLine.split(":").at(1).toDouble();
				}
				else if (readPredictionLine.split(":").at(0) == "Center Prediction Rate") {
					tempData.mCenterPredictionRate = readPredictionLine.split(":").at(1).toDouble();
				}
				else if (readPredictionLine.split(":").at(0) == "Average Prediction Rate") {
					tempData.mAveragePredictionRate = readPredictionLine.split(":").at(1).toDouble();
				}
				else if (readPredictionLine.split(":").at(0) == "Up Probability") {
					tempData.mUpProbability = readPredictionLine.split(":").at(1).toDouble();
				}
				else if (readPredictionLine.split(":").at(0) == "Down Probability") {
					tempData.mDownProbability = readPredictionLine.split(":").at(1).toDouble();
				}
				else if (readPredictionLine.split(":").at(0) == "Same Probability") {
					tempData.mSameProbability = readPredictionLine.split(":").at(1).toDouble();
				}
			}
			mScreeningData.push_back(tempData);
		}

		predictionFile.close();
	}

	// �������ݗp�t�@�C���I�[�v��
	screeningFilePath = rootPath.toLocal8Bit() + mTemp1.setNum(start).toLocal8Bit() + "_Screening\\target.csv";
	screeningFile.setFileName(screeningFilePath);
	if (!screeningFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}

	// �������ݏ���
	QTextStream out(&screeningFile);
	out << "Target ID,";
	out << "Max Prediction Rate,";
	out << "Min Prediction Rate,";
	out << "Center Prediction Rate,";
	out << "Average Prediction Rate,";
	out << "Up Probability,";
	out << "Down Probability,";
	out << "Same Probability\n";
	for (int i = 0; i < (int)mScreeningData.size(); i++) {
		// ���v�f�[�^�̏�������
		out << (mScreeningData[i].mID) << ","
			<< (mScreeningData[i].mMaxPredictionRate) << ","
			<< (mScreeningData[i].mMinPredictionRate) << ","
			<< (mScreeningData[i].mCenterPredictionRate) << ","
			<< (mScreeningData[i].mAveragePredictionRate) << ","
			<< (mScreeningData[i].mUpProbability) << ","
			<< (mScreeningData[i].mDownProbability) << ","
			<< (mScreeningData[i].mSameProbability) << "\n";
	}

	screeningFile.close();
}
