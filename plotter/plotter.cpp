#include "plotter.h"

/*!
 * \brief Plotter::Plotter
 * Class constructor for working with drawing in qcustomplot.
 * \param main_Plot - Main plot for drawing.
 * \param trc_list - List of traces from project.
 */
Plotter::Plotter(QCustomPlot *main_Plot, QVector<Trace *> *trc_list, QObject *parrent):QObject(parrent)
{
    mPlot = main_Plot;
    trace_list = trc_list;
}

/*!
 * \brief Plotter::draw_graphs
 * Method for draw graphs by index from index_in_tl.
 * \param index_in_tl - List of traces index for drawing.
 */
void Plotter::draw_graphs(QVector<int> *index_in_tl)
{
    QVector<double> time;
    QVector<double> value;
    QColor trace_color;
    for (int i = 0; i < index_in_tl->length(); i++)
    {
        int index = index_in_tl->at(i);
        if (trace_list->at(index)->get_plot_points(&time, &value))
        {
            trace_color = trace_list->at(index)->get_color();
            if (mPlot->graph(index) == nullptr)
            {
                mPlot->addGraph();
            }
            mPlot->graph(index)->setKeyAxis(mPlot->xAxis);
            mPlot->graph(index)->setValueAxis(mPlot->yAxis);
            mPlot->graph(index)->setData(time, value);
            mPlot->graph(index)->setPen(QPen(trace_color));
        }
    }
    mPlot->rescaleAxes();
    mPlot->replot();
};

/*!
 * \brief Plotter::draw_graphs
 * Overloaded method for drawing graphs.
 * Drawing all graphs from trace list.
 */
void Plotter::draw_graphs()
{
    QVector<int> all_index;
    for (int i = 0; i < trace_list->length(); i++)
    {
        all_index.append(i);
    }
    draw_graphs(&all_index);
}

/*!
 * \brief Plotter::set_colors
 */
void Plotter::set_colors()
{
    QVector<int> all_index;
    for (int i = 0; i < trace_list->length(); i++)
    {
        all_index.append(i);
    }
    set_colors(&all_index);
}

void Plotter::set_trace(int index)
{
    trace_list->at(index)->get_points(&time_buffer, &value_buffer);
}

/*!
 * \brief Plotter::update_graph
 * Method for drawing definite trace
 * \param index - Graph index for update
 */
void Plotter::update_graph(int index)
{
    QVector<double> time;
    QVector<double> value;
    QColor trace_color = trace_list->at(index)->get_color();
    double x_offset = trace_list->at(index)->get_x_offset();
    double y_offset = trace_list->at(index)->get_y_offset();
    double x_scale = trace_list->at(index)->get_x_scale();
    double y_scale = trace_list->at(index)->get_y_scale();
    bool show = trace_list->at(index)->get_show();
    if (show == true)
    {
        if (time_buffer.length() == value_buffer.length())
        {
            for (int i = 0; i < time_buffer.length(); i++)
            {                
                time.append(time_buffer.at(i)*x_scale+x_offset);
                value.append(value_buffer.at(i)*y_scale+y_offset);
            }
        }
        else
        {
            qDebug() << "Time and value have different length";
        }
        mPlot->graph(index)->setData(time, value, true);
    }
    mPlot->graph(index)->setPen(QPen(trace_color));
    mPlot->graph(index)->setVisible(show);
    mPlot->replot();
}

/*!
 * \brief Plotter::set_colors
 * Method for setting colors drawing for all traces in list.
 * Have 25 different color.
 */
void Plotter::set_colors(QVector<int> *index_in_tl)
{
    QVector<QColor> colors = {QColor(0,117,220), QColor(0,92,49), QColor(153,0,0), QColor(76,0,92), QColor(255,80,5),
                              QColor(0,51,128), QColor(255,204,153), QColor(0,153,143), QColor(194,0,136), QColor(255,164,5),
                              QColor(240,163,255), QColor(153,63,0), QColor(25,25,25), QColor(43,206,72), QColor(66,102,0),
                              QColor(128,128,128), QColor(148,255,181), QColor(157,204,0), QColor(143,124,0), QColor(255,168,187),
                              QColor(255,0,16), QColor(94,241,242), QColor(224,255,102), QColor(116,10,255), QColor(255,255,0),};
    for (int i = 0; i < index_in_tl->length(); i++)
    {
        int index = index_in_tl->at(i);
        if (index < 24)
        {
            trace_list->at(index)->set_color(colors.at(index));
        }
        else
        {
            trace_list->at(index)->set_color(QColor(0,0,0));
        }
        if (mPlot->graph(index) != nullptr)
        {
            mPlot->graph(index)->setPen(QPen(trace_list->at(index)->get_color()));
        }
    }
    mPlot->replot();
}

/*!
 * \brief Plotter::remove_graph
 * Method for remove graph from main plot.
 * \param index - Index graph for remove.
 */
void Plotter::remove_graph(int index)
{
    mPlot->removeGraph(index);
    mPlot->replot();
}

/*!
 * \brief Plotter::clear_graphs
 * Method for clear all graphs.
 */
void Plotter::clear_graphs()
{
    mPlot->clearGraphs();
    mPlot->replot();
}
