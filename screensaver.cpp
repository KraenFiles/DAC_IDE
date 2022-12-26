#include "screensaver.h"
#include "ui_screensaver.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPair>
#include "languagedialog.h"
#include <QThread>

Screensaver::Screensaver(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Screensaver)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	QFile projects("settings.json");//Файл с настройками среды
	projects.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = projects.readAll();
	projects.close();

	QStringList listProject;

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QJsonArray listProjects = all["projectsList"].toArray();
	foreach(const QJsonValue& value, listProjects){
		if (value.isObject()){
			QJsonObject project = value.toObject();
			QString projectName = project["name"].toString();
			QString projectPath = project["path"].toString();
			QString projectDate = project["date"].toString();
			QString projectLang = project["language"].toString();

			listProject.append(projectName+"\t\t"+projectPath+"\n"+projectDate+"\t\t"+projectLang);//Заполнение списка для быстрой загрузки проектов
		}
	}

	ui->listProjects->addItems(listProject);

	QString theme = all["theme"].toString();
	if(theme=="Dark")
	{
		/*Установка темной темы для приложения*/
		QPalette darkPalette;
		darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));//Цвет фона окна
		darkPalette.setColor(QPalette::WindowText, QColor(230,230,230));//Цвет текста в окне
		darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));//Цвет заднего фона виджетов
		darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));//Альтернативный цвет заднего фона виджетов
		darkPalette.setColor(QPalette::ToolTipBase, QColor(53, 53, 53));//Задний фон для всплывающий окон
		darkPalette.setColor(QPalette::ToolTipText, QColor(200,200,200));//Цвет текста для всплывающий окон
		darkPalette.setColor(QPalette::Text, QColor(186, 186, 186));//Цвет текста
		darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));//Цвет фона кнопок
		darkPalette.setColor(QPalette::ButtonText, QColor(230,230,230));//Цвет текста кнопок
		darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));//Цвет ссылки
		darkPalette.setColor(QPalette::Highlight, QColor(150,150,150));//Цвет фона выбранного элемента
		darkPalette.setColor(QPalette::HighlightedText, Qt::black);//Цвет текста выбранного элемента
		qApp->setPalette(darkPalette);//Установка палитры для приложения
		/*------------------------------------*/
	}
}

Screensaver::~Screensaver()
{
	delete ui;
}

/*Функция срабатываемая при нажатии на кнопку "новый проект"*/
void Screensaver::on_newProject_clicked()
{
	SettingsProject *form = new SettingsProject;
	form->show();
	this->hide();
}

/*Функция срабатываемая при нажатии на кнопку "загрузить проект"*/
void Screensaver::on_loadProject_clicked()
{
	QFile file("settings.json");//Файл с настроками проекта
	QStringList stringList;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QString pathProjects = all["pathProjects"].toString();

	QString project = QFileDialog::getExistingDirectory(0, "Directory Dialog", pathProjects);//открытие диалогового окна для выбора директории проекта
	if(!project.isEmpty())//Проверка нажатия кнопки отмена
	{
		QString lang = nullptr;
		LanguageDialog *dialog = new LanguageDialog(this);
		if(dialog->exec() == QDialog::Accepted){
			lang = dialog->currentLang;
		}
		if(lang != nullptr){
			QStringList name=project.split("/");
			QJsonArray listProjects = all["projectsList"].toArray();

			int i = 0;
			foreach(const QJsonValue& value, listProjects){
				if (value.isObject()){
					QJsonObject project = value.toObject();
					QString projectName = project["name"].toString();
					if(projectName == name[name.size()-1]){
						listProjects.removeAt(i);
					}
					i++;
				}
			}
			QDateTime thisDateTime= QDateTime::currentDateTime();//Получение данной даты и времени
			QJsonObject newProject = QJsonObject({
													 qMakePair(QString("name"), name[name.size()-1]),
													 qMakePair(QString("path"), project),
													 qMakePair(QString("date"), thisDateTime.toString(Qt::SystemLocaleDate)),
													 qMakePair(QString("language"), lang)
												 });
			listProjects.push_front(QJsonValue(newProject));
			all.remove("projectsList");
			all.insert("projectsList", listProjects);

			file.open(QIODevice::WriteOnly);
			file.write(QJsonDocument(all).toJson());//Перезапись файла
			file.close();

			MainWindow *mainWindow= new MainWindow(nullptr, project, lang);
			mainWindow->show();//Открытие выбранного проекта
			this->close();
		}
	}
}

/*Функция срабатываемая при двойном нажатии на элемент списка с проектами*/
void Screensaver::on_listProjects_itemClicked(QListWidgetItem *item)
{
	QStringList listsProj = (item->text()).split("\n");//Получение выбранного элемента

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
	QString nameProj = (listsProj[0].split("\t\t"))[0];
	QString pathProj = (listsProj[0].split("\t\t"))[1];
	QString langProj = (listsProj[1].split("\t\t"))[1];

	int i = 0;
	foreach(const QJsonValue& value, listProjects){
		if (value.isObject()){
			QJsonObject project = value.toObject();
			projectName = project["name"].toString();
			projectPath = project["path"].toString();
			if(projectName == nameProj){
				listProjects.removeAt(i);
			}
			i++;
		}
	}
	QDateTime thisDateTime= QDateTime::currentDateTime();//Получение данной даты и времени
	QJsonObject newProject = QJsonObject({
				qMakePair(QString("name"), nameProj),
				qMakePair(QString("path"), pathProj),
				qMakePair(QString("date"), thisDateTime.toString(Qt::SystemLocaleDate)),
				qMakePair(QString("language"), langProj)
			});
	listProjects.push_front(QJsonValue(newProject));
	all.remove("projectsList");
	all.insert("projectsList", QJsonValue(listProjects));

	file.open(QIODevice::WriteOnly);
	file.write(QJsonDocument(all).toJson());//Перезапись файла
	file.close();

	MainWindow *mainWindow = new MainWindow(nullptr, pathProj, langProj);
	mainWindow->show();//Открытие выбранного проекта
	this->close();
}
