#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dockersettings.h"

MainWindow::MainWindow(QWidget *parent, QString projectPath, QString projectLanguage):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	pathProject=projectPath;
	languageProject = projectLanguage;
	newVariant = 0;

	listHelp = new QListWidget(this);
	listHelp->show();
	listHelp->setVisible(false);
	listHelp->setMinimumHeight(100);

	codeEditor=new CodeEditor();
	codeEditor->setCursorWidth(2);
	ui->codeFilesSlitter->setStretchFactor(1,1);
	ui->CodeConsoleSlitter->setStretchFactor(0,1);
	allFilesInProject = new QStringList[2];
	int fontWidth = QFontMetrics(codeEditor->currentCharFormat().font()).averageCharWidth();
	codeEditor->setTabStopDistance(4 * fontWidth);

	QFile file("settings.json");//Файл с настроками проекта
	QStringList stringList;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QString font = all["font"].toString();
	int fontSize = all["fontSize"].toInt();

	codeEditor->setFont(QFont(font, fontSize));
	codeEditor->document()->setDefaultFont(QFont(font, fontSize,QFont::Normal));
	new SyntaxHighlighter(codeEditor->document());
	ui->codeBlock->addWidget(codeEditor);
	codeEditor->setMinimumWidth(350);
	this->showMaximized();
	QStringList name=projectPath.split("/");
	this->setWindowTitle(name[name.size()-1]+" - Donative Alive Code");
	filesTree();
	codeEditor->setEnabled(false);
	process = new QProcess(this);

	connect(ui->save,&QAction::triggered,this,&MainWindow::clicked_on_save);
	connect(ui->creationFile,&QAction::triggered,this,&MainWindow::clicked_on_new_file);
	connect(ui->creationClass,&QAction::triggered,this,&MainWindow::clicked_on_new_class);
	connect(ui->saveAll,&QAction::triggered,this,&MainWindow::clicked_on_saveAll);
	connect(ui->saveAs,&QAction::triggered,this,&MainWindow::clicked_on_saveAs);
	connect(ui->createNewProj,&QAction::triggered,this,&MainWindow::clicked_on_createNewProject);
	connect(ui->openProj,&QAction::triggered,this,&MainWindow::clicked_on_loadProject);
	connect(ui->fullscrean,&QAction::triggered,this,&MainWindow::clicked_on_fullscreen);
	connect(ui->settingsWindow,&QAction::triggered,this,&MainWindow::clicked_on_settingsWindow);
	connect(ui->run,&QAction::triggered,this,&MainWindow::on_startButton_clicked);
	connect(ui->build,&QAction::triggered,this,&MainWindow::on_buildButton_clicked);

	connect(ui->cancel,&QAction::triggered,codeEditor,&CodeEditor::undo);
	connect(ui->repeat,&QAction::triggered,codeEditor,&CodeEditor::redo);
	connect(ui->carve,&QAction::triggered,codeEditor,&CodeEditor::cut);
	connect(ui->copy,&QAction::triggered,codeEditor,&CodeEditor::copy);
	connect(ui->embed,&QAction::triggered,codeEditor,&CodeEditor::paste);
	connect(ui->highlightAll,&QAction::triggered,codeEditor,&CodeEditor::selectAll);

	connect(codeEditor,&QPlainTextEdit::textChanged,this,&MainWindow::saves_files_changed);
	connect(codeEditor,&QPlainTextEdit::textChanged,this,&MainWindow::helpListWidget);
	connect(codeEditor,&QPlainTextEdit::cursorPositionChanged,this,&MainWindow::newCursorPosition);
	connect(listHelp, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(doubleClickList(QListWidgetItem *)));

	ui->creationFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F));
	ui->creationClass->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_L));
	ui->save->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
	ui->saveAll->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
	ui->createNewProj->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
	ui->openProj->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
	ui->fullscrean->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_F));
	ui->cancel->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
	ui->repeat->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Z));
	ui->carve->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_X));
	ui->copy->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
	ui->embed->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_V));
	ui->highlightAll->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
	ui->run->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
	ui->build->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_B));

	QFile wordsSettings("KeyWords.json");//Файл с настройками среды
	wordsSettings.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allWordsSettings = wordsSettings.readAll();
	wordsSettings.close();

	QJsonDocument jsonWords = QJsonDocument::fromJson(allWordsSettings.toUtf8());
	QJsonObject allWords = jsonWords.object();
	QJsonObject listKeyWordsSettings = allWords["keyWords"].toObject();
	QJsonArray listKeyWords = listKeyWordsSettings["words"].toArray();
	foreach(const QJsonValue& value, listKeyWords){
		allKeyWords << value.toString();
	}
	QJsonObject listTypesSettings = allWords["types"].toObject();
	QJsonArray listTypes = listTypesSettings["words"].toArray();
	foreach(const QJsonValue& value, listTypes){
		allKeyWords << value.toString();
	}

	allKeyWords.sort();

	QDir buildDir(pathProject+"/build");
	if(!buildDir.exists())
	{
		QDir newDir;
		newDir.mkpath(pathProject+"/build");
	}

	QFile make(pathProject + "/build/makefile");
	if(!make.exists()){
		QString makeString;
		QStringList nameProj = projectPath.split("/");

		if(projectLanguage == "C++"){
			makeString = "PROJDIR := /home\n"
	"BUILDDIR := $(PROJDIR)/build/\n"
	"TARGET = $(BUILDDIR)" + nameProj[nameProj.size()-1] + "\n\n"
	"SOURCES = $(wildcard *.cpp)\n"
	"OBJS = $(addprefix $(BUILDDIR),$(SOURCES:.cpp=.o))\n\n"
	"CC = x86_64-w64-mingw32-g++\n"
	"CLFLAGS = -c -Wall -Wextra -Wconversion -Woverloaded-virtual\n\n"
	"all: clean $(TARGET)\n\n"
	"$(TARGET): $(OBJS)\n"
		"\t@echo Linking $@\n"
		"\t$(CC) $(OBJS) -o $(TARGET)\n\n"
	"$(BUILDDIR)%.o: %.cpp\n"
		"\t@echo Building $@\n"
		"\t$(CC) $(CLFLAGS) $< -o $@\n\n"
	"clean:\n"
		"\trm -rf $(OBJS) $(TARGET).exe\n"
		"\t@echo Cleaning done!\n\n";

			QFile libcpp(pathProject + "/build/libstdc++-6.dll");
			QFile libgcc(pathProject + "/build/libgcc_s_seh-1.dll");
			QFile libtread(pathProject + "/build/libwinpthread-1.dll");
			if(!libcpp.exists()){
				QFile::copy("libstdc++-6.dll", libcpp.fileName());
			}

			if(!libgcc.exists()){
				QFile::copy("libgcc_s_seh-1.dll", libgcc.fileName());
			}

			if(!libtread.exists()){
				QFile::copy("libwinpthread-1.dll", libtread.fileName());
			}
		}
		else{
			if(projectLanguage == "C"){
				makeString = "PROJDIR := /home\n"
		"BUILDDIR := $(PROJDIR)/build/\n"
		"TARGET = $(BUILDDIR)" + nameProj[nameProj.size()-1] + "\n\n"
		"SOURCES = $(wildcard *.c)\n"
		"OBJS = $(addprefix $(BUILDDIR),$(SOURCES:.c=.o))\n\n"
		"CC = x86_64-w64-mingw32-gcc\n"
		"CLFLAGS = -c -Wall -Wextra -Wconversion\n\n"
		"all: clean $(TARGET)\n\n"
		"$(TARGET): $(OBJS)\n"
			"\t@echo Linking $@\n"
			"\t$(CC) $(OBJS) -o $(TARGET)\n\n"
		"$(BUILDDIR)%.o: %.c\n"
			"\t@echo Building $@\n"
			"\t$(CC) $(CLFLAGS) $< -o $@\n\n"
		"clean:\n"
			"\trm -rf $(OBJS) $(TARGET).exe\n"
			"\t@echo Cleaning done!\n\n";
			}
			else{
				if(projectLanguage == "Go"){
					makeString = "PROJDIR := /home\n"
		"BUILDDIR := $(PROJDIR)/build/\n"
		"TARGET = $(BUILDDIR)" + nameProj[nameProj.size()-1] + ".exe\n\n"
		"SOURCES = $(wildcard *.go)\n\n"
		"CC = go build\n"
		"OS = GOOS=windows\n\n"
		"all: clean $(TARGET)\n\n"
		"$(TARGET):\n"
			"\t@echo Building $@\n"
			"\t$(OS) $(CC) -o $(TARGET) $(SOURCES)\n\n"
		"clean:\n"
			"\trm -rf $(TARGET)\n"
			"\t@echo Cleaning done!\n\n";
				}
			}
		}

		make.open(QIODevice::WriteOnly);
		make.write(makeString.toUtf8().constData());
		make.close();
	}

	process->start("cmd.exe", QStringList() << "/C" << "docker run -dt --mount type=bind,source=" + projectPath + ",target=/home/ --name compilingblock compiler > nul");
	process->waitForFinished();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	process->start("cmd.exe", QStringList() << "/C" << "docker stop compilingblock");
	process->waitForFinished();
	process->start("cmd.exe", QStringList() << "/C" << "docker rm compilingblock");
	process->waitForFinished();
}

