#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QDir>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QDateTime>
#include <QtCore>
#include <QListWidget>
#include "codeeditor.h"
#include "treemodel.h"
#include "newfile.h"
#include "newclass.h"
#include "settingsproject.h"
#include "settingswindow.h"
#include "syntaxhighlighter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr, QString projectPath=nullptr, QString projectLanguage = nullptr);
	~MainWindow();
	QString pathProject;
	QString languageProject;
	CodeEditor *codeEditor;
	QStringList *allFilesInProject;
	QListWidget* listHelp;

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void on_treeProjectFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_updateFilesTree_clicked();

	void clicked_on_new_file();
	void clicked_on_new_class();
	void clicked_on_save();
	void clicked_on_saveAs();
	void clicked_on_saveAll();
	void clicked_on_createNewProject();
	void clicked_on_loadProject();
	void clicked_on_fullscreen();
	void clicked_on_settingsWindow();
	void clicked_on_settingsDocker();

	void saves_files_changed();

	void on_startButton_clicked();
	void on_buildButton_clicked();
	void doubleClickList(QListWidgetItem *item);

private:
	Ui::MainWindow *ui;
	QString thisFileName;
	bool newVariant;
	int thisFileIndex;
	void filesTree();
	void operationCompile(QString &nameProject);
	void helpListWidget();
	void newCursorPosition();

	QProcess *process;
	QStringList allKeyWords;
};

#endif // MAINWINDOW_H
