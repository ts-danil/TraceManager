#ifndef HEADER_PARSER_H
#define HEADER_PARSER_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QFile>

class Header_parser
{
public:
    Header_parser(QString *file_p);
    QMap<QString, QString> get();
    QStringList get_columns_headers();
    int lines_count();
    int columns_count();

private:
    QString file_path;
    QStringList header;
    QStringList read();
    QStringList default_headers(int count);


};

#endif // HEADER_PARSER_H