void MainWindow::clicked_on_new_file()
{
	NewFile *file=new NewFile(pathProject);
	file->show();
}

void MainWindow::clicked_on_new_class()
{
	NewClass *file=new NewClass(pathProject);
	file->show();
}

void MainWindow::clicked_on_save()
{
	if(codeEditor->isEnabled())
	{
		QFile thisFiles(pathProject+"/"+thisFileName);
		thisFiles.open(QIODevice::WriteOnly);
		thisFiles.write((codeEditor->toPlainText()).toUtf8().constData());
		thisFiles.close();
	}
}

void MainWindow::clicked_on_saveAs()
{
	if(codeEditor->isEnabled())
	{
		QString newFileName = QFileDialog::getSaveFileName();
		QFile newFile(newFileName);
		newFile.open(QIODevice::WriteOnly);
		newFile.write((codeEditor->toPlainText()).toUtf8().constData());
		newFile.close();
	}
}

void MainWindow::clicked_on_saveAll()
{
	int i=0;
	Q_FOREACH(auto file,allFilesInProject[0])
	{
		QFile thisFiles(pathProject+"/"+file);
		thisFiles.open(QIODevice::WriteOnly);
		thisFiles.write((allFilesInProject[1][i]).toUtf8().constData());
		thisFiles.close();
		i++;
	}
}

