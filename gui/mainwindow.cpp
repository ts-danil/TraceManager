#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);

    project = new Project_struct;
    mPlot = new QCustomPlot();
    plotter = new Plotter(mPlot, &project->trace_list);
    this->setWindowTitle("New project");

    ui->horizontalLayout->addWidget(mPlot);
    //mPlot->xAxis->setRange(0, 0.0003);
    //mPlot->yAxis->setRange(-0.05, 0.05);
    mPlot->setInteraction(QCP::iRangeDrag, true);
    mPlot->setInteraction(QCP::iRangeZoom, true);
    //mPlot->axisRect()->setRangeZoomAxes(mPlot->xAxis, nullptr);

    property_browser = new Property_browser_widget(ui->multi_widget_Layout, &project->trace_list);


    connect(this, &MainWindow::trace_select,
            plotter, &Plotter::set_trace);
    connect(this, &MainWindow::trace_select,
            property_browser, &Property_browser_widget::set_trace);

    connect(property_browser, &Property_browser_widget::t_name_changed,
            this, &MainWindow::t_name_update);

    connect(property_browser, &Property_browser_widget::replot_graph,
            plotter, &Plotter::update_graph);

    connect(this, &MainWindow::replot_graph,
            plotter, &Plotter::update_graph);


    ui->trace_tableWidget->addAction(ui->add_trace);
    ui->trace_tableWidget->addAction(ui->actionRemoveTrace);

    ui->actionRemoveTrace->setEnabled(false);
    connect(ui->actionRemoveTrace, &QAction::triggered,
            this, &MainWindow::remove_trace);

    ui->trace_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->trace_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->trace_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->trace_tableWidget->resizeColumnToContents(0);
    ui->trace_tableWidget->resizeColumnToContents(1);
    ui->trace_tableWidget->horizontalHeader()->setStretchLastSection(true);

    //ui->trace_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui->trace_tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::custom_menu_requested);

    }

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::add_trace
 */
void MainWindow::add_trace()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*.*);;CSV Files (*.csv);;Text Files (*.txt)"));
    if (file_path != "")
    {
        Header_parser header(&file_path);
        QVector<int> csv_index;
        QVector<int> for_draw;
        if (header.columns_count() == 2)
        {
            csv_index.append(0);
        }
        else
        {
            Open_file_window open_file(&file_path, &csv_index);
            open_file.exec();
        }
        for (int i = 0; i < csv_index.length(); i++)
        {
            project->trace_list.append(new Trace(file_path, csv_index.at(i)));
            for_draw.append(project->trace_list.length()-1);
        }
        plotter->set_colors(&for_draw);
        plotter->draw_graphs(&for_draw);
        update_TableWidget();
    }
}

/*!
 * \brief MainWindow::remove_trace
 */
void MainWindow::remove_trace()
{
    int selected_trace = ui->trace_tableWidget->currentRow();
    project->trace_list.remove(selected_trace);
    plotter->remove_graph(selected_trace);
    if(project->trace_list.length() > 0)
    {
        ui->trace_tableWidget->selectRow(selected_trace-1);
        update_TableWidget();
    }
    else
    {
        dockWidget_clear();
    }
}

/*!
 * \brief MainWindow::update_TableWidget
 */
void MainWindow::update_TableWidget()
{
    ui->trace_tableWidget->clearContents();
    ui->trace_tableWidget->setRowCount(project->trace_list.length());
    QBrush cell_background(Qt::SolidPattern);
    for (int i = 0; i < project->trace_list.length(); i++)
    {
        QTableWidgetItem *color = new QTableWidgetItem;
        cell_background.setColor(project->trace_list.at(i)->get_color());
        color->setBackground(cell_background);
        ui->trace_tableWidget->setItem(i, 0, color);


        QCheckBox *show_checkBox = new QCheckBox;
        if (project->trace_list.at(i)->get_show())
        {
            show_checkBox->setCheckState(Qt::Checked);
        }
        connect(show_checkBox, &QCheckBox::toggled,
                this, [this, i](bool state){
            project->trace_list.at(i)->set_show(state);
            emit replot_graph(i);
        });
        ui->trace_tableWidget->setCellWidget(i, 1, show_checkBox);

        QTableWidgetItem *name = new QTableWidgetItem;
        name->setText(project->trace_list.at(i)->get_t_name_user());
        ui->trace_tableWidget->setItem(i, 2, name);
    }
}


