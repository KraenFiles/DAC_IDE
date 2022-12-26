#include "screensaver.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);//Создание приложение

	qApp->setStyle(QStyleFactory::create("Fusion"));//Установка начального вида приложения
	/*Подключение новых шрифтов*/
	QFontDatabase::addApplicationFont("fonts/hack-regular.ttf");
	QFontDatabase::addApplicationFont("fonts/Menlo-Regular.ttf");
	QFontDatabase::addApplicationFont("fonts/Monaco.ttf");
	QFontDatabase::addApplicationFont("fonts/3270Medium.ttf");
	QFontDatabase::addApplicationFont("fonts/anonymous-pro.ttf");
	QFontDatabase::addApplicationFont("fonts/consola.ttf");
	QFontDatabase::addApplicationFont("fonts/dejavu-sans-mono.ttf");
	QFontDatabase::addApplicationFont("fonts/droid-sans-mono.ttf");
	QFontDatabase::addApplicationFont("fonts/FiraCode-Regular.otf");
	QFontDatabase::addApplicationFont("fonts/SourceCodePro-Regular.otf");
	QFontDatabase::addApplicationFont("fonts/SpaceMono-Regular.ttf");
	/*-------------------------*/
	Screensaver w;//Создание стартового окна
	w.setFixedSize(QSize(650,400));//Установка фиксированного размера окна
	w.show();//Отображение окна

	return a.exec();//Закрытие приложения
}