void MainWindow::clicked_on_createNewProject()
{
	SettingsProject *form = new SettingsProject(nullptr,false,nullptr,nullptr);
	form->show();
}

void MainWindow::clicked_on_loadProject()
{
	QFile file("settings.json");//Файл с настроками проекта
	QStringList stringList;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QString pathProjects = all["pathProjects"].toString();

	QString project = QFileDialog::getExistingDirectory(0, "Directory Dialog", pathProjects);
	if(!project.isEmpty())
	{

		QStringList name = project.split("/");

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
												 qMakePair(QString("date"), thisDateTime.toString(Qt::SystemLocaleDate))
											 });
		listProjects.push_front(QJsonValue(newProject));
		all.remove("projectsList");
		all.insert("projectsList", listProjects);

		file.open(QIODevice::WriteOnly);
		file.write(QJsonDocument(all).toJson());//Перезапись файла
		file.close();

		MainWindow *mainWindow= new MainWindow(nullptr,project);
		mainWindow->show();
		this->close();
	}
}

void MainWindow::clicked_on_fullscreen()
{
	if(!this->isFullScreen())
		this->showFullScreen();
	else
		this->showMaximized();
}

void MainWindow::clicked_on_settingsWindow()
{
	SettingsWindow *setting = new SettingsWindow();
	setting->show();
}

void MainWindow::clicked_on_settingsDocker()
{
	DockerSettings *setting = new DockerSettings();
	setting->show();
}

void MainWindow::on_treeProjectFiles_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	thisFileName = item->text(column);
	if(thisFileName != "Заголовочные" && thisFileName != "Исходники" && thisFileName != "Другие")
	{
		codeEditor->setEnabled(true);
		int i=0;
		Q_FOREACH(auto file,allFilesInProject[0])
		{
			if(thisFileName==file)
			{
				break;
			}
			i++;
		}
		thisFileIndex=i;
		codeEditor->setPlainText(allFilesInProject[1][i]);
	}
}

void MainWindow::on_updateFilesTree_clicked()
{
	codeEditor->setEnabled(false);
	ui->treeProjectFiles->clear();
	filesTree();
}

void MainWindow::saves_files_changed()
{
	allFilesInProject[1][thisFileIndex]=codeEditor->toPlainText();
}

