#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QMainWindow>
#include <QInputEvent>
#include <QGraphicsDropShadowEffect>
#include <QToolBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QListView>
#include <QShortcut>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QThread>
#include <QtConcurrent>
#include <QPixmap>
#include <QTreeWidgetItem>
#include <QMovie>
#include <QDir>
#include "GUI/dialog_addnewword.h"

#include "operation/database.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class mainpage; }
QT_END_NAMESPACE

class mainpage : public QMainWindow
{
    Q_OBJECT

public:
    mainpage(QWidget *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *event);
    ~mainpage();

    Database database;
    Ui::mainpage *ui = nullptr;

    bool s_status = false;
    int word_or_def = 0;
    QString anschoice = "";

    std::vector<Word> random_cache;
    size_t index_random_cache;

    QFuture<void> load_thread;

private slots:
    void on_datasetBtt_clicked();

    void on_recommendationBar_itemClicked(QListWidgetItem *item);

    void on_quizBtt_clicked();

    void on_favlistBtt_clicked();

    void on_historyBtt_clicked();

    void on_choice1Btt_clicked();

    void on_choice2Btt_clicked();

    void on_choice3Btt_clicked();

    void on_choice4Btt_clicked();

    void on_changeModeBtt_clicked();

    void on_generateQuesBtt_clicked();

    void on_favFuncBtt_clicked();

    void on_editWordBtt_clicked();

    void on_randomWord_clicked();

    void on_deleteWordBtt_clicked();

    void on_searchbyBtt_clicked();

    void on_searchByWordBtt_clicked();

    void on_searchByDefBtt_clicked();

    void on_resetDataBtt_clicked();

    void on_addWordBtt_clicked();

    void on_deleteHis_clicked();

    void on_historyTable_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_favTable_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_3_clicked();

    void closeEvent(QCloseEvent* event);

signals:
    void loadSucceed();

private:

    QString path = "";

    bool animationAlreadyDone = false;

    bool animationEnded = false;

    void getPrediction(QString s);

    bool isInFavList(QString temp);

    bool isTrue(QString def);

    void turnOnButton(QPushButton* btt);

    void turnOffButton(QPushButton* btt);

    void revealAnswer();

    void lockChoice();

    void unlockChoice();

    void updateUIWord(QString word);

    void setButtonIcon(QPushButton* btt, int frame);
};

#endif // MAINPAGE_H
