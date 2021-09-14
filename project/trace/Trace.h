#ifndef TRACE_H
#define TRACE_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QMap>
#include <QColor>
#include <QElapsedTimer>

#include "./project/trace/header_parser.h"

class Trace : public QObject
{

public slots:
    void set_color(QColor new_color);

public:
    Trace() {}
    Trace(QString file_path, int index, QObject *parrent = nullptr);
    Trace(QString trace_name_user, QString file_path, int index_in_csv, double x_off, double y_off, double x_scl, double y_scl, QObject *parrent = nullptr);
    void set_trace_data(QString *file_path, int *index);

    void set_t_name_user(QString name){t_name_user = name;}
    void set_csv_path(QString new_path){csv_path = new_path;}
    void set_csv_index(int csv_ind){csv_index = csv_ind;}
    void set_x_offset(double x){x_offset = x;};
    void set_y_offset(double y){y_offset = y;};
    void set_x_scale(double x){x_scale = x;};
    void set_y_scale(double y){y_scale = y;};

    void set_info_from_file();
    void set_show(bool value){show = value;};

    bool get_plot_points(QVector<double> *time, QVector<double> *value);
    bool get_points(QVector<double> *time, QVector<double> *value);


    QString get_t_name_user(){return t_name_user;}
    QString get_csv_path(){return csv_path;}
    int get_csv_index(){return csv_index;}
    double get_x_offset(){return x_offset;}
    double get_y_offset(){return y_offset;}
    double get_x_scale(){return x_scale;}
    double get_y_scale(){return y_scale;}
    QColor get_color(){return color;}

    QString get_t_name_csv(){return t_name_csv;}
    bool get_show(){return show;}

private:
    QString t_name_user;
    QString csv_path = "";
    int csv_index;
    double x_offset = 0.0, y_offset = 0.0, x_scale = 1.0, y_scale = 1.0;

    QString t_name_csv;
    int header_lines_count;
    QColor color;
    bool show = true;


};

#endif // TRACE_H
