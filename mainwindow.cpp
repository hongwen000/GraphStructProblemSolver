#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "create.h"
#include "draw.h"
#include "QTabBar"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), is_stored(false)
{
    ui->setupUi(this);
    on_actionCreat_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCreat_triggered()
{
    class create* c = new class create(this);
    c->show();
}

void MainWindow::create_graph()
{
    if(!vex_num) return;
    for(int i = 0; i < vex_num; ++i) {
        if(is_V_n) {
            nodeName[i] = QString("V") + QString::number(i + 1);
        } else {
            if(is_alpha)
                nodeName[i] = QString('A' + i);
            else
                nodeName[i] = QString::number(i + 1);
        }
    }
    for(int i = 0; i < 2 * vex_num; ++i) {
        pLabel.push_back(new QLabel(nodeName[i % vex_num], this));
        if(!has_weight)
            ui->gridLayout->addWidget(pLabel[i], i < vex_num ? 0 : i % vex_num + 1, i < vex_num ? i + 1 : 0, Qt::AlignCenter);
        else
            ui->gridLayout->addWidget(pLabel[i], i < vex_num ? 0 : i % vex_num + 1, i < vex_num ? 2 * i + 1 : 0, Qt::AlignCenter);
    }
    for(int i = 0; i < vex_num; ++i) {
        pCheckBox.push_back(vector<QCheckBox*>());
        pLineEdit.push_back(vector<QLineEdit*>());
        if(!has_weight) {
            for(int j = 0; j < vex_num; ++j){
                pCheckBox[i].push_back(new QCheckBox(this));
                ui->gridLayout->addWidget(pCheckBox[i][j], i + 1, j + 1, Qt::AlignCenter);
            }
        } else {
            int k = 1;
            for(int j = 0; j < vex_num; ++j){
                pCheckBox[i].push_back(new QCheckBox(this));
                ui->gridLayout->addWidget(pCheckBox[i][j], i + 1, k, Qt::AlignCenter);
                k += 1;
                pLineEdit[i].push_back(new QLineEdit(this));
                pLineEdit[i][j]->setDisabled(true);
                pLineEdit[i][j]->setPlaceholderText(i == j ? "0" : "∞");
                ui->gridLayout->addWidget(pLineEdit[i][j], i + 1, k, Qt::AlignCenter);
                k += 1;
            }
        }
    }
    for(int i = 0; i < vex_num; ++i) {
        for(int j = 0; j < vex_num; ++j) {
            if(!has_direct) {
               connect(pCheckBox[i][j], &QCheckBox::stateChanged,
                       pCheckBox[j][i], &QCheckBox::setChecked);
           }
            if(has_weight) {
                if(!has_direct)
                    connect(pLineEdit[i][j], &QLineEdit::textChanged,
                            pLineEdit[j][i], &QLineEdit::setText);
               connect(pCheckBox[i][j], &QCheckBox::stateChanged,
                    pLineEdit[i][j], &QLineEdit::setEnabled);
                connect(pCheckBox[i][j], &QCheckBox::stateChanged,
                    pLineEdit[i][j], &QLineEdit::clear);
            }
            if(i == j) {
                pCheckBox[i][j]->setEnabled(false);
            }
            connect(pCheckBox[i][j], &QCheckBox::stateChanged,
                    [this]() {this->is_stored = false;});
        }
    }
}

void MainWindow::clear()
{
    if(ui->gridLayout->layout()) {
        while(auto item = ui->gridLayout->layout()->takeAt(0)) {
            delete item->widget();
            delete item;
        }
    }
    pCheckBox.clear();
    pLabel.clear();
    nodeName.clear();
}

bool MainWindow::loadFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Open file", ".");
    QFile file(path);
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Version::Qt_5_9);
    clear();
    is_stored = false;
    graph.matrixG = new MatrixGraph();
    in >> has_direct;
    in >> has_weight;
    in >> is_alpha;
    in >> is_V_n;
    in >> type;
    in >> (*graph.matrixG);
    for(int i = 0; i < graph.matrixG->vex_num; ++i) {
        nodeName[i] = graph.matrixG->node_name[i];
    }
    is_stored = true;
    file.close();
    return true;
}

