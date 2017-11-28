#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <regex>
#include <sstream>
#include <QFile>
#include <QFileDialog>
#include <QDataStream>
#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include "graph.h"
#include <vector>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionCreat_triggered();

    void on_actionDraw_triggered();

    void on_actionDjikstra_triggered();

    void on_actionBFS_triggered();

    void on_actionFloyd_triggered();

    void on_actionGetInfo_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionDFS_triggered();

    void on_actionPrim_triggered();

    void on_actionKruskal_triggered();

private:
    Ui::MainWindow *ui;

public:
    int vex_num = 0;

public:
    void create_graph();
    void clear();
    bool toFile();
    bool loadFile();
    bool loadAdjList(const QString& str, int type);
    bool has_direct;
    bool has_weight;
    bool is_alpha;
    bool is_V_n;
    bool type;
private:
    vector<vector<QCheckBox*>> pCheckBox;
    vector<QLabel*> pLabel;
    vector<vector<QLineEdit*>> pLineEdit;
    graph_t graph;
    bool is_stored;
    graph_t get_graph();
    map<int, QString> nodeName;
};

#endif // MAINWINDOW_H
