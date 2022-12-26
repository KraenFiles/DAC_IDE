#include "syntaxhighlighter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent):QSyntaxHighlighter(parent)
{
	QFile wordsSettings("KeyWords.json");//Файл с настройками среды
	wordsSettings.open(QIODevice::ReadOnly | QIODevice::Text);
	QString allSettings = wordsSettings.readAll();
	wordsSettings.close();

	QJsonDocument json = QJsonDocument::fromJson(allSettings.toUtf8());
	QJsonObject all = json.object();
	QJsonObject listKeyWordsSettings = all["keyWords"].toObject();
	QJsonArray listKeyWords = listKeyWordsSettings["words"].toArray();
	foreach(const QJsonValue& value, listKeyWords){
		keywords << value.toString();
	}
	QJsonObject color = listKeyWordsSettings["keyWordsColor"].toObject();
	int red = color["red"].toInt();
	int green = color["green"].toInt();
	int blue = color["blue"].toInt();
	keyWordsColor = QColor(red, green, blue);

	QJsonObject listTypesSettings = all["types"].toObject();
	QJsonArray listTypes = listTypesSettings["words"].toArray();
	foreach(const QJsonValue& value, listTypes){
		typesVar << value.toString();
	}
	color = listTypesSettings["typeColor"].toObject();
	red = color["red"].toInt();
	green = color["green"].toInt();
	blue = color["blue"].toInt();
	typeColor = QColor(red, green, blue);

	color = all["commentOneColor"].toObject();
	red = color["red"].toInt();
	green = color["green"].toInt();
	blue = color["blue"].toInt();
	commentOneColor = QColor(red, green, blue);

	color = all["commentMultyColor"].toObject();
	red = color["red"].toInt();
	green = color["green"].toInt();
	blue = color["blue"].toInt();
	commentMultyColor = QColor(red, green, blue);

	color = all["stringColor"].toObject();
	red = color["red"].toInt();
	green = color["green"].toInt();
	blue = color["blue"].toInt();
	stringColor = QColor(red, green, blue);

	color = all["numbersColor"].toObject();
	red = color["red"].toInt();
	green = color["green"].toInt();
	blue = color["blue"].toInt();
	numbersColor = QColor(red, green, blue);

	color = all["gridColor"].toObject();
	red = color["red"].toInt();
	green = color["green"].toInt();
	blue = color["blue"].toInt();
	gridColor = QColor(red, green, blue);
}


void SyntaxHighlighter::highlightBlock(const QString & str)
{
	int nState = previousBlockState();
	int nStart = 0;
	for (int i=0;i<str.length();++i) {
		if (nState == InsideCStyleComment)
		{
			if(str.mid(i,2)=="*/")
			{
				nState=NormalState;
				setFormat(nStart,i-nStart+2, commentMultyColor);
				i++;
			}
		}
		else if(nState == InsideCString){
			if(str.mid(i,1)=="\""|| str.mid(i,1)=="\'")
			{
				if(str.mid(i-1,2)!="\\\"" && str.mid(i-1,2)!="\\\'")
				{
					nState=NormalState;
					setFormat(nStart,i-nStart+1, stringColor);
				}
			}
		}
		else
		{
			if(str.mid(i,2)=="//")
			{
				setFormat(i,str.length()-i, commentOneColor);
				break;
			}
			else if(str.mid(i,1)=="#")
			{
				setFormat(i,str.length()-i, gridColor);
				break;
			}
			else if (str.at(i).isNumber()){
				setFormat(i,1, numbersColor);
			}
			else if (str.mid(i,2)=="/*")
			{
				nStart=i;
				nState=InsideCStyleComment;
			}
			else if(str.mid(i,1)=="\""||str.mid(i,1)=="\'"){
				nStart=i;
				nState=InsideCString;
			}
			else {
				QColor colorThisWord;
				QString strKeyword = getKeyword(i,str,colorThisWord);
				if(!strKeyword.isEmpty()){
					setFormat(i,strKeyword.length(),colorThisWord);
					i+=strKeyword.length()-1;
				}
			}
		}
	}
	if(nState==InsideCStyleComment)
		setFormat(nStart,str.length()-nStart, commentMultyColor);
	if(nState==InsideCString)
		setFormat(nStart,str.length()-nStart, stringColor);
	setCurrentBlockState(nState);
}

QString SyntaxHighlighter::getKeyword(int i, const QString &str, QColor &color) const
{
	QString strTemp="";
	bool checkGroup=false;
	foreach(QString strKeyword, typesVar)
	{
		if(str.mid(i,strKeyword.length()) == strKeyword)
		{
			strTemp = strKeyword;
			color = typeColor;
			checkGroup = true;
			break;
		}
	}
	if(!checkGroup){
		foreach(QString strKeyword, keywords)
		{
			if(str.mid(i,strKeyword.length()) == strKeyword)
			{
				strTemp = strKeyword;
				color = keyWordsColor;
				break;
			}
		}
	}
	return strTemp;
}