bool MainWindow::loadAdjList(const QString &str, int type)
{
    string string;
    stringstream ss(str.toStdString());
    graph.matrixG = new MatrixGraph;
    for(int i = 0; i < MAX_V_NUM; ++i) {
        for (int j = 0; j < MAX_V_NUM; ++j) {
            graph.matrixG->M[i][j] = false;
        }
    }
    if(type == 0) {
        has_direct = false;
        graph.matrixG->has_direct = false;
    } else {
        has_direct = true;
        graph.matrixG->has_direct = true;
    }
    has_weight = false;
    graph.matrixG->has_weight = false;
    while (ss >> string) {
        vex_num++;
        graph.matrixG->vex_num++;
        std::regex ptn ("[0-9]+");
        std::smatch sm;
        vector<int> chain;
        while(regex_search(string,sm,ptn)) {
            chain.push_back(stoi(sm.str()));
            string = sm.suffix();
        }
        if(type == 0) {
            for(size_t i = 1; i < chain.size(); ++i) {
                graph.matrixG->M[chain[0]][chain[i]] = true;
                graph.matrixG->M[chain[i]][chain[0]] = true;
            }
        } else if (type == 1) {
            for(size_t i = 1; i < chain.size(); ++i) {
                graph.matrixG->M[chain[0]][chain[i]] = true;
            }
        } else if (type == 2) {
            for(size_t i = 1; i < chain.size(); ++i) {
                graph.matrixG->M[chain[i]][chain[0]] = true;
            }
        }
    }
    for(int i = 0; i < vex_num; ++i) {
        if(is_V_n) {
            nodeName[i] = QString("V") + QString::number(i + 1);
            graph.matrixG->node_name[i] = nodeName[i];
        } else {
            if(is_alpha) {
                nodeName[i] = QString('A' + i);
                graph.matrixG->node_name[i] = nodeName[i];
            }
            else {
                nodeName[i] = QString::number(i);
                graph.matrixG->node_name[i] = nodeName[i];
            }
        }
    }
    is_stored = true;
    return true;
}

bool MainWindow::toFile()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString path = dialog.getSaveFileName(this, "Save to file", ".");
    QFile file(path);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Version::Qt_5_9);
    out << has_direct;
    out << has_weight;
    out << is_alpha;
    out << is_V_n;
    out << type;
    out << *(get_graph().matrixG);
    file.close();
    return true;
}

graph_t MainWindow::get_graph()
{
    if(is_stored)
        return graph;
    else if(type == MATRIX_GRAPH) {
        graph.matrixG = new MatrixGraph();
        auto gPtr = graph.matrixG;
        gPtr->vex_num = vex_num;
        gPtr->has_direct = has_direct;
        gPtr->has_weight = has_weight;
        for(int i  = 0; i < vex_num; ++i) {
            gPtr->node_name[i] = nodeName[i];
        }
        for(int i = 0; i < vex_num; ++i) {
            for(int j = 0; j < vex_num; ++j) {
                if(pCheckBox[i][j]->isChecked())
                    gPtr->M[i][j] = true;
                else
                    gPtr->M[i][j] = false;
                if(has_weight){
                    if(i == j)
                        gPtr->W[i][j] = 0;
                    else if(!pLineEdit[i][j]->text().isEmpty())
                        gPtr->W[i][j] = pLineEdit[i][j]->text().toInt();
                    else
                        gPtr->W[i][j] = INT_MAX;
                }
            }
        }
        is_stored = true;
        return graph;
    }
    else{
        return graph;
    }
}

void MainWindow::on_actionDraw_triggered()
{
    draw* pDraw = new draw(this);
    pDraw->drawGraph(get_graph(), type, nodeName, has_direct, has_weight);
    pDraw->show();
}

void MainWindow::on_actionDjikstra_triggered()
{
    bool isOK1 = false, isOK2 = false;
    int start = QInputDialog::getInt(this, "Input Start", "Start: ", QLineEdit::Normal,1, graph.matrixG->vex_num,1,&isOK1);
    int dest = 1;
    if(isOK1) dest = QInputDialog::getInt(this, "Input Dest", "Dest: ", QLineEdit::Normal,1, graph.matrixG->vex_num,1,&isOK2);
    if(isOK2) QMessageBox::about(this, "result", QString::number(dijkstra(get_graph().matrixG, start, dest)));
}

void MainWindow::on_actionBFS_triggered()
{
    bool isOK1 = false;
    int start = QInputDialog::getInt(this, "Input Start", "Start: ", QLineEdit::Normal,1, graph.matrixG->vex_num,1,&isOK1);
    if(isOK1) BFS(get_graph().matrixG, start);

}

void MainWindow::on_actionFloyd_triggered()
{
    floyd(get_graph().matrixG);
}

void MainWindow::on_actionGetInfo_triggered()
{
    getGraphInfo(get_graph().matrixG);
}

void MainWindow::on_actionOpen_triggered()
{
    if (!loadFile()) {
        QMessageBox::critical(this, "Error", "Error loading data file", QMessageBox::StandardButton::Ok);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (!toFile()) {
        QMessageBox::critical(this, "Error", "Error loading data file", QMessageBox::StandardButton::Ok);
    }
}

void MainWindow::on_actionDFS_triggered()
{
    bool isOK1 = false;
    int start = QInputDialog::getInt(this, "Input Start", "Start: ", QLineEdit::Normal,1, graph.matrixG->vex_num,1,&isOK1);
    if(isOK1) DFS(get_graph().matrixG, start);
}

void MainWindow::on_actionPrim_triggered()
{
    prim(*(get_graph().matrixG));
}

void MainWindow::on_actionKruskal_triggered()
{
    kruskal(*(get_graph().matrixG));
}
