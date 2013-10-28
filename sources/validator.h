#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__

#include <QDialog>
#include <QDir>

class QDialogButtonBox;
class QTextEdit;
class QPushButton;
class QLabel;

class ValidatorDialog : public QDialog
{
    Q_OBJECT

public:
    ValidatorDialog(QWidget *parent = 0);
    
private slots:
	void reject();
	void openDirectory();
	void stopAll();

private:
	QTextEdit *textEdit;
	QPushButton *openDir;
	QPushButton *stopButton;	
	QLabel *dirName;
    QDialogButtonBox *buttonBox;
    
	QStringList getAllFilesFromPath(const QString &, const QStringList &);
	QStringList getAllSubdirs(const QString &);  	
	bool stop;
};

#endif // __VALIDATOR_H__
