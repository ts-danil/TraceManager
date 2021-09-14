#include "view_project_window.h"
#include "ui_view_project_window.h"

/*!
 * \brief View_project_window::View_project_window
 * Class constructor for view project window.
 * Used for setting name and path to project.
 * \param proj - Main struct project
 * \param parent - parent window.
 */
View_project_window::View_project_window(Project_struct *proj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::View_project_window)
{
    ui->setupUi(this);
    project = proj;

    if(project->name != "" && project->path != "")
    {
        ui->project_name_lineEdit->setText(project->name);
        ui->project_path_lineEdit->setText(project->path);
    }

}

View_project_window::~View_project_window()
{
    delete ui;
}

/*!
 * \brief View_project_window::on_browse_path_pushButton_clicked
 * Browse path button click handler.
 * Used for setting path to project file.
 */
void View_project_window::on_browse_path_pushButton_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Save project"), ui->project_path_lineEdit->text() , tr("XML file (*.xml)"));
    if (file_path != "")
    {
        ui->project_path_lineEdit->setText(file_path);
    }
}

/*!
 * \brief View_project_window::on_ok_pushButton_clicked
 * OK button click handler.
 * Create new project with setting name and path to project.
 * Save new project file.
 */
void View_project_window::on_ok_pushButton_clicked()
{
    QString name = ui->project_name_lineEdit->text();
    QString path = ui->project_path_lineEdit->text();
    if (name != "" && path != "")
    {
        project->name = name;
        project->path = path;
        Project_manager::save(project);
        emit accept();
    }
}

/*!
 * \brief View_project_window::on_cancel_pushButton_clicked
 * Cancel button click handler.
 * Close window without save change.
 */
void View_project_window::on_cancel_pushButton_clicked()
{
    emit reject();
}

