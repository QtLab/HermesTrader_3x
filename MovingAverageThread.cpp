#include "MovingAverageThread.h"
#include <direct.h>

MovingAverageThread::MovingAverageThread()
{
}

/**
 *	�X���b�h�������s
 */
void MovingAverageThread::run()
{
	Calcuration(mN);
}

/**
 *	�A���S���Y���Ŏg�p����p�����[�^��ǂݍ���
 *
 *	@param[in] n			�T���v����
 */
void MovingAverageThread::ReadParam(int n)
{
	mN			= n;
}

/**
 *	�P���ړ�����(SMA)�v�Z
 *
 *	@param[in] alg			�A���S���Y��(SMA, EMA, RMA, WMA)
 *	@param[in] n			�T���v����
 */
void MovingAverageThread::Calcuration(QString alg, int n)
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	vector <QDate> date;			// ���t�f�[�^
	vector <double> close;			// �I�l�f�[�^
	double ma;						// MA
	QString temp_1;					// �ꎞ�ۑ��p������
	QString temp_2;					// �ꎞ�ۑ��p������

	// �ϐ�������
	date.clear();
	close.clear();

	if (alg == "SMA") {
		// SMA�f�B���N�g������
		_mkdir("DB\\SMA_" + temp_1.setNum(n).toLocal8Bit());
	}

	// �eID�ɂ��ď������s
	for (int i = 1000; i < 1000000; i++) {
		if (((i > 10000) && !((i == 998405) || (i == 998407)))) {
			continue;
		}

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
				date.push_back(QDate::fromString((readFileLine.split(",").at(0)), "yyyy/M/d"));
				close.push_back(readFileLine.split(",").at(6).toDouble());
			}

			if (alg == "SMA") {
				writeFileName = "DB\\SMA_" + temp_1.setNum(n) + "\\" + temp_2.setNum(i) + ".dat";
			}

			// �������ݗp�t�@�C���I�[�v��
			writeFile.setFileName(writeFileName);
			if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}

			// �������ݏ���
			for (int i = n; i < (int)close.size() - n; i++) {
				QTextStream out(&writeFile);

				if (alg == "SMA") {
					// SMA�v�Z
					ma = 0;
					for (int j = 0; j < n; j++) {
						ma += close[i-j];
					}
					ma = ma / n;
				}

				// SMA�̏�������
				out << (date[i].toString("yyyy/M/d")) << "," << ma << "\n";
			}

			writeFile.close();
		}

		readFile.close();
	}
}