void MainWindow::filesTree()
{
	delete [] allFilesInProject;
	QDir filesProj;
	filesProj.setPath(pathProject);
	allFilesInProject = new QStringList[2];
	QStringList nameFiles;
	QStringList textFiles;
	QFile listFile;
	QStringList filtres;
	QFileInfoList list;
	QIcon iconHeader;
	iconHeader.addFile("header.png");
	QIcon iconSource;
	iconSource.addFile("source.png");
	QIcon iconSourceC;
	iconSourceC.addFile("source_c.png");
	QIcon iconGo;
	iconGo.addFile("golang.png");
	if(languageProject == "Go"){
		filtres << "*.go";
		filesProj.setNameFilters(filtres);
		filesProj.setFilter(QDir::Files);
		list = filesProj.entryInfoList();
		if(!list.isEmpty())
		{
			QTreeWidgetItem *itemGoFiles = new QTreeWidgetItem(ui->treeProjectFiles);
			itemGoFiles->setText(0,"Исходники");
			Q_FOREACH(auto file,list)
			{
				QTreeWidgetItem *itemGoFile=new QTreeWidgetItem(itemGoFiles);
				itemGoFile->setText(0, file.fileName());
				itemGoFile->setIcon(0, iconGo);
				nameFiles.append(file.fileName());
				listFile.setFileName(file.filePath());
				listFile.open(QIODevice::ReadOnly);
				textFiles.append(listFile.readAll());
				listFile.close();
			}
		}
		filtres.clear();
	}
	else{
		filtres<< "*.h";
		filesProj.setNameFilters(filtres);
		filesProj.setFilter(QDir::Files);
		list = filesProj.entryInfoList();
		if(!list.isEmpty())
		{
			QTreeWidgetItem *itemHeadline=new QTreeWidgetItem(ui->treeProjectFiles);
			itemHeadline->setText(0,"Заголовочные");
			Q_FOREACH(auto file,list)
			{
				QTreeWidgetItem *itemHeadlineFile=new QTreeWidgetItem(itemHeadline);
				itemHeadlineFile->setText(0,file.fileName());

				itemHeadlineFile->setIcon(0,iconHeader);
				nameFiles.append(file.fileName());
				listFile.setFileName(file.filePath());
				listFile.open(QIODevice::ReadOnly);
				textFiles.append(listFile.readAll());
				listFile.close();
			}
		}
		filtres.clear();
		filtres<< "*.cpp";
		filesProj.setNameFilters(filtres);
		filesProj.setFilter(QDir::Files);
		list = filesProj.entryInfoList();
		QTreeWidgetItem *itemSource = new QTreeWidgetItem(ui->treeProjectFiles);
		itemSource->setText(0,"Исходники");
		if(!list.isEmpty())
		{
			Q_FOREACH(auto file,list)
			{
				QTreeWidgetItem *itemSourcePlusFile=new QTreeWidgetItem(itemSource);
				itemSourcePlusFile->setText(0,file.fileName());
				itemSourcePlusFile->setIcon(0,iconSource);
				nameFiles.append(file.fileName());
				listFile.setFileName(file.filePath());
				listFile.open(QIODevice::ReadOnly);
				textFiles.append(listFile.readAll());
				listFile.close();
			}
		}
		filtres.clear();
		filtres<< "*.c";
		filesProj.setNameFilters(filtres);
		filesProj.setFilter(QDir::Files);
		list = filesProj.entryInfoList();
		if(!list.isEmpty())
		{
			Q_FOREACH(auto file,list)
			{
				QTreeWidgetItem *itemSourceFile=new QTreeWidgetItem(itemSource);
				itemSourceFile->setText(0,file.fileName());
				itemSourceFile->setIcon(0,iconSourceC);
				nameFiles.append(file.fileName());
				listFile.setFileName(file.filePath());
				listFile.open(QIODevice::ReadOnly);
				textFiles.append(listFile.readAll());
				listFile.close();
			}
		}
		filtres.clear();
	}

	filtres<<"*.txt" << "*.json";
	filesProj.setNameFilters(filtres);
	filesProj.setFilter(QDir::Files);
	list = filesProj.entryInfoList();
	if(!list.isEmpty())
	{
		QTreeWidgetItem *itemHeadline=new QTreeWidgetItem(ui->treeProjectFiles);
		itemHeadline->setText(0,"Другие");
		Q_FOREACH(auto file,list)
		{
			QTreeWidgetItem *itemHeadlineFile=new QTreeWidgetItem(itemHeadline);
			itemHeadlineFile->setText(0,file.fileName());
			nameFiles.append(file.fileName());
			listFile.setFileName(file.filePath());
			listFile.open(QIODevice::ReadOnly);
			textFiles.append(listFile.readAll());
			listFile.close();
		}
	}

	QTreeWidgetItem *itemMakeFile = new QTreeWidgetItem(ui->treeProjectFiles);
	itemMakeFile->setText(0, "/build/makefile");
	nameFiles.append("/build/makefile");
	listFile.setFileName(pathProject + "/build/makefile");
	listFile.open(QIODevice::ReadOnly);
	textFiles.append(listFile.readAll());
	listFile.close();

	allFilesInProject[0].append(nameFiles);
	allFilesInProject[1].append(textFiles);
}

