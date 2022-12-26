#include "settingslanguage.h"
#include "ui_settingslanguage.h"

QString nameProj;
QString wayProj;
int indexLang;
bool checkWindows;

settingsLanguage::settingsLanguage(QWidget *parent, QString nameProject, QString wayProject, bool checkWindow) :
	QWidget(parent),
	ui(new Ui::settingsLanguage)
{

	ui->setupUi(this);
	ui->comboBoxLanguage->addItem("C++");
	ui->comboBoxLanguage->addItem("C");
	ui->comboBoxLanguage->addItem("Go");
	startCode = "#include <iostream>\n\n"
              "using namespace std;\n\n"
              "int main()\n"
              "{\n"
              "\tcout << \"Hello World!\" << endl;\n"
              "\treturn 0;\n"
              "}\n";
	ui->exampleStartCode->setText(startCode);
	int fontWidth = QFontMetrics(ui->exampleStartCode->currentCharFormat().font()).averageCharWidth();
	ui->exampleStartCode->setTabStopDistance(4 * fontWidth);

	QFile file("settings.json");//Файл с настроками проекта
	QStringList stringList;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QString font = all["font"].toString();
	int fontSize = all["fontSize"].toInt();

	ui->exampleStartCode->setFont(QFont(font, fontSize));
	nameProj=nameProject;
	wayProj=wayProject;
	checkWindows = checkWindow;
}

settingsLanguage::~settingsLanguage()
{
	delete ui;
}

void settingsLanguage::on_readyButton_clicked()
{
	QFile file("settings.json");
	QStringList stringList;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QString lang;
	QString path = wayProj.append("/")+nameProj;
	QDir directory;
	QFile mainFile;
	directory.mkpath(path);
	if(indexLang==0){
		mainFile.setFileName(path+"/main.cpp");
		lang = "C++";
	}
	else{
		if(indexLang==1){
			mainFile.setFileName(path+"/main.c");
			lang = "C";
		}
		else{
			mainFile.setFileName(path+"/main.go");
			lang = "Go";
		}
	}

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QString pathProjects = all["pathProjects"].toString();

	if(wayProj != pathProjects)
	{
		all.remove("pathProjects");
		all.insert("pathProjects", QJsonValue(wayProj));
	}

	QJsonArray listProjects = all["projectsList"].toArray();
	QDateTime thisDateTime= QDateTime::currentDateTime();//Получение данной даты и времени
	QJsonObject newProject = QJsonObject({
				qMakePair(QString("name"), nameProj),
				qMakePair(QString("path"), wayProj.append("/")+nameProj),
				qMakePair(QString("date"), thisDateTime.toString(Qt::SystemLocaleDate)),
				qMakePair(QString("language"), lang)
			});
	listProjects.push_front(QJsonValue(newProject));
	all.remove("projectsList");
	all.insert("projectsList", listProjects);

	file.open(QIODevice::WriteOnly);
	file.write(QJsonDocument(all).toJson());//Перезапись файла
	file.close();


	mainFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream writeMainStream(&mainFile);
	writeMainStream << startCode;
	mainFile.close();
	MainWindow *mainWindow = new MainWindow(nullptr, path, lang);
	mainWindow->show();
	this->hide();
}

void settingsLanguage::on_comboBoxLanguage_activated(int index)
{
	indexLang=index;
	if(index==0)
	{
		startCode = "#include <iostream>\n\n"
                  "using namespace std;\n\n"
                  "int main()\n"
                  "{\n"
                  "\tcout << \"Hello World!\" << endl;\n"
                  "\treturn 0;\n"
                  "}\n";
		ui->exampleStartCode->setText(startCode);
	}
	else
		if(index==1)
		{
			startCode = "#include <stdio.h>\n\n"
                    "int main()\n"
                    "{\n"
                    "\tprintf(\"Hello World!\\n\");\n"
                    "\treturn 0;\n"
                    "}\n";
			ui->exampleStartCode->setText(startCode);
		}
		else{
			if(index == 2){
				startCode = "package main\n\n"
						"import \"fmt\"\n\n"
						"func main() {\n"
						"\tfmt.Println(\"Hello, World!\")\n"
						"}\n";
				ui->exampleStartCode->setText(startCode);
			}
		}
}

void settingsLanguage::on_backButton_clicked()
{
	SettingsProject *settings = new SettingsProject(nullptr,checkWindows,nameProj,wayProj);
	settings->show();
	this->hide();
}
