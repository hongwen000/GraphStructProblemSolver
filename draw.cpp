#include "draw.h"

draw::draw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::draw)
{
    ui->setupUi(this);
    _scene = new QGVScene("DEMO", this);
    ui->graphicsView->setScene(_scene);

}

draw::~draw()
{
    delete ui;
}

void draw::drawGraph(graph_t graph, bool type, map<int, QString> nodeName, bool has_direct, bool has_weight)
{
    _scene->setNodeAttribute("shape", "circle");
    _scene->setNodeAttribute("width", "0.5");
    _scene->setNodeAttribute("fixedsize","true");
    if(!has_direct)
        _scene->setEdgeAttribute("arrowhead", "none");

    if(type == MATRIX_GRAPH) {
        int vex_num = graph.matrixG->vex_num;
        bool is_added[vex_num];
        bool is_edge_added[vex_num][vex_num];
        for(auto &i : is_added)
            i = false;
        for(auto &i : is_edge_added)
            for(auto &j : i)
                j = false;
        for(int i = 0; i < vex_num; ++i) {
            for(int j = 0; j < vex_num; ++j) {
                if(graph.matrixG->M[i][j]) {
                    if(!is_added[i]){
                        N.push_back(_scene->addNode(nodeName[i]));
                        //Graph Matrix index to Node pointer
                        mp[i] = N.back();
                        is_added[i] = true;
                    }
                    if(!is_added[j]){
                        N.push_back(_scene->addNode(nodeName[j]));
                        mp[j] = N.back();
                        is_added[j] = true;
                    }
                    if(!has_direct)
                        if(is_edge_added[j][i])
                            continue;
                    if(!has_weight)
                        E.push_back(_scene->addEdge(mp[i], mp[j]));
                    else{
                        E.push_back(_scene->addEdge(mp[i], mp[j], QString::number(graph.matrixG->W[i][j])));
                        E.back()->setAttribute("minlen", QString::number(graph.matrixG->W[i][j] * ratio_len_weight));
                    }
                    is_edge_added[i][j] = true;
                    _scene->applyLayout();
                    ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
                }
            }
        }
    }
}
