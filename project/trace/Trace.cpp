#include "Trace.h"

/*!
 * \brief Trace::Trace
 * Trace class constructor.
 * Trace has path to file, index in file, name in file, name from user,
 * color value, offset and scale value.
 * This method used when add new trace from file.
 * Uses set_trace_data.
 * \param file_path - Path to file
 * \param index - Trace index in file.
 */
Trace::Trace(QString file_path, int index, QObject *parrent):QObject(parrent)
{
    set_trace_data(&file_path, &index);
}

/*!
 * \brief Trace::Trace
 * Trace class constructor.
 * This overloaded constructor for add trace from existing project.
 * \param trace_name_user - Trace name from user.
 * \param file_path - Path to file with Trace.
 * \param index_in_csv - Trace index in file.
 * \param x_off - X offset value.
 * \param y_off - Y offset value.
 * \param x_scl - X scale value.
 * \param y_scl - Y scale value.
 */
Trace::Trace(QString trace_name_user, QString file_path, int index_in_csv,
             double x_off, double y_off, double x_scl, double y_scl, QObject *parrent):QObject(parrent)
{
    set_t_name_user(trace_name_user);
    set_csv_path(file_path);
    set_csv_index(index_in_csv);
    set_x_offset(x_off);
    set_y_offset(y_off);
    set_x_scale(x_scl);
    set_y_scale(y_scl);
}

/*!
 * \brief Trace::set_trace_data
 * Method for add info in emty Trace.
 * \param file_path - Path to file.
 * \param index - Trace index in file.
 */
void Trace::set_trace_data(QString *file_path, int *index)
{
    if (QFile::exists(*file_path) == true)
    {
        csv_path = *file_path;
        csv_index = *index;
        set_info_from_file();
        QFileInfo fi(*file_path);
        t_name_user = t_name_csv + ", " + fi.baseName();
    }

}

void Trace::set_info_from_file()
{
    Header_parser head(&csv_path);
    header_lines_count = head.lines_count();
    t_name_csv = head.get_columns_headers().at(csv_index + 1);
}


void Trace::set_color(QColor new_color)
{
    color = new_color;
}

/*!
 * \brief Trace::get_data
 * \param time
 * \param value
 * \return
 */
bool Trace::get_plot_points(QVector<double> *time, QVector<double> *value)
{
    time->clear();
    value->clear();
    if (QFile::exists(csv_path) == true)
    {
        QFile file(csv_path);
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        QList<QByteArray> data_list = data.split('\n');
        QList<QByteArray> split_str;
        double x, y;
        for (int i = header_lines_count+1; i < data_list.length(); i++)
        {
            if (data_list.at(i) != "")
            {
                split_str = data_list.at(i).split(',');
                x = split_str[0].toDouble() * x_scale + x_offset;
                y = split_str[csv_index+1].toDouble() * y_scale + y_offset;
                time->append(x);
                value->append(y);
            }
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool Trace::get_points(QVector<double> *time, QVector<double> *value)
{
    time->clear();
    value->clear();
    if (QFile::exists(csv_path) == true)
    {
        QFile file(csv_path);
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        QList<QByteArray> data_list = data.split('\n');
        QList<QByteArray> split_str;
        double x, y;
        for (int i = header_lines_count+1; i < data_list.length(); i++)
        {
            if (data_list.at(i) != "")
            {
                split_str = data_list.at(i).split(',');
                x = split_str[0].toDouble();
                y = split_str[csv_index+1].toDouble();
                time->append(x);
                value->append(y);
            }
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
