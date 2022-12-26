#include "settingsproject.h"
#include "ui_settingsproject.h"

SettingsProject::SettingsProject(QWidget *parent, bool checkWindow, QString nameProject, QString wayProject) :
	QWidget(parent),
	ui(new Ui::SettingsProject)
{
	ui->setupUi(this);

	this->setFixedSize(QSize(750,462));//Установка окну фиксированного значения
	/*Проверка на запись имени и пути из файла или имеющиеся значения*/
	QFile file("settings.json");//Файл с настройками поекта
	QStringList stringList;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QString name = all["projectName"].toString();
	QString path = all["pathProjects"].toString();

	if(name == nameProject || nameProject == nullptr)
		ui->lineEditName->setText(name);//Запись имени из файла
	else
		ui->lineEditName->setText(nameProject);//Запись существующего имени

	ui->lineEditWay->setText(path);

	if(path == wayProject || wayProject == nullptr)
		ui->lineEditWay->setText(path);//Запись пути из файла
	else
		ui->lineEditWay->setText(wayProject);//Запись существующего пути

	checkOpenWindow=checkWindow;//Запись значения предыдущего окна
}

SettingsProject::~SettingsProject()
{
	delete ui;
}

/*Нажатие на кнопку "Далее"*/
void SettingsProject::on_nextButton_clicked()
{
	settingsLanguage *nextStage = new settingsLanguage(nullptr,ui->lineEditName->text(),ui->lineEditWay->text(), checkOpenWindow);//Переход к настройке языка проекта
	nextStage->setFixedSize(QSize(750,462));//Фиксация размеров окна
	nextStage->show();
	this->hide();
}

/*Нажатие на кнопку "Обзор"*/
void SettingsProject::on_overviewButton_clicked()
{
	QString way = ui->lineEditWay->text();
	QString newWay = QFileDialog::getExistingDirectory(0, "Directory Dialog", way);//Выбор пути размещения проекта
	if(!newWay.isEmpty())//Проверка нажатия кнопки отмена
		ui->lineEditWay->setText(newWay);
	else
		ui->lineEditWay->setText(way);
}

/*Нажатие на кнопку "Назад"*/
void SettingsProject::on_backButton_clicked()
{
	if(checkOpenWindow)
	{
		Screensaver *backWindow = new Screensaver();
		backWindow->show();
		this->hide();
	}
	else
		this->hide();
}

/*Изменение поля имени*/
void SettingsProject::on_lineEditName_textEdited(const QString &arg1)
{
	QString enterName = ui->lineEditName->text();
	QString enterPath = ui->lineEditWay->text();

	QFile file("settings.json");//Файл с настроками проекта
	QStringList stringList;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QJsonArray listProjects = all["projectsList"].toArray();
	QString projectName;
	QString projectPath;

	foreach(const QJsonValue& value, listProjects){
		if (value.isObject()){
			QJsonObject project = value.toObject();
			projectName = project["name"].toString();
			projectPath = project["path"].toString();
			if(arg1.isEmpty() || arg1.length() == 2 || (projectName == enterName && enterPath == projectPath))//Проверка вводимого значения
			{
				ui->lineEditName->setStyleSheet("border: 1px solid red");//Изменение цвета рамки поля имени
				ui->nextButton->setEnabled(false);//Скрытие кнопки "Далее"
			}
			else
			{
				ui->lineEditName->setStyleSheet("border: 1px solid black");
				ui->nextButton->setEnabled(true);
			}
		}
	}
}