/*!
 * \brief MainWindow::color_dialog
 * \param trc_index
 */
void MainWindow::color_dialog(int trc_index)
{
    QColor current_color = project->trace_list.at(trc_index)->get_color();

    QColorDialog *color_window = new QColorDialog(current_color, this);
    connect(color_window, &QColorDialog::colorSelected,
            project->trace_list.at(trc_index), &Trace::set_color);
    connect(color_window, &QColorDialog::rejected, color_window, &QColorDialog::deleteLater);
    connect(color_window, &QColorDialog::accepted, color_window, &QColorDialog::deleteLater);

    color_window->exec();
    update_TableWidget();
    emit replot_graph(trc_index);
}

/*!
 * \brief MainWindow::dockWidget_clear
 */
void MainWindow::dockWidget_clear()
{
    ui->trace_tableWidget->setRowCount(0);
    property_browser->clear();
}

/*!
 * \brief MainWindow::t_name_update
 * \param index
 */
void MainWindow::t_name_update(int index)
{
    ui->trace_tableWidget->item(index,2)->setText(project->trace_list.at(index)->get_t_name_user());
}

/*!
 * \brief MainWindow::on_add_trace_triggered
 * Method for "Add trace.." button.
 * Add trace, if it one in file.
 * Open "Open_file_window", if file have a few traces, to select necessary.
 * Trace add in project with "add_trace" method.
 */
void MainWindow::on_add_trace_triggered()
{
    add_trace();
}

/*!
 * \brief MainWindow::on_create_project_triggered
 * Method for "Create project.." button.
 * Open "View_project_window", if other project not open.
 */
void MainWindow::on_create_project_triggered()
{
    if((project->name == "") && (project->path == "") && (project->trace_list.length() == 0))
    {
        View_project_window window(project);
        window.exec();
        Project_manager::save(project);
    }
    else
    {
        qDebug() << "Сохранить текущий?";
    }
    this->setWindowTitle(project->name);
}

/*!
 * \brief MainWindow::on_save_project_triggered
 * Method for "Save" button.
 * Save existing project.
 * Open "View_project_window", if project does not exist.
 */
void MainWindow::on_save_project_triggered()
{
    if ((project->name == "") && (project->path == ""))
    {
        View_project_window window(project);
        window.exec();
        this->setWindowTitle(project->name);
    }
    Project_manager::save(project);
}

/*!
 * \brief MainWindow::on_save_as_triggered
 * Method for "Save as.." button.
 * Open "View_project_window" and save project with new name and path project.
 */
void MainWindow::on_save_as_triggered()
{
    View_project_window window(project);
    window.exec();
    Project_manager::save(project);
    this->setWindowTitle(project->name);
}

/*!
 * \brief MainWindow::on_close_project_triggered
 * Method for "Close project" button.
 * Clear current project name, path and trace_list.
 */
void MainWindow::on_close_project_triggered()
{
    project->name = "";
    project->path = "";
    plotter->clear_graphs();
    project->trace_list.clear();
    dockWidget_clear();
    this->setWindowTitle("New project");
}

/*!
 * \brief MainWindow::on_open_project_triggered
 * Method for "Open Project.." button.
 * Open QFileDialog for select project file (XML).
 * Call "Project_manager::open", draw graps.
 */
void MainWindow::on_open_project_triggered()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("XML Files (*.xml);;All Files (*.*)"));
    if (file_path != "")
    {
        Project_manager::open(project, &file_path);
    }
    plotter->set_colors();
    plotter->draw_graphs();
    update_TableWidget();
    this->setWindowTitle(project->name);
}

/*!
 * \brief MainWindow::on_trace_tableWidget_itemSelectionChanged
 */
void MainWindow::on_trace_tableWidget_itemSelectionChanged()
{
    int current_row = ui->trace_tableWidget->currentRow();
    //dockWidget_clear emit itemSelectionChanged with current_row = -1
    ui->actionRemoveTrace->setEnabled(!ui->trace_tableWidget->selectedItems().isEmpty());
    if (current_row >= 0)
    {
        emit trace_select(current_row);
    }
}

/*!
 * \brief MainWindow::on_trace_tableWidget_cellDoubleClicked
 * \param row
 * \param column
 */
void MainWindow::on_trace_tableWidget_cellDoubleClicked(int row, int column)
{
    if(column == 0)
    {
        color_dialog(row);
    }

}
