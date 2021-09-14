#ifndef PLOTTER_H
#define PLOTTER_H

#include <QColor>

#include "./project/trace/Trace.h"
#include "./plotter/qcustomplot/qcustomplot.h"


class Plotter : public QObject
{
    Q_OBJECT

public slots:
    void update_graph(int index);
    void set_trace(int index);

public:
    Plotter(QCustomPlot *main_Plot, QVector<Trace*> *trc_list,  QObject *parrent = nullptr);
    void set_colors(QVector<int> *index_in_tl);
    void draw_graphs(QVector<int> *index_in_tl);
    void set_colors();
    void draw_graphs();
    void remove_graph(int index);
    void clear_graphs();

private:
    QCustomPlot *mPlot;
    QVector<Trace*> *trace_list;
    QVector<double> time_buffer;
    QVector<double> value_buffer;

};

#endif // PLOTTER_H
