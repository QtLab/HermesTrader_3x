#ifndef TARGET_H
#define TARGET_H

#include <QtGui>
#include <QDate>

using namespace std;

class Target
{
public:
								Target();
	void						Restruct(QString rootPath);															// TargetRate�t�@�C���̍쐬

private:
	vector <QDate>				mDate;
	vector <double>				mClose;
	QString						mTemp1;
};

extern Target*					gpTarget;																			// Target�̃O���[�o���|�C���^

#endif // TARGET_H
