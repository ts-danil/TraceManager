#include "open_file_window.h"
#include "ui_open_file_window.h"

/*!
 * \brief Open_file_window::Open_file_window
 * Class constructor window for select traces from file.
 * Used if file have a few traces.
 * \param parent - parent window.
 */
Open_file_window::Open_file_window(QString *file_path, QVector<int> *index_in_csv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Open_file_window)
{
    ui->setupUi(this);
    list_of_selected = index_in_csv;
    set_info(file_path);
    create_checkbox(file_path);
}

/*!
 * \brief Open_file_window::~Open_file_window
 * Class destructor.
 */
Open_file_window::~Open_file_window()
{
    delete ui;
}

/*!
 * \brief Open_file_window::create_window
 * Method for display file name and trace quantity.
 * \param f_path - path to file.
 */
void Open_file_window::set_info(QString *file_path)
{    
    Header_parser header(file_path);
    QString text = tr("File ") + file_path->mid(file_path->lastIndexOf('/')+1) + tr(" contains ") + QString::number(header.columns_count()-1) + tr(" traces.");
    ui->label_info->setText(text);
}

/*!
 * \brief Open_file_window::create_checkbox
 * Method for create checkbox.
 * Number of checkbox equals number of traces in file.
 * \param file_path - path to file.
 */
void Open_file_window::create_checkbox(QString *file_path)
{
    Header_parser header(file_path);
    QStringList columns_headers = header.get_columns_headers();
    columns_headers.removeFirst();
    for (int i = 0; i < columns_headers.length(); i++)
    {
        QCheckBox *ch = new QCheckBox(this);
        QListWidgetItem *wItem = new QListWidgetItem();
        ch->setText(columns_headers.at(i));
        wItem->setSizeHint(ch->sizeHint());
        ui->listWidget->addItem(wItem);
        ui->listWidget->setItemWidget(wItem, ch);
    }
}

/*!
 * \brief Open_file_window::on_pushButton_ok_clicked
 * OK button click handler.
 * Transmit index selected Traces in "index_in_csv" to add later.
 */
void Open_file_window::on_pushButton_ok_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QCheckBox *chBox = qobject_cast<QCheckBox *>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
        if (chBox->isChecked() == true)
        {
            list_of_selected->append(i);
        }
    }
    emit accept();
}

/*!
 * \brief Open_file_window::on_pushButton_cancel_clicked
 * Cancel button click handler.
 * Close window without add selected traces in project.
 */
void Open_file_window::on_pushButton_cancel_clicked()
{
    emit reject();
}

void Open_file_window::on_select_all_checkBox_stateChanged(int arg1)
{
    bool value = ui->select_all_checkBox->isChecked();
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QCheckBox *chBox = qobject_cast<QCheckBox *>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
        chBox->setChecked(value);
    }
}

