#include "project_manager.h"

/*!
 * \brief Project_manager::Project_manager
 * Class for work with project.
 * Has static method.
 */
Project_manager::Project_manager()
{

}

/*!
 * \brief Project_manager::open
 * Method for open existing project
 * \param project - Main struct project
 * \param file_path - Path to open file
 */
void Project_manager::open(Project_struct *project, QString *file_path)
{
    project->path = (*file_path);
    QFile file(*file_path);
    if (file.open(QIODevice::ReadOnly))
    {
        read_xml(&file, project);
    }
    file.close();
    set_absolute_path(project);
    for (int i = 0; i < project->trace_list.length(); i++)
    {
        project->trace_list.at(i)->set_info_from_file();
    }

}

/*!
 * \brief Project_manager::save
 * Method for save current project.
 * \param project - Main struct project for save
 * \return
 */
bool Project_manager::save(Project_struct *project)
{
    set_relative_path(project);
    QFile file(project->path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    write_xml(&file, project);
    file.close();
    set_absolute_path(project);
    return true;
}

/*!
 * \brief Project_manager::write_xml
 * Method for write project in XML-file.
 * \param file - XML file for write.
 * \param project - Main struct project.
 */
void Project_manager::write_xml(QFile *file, Project_struct *project)
{
    QXmlStreamWriter xml_writer(file);
    xml_writer.setAutoFormatting(true);
    xml_writer.writeStartDocument();
    xml_writer.writeStartElement("project");
            xml_writer.writeAttribute("name", project->name);
            for (int i = 0; i < project->trace_list.length(); i++)
            {
                xml_writer.writeStartElement("trace");
                    xml_writer.writeAttribute("trace_name_user", project->trace_list.at(i)->get_t_name_user());
                    xml_writer.writeAttribute("csv_path", project->trace_list.at(i)->get_csv_path());
                    xml_writer.writeAttribute("csv_index", QString::number(project->trace_list.at(i)->get_csv_index()));
                    xml_writer.writeAttribute("x_offset", QString::number(project->trace_list.at(i)->get_x_offset()));
                    xml_writer.writeAttribute("y_offset", QString::number(project->trace_list.at(i)->get_y_offset()));
                    xml_writer.writeAttribute("x_scale", QString::number(project->trace_list.at(i)->get_x_scale()));
                    xml_writer.writeAttribute("y_scale", QString::number(project->trace_list.at(i)->get_y_scale()));
                xml_writer.writeEndElement();
            }
    xml_writer.writeEndElement();
}

/*!
 * \brief Project_manager::read_xml
 * Method for read project from XML-file.
 * \param file - XML-file for read.
 * \param project - Main struct project.
 */
void Project_manager::read_xml(QFile *file, Project_struct *project)
{
    QXmlStreamReader xml_reader;
    xml_reader.setDevice(file);
    while(!xml_reader.atEnd())
    {
        xml_reader.readNext();
        if (xml_reader.isStartElement())
        {
            if (xml_reader.name() == "project")
            {
                QXmlStreamAttributes attr = xml_reader.attributes();
                project->name = attr.value("name").toString();
            }
            if (xml_reader.name() == "trace")
            {
                QXmlStreamAttributes attr = xml_reader.attributes();
                QString t_name_user = attr.value("trace_name_user").toString();
                QString csv_path = attr.value("csv_path").toString();
                int csv_index = attr.value("csv_index").toInt();
                double x_offset = attr.value("x_offset").toDouble();
                double y_offset = attr.value("y_offset").toDouble();
                double x_scale = attr.value("x_scale").toDouble();
                double y_scale = attr.value("y_scale").toDouble();
                Trace *new_trc = new Trace(t_name_user, csv_path, csv_index, x_offset, y_offset, x_scale, y_scale);
                project->trace_list.append(new_trc);
            }
        }
    }
}

/*!
 * \brief Project_manager::set_relative_path
 * Method for setting relative path for all traces in current project.
 * Setting relative path regarding the project path.
 * \param project - Main struct project.
 */
void Project_manager::set_relative_path(Project_struct *project)
{
    QDir project_p(project->path);
    QString old_path;
    QString new_path;
    for (int i = 0; i < project->trace_list.length(); i++)
    {
        old_path = project->trace_list.at(i)->get_csv_path();
        new_path = project_p.relativeFilePath(old_path);
        project->trace_list.at(i)->set_csv_path(new_path);
    }
}

/*!
 * \brief Project_manager::set_absolute_path
 * Method for setting absolute path for all traces in current project.
 * Setting absolute path regarding the project path.
 * \param project
 */
void Project_manager::set_absolute_path(Project_struct *project)
{
    QDir project_p(project->path);
    QString old_path;
    QString new_path;
    for (int i = 0; i < project->trace_list.length(); i++)
    {
        old_path = project->trace_list.at(i)->get_csv_path();
        QFileInfo file(project_p, old_path);
        new_path = file.canonicalFilePath();
        project->trace_list.at(i)->set_csv_path(new_path);
    }
}
