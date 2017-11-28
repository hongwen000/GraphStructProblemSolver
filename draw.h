#ifndef DRAW_H
#define DRAW_H

#include <QMainWindow>
#include "vector"
#include <QDebug>
#include "ui_draw.h"
#include "QGVScene.h"
#include "QGVNode.h"
#include "QGVEdge.h"
#include "QGVSubGraph.h"
#include <map>
#include <graph.h>


namespace Ui {
class draw;
}

class draw : public QMainWindow
{
    Q_OBJECT

public:
    explicit draw(QWidget *parent = 0);
    ~draw();
    void drawGraph(graph_t, bool type, std::map<int, QString> nodeName, bool has_direct, bool has_weight);

private:
    Ui::draw *ui;
    QGVScene *_scene;
    std::vector<QGVNode*> N;
    std::vector<QGVEdge*> E;
    std::map<int, QGVNode*> mp;
    double ratio_len_weight = 0.3;
};

#endif // DRAW_H
