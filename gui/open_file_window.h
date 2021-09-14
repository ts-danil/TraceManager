#ifndef OPEN_FILE_WINDOW_H
#define OPEN_FILE_WINDOW_H

#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include "./project/trace/header_parser.h"

namespace Ui {
class Open_file_window;
}

class Open_file_window : public QDialog
{
    Q_OBJECT

public:
    Open_file_window(QString *file_path, QVector<int> *index_in_csv, QWidget *parent = nullptr);
    ~Open_file_window();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_select_all_checkBox_stateChanged(int arg1);

private:
    Ui::Open_file_window *ui;
    QVector<int> *list_of_selected;
    void set_info(QString *file_path);
    void create_checkbox(QString *file_path);
};

#endif // OPEN_FILE_WINDOW_H
