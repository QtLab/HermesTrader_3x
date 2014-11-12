#include "DlgAppStart.h"
#include "ui_DlgAppStart.h"
#include <algorithm>

/**
 *	�R���X�g���N�^
 */
DlgAppStart::DlgAppStart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAppStart)
{
    ui->setupUi(this);

	connect(&mNetManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

/**
 *	�f�X�g���N�^
 */
DlgAppStart::~DlgAppStart()
{
	delete ui;
}

/**
 *	closeEvent()
 *
 *	@param[in] e �󂯎�����C�x���g
 */
void DlgAppStart::closeEvent(QCloseEvent *)
{
	// Config.ini�ւ̏�������
	QSettings iniWriter("Config.ini", QSettings::IniFormat);

	// DB�Z�N�V����
	iniWriter.beginGroup("DB");
	iniWriter.setValue("GetLastDataDate", ui->dedtLastDBUpdateDate->date().toJulianDay());									// �ŏIDB�X�V��
	iniWriter.endGroup();
}

/**
 *	�f�[�^�̃Z�b�g
 *
 *	@param[in] lastDBUpdateDate						�ŏI�X�V��
 *	@param[in] latestDBUpdateDate					�ŐV�X�V�\��
 */
void DlgAppStart::SetData(QDate lastDBUpdateDate, QDate latestDBUpdateDate)
{
	// UI�ւ̃f�[�^���f
	ui->dedtLastDBUpdateDate->setDate(lastDBUpdateDate);				// �ŏI�X�V��
	ui->dedtLatestDBUpdateDate->setDate(latestDBUpdateDate);			// �ŐV�X�V�\��
}

/**
 *	DB�̍X�V
 *
 *	@param[in] startDate							�f�[�^�擾�J�n��
 *	@param[in] endDate								�f�[�^�擾�I����
 */
void DlgAppStart::UpdateDB(int id, QDate startDate, QDate endDate)
{
	// URL����
	mURL =	"http://info.finance.yahoo.co.jp/history/";				// Yahoo�t�@�C�i���X�̎��n��y�[�W
	mURL +=	"?code=" + mTemp1.setNum(id);							// �،��R�[�h
	mURL +=	"&sy=" + mTemp1.setNum(startDate.year());				// �J�n�N
	mURL +=	"&sm=" + mTemp1.setNum(startDate.month());				// �J�n��
	mURL +=	"&sd=" + mTemp1.setNum(startDate.day());				// �J�n��
	mURL +=	"&ey=" + mTemp1.setNum(endDate.year());					// �I���N
	mURL +=	"&em=" + mTemp1.setNum(endDate.month());				// �I����
	mURL +=	"&ed=" + mTemp1.setNum(endDate.day());					// �I����
	mURL += "&p=" + mTemp1.setNum(mPage);							// �y�[�W�i���o�[

	// �A�N�Z�X
	mNetManager.get(QNetworkRequest(QUrl(mURL)));
}

/**
 *	DB�̐��`
 */
void DlgAppStart::FormatDB()
{
	// �ϐ��錾
	QString readFileName;			// �ǂݍ��݃t�@�C����
	QFile readFile;					// �ǂݍ��݃t�@�C��
	QString readFileLine;			// �ǂݍ��݃t�@�C���s
	QString writeFileName;			// �������݃t�@�C����
	QFile writeFile;				// �������݃t�@�C��
	QString temp_1;					// �ꎞ�ۑ��p������
	PliceData tempData;				// ���n��ꎞ�f�[�^
	vector <PliceData> pliceData;	// ���n��f�[�^�z��

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

		if (!readFile.exists()) {
			continue;
		}

		if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// �錾&������
			QTextStream readFileReader(&readFile);
			readFileLine = "";

			// ���n��f�[�^�̓ǂݎ��
			while (!readFileReader.atEnd()) {
				readFileLine = readFileReader.readLine();

				// TOPIX�Ɠ��o���ς̂Ƃ��̓f�[�^����5�܂���7��
				if ((i == 998405) || (i == 998407)) {
					if (readFileLine.split(",").size() == 5) {
						// �f�[�^�擾
						tempData.mDate		= QDate::fromString((readFileLine.split(",").at(0)), "yyyy/M/d");
						tempData.mOpen		= readFileLine.split(",").at(1).toDouble();
						tempData.mHigh		= readFileLine.split(",").at(2).toDouble();
						tempData.mLow		= readFileLine.split(",").at(3).toDouble();
						tempData.mClose		= readFileLine.split(",").at(4).toDouble();
						tempData.mVolume	= 0;
						tempData.mAClose	= readFileLine.split(",").at(4).toDouble();
						pliceData.push_back(tempData);
					}
					else if (readFileLine.split(",").size() == 7) {
						// �f�[�^�擾
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
				else {
					// ����ȊO�̓f�[�^����7�B���������Ȃ�����f�[�^�̂ݒ��o
					if (readFileLine.split(",").size() == 7) {
						// �f�[�^�擾
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
			}
		}

		readFile.close();

		// ���t���Ƀ\�[�g
		sort(pliceData.begin(), pliceData.end(), compareDate<PliceData>());

		// ����f�[�^�̍폜
		for (int j = 0; j < (int)pliceData.size(); j++) {
			if (j == 0) {
				continue;
			}
			// ���s�s�ƒ��O�s�̓��t�������ꍇ�͌��s�s���폜
			if (pliceData[j].mDate == pliceData[j-1].mDate) {
				for (int k = j; k < (int)pliceData.size() - 1; k++) {
					pliceData[k] = pliceData[k+1];
				}
				pliceData.pop_back();
				j--;
			}
		}

		// �������ݗp�t�@�C���I�[�v��
		writeFileName = readFileName;
		writeFile.setFileName(writeFileName);
		if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			return;
		}

		// �������ݏ���
		for (int j = 0; j < (int)pliceData.size(); j++) {
			QTextStream out(&writeFile);

			// ���`��f�[�^�̏�������
			out << (pliceData[j].mDate.toString("yyyy/M/d")) << ","
				<< (pliceData[j].mOpen) << ","
				<< (pliceData[j].mHigh) << ","
				<< (pliceData[j].mLow) << ","
				<< (pliceData[j].mClose) << ","
				<< (pliceData[j].mVolume) << ","
				<< (pliceData[j].mAClose) << "\n";
		}

		writeFile.close();
	}
}

/**
 *	HTTP�f�[�^��M�������̃C�x���g
 *
 *	@param[in] reply								reply
 */
void DlgAppStart::replyFinished(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError) {
		// �G���[���b�Z�[�W
		QMessageBox::warning(this, tr("Error"),
							 mTemp1.setNum(mID),
							 QMessageBox::Ok
							 );

		// �ۑ��p�t�@�C�������
		mFile.close();

		// ���g���C
		mPage = 1;
		UpdateDB(mID, ui->dedtLastDBUpdateDate->date(), ui->dedtLatestDBUpdateDate->date());

		return;
	}
	else {
		// �擾�f�[�^
		QString str = QString::fromUtf8(reply->readAll().data());

		// �擾�f�[�^�𐮂���
		QTextCodec* tc = QTextCodec::codecForLocale();
		str.replace("</table>", "\n");									// table�s�̏I��肪�f�[�^�̏I���Ȃ̂ŁA���s���Ĉȍ~�̃f�[�^��ǂ܂Ȃ�����
		str.replace(",", "");											// �J���}���Ƃ�
		str.replace(QRegExp("\\s+"), "");								// �X�y�[�X���Ƃ�
		str.replace("class=\"stocksPriceRed\"", "");					// �폜
		str.replace("class=\"stocksPriceGreen\"", "");					// �폜
		str.replace("</td><td>", ",");									// �e�f�[�^�l�̋�؂蕶�����J���}�ɂ���
		str.replace(tc->toUnicode("�N"), "/");							// �N��/�ɂ���
		str.replace(tc->toUnicode("��"), "/");							// ����/�ɂ���
		str.replace(tc->toUnicode("��"), "");							// �����Ȃ�
		QStringList row = str.split("<tr><td>");

		// �f�[�^�o��
		if (row.size() > 1) {
			if (mPage == 1) {
				// �t�@�C�������ɑ��݂��Ă���ꍇ��Append���[�h�ŊJ��
				if (mFile.exists()) {
					// �t�@�C�����J��
					mFile.open(QIODevice::Append);
				}
				// �t�@�C�����Ȃ��ꍇ�͐V�K�쐬����
				else {
					// �t�@�C�����J��
					mFile.open(QIODevice::WriteOnly);
				}
			}

			QTextStream out(&mFile);

			for (int i = 1; i < row.size(); i++) {
				out << row.at(i).split("</td></tr>").at(0) << "\n";
			}

			// ���̃y�[�W��ǂݍ���
			mPage++;
			UpdateDB(mID, ui->dedtLastDBUpdateDate->date(), ui->dedtLatestDBUpdateDate->date());
		}
		else {
			// �ۑ��p�t�@�C�������
			mFile.close();

			// ����ID��ǂݍ���
			mID++;
			mPage = 1;
			if (mID == 10000) {
				mID = 998405;
				// ���̏o�̓t�@�C����
				mFile.setFileName("Yahoo\\" + mTemp1.setNum(mID) + ".csv");

				// ����ID��ǂݍ���
				UpdateDB(mID, ui->dedtLastDBUpdateDate->date(), ui->dedtLatestDBUpdateDate->date());
			}
			else if (mID == 998406) {
				mID = 998407;
				// ���̏o�̓t�@�C����
				mFile.setFileName("Yahoo\\" + mTemp1.setNum(mID) + ".csv");

				// ����ID��ǂݍ���
				UpdateDB(mID, ui->dedtLastDBUpdateDate->date(), ui->dedtLatestDBUpdateDate->date());
			}
			else if (mID == 998408) {
				// �S�f�[�^�擾����������EndDate-5(���Y��h�~)��StartDate�ɑ������
				ui->dedtLastDBUpdateDate->setDate(QDate::fromJulianDay(ui->dedtLatestDBUpdateDate->date().toJulianDay() - 5));
				ui->btnUpdateDB->setEnabled(true);
				// �S�f�[�^���`����
				FormatDB();
			}
			else if (mID < 10000) {
				// ���̏o�̓t�@�C����
				mFile.setFileName("Yahoo\\" + mTemp1.setNum(mID) + ".csv");

				// ����ID��ǂݍ���
				UpdateDB(mID, ui->dedtLastDBUpdateDate->date(), ui->dedtLatestDBUpdateDate->date());
			}
		}
	}
}

/**
 *	[UpdateDB]�{�^����clicked()�V�O�i�����󂯎��X���b�g
 */
void DlgAppStart::on_btnUpdateDB_clicked()
{
	// �f�[�^�擾�J�n
	mID = 1300;
	mPage = 1;
	ui->btnUpdateDB->setEnabled(false);
	UpdateDB(mID, ui->dedtLastDBUpdateDate->date(), ui->dedtLatestDBUpdateDate->date());
}

/**
 *	[Close]�{�^����clicked()�V�O�i�����󂯎��X���b�g
 */
void DlgAppStart::on_btnUpdateClose_clicked()
{
	this->close();
}
