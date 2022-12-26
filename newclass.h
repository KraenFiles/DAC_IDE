#ifndef NEWCLASS_H
#define NEWCLASS_H

#include <QWidget>
#include <QFile>

namespace Ui {
  class NewClass;
}

class NewClass : public QWidget
{
  Q_OBJECT

public:
  explicit NewClass(QString path,QWidget *parent = nullptr);
  ~NewClass();

private slots:
  void on_editNameClass_textEdited(const QString &arg1);

  void on_cancelButton_clicked();

  void on_readyButton_clicked();

private:
  Ui::NewClass *ui;
};

#endif // NEWCLASS_H