void MainWindow::on_startButton_clicked()
{
	clicked_on_saveAll();
	QString nameProject;
	operationCompile(nameProject);

	QFile bat(pathProject+"/build/"+nameProject+".bat");
	if(!bat.exists()){
		bat.open(QIODevice::WriteOnly);
		bat.write("@echo off\n"+nameProject.toUtf8()+".exe\n"
                                                   "pause\n"
                                                   "exit\n");
		bat.close();
	}
	QFile exe(pathProject+"/build/"+nameProject+".exe");
	if(exe.exists()){
		process->setWorkingDirectory(pathProject+"/build/");
		if(QSysInfo::productType()=="windows")
		{
			process->start("cmd.exe",QStringList()<<"/C"<<"start"<<nameProject+".bat\n");
			process->waitForFinished();
		}
	}
}

void MainWindow::on_buildButton_clicked()
{
	clicked_on_saveAll();
	QString nameProject;
	operationCompile(nameProject);
}

void MainWindow::doubleClickList(QListWidgetItem *item)
{
	QTextCursor textCursor = codeEditor->textCursor();
	uint textCursorPosition = textCursor.position();
	QString text = codeEditor->toPlainText();
	if(textCursorPosition > 0){
		textCursorPosition--;
		int i = textCursorPosition;
		while((text[i].unicode() != 32 && text[i].unicode() != 9 && text[i].unicode() != 10) && i > 0){
			i--;
		}
		text.remove(i+1, textCursorPosition-i);
		text.insert(i+1, item->text());
		codeEditor->setPlainText(text);
		textCursor.setPosition(i+1+item->text().size());
		codeEditor->setTextCursor(textCursor);
	}
}

void MainWindow::operationCompile(QString &nameProject)
{
	QStringList nameProj = pathProject.split("/");
	nameProject = nameProj[nameProj.size()-1];
	if(QSysInfo::productType()=="windows")
	{
		//QTextCodec *codec = QTextCodec::codecForLocale();
		ui->consolOutput->append("---------Start compiling---------\n");
		process->start("cmd.exe", QStringList() << "/C" << "docker exec -t compilingblock make -f ./build/makefile");
		process->waitForFinished();
		QString read = process->readAll();
		ui->consolOutput->append(read);
		ui->consolOutput->append("---------Finish compiling---------");
	}
	else
		ui->consolOutput->append(process->readAll());
}

void MainWindow::helpListWidget()
{
	QTextCursor textCursor = codeEditor->textCursor();
	uint textCursorPosition = textCursor.position();
	QString text = codeEditor->toPlainText();
	if(textCursorPosition > 0){
		textCursorPosition--;
		uint i = textCursorPosition;

		if(text[i].unicode() == 10 && newVariant){
			text.remove(i, 1);
			codeEditor->setPlainText(text);
			textCursor.setPosition(i);
			codeEditor->setTextCursor(textCursor);
			doubleClickList(listHelp->currentItem());
			listHelp->setVisible(false);
			newVariant = 0;
		}
		else{

			if((text[i].unicode() == 32 || text[i].unicode() == 9) && listHelp->isVisible()){
				listHelp->setVisible(false);
			}
			while((text[i].unicode() != 32 && text[i].unicode() != 9 && text[i].unicode() != 10) && i > 0){
				i--;
			}
			if(textCursorPosition - i >= 2){
				QRect cursor = codeEditor->cursorRect();
				QPoint positionEditor = codeEditor->mapToGlobal(this->pos());
				QPoint positionCursor = cursor.bottomRight() + positionEditor;

				QStringList list = allKeyWords.filter(text.mid(i + 1, textCursorPosition - i));
				if(!list.isEmpty()){
					listHelp->clear();
					listHelp->addItems(list);
					listHelp->move(positionCursor);
					listHelp->setVisible(true);
					listHelp->setCurrentRow(0);
					newVariant = 1;
				}
			}
		}
	}
}

void MainWindow::newCursorPosition()
{
	if(listHelp->isVisible()){
		listHelp->setVisible(false);
		newVariant = 1;
	}
	else{
		newVariant = 0;
	}
}
