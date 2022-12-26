#include "languagedialog.h"
#include "ui_languagedialog.h"

LanguageDialog::LanguageDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LanguageDialog)
{
	ui->setupUi(this);
}

LanguageDialog::~LanguageDialog()
{
	delete ui;
}

void LanguageDialog::on_LanguageDialog_finished(int result)
{
	if(result == QDialog::Accepted){
		currentLang = ui->languageBox->currentText();
	}
}
