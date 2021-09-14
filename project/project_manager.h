#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <QDebug>
#include <QString>
#include <QDir>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "./project/trace/Trace.h"
#include "./project/project_struct.h"

class Project_manager
{
public:
    Project_manager();
    static void open(Project_struct *project, QString *file_path);
    static bool save(Project_struct *project);


private:
    static void write_xml(QFile *file, Project_struct *project);
    static void read_xml(QFile *file, Project_struct *project);
    static void set_relative_path(Project_struct *project);
    static void set_absolute_path(Project_struct *project);

};

#endif // PROJECT_MANAGER_H
