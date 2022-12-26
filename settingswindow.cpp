#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPainter>
#include <QColorDialog>

#include "clickablelabel.h"

bool them;
bool checkChanges=false;

SettingsWindow::SettingsWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SettingsWindow)
{
	ui->setupUi(this);

	QStringList fonts;
	fonts<<"Hack"<<"DejaVu Sans Mono"<<"Monaco"<<"Source Code Pro"<< "Menlo"<< "Consolas"<< "Space Mono"<< "FiraCode"<< "Anonymous Pro"<< "IBM 3270"<< "Droid Sans Mono";
	ui->fontList->addItems(fonts);

	QFile file("settings.json");//Файл с настроками проекта
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = file.readAll();
	file.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QString font = all["font"].toString();
	int fontSize = all["fontSize"].toInt();
	QString theme = all["theme"].toString();

	ui->fontList->setCurrentText(font);
	ui->sizeFont->setValue(fontSize);
	if(theme == "Dark"){
		them=true;
		ui->darkButton->setEnabled(false);
	}
	else{
		them=false;
		ui->brightButton->setEnabled(false);
	}

	QFile colorFile("KeyWords.json");
	colorFile.open(QIODevice::ReadOnly | QIODevice::Text);
	allColorString = colorFile.readAll();
	colorFile.close();

	json = QJsonDocument::fromJson(allColorString.toUtf8());
	QJsonObject allColors = json.object();
	QJsonObject block = allColors["keyWords"].toObject();
	QJsonObject blockColor = block["keyWordsColor"].toObject();
	QColor* color = new QColor(blockColor["red"].toInt(), blockColor["green"].toInt(), blockColor["blue"].toInt());
	QPixmap pixmap = paintPixmap(color);
	ui->colorKeyWords->setPixmap(pixmap);
	keyWordColor = *color;

	block = allColors["types"].toObject();
	blockColor = block["typeColor"].toObject();
	color = new QColor(blockColor["red"].toInt(), blockColor["green"].toInt(), blockColor["blue"].toInt());
	pixmap = paintPixmap(color);
	ui->colorType->setPixmap(pixmap);
	typeColor = *color;

	blockColor = allColors["commentOneColor"].toObject();
	color = new QColor(blockColor["red"].toInt(), blockColor["green"].toInt(), blockColor["blue"].toInt());
	pixmap = paintPixmap(color);
	ui->colorOneComment->setPixmap(pixmap);
	oneCommentColor = *color;

	blockColor = allColors["commentMultyColor"].toObject();
	color = new QColor(blockColor["red"].toInt(), blockColor["green"].toInt(), blockColor["blue"].toInt());
	pixmap = paintPixmap(color);
	ui->colorMultyComment->setPixmap(pixmap);
	multyCommentColor = *color;

	blockColor = allColors["stringColor"].toObject();
	color = new QColor(blockColor["red"].toInt(), blockColor["green"].toInt(), blockColor["blue"].toInt());
	pixmap = paintPixmap(color);
	ui->colorString->setPixmap(pixmap);
	stringColor = *color;

	blockColor = allColors["numbersColor"].toObject();
	color = new QColor(blockColor["red"].toInt(), blockColor["green"].toInt(), blockColor["blue"].toInt());
	pixmap = paintPixmap(color);
	ui->colorNumbers->setPixmap(pixmap);
	numberColor = *color;

	blockColor = allColors["gridColor"].toObject();
	color = new QColor(blockColor["red"].toInt(), blockColor["green"].toInt(), blockColor["blue"].toInt());
	pixmap = paintPixmap(color);
	ui->colorGrid->setPixmap(pixmap);
	gridColor = *color;

	connect(ui->colorKeyWords, SIGNAL(clicked()), this, SLOT(clicked_Key_Words()));
	connect(ui->colorType, SIGNAL(clicked()), this, SLOT(clicked_Type()));
	connect(ui->colorOneComment, SIGNAL(clicked()), this, SLOT(clicked_OneComment()));
	connect(ui->colorMultyComment, SIGNAL(clicked()), this, SLOT(clicked_MultyComment()));
	connect(ui->colorString, SIGNAL(clicked()), this, SLOT(clicked_String()));
	connect(ui->colorNumbers, SIGNAL(clicked()), this, SLOT(clicked_Number()));
	connect(ui->colorGrid, SIGNAL(clicked()), this, SLOT(clicked_Grid()));
}

