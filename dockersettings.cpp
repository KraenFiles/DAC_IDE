#include "dockersettings.h"
#include "ui_dockersettings.h"

#include <QFile>
#include <QProcess>
#include <QDir>

DockerSettings::DockerSettings(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DockerSettings)
{
	ui->setupUi(this);

	QFile dockerFile("Dockerfile");
	dockerFile.open(QIODevice::ReadOnly);
	QString result = dockerFile.readAll();
	dockerFile.close();

	ui->dockerFile->setPlainText(result);
}

DockerSettings::~DockerSettings()
{
	delete ui;
}

void DockerSettings::on_backButton_clicked()
{
	this->close();
}

void DockerSettings::on_enterButton_clicked()
{
	QString text = ui->dockerFile->toPlainText();

	QFile dockerFile("Dockerfile");
	dockerFile.open(QIODevice::WriteOnly);
	dockerFile.write(text.toUtf8());
	dockerFile.close();

	QProcess* process = new QProcess();
	process->setWorkingDirectory(QDir::currentPath());
	if(QSysInfo::productType()=="windows")
	{
		process->start("cmd.exe",QStringList()<<"/C"<<"docker build --tag compiler .\n");
	}
	this->close();
}
