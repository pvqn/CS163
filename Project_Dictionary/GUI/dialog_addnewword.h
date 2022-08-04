#ifndef DIALOG_ADDNEWWORD_H
#define DIALOG_ADDNEWWORD_H

#include <QMainWindow>
#include "operation/database.h"

namespace Ui {
class dialog_addnewword;
}

class dialog_addnewword : public QMainWindow
{
    Q_OBJECT

public:
    explicit dialog_addnewword(QWidget *parent = nullptr, QString word = "", QString def = "");
    ~dialog_addnewword();

signals:
    void buttonClicked(QString word, QString def);

private slots:
    void on_addBtt_clicked();

private:
    Ui::dialog_addnewword *ui;
};

#endif // DIALOG_ADDNEWWORD_H
