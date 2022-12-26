#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QMainWindow>
#include <QDateTime>
#include <QListWidgetItem>
#include "settingsproject.h"
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Screensaver; }
QT_END_NAMESPACE

class Screensaver : public QMainWindow
{
  Q_OBJECT

public:
  Screensaver(QWidget *parent = nullptr);
  ~Screensaver();

private slots:
  void on_newProject_clicked();//Загрузка формы для создания нового проекта

  void on_loadProject_clicked();//Загрузка проекта

  void on_listProjects_itemClicked(QListWidgetItem *item);//Загрузка проекта из списка

private:
  Ui::Screensaver *ui;
};
#endif // SCREENSAVER_H
