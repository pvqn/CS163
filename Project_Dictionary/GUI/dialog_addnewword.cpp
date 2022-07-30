#include "dialog_addnewword.h"
#include "ui_dialog_addnewword.h"

dialog_addnewword::dialog_addnewword(QWidget *parent, QString word, QString def) :
    QMainWindow(parent),
    ui(new Ui::dialog_addnewword)
{
    this->setFixedSize(384, 312);
    this->setUnifiedTitleAndToolBarOnMac(true); // activate Mac-style toolbar
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.985);
    QCoreApplication::setApplicationName("Add New Word");
    ui->setupUi(this);
    ui->newWordLEd->setText(word);
    ui->defEd->setPlainText(def);
}

dialog_addnewword::~dialog_addnewword()
{
    delete ui;
}

void dialog_addnewword::on_addBtt_clicked()
{
    emit buttonClicked(ui->newWordLEd->text(), ui->defEd->document()->toPlainText());
    this->close();
}

