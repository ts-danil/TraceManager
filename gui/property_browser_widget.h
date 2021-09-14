#ifndef PROPERTY_BROWSER_WIDGET_H
#define PROPERTY_BROWSER_WIDGET_H

#include <QObject>
#include <QBoxLayout>
#include <QtMath>
#include <qtvariantproperty.h>
#include "qtpropertymanager.h"
#include <qttreepropertybrowser.h>
#include <qtgroupboxpropertybrowser.h>
#include "qteditorfactory.h"

#include "./project/trace/Trace.h"



class Property_browser_widget : public QObject
{
    Q_OBJECT

signals:
    void t_name_changed(int);
    void replot_graph(int);

public slots:
    void set_trace(int index);

public:
    Property_browser_widget(QBoxLayout *dockWidget_layout, QVector<Trace*> *trc_list, QObject *parrent = nullptr);
    void clear();

private slots:
    void valueChanged(QtProperty *property, const QVariant &value);

private:
    QVector<Trace*> *trace_list;
    int selected_trace;

    QtVariantPropertyManager *variantManager;

    QtGroupBoxPropertyBrowser *variantEditor;

    QtVariantProperty *t_name_user;
    QtVariantProperty *t_name_csv;
    QtVariantProperty *x_offset;
    QtVariantProperty *x_offset_units;
    QtVariantProperty *x_offset_step;
    QtVariantProperty *y_offset;
    QtVariantProperty *y_offset_units;
    QtVariantProperty *y_offset_step;
    QtVariantProperty *x_scale;
    QtVariantProperty *y_scale;

};

#endif // PROPERTY_BROWSER_WIDGET_H
