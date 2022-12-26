#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <QDialog>

namespace Ui {
class LanguageDialog;
}

class LanguageDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LanguageDialog(QWidget *parent = nullptr);
	~LanguageDialog();

	QString currentLang = nullptr;

private slots:
	void on_LanguageDialog_finished(int result);

private:
	Ui::LanguageDialog *ui;
};

#endif // LANGUAGEDIALOG_H