SettingsWindow::~SettingsWindow()
{
	delete ui;
}

void SettingsWindow::on_exitButton_clicked()
{
	this->hide();
}

void SettingsWindow::on_applyButton_clicked()
{
	if(checkChanges){
		QFile file("settings.json");//Файл с настроками проекта
		QStringList stringList;
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QString allSettings = file.readAll();
		file.close();

		QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
		QJsonObject all = json.object();

		all.remove("font");
		all.remove("fontSize");
		all.remove("theme");
		if(them)
			all.insert("theme", "Dark");
		else
			all.insert("theme", "Bright");
		QString font = ui->fontList->currentText();
		QString fontSize = ui->sizeFont->text();
		all.insert("font", QJsonValue::fromVariant(font));
		all.insert("fontSize", QJsonValue::fromVariant(fontSize.toInt()));

		file.open(QIODevice::WriteOnly);
		file.write(QJsonDocument(all).toJson());//Перезапись файла
		file.close();

		json = QJsonDocument::fromJson(allColorString.toUtf8());
		QJsonObject allColors = json.object();
		QJsonObject block = allColors["keyWords"].toObject();
		QJsonObject blockColor = block["keyWordsColor"].toObject();

		blockColor.remove("red");
		blockColor.remove("green");
		blockColor.remove("blue");
		blockColor.insert("red", keyWordColor.red());
		blockColor.insert("green", keyWordColor.green());
		blockColor.insert("blue", keyWordColor.blue());

		block.remove("keyWordsColor");
		block.insert("keyWordsColor", blockColor);
		allColors.remove("keyWords");
		allColors.insert("keyWords", block);


		block = allColors["types"].toObject();
		blockColor = block["typeColor"].toObject();

		blockColor.remove("red");
		blockColor.remove("green");
		blockColor.remove("blue");
		blockColor.insert("red", typeColor.red());
		blockColor.insert("green", typeColor.green());
		blockColor.insert("blue", typeColor.blue());

		block.remove("typeColor");
		block.insert("typeColor", blockColor);
		allColors.remove("types");
		allColors.insert("types", block);


		blockColor = block["commentOneColor"].toObject();

		blockColor.remove("red");
		blockColor.remove("green");
		blockColor.remove("blue");
		blockColor.insert("red", oneCommentColor.red());
		blockColor.insert("green", oneCommentColor.green());
		blockColor.insert("blue", oneCommentColor.blue());

		allColors.remove("commentOneColor");
		allColors.insert("commentOneColor", blockColor);


		blockColor = block["commentMultyColor"].toObject();

		blockColor.remove("red");
		blockColor.remove("green");
		blockColor.remove("blue");
		blockColor.insert("red", multyCommentColor.red());
		blockColor.insert("green", multyCommentColor.green());
		blockColor.insert("blue", multyCommentColor.blue());

		allColors.remove("commentMultyColor");
		allColors.insert("commentMultyColor", blockColor);


		blockColor = block["stringColor"].toObject();

		blockColor.remove("red");
		blockColor.remove("green");
		blockColor.remove("blue");
		blockColor.insert("red", stringColor.red());
		blockColor.insert("green", stringColor.green());
		blockColor.insert("blue", stringColor.blue());

		allColors.remove("stringColor");
		allColors.insert("stringColor", blockColor);


		blockColor = block["numbersColor"].toObject();

		blockColor.remove("red");
		blockColor.remove("green");
		blockColor.remove("blue");
		blockColor.insert("red", numberColor.red());
		blockColor.insert("green", numberColor.green());
		blockColor.insert("blue", numberColor.blue());

		allColors.remove("numbersColor");
		allColors.insert("numbersColor", blockColor);


		blockColor = block["gridColor"].toObject();

		blockColor.remove("red");
		blockColor.remove("green");
		blockColor.remove("blue");
		blockColor.insert("red", gridColor.red());
		blockColor.insert("green", gridColor.green());
		blockColor.insert("blue", gridColor.blue());

		allColors.remove("gridColor");
		allColors.insert("gridColor", blockColor);


		QFile colorFile("KeyWords.json");
		colorFile.open(QIODevice::WriteOnly);
		colorFile.write(QJsonDocument(allColors).toJson());
		colorFile.close();

		QMessageBox::information(this, "Объявление", "Изменения вступят в силу после перезагрузки");
	}
	this->hide();
}

