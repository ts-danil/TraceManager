#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <QString>

#include "./project/trace/Trace.h"

struct Project_struct
{
    QString name;
    QString path;
    QVector<Trace *> trace_list;
};

#endif // PROJECT_STRUCT_H
