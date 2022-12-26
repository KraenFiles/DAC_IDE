#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>

namespace Ui {
  class SettingsWindow;
}

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget *parent = nullptr);
  ~SettingsWindow();

private slots:
  void on_exitButton_clicked();

  void on_applyButton_clicked();

  void on_darkButton_clicked();

  void on_brightButton_clicked();

  void on_fontList_activated(const QString &arg1);

  void on_sizeFont_textChanged(const QString &arg1);

  void clicked_Key_Words();
  void clicked_Type();
  void clicked_OneComment();
  void clicked_MultyComment();
  void clicked_String();
  void clicked_Number();
  void clicked_Grid();

private:
  Ui::SettingsWindow *ui;

  QString allColorString;
  QPixmap paintPixmap(QColor* );

  QColor keyWordColor;
  QColor typeColor;
  QColor oneCommentColor;
  QColor multyCommentColor;
  QColor stringColor;
  QColor numberColor;
  QColor gridColor;
};

#endif // SETTINGSWINDOW_H
