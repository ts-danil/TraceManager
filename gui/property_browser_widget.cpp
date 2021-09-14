#include "property_browser_widget.h"

Property_browser_widget::Property_browser_widget(QBoxLayout *dockWidget_layout,
                                                 QVector<Trace*> *trc_list, QObject *parrent):QObject(parrent)
{
    trace_list = trc_list;

    variantManager = new QtVariantPropertyManager();

    t_name_user = variantManager->addProperty(QVariant::String, "Name: ");
    t_name_csv = variantManager->addProperty(QVariant::String, "Index in CSV: ");

    x_offset = variantManager->addProperty(QVariant::Double, "X offset: ");
    x_offset->setAttribute("decimals", 10);

    x_offset_units = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), "X Units: ");
    QStringList x_offset_enumNames;
    x_offset_enumNames << "s" << "ms" << "us" << "ns";
    x_offset_units->setAttribute(QLatin1String("enumNames"), x_offset_enumNames);
    x_offset_units->setValue(0);

    x_offset_step = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), "X Step: ");
    QStringList x_offset_enumNumbers;
    x_offset_enumNumbers << "0.1" << "1" << "10" << "100";
    x_offset_step->setAttribute(QLatin1String("enumNames"), x_offset_enumNumbers);
    x_offset_step->setValue(1);

    y_offset = variantManager->addProperty(QVariant::Double, "Y offset: ");
    y_offset->setAttribute("decimals", 10);

    y_offset_units = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Y Units: ");
    QStringList y_offset_enumNames;
    y_offset_enumNames << "V" << "mV" << "uV" << "nV";
    y_offset_units->setAttribute(QLatin1String("enumNames"), y_offset_enumNames);
    y_offset_units->setValue(0);

    y_offset_step = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Y Step: ");
    QStringList y_offset_enumNumbers;
    y_offset_enumNumbers << "0.1" << "1" << "10" << "100";
    y_offset_step->setAttribute(QLatin1String("enumNames"), y_offset_enumNumbers);
    y_offset_step->setValue(1);

    x_scale = variantManager->addProperty(QVariant::Double, "X scale: ");
    x_scale->setAttribute("singleStep", 0.1);
    x_scale->setAttribute("minimum", 0);

    y_scale = variantManager->addProperty(QVariant::Double, "Y scale: ");
    y_scale->setAttribute("singleStep", 0.1);
    y_scale->setAttribute("minimum", 0);

    QtGroupPropertyManager *groupManager = new QtGroupPropertyManager();

    QtProperty *offset_group = groupManager->addProperty(QObject::tr("Offset"));
    offset_group->addSubProperty(x_offset);
    x_offset->addSubProperty(x_offset_units);
    x_offset->addSubProperty(x_offset_step);
    offset_group->addSubProperty(y_offset);
    y_offset->addSubProperty(y_offset_units);
    y_offset->addSubProperty(y_offset_step);

    QtProperty *scale_group = groupManager->addProperty(QObject::tr("Scale"));
    scale_group->addSubProperty(x_scale);
    scale_group->addSubProperty(y_scale);


    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

    variantEditor = new QtGroupBoxPropertyBrowser();
    variantEditor->setFactoryForManager(variantManager, variantFactory);
    variantEditor->addProperty(t_name_user);
    variantEditor->addProperty(t_name_csv);
    variantEditor->addProperty(offset_group);
    variantEditor->addProperty(scale_group);

    dockWidget_layout->addWidget(variantEditor);
    variantEditor->setEnabled(false);

    connect(variantManager, &QtVariantPropertyManager::valueChanged,
                this, &Property_browser_widget::valueChanged);

}

void Property_browser_widget::set_trace(int index)
{
    selected_trace = index;
    Trace *trc = trace_list->at(index);
    t_name_user->setValue(trc->get_t_name_user());
    t_name_csv->setValue(trc->get_t_name_csv());
    x_offset->setValue(trc->get_x_offset());
    y_offset->setValue(trc->get_y_offset());
    x_scale->setValue(trc->get_x_scale());
    y_scale->setValue(trc->get_y_scale());
    variantEditor->setEnabled(true);
}

void Property_browser_widget::clear()
{
    selected_trace = -1;
    t_name_user->setValue("");
    t_name_csv->setValue("");
    x_offset->setValue(0);
    x_offset_units->setValue(0);
    x_offset_step->setValue(1);
    y_offset->setValue(0);
    y_offset_units->setValue(0);
    y_offset_step->setValue(1);
    variantEditor->setEnabled(false);
}

void Property_browser_widget::valueChanged(QtProperty *property, const QVariant &value)
{
    if (selected_trace != -1)
    {

        if (property->propertyName() == "Name: ")
        {
            trace_list->at(selected_trace)->set_t_name_user(value.toString());
            emit t_name_changed(selected_trace);
        }
        if (property->propertyName() == "X offset: ")
        {
            double new_x_offset = value.toDouble();
            if (new_x_offset != trace_list->at(selected_trace)->get_x_offset())
            {
                trace_list->at(selected_trace)->set_x_offset(new_x_offset);
                emit replot_graph(selected_trace);
            }
        }
        if (property->propertyName() == "X Units: ")
        {
            x_offset_step->setValue(1);
            x_offset->setAttribute("singleStep", qPow(10.0, (-3 * value.toDouble())));
        }
        if (property->propertyName() == "X Step: ")
        {
            double units = qPow(10.0, (-3 * x_offset_units->value().toDouble()));
            double step = 0.1 * qPow(10.0, value.toInt());
            x_offset->setAttribute("singleStep", units*step);
        }
        if (property->propertyName() == "Y offset: ")
        {
            double new_y_offset = value.toDouble();
            if (new_y_offset != trace_list->at(selected_trace)->get_y_offset())
            {
                trace_list->at(selected_trace)->set_y_offset(new_y_offset);
                emit replot_graph(selected_trace);
            }
        }
        if (property->propertyName() == "Y Units: ")
        {
            y_offset_step->setValue(1);
            y_offset->setAttribute("singleStep", qPow(10.0, (-3 * value.toDouble())));
        }
        if (property->propertyName() == "Y Step: ")
        {
            double units = qPow(10.0, (-3 * y_offset_units->value().toDouble()));
            double step = 0.1 * qPow(10.0, value.toInt());
            y_offset->setAttribute("singleStep", units*step);
        }
        if (property->propertyName() == "X scale: ")
        {
            trace_list->at(selected_trace)->set_x_scale(value.toDouble());
            emit replot_graph(selected_trace);
        }
        if (property->propertyName() == "Y scale: ")
        {
            trace_list->at(selected_trace)->set_y_scale(value.toDouble());
            emit replot_graph(selected_trace);
        }
    }

}
