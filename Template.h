#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QtGui>
#include <QDate>

using namespace std;

class Template
{
public:
								Template();
	void						Restruct(QString rootPath);															// TemplateRate�t�@�C���̍쐬

private:
	vector <QDate>				mDate;
	vector <double>				mClose;
	QString						mTemp1;
};

extern Template*				gpTemplate;																			// Template�̃O���[�o���|�C���^

#endif // TEMPLATE_H
