#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "^([lL]([aA]([yY]([eE][rR]?)?)?)?)\\b"
                    << "^([cC]([oO]([nN]([tT]([aA]([cC][tT]?)?)?)?)?)?)\\b"
                    << "^([mM]([aA]([tT]([fF]([iI]([lL][eE]?)?)?)?)?)?)\\b"
                    << "^([sS]([cC]([hH]([rR][oO]?)?)?)?)\\b"
                    << "^([pP]([oO]([lL]([aA][rR]?)?)?)?)\\b"
                    << "^([tT]([eE]([mM][pP]?)?)?)\\b";
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    paramFormat.setForeground(Qt::darkRed);
    paramFormat.setFontWeight(QFont::Normal);
    QStringList paramPatterns;
    paramPatterns << "\\b(d)=" << "\\b(d{0,2}[XY])=" << "\\b(tern[XY])="
                  << "\\b(d{0,2}Ef[np])=" << "\\b(d{0,2}E[da]\\S{1,})="
                  << "\\b(d{0,2}N[da]\\S{1,})="
                  << "\\b(scale|sub)=";
    foreach (const QString &pattern, paramPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = paramFormat;
        highlightingRules.append(rule);
    }

    equalFormat.setFontWeight(QFont::ExtraBold);
    equalFormat.setForeground(Qt::darkYellow);
    rule.pattern = QRegExp("\\B(=)\\B");
    rule.format = equalFormat;
    highlightingRules.append(rule);

    numberFormat.setFontWeight(QFont::Normal);
    numberFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("([\\+\\-]?([0-9]+|[0-9]*\\.[0-9]+)(|[eE][\\-\\+]?[0-9]+))(\\s|$)");
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}