void SettingsWindow::on_darkButton_clicked()
{
	them=true;
	ui->darkButton->setEnabled(false);
	ui->brightButton->setEnabled(true);
	checkChanges=true;
}

void SettingsWindow::on_brightButton_clicked()
{
	them=false;
	ui->brightButton->setEnabled(false);
	ui->darkButton->setEnabled(true);
	checkChanges=true;
}

void SettingsWindow::on_fontList_activated(const QString &arg1)
{
	checkChanges=true;
}

void SettingsWindow::on_sizeFont_textChanged(const QString &arg1)
{
	checkChanges=true;
}

void SettingsWindow::clicked_Key_Words()
{
	QColorDialog* dialog = new QColorDialog(this);
	QColor color = dialog->getColor(keyWordColor);
	if(color != nullptr){
		QPixmap pixmap = paintPixmap(&color);
		ui->colorKeyWords->setPixmap(pixmap);
		keyWordColor = color;
		checkChanges=true;
	}
}

void SettingsWindow::clicked_Type()
{
	QColorDialog* dialog = new QColorDialog(this);
	QColor color = dialog->getColor(typeColor);
	if(color != nullptr){
		QPixmap pixmap = paintPixmap(&color);
		ui->colorType->setPixmap(pixmap);
		typeColor = color;
		checkChanges=true;
	}
}

void SettingsWindow::clicked_OneComment()
{
	QColorDialog* dialog = new QColorDialog(this);
	QColor color = dialog->getColor(oneCommentColor);
	if(color != nullptr){
		QPixmap pixmap = paintPixmap(&color);
		ui->colorOneComment->setPixmap(pixmap);
		oneCommentColor = color;
		checkChanges=true;
	}
}

void SettingsWindow::clicked_MultyComment()
{
	QColorDialog* dialog = new QColorDialog(this);
	QColor color = dialog->getColor(multyCommentColor);
	if(color != nullptr){
		QPixmap pixmap = paintPixmap(&color);
		ui->colorMultyComment->setPixmap(pixmap);
		multyCommentColor = color;
		checkChanges=true;
	}
}

void SettingsWindow::clicked_String()
{
	QColorDialog* dialog = new QColorDialog(this);
	QColor color = dialog->getColor(stringColor);
	if(color != nullptr){
		QPixmap pixmap = paintPixmap(&color);
		ui->colorString->setPixmap(pixmap);
		stringColor = color;
		checkChanges=true;
	}
}

void SettingsWindow::clicked_Number()
{
	QColorDialog* dialog = new QColorDialog(this);
	QColor color = dialog->getColor(numberColor);
	if(color != nullptr){
		QPixmap pixmap = paintPixmap(&color);
		ui->colorNumbers->setPixmap(pixmap);
		numberColor = color;
		checkChanges=true;
	}
}

void SettingsWindow::clicked_Grid()
{
	QColorDialog* dialog = new QColorDialog(this);
	QColor color = dialog->getColor(gridColor);
	if(color != nullptr){
		QPixmap pixmap = paintPixmap(&color);
		ui->colorGrid->setPixmap(pixmap);
		gridColor = color;
		checkChanges=true;
	}
}

QPixmap SettingsWindow::paintPixmap(QColor* penColor)
{
	QPixmap pixmap(100,15);
	QPainter painter;

	painter.begin(&pixmap);
		painter.drawRect(0, 0, 100, 15);
		painter.fillRect(0, 0, 100, 15, *penColor);
	painter.end();

	return pixmap;
}
