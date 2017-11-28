#ifndef CREATE_H
#define CREATE_H

#include <QDialog>

namespace Ui {
class create;
}

class create : public QDialog
{
    Q_OBJECT

public:
    explicit create(QWidget *parent = 0);
    ~create();

private slots:
    void on_buttonBox_accepted();

    void on_checkBox_4_stateChanged(int arg1);

private:
    Ui::create *ui;
};

#endif // CREATE_H
