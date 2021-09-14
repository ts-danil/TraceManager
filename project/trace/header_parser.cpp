#include "header_parser.h"

/*!
 * \brief Header_parser::Header_parser
 * Class constructor for work with header in csv file
 * \param file_p
 * Takes the path to csv file
 */
Header_parser::Header_parser(QString *file_p)
{
    file_path = *file_p;
    header = read();
}

/*!
 * \brief Header_parser::read
 * Method for read header, defines header before line with numerical values.
 * Used in class constructor and write in variable.
 * \return
 * Returns a list with header lines
 */
QStringList Header_parser::read()
{
    QStringList result;
    QString str;
    QStringList split_str;
    bool number_str = false;
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    while (number_str == false)
    {
        str = file.readLine();
        split_str = str.split(QLatin1Char(','));
        for (int i = 0; i < split_str.length(); i++)
        {
            split_str.at(i).toDouble(&number_str);
            if(number_str == false){break;}
        }
        if(number_str == false)
        {
            result.append(str.simplified());
        }
    }
    file.close();
    return result;
}


/*!
 * \brief Header_parser::get
 * Method for get header in QMap with key, value. It not return empty line and line with number of values not equal to two.
 * \return
 * Returns Qmap with key, value.
 */
QMap<QString, QString> Header_parser::get()
{
    QMap<QString, QString> result;
    QStringList split_line;
    for (int i = 0; i < header.length(); i++)
    {
        split_line = header.at(i).split(QLatin1Char(','));
        if (split_line.length() == 2)
        {
            result.insert(split_line.at(0).simplified(), split_line.at(1).simplified());
        }
    }
    return result;
}


/*!
 * \brief Header_parser::get_columns_headers
 * Method for get columns headers from file.
 * It looks for a string with upcase and number of values comma separated, how number of columns in file.
 * If header or necessary string not found, method returns default value from "default_headers".
 * If necessary string found, but it have empty value, this value named "Untitled".
 * \return
 * Returns a list with columns headers
 */
QStringList Header_parser::get_columns_headers()
{
    QStringList result;
    int col_count = columns_count();
    //if file without header, get default_headers
    if (header.length() == 0)
    {
        result = default_headers(col_count);
    }
    else
    {
        QString str;
        QStringList split_str;
        //Cycle from last line header
        QStringList::reverse_iterator i;
        for (i = header.rbegin(); i != header.rend(); i++)
        {
            str = *i;
            split_str = str.split(QLatin1Char(','));
            //Checking number of value and upcase
            if (split_str.length() == col_count && str == str.toUpper())
            {
                result = split_str;
                break;
            }
        }
        //If necessary line found, filling empty value
        if (result.length() != 0)
        {
            for (int i = 0; i < result.length(); i++)
            {
                if (result.at(i) == "")
                {
                    result[i] = "Untitled";
                }
            }
        }
        //If necessary line not found, get default_headers
        else
        {
            result = default_headers(col_count);
        }
    }
    return result;
}

/*!
 * \brief Header_parser::default_headers
 * Method for get default name headers.
 * First column is "TIME" and other is "CH.." with number.
 * \param count - quantity columns in file
 * \return
 * Returns a list with default headers names
 */
QStringList Header_parser::default_headers(int count)
{
    QStringList result;
    result.append("TIME");
    for (int i = 0; i < count - 1; i++)
    {
        result.append("CH" + QString::number(i+1));
    }
    return result;
}

/*!
 * \brief Header_parser::lines_count
 * Method for get number of header lines.
 * \return
 * Returns number of lines.
 */
int Header_parser::lines_count()
{
    return header.length();
}

/*!
 * \brief Header_parser::column_count
 * Method for get number of columns.
 * \return
 */
int Header_parser::columns_count()
{
    QString value_str;
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    for (int i = 0; i < header.length(); i++) {file.readLine();}
    value_str = file.readLine();
    return value_str.split(QLatin1Char(',')).length();
}


