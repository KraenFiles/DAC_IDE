#ifndef NEWFILE_H
#define NEWFILE_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>

namespace Ui {
  class NewFile;
}

class NewFile : public QWidget
{
  Q_OBJECT

public:
  explicit NewFile(QString path, QWidget *parent = nullptr);
  ~NewFile();

private slots:
  void on_readyButton_clicked();

  void on_backButton_clicked();

  void on_nameNewFile_textEdited(const QString &arg1);

private:
  Ui::NewFile *ui;
  QString pathProj;
};

#endif // NEWFILE_H
