#ifndef VIEW_PROJECT_WINDOW_H
#define VIEW_PROJECT_WINDOW_H

#include <QFileDialog>

#include "./project/project_struct.h"
#include "./project/project_manager.h"

namespace Ui {
class View_project_window;
}

class View_project_window : public QDialog
{
    Q_OBJECT

public:
    explicit View_project_window(Project_struct *proj, QWidget *parent = nullptr);
    ~View_project_window();

private slots:
    void on_browse_path_pushButton_clicked();
    void on_ok_pushButton_clicked();
    void on_cancel_pushButton_clicked();

private:
    Ui::View_project_window *ui;
    Project_struct *project;


};

#endif // VIEW_PROJECT_WINDOW_H
