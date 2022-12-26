#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class QTextDocument;

class SyntaxHighlighter: public QSyntaxHighlighter
{
  Q_OBJECT
private:
  QStringList keywords;
  QStringList typesVar;
  QColor keyWordsColor;
  QColor typeColor;
  QColor commentOneColor;
  QColor commentMultyColor;
  QColor stringColor;
  QColor numbersColor;
  QColor gridColor;

protected:
  enum { NormalState = -1, InsideCStyleComment, InsideCString };

  virtual void highlightBlock(const QString&);

  QString getKeyword(int i, const QString& str, QColor &color) const;

public:
  SyntaxHighlighter(QTextDocument* parent=0);
};

#endif // SYNTAXHIGHLIGHTER_H
