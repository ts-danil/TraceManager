#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QBrush>
#include <QPoint>
#include <QColorDialog>

#include "./project/trace/Trace.h"
#include "./project/trace/header_parser.h"

#include "./project/project_struct.h"
#include "./project/project_manager.h"

#include "./plotter/qcustomplot/qcustomplot.h"
#include "./plotter/plotter.h"

#include "./gui/open_file_window.h"
#include "./gui/view_project_window.h"
#include "./gui/property_browser_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void trace_select(int);
    void replot_graph(int);

public slots:
    void update_TableWidget();
    void t_name_update(int index);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_trace_triggered();
    void on_create_project_triggered();
    void on_save_project_triggered();
    void on_save_as_triggered();
    void on_open_project_triggered();
    void on_close_project_triggered();
    void on_trace_tableWidget_itemSelectionChanged();
    void remove_trace();
    void add_trace();

    void on_trace_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    QCustomPlot *mPlot = nullptr;
    Plotter *plotter = nullptr;
    Project_struct *project = nullptr;
    Property_browser_widget *property_browser = nullptr;
    bool is_update = false;

    void color_dialog(int trc_index);
    void dockWidget_clear();


};
#endif // MAINWINDOW_H
