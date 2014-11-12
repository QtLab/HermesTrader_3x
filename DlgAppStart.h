#ifndef DLGAPPSTART_H
#define DLGAPPSTART_H

#include <QDialog>
#include <QtGui>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "WndMain.h"

using namespace std;

namespace Ui {
    class DlgAppStart;
}

class DlgAppStart : public QDialog
{
	Q_OBJECT
public:
	explicit					DlgAppStart(QWidget *parent = 0);													// �R���X�g���N�^
								~DlgAppStart();																		// �f�X�g���N�^
	void						SetData(QDate lastDBUpdateDate, QDate latestDBUpdateDate);							// �f�[�^�̃Z�b�g

protected:
	void						closeEvent(QCloseEvent *);															// �_�C�A���O�����Ƃ��̏���
	void						UpdateDB(int id, QDate startDate, QDate endDate);									// DB�̍X�V
	void						FormatDB();																			// DB�̐��`

private slots:
	void						replyFinished(QNetworkReply *);														// HTTP�f�[�^��M�������̃C�x���g
	void						on_btnUpdateDB_clicked();															// [UpdateDB]�{�^����clicked()�V�O�i�����󂯎��X���b�g
	void						on_btnUpdateClose_clicked();														// [Close]�{�^����clicked()�V�O�i�����󂯎��X���b�g

private:
	Ui::DlgAppStart				*ui;
	QNetworkAccessManager		mNetManager;
	QFile						mFile;
	QString						mURL;
	int							mID;
	int							mPage;
	QString						mTemp1;
	QString						mTemp2;
};

// PliceData�̓��t��r�֐�(����)
template <class PliceData> class compareDate:
		public binary_function<PliceData, PliceData, bool>
{
public:
	bool operator()(PliceData a, PliceData b)
	{
		return a.mDate.toJulianDay() < b.mDate.toJulianDay();
	}
};

#endif // DLGAPPSTART_H
