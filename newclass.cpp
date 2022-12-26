#include "newclass.h"
#include "ui_newclass.h"

QString pathProj;

NewClass::NewClass(QString path, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::NewClass)
{
	ui->setupUi(this);
	pathProj=path;
	ui->readyButton->setEnabled(false);
	this->setFixedSize(610,400);
}

NewClass::~NewClass()
{
	delete ui;
}

void NewClass::on_editNameClass_textEdited(const QString &arg1)
{
	if(arg1.isEmpty() || arg1.length()==1)
	{
		ui->editNameClass->setStyleSheet("border: 1px solid red");
		ui->readyButton->setEnabled(false);
	}
	else
	{
		ui->readyButton->setEnabled(true);
		ui->editNameClass->setStyleSheet("border: 1px solid black");
	}
	ui->editHeader->setText(arg1.toLower()+".h");
	ui->editSource->setText(arg1.toLower()+".cpp");
}

void NewClass::on_cancelButton_clicked()
{
	this->hide();
}

void NewClass::on_readyButton_clicked()
{
	QFile headerFile(pathProj+"/"+ui->editHeader->text());
	QString fileName=ui->editNameClass->text();
	headerFile.open(QIODevice::WriteOnly);
	headerFile.write(("#ifndef "+fileName.toUpper()+"_H\n#define "+fileName.toUpper()+"_H\n\nclass "+fileName+"\n{\npublic:\n\t"+fileName+"();\n};\n\n#endif // "+fileName.toUpper()+"_H").toUtf8().constData());
	headerFile.close();
	headerFile.setFileName(pathProj+"/"+ui->editSource->text());
	headerFile.open(QIODevice::WriteOnly);
	headerFile.write(("#include \""+ui->editHeader->text()+"\"\n\n"+fileName+"::"+fileName+"()\n{\n\n}").toUtf8().constData());
	headerFile.close();
	this->hide();
}
