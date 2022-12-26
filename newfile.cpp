#include "newfile.h"
#include "ui_newfile.h"

NewFile::NewFile(QString path, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::NewFile)
{
	ui->setupUi(this);
	this->setFixedSize(610,400);
	pathProj=path;
	ui->readyButton->setEnabled(false);
}

NewFile::~NewFile()
{
	delete ui;
}

void NewFile::on_readyButton_clicked()
{
	QFile newFile((pathProj+"/"+ui->nameNewFile->text()).toLower());
	QStringList list=(ui->nameNewFile->text()).toLower().split(".");
	newFile.open(QIODevice::WriteOnly);
	if(list[1]=="h")
	{
		newFile.write(("#ifndef "+list[0].toUpper()+"_"+list[1].toUpper()+"\n#define "+list[0].toUpper()+"_"+list[1].toUpper()+"\n\n#endif // "+list[0].toUpper()+"_"+list[1].toUpper()).toUtf8().constData());
		newFile.close();
		this->hide();
	}
	else
		if(list[1]=="c"||list[1]=="cpp"||list[1]=="txt")
		{
			newFile.close();
			this->hide();
		}
		else
		{
			ui->nameNewFile->setStyleSheet("border: 1px solid red");
			ui->readyButton->setEnabled(false);
			QMessageBox::critical(this,"Ошибка ввода","Введен неверный формат документа");
		}
}

void NewFile::on_backButton_clicked()
{
	this->hide();
}

void NewFile::on_nameNewFile_textEdited(const QString &arg1)
{
	if(arg1.isEmpty() || arg1.length()==1)
	{
		ui->nameNewFile->setStyleSheet("border: 1px solid red");
		ui->readyButton->setEnabled(false);
	}
	else
	{
		ui->readyButton->setEnabled(true);
		ui->nameNewFile->setStyleSheet("border: 1px solid black");
	}
}
