#ifndef DOCKERSETTINGS_H
#define DOCKERSETTINGS_H

#include <QWidget>

namespace Ui {
class DockerSettings;
}

class DockerSettings : public QWidget
{
	Q_OBJECT

public:
	explicit DockerSettings(QWidget *parent = nullptr);
	~DockerSettings();

private slots:
	void on_backButton_clicked();

	void on_enterButton_clicked();

private:
	Ui::DockerSettings *ui;
};

#endif // DOCKERSETTINGS_H
