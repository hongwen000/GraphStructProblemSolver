#include "create.h"
#include "ui_create.h"
#include <mainwindow.h>
#include <QDebug>

create::create(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create)
{
    ui->setupUi(this);
    ui->plainTextEdit->setEnabled(false);
    ui->comboBox->setEnabled(false);
}

create::~create()
{
    delete ui;
}

void create::on_buttonBox_accepted()
{
    MainWindow* pMW = (MainWindow*) parentWidget();
    pMW->type = MATRIX_GRAPH;
    if(ui->checkBox_4->isChecked()) {
        pMW->is_V_n = ui->checkBox_5->isChecked();
        pMW->is_alpha = ui->checkBox_2->isChecked();
        pMW->clear();
        pMW->loadAdjList(ui->plainTextEdit->document()->toPlainText(), ui->comboBox->currentIndex());
        pMW->show();
        this->close();
    } else {
        pMW->vex_num = ui->lineEdit->text().toInt();
        pMW->has_direct = ui->checkBox->isChecked();
        pMW->is_alpha = ui->checkBox_2->isChecked();
        pMW->has_weight = ui->checkBox_3->isChecked();
        pMW->is_V_n = ui->checkBox_5->isChecked();
        pMW->clear();
        pMW->create_graph();
        pMW->show();
        this->close();
    }
}

void create::on_checkBox_4_stateChanged(int arg1)
{
    ui->plainTextEdit->setEnabled(arg1);
    ui->plainTextEdit->setPlainText("");
    ui->comboBox->setEnabled(arg1);
    ui->checkBox->setEnabled(!arg1);
    ui->checkBox_3->setEnabled(!arg1);
    ui->lineEdit->setEnabled(!arg1);
}
