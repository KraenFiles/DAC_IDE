#ifndef SETTINGSLANGUAGE_H
#define SETTINGSLANGUAGE_H

#include <QWidget>
#include <QDir>
#include <QTextStream>
#include "mainwindow.h"
#include "settingsproject.h"

namespace Ui {
  class settingsLanguage;
}

class settingsLanguage : public QWidget
{
  Q_OBJECT

public:
  explicit settingsLanguage(QWidget *parent = nullptr, QString nameProject=nullptr, QString wayProject=nullptr,bool checkWindow=true);
  ~settingsLanguage();
  QString startCode;


private slots:
  void on_readyButton_clicked();

  void on_comboBoxLanguage_activated(int index);

  void on_backButton_clicked();

private:
  Ui::settingsLanguage *ui;
};

#endif // SETTINGSLANGUAGE_H
