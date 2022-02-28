#ifndef STYLEJSONFILE_H
#define STYLEJSONFILE_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QDebug>

#include "common/common.h"

/*!
 * \brief The StyleJsonFile class
 * 该类线程安全
 */
class StyleJsonFilePrivate;
class StyleJsonFile : public QObject
{
    Q_OBJECT
    StyleJsonFilePrivate *const d;
public:
    enum_def(Key_1, QString)
    {
        enum_exp Self = "Self";
        enum_exp Namespace = "Namespace";
        enum_exp Type = "Type";
        enum_exp Class = "class";
        enum_exp Variable = "Variable";
        enum_exp Property = "Property";
        enum_exp Text = "Text";
        enum_exp Link = "Link";
        enum_exp Selection = "Selection";
        enum_exp LineNumber = "Line Number";
        enum_exp Keyword = "Keyword";
        enum_exp Punctuation = "Punctuation";
        enum_exp Operators = "Operators";
        enum_exp OverloadedOperators = "Overloaded Operators";
        enum_exp Preprocessor = "Preprocessor";
        enum_exp SearchResult = "Search Result";
        enum_exp SearchScope = "Search Scope";
        enum_exp Parentheses = "Parentheses";
        enum_exp MismatchedParentheses = "Mismatched Parentheses";
        enum_exp AutoComplete = "Auto Complete";
        enum_exp CurrentLine = "Current Line";
        enum_exp CurrentLineNumber = "Current Line Number";
        enum_exp Occurrences = "Occurrences";
        enum_exp UnusedOccurrences = "Unused Occurrences";
        enum_exp RenamingOccurrences = "Renaming Occurrences";
        enum_exp Number = "Number";
        enum_exp String = "String";
        enum_exp PrimitiveType = "Primitive Type";
        enum_exp Local = "Local";
        enum_exp Field = "Field";
        enum_exp Global = "Global";
        enum_exp Enumeration = "Enumeration";
        enum_exp Function = "Function";
        enum_exp FunctionDeclaration = "Function Declaration";
        enum_exp FunctionDefinition = "Function Definition";
        enum_exp VirtualFunction = "Virtual Function";
        enum_exp Comment = "Comment";
        enum_exp ErrorContext = "Error Context";
        enum_exp Warning = "Warning";
        enum_exp WarningContext = "Warning Context";
    };

    enum_def(Key_2, QString)
    {
        enum_exp Background = "Background";
        enum_exp Foreground = "Foreground";
        enum_exp Cursor = "Cursor";
        enum_exp FontSize = "FontSize";
        enum_exp UnderLine = "UnderLine";
    };

    enum_def(Key_3, QString)
    {
        enum_exp Color = "Color";
        enum_exp Style = "Style";
    };

    StyleJsonFile();
    virtual ~StyleJsonFile();
    bool setLanguage(const QString &languageID);
    QStringList themes();
    bool setTheme(const QString &theme);
    QJsonValue value(const QString &Key) const;
};

#endif // STYLEJSONFILE_H