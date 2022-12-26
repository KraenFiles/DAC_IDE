#ifndef SETTINGSPROJECT_H
#define SETTINGSPROJECT_H

#include "settingslanguage.h"
#include "screensaver.h"
#include <QFile>
#include <QFileDialog>

namespace Ui {
  class SettingsProject;
}

class SettingsProject : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsProject(QWidget *parent = nullptr, bool checkWindow=true, QString nameProject=nullptr, QString wayProject=nullptr);
  ~SettingsProject();

private slots:
  void on_nextButton_clicked();//Переход к следующему этапу настройки создаваемого проекта

  void on_overviewButton_clicked();//Выбор пути размещения проекта

  void on_backButton_clicked();//Возвращение к предыдущему окну

  void on_lineEditName_textEdited(const QString &arg1);//Проверка правильности ввода в зависимости от внесенных изменений

private:
  Ui::SettingsProject *ui;

  bool checkOpenWindow;//Переменная для проверки предыдущего окна
};

#endif // SETTINGSPROJECT_H
