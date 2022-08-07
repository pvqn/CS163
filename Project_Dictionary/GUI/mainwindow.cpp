#include "GUI/mainwindow.h"

#include "./ui_mainwindow.h"

#include <QRandomGenerator>

#include <QFuture>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>


mainpage::mainpage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainpage)
{
    path = QCoreApplication::applicationDirPath();

    this->setFixedSize(800, 500);

    QCoreApplication::instance()->installEventFilter(this);

    ui->setupUi(this);

    ui->comboBox->clear();

    for (const QString& file_name : database.get_databse_list())
    {
        ui->comboBox->addItem(file_name);
    }

    database.change_dataset(ui->comboBox->currentText());

    ui->recommendationBar->setVisible(false);
    turnOnButton(ui->searchByWordBtt);

    ui->dataDetectBtt->setText(QString("Currently using: ") +
                               QString("<span style=' font-weight: bold; color:#aa0000;'>" +
                                                 database.get().get_dataset_name() + "</span>"));
    ui->sizeData->setText(QString("This dataset has a total of ")
                          + QString::number(database.get().get_dictionary_size()) + QString(" words."));

    QPixmap pixmap1(path + "/resources/refresh_black.gif");
    QIcon ButtonIcon1(pixmap1);
    ui->resetDataBtt->setIcon(ButtonIcon1);

    /**/
    QPixmap pixmap2(path + "/resources/add_black.gif");
    QIcon ButtonIcon2(pixmap2);
    ui->addWordBtt->setIcon(ButtonIcon2);

    /**/

    QPixmap pixmap3(path + "/resources/shuffle_black.gif");
    QIcon ButtonIcon3(pixmap3);
    ui->randomWord->setIcon(ButtonIcon3);

    /**/

    QPixmap pixmap4(path + "/resources/change_black.gif");
    QIcon ButtonIcon4(pixmap4);
    ui->changeModeBtt->setIcon(ButtonIcon4);

    /**/

    QPixmap pixmap5(path + "/resources/generate_black.gif");
    QIcon ButtonIcon5(pixmap5);
    ui->generateQuesBtt->setIcon(ButtonIcon5);

    /**/

    QPixmap pixmap6(path + "/resources/fav_black.gif");
    QIcon ButtonIcon6(pixmap6);
    ui->favFuncBtt->setIcon(ButtonIcon6);

    /**/

    QPixmap pixmap7(path + "/resources/edit_black.gif");
    QIcon ButtonIcon7(pixmap7);
    ui->editWordBtt->setIcon(ButtonIcon7);


    /**/

    QPixmap pixmap8(path + "/resources/trash_black.gif");
    QIcon ButtonIcon8(pixmap8);
    ui->deleteWordBtt->setIcon(ButtonIcon8);

   /* */
    ui->historyTable->setHeaderLabel("Double click to quick-search the word");
    ui->historyTable->header()->setStyleSheet("QHeaderView::section {\
                                                padding: 2px;\
                                                border: 0px solid #567dbc;\
                                                border-left:0px;\
                                                border-right:0px;\
                                                background: rgb(226, 226, 232);\
                                            }");

}

void mainpage::getPrediction(QString pref) {
    ui->recommendationBar->clear();

    if (!load_thread.isFinished()) return;

    std::vector<QString> preds = ((this->s_status) ?
                                      database.get().get_word_prediction(pref) :
                                      database.get().get_keyword_prediction(pref));
    for (int i = 0; i < preds.size(); i++) {
        qDebug() << preds[i] << '\n';

        QListWidgetItem* item = new QListWidgetItem;
        item->setText(preds[i]);
        ui->recommendationBar->addItem(item);
    }
}

void setButtonIcon(QMovie* myMovie, QPushButton* btt, int frame) {
    btt->setIcon(QIcon(myMovie->currentPixmap()));
}

void mainpage::turnOnButton(QPushButton* btt) {
    btt->setStyleSheet("text-align: center; background-color: rgb(215, 255, 181);");
    this->s_status = (btt == ui->searchByWordBtt);
   // if (this->s_status) qDebug() <<"True \n"; else qDebug() <<"False \n";
}

void mainpage::turnOffButton(QPushButton* btt) {
    btt->setStyleSheet("text-align: center; background-color: rgb(193, 193, 193);");
}

bool mainpage::eventFilter(QObject *obj, QEvent *event)
{
    if ( !dynamic_cast<QInputEvent*>( event ) ) return false;
    while ( obj != NULL ) {
        if (obj == (QObject*)ui->datasetBtt || obj == (QObject*)ui->searchbyBtt || obj == (QObject*)ui->favlistBtt || obj == (QObject*)ui->historyBtt || obj == (QObject*)ui->quizBtt) {
            QPushButton* k = (QPushButton*) obj;
            if (event->type() == QEvent::HoverEnter)
            {
                k->setStyleSheet("background-color: rgb(241, 242, 246); border-radius: 7px; text-align: left; padding-left: 7px;");
                QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect( );
                effect->setBlurRadius( 10 );
                effect->setOffset(1.0);
                k->setGraphicsEffect( effect );
            }
            else if (event->type() == QEvent::HoverLeave) {
                k->setStyleSheet("background-color: rgb(193, 193, 193); border-radius: 7px; text-align: left; padding-left: 7px;");
                k->setFocusPolicy(Qt::NoFocus);
                k->setGraphicsEffect( nullptr );
            }
            else if (event->type() == QEvent::MouseButtonPress) {
                QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect( );
                effect->setBlurRadius( 10 );
                effect->setOffset(3.0);
                k->setGraphicsEffect( effect );
                effect->setOffset(1.0);
                k->setGraphicsEffect( effect );
            }
        }
        else if (obj == (QObject*) ui->searchBar) {
            QLineEdit* k = (QLineEdit*) obj;
            if (event->type() == QEvent::KeyRelease) {
                QKeyEvent* events = static_cast<QKeyEvent*>(event);
                if (events->key() == Qt::Key_Backspace) {
                    QString temptext = k->text();
                    if (!this->animationEnded && temptext.length() == 0) {
                        //ui->leftpage->setGeometry(QRect(0, 0, 251, 425) );
                        QParallelAnimationGroup* groupAni = new QParallelAnimationGroup;
                        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->settingText, "geometry");
                        animation1->setDuration(100);
                        animation1->setStartValue(QRect(0, ui->settingText->geometry().y(), 251, 425));
                        animation1->setEndValue(QRect(0, 42, 251, 425));
                        groupAni->addAnimation(animation1);
                        QPropertyAnimation *animation2 = new QPropertyAnimation(ui->datasetBtt, "geometry");
                        animation2->setDuration(100);
                        animation2->setStartValue(QRect(0, ui->datasetBtt->geometry().y(), 251, 425));
                        animation2->setEndValue(QRect(0, 72, 251, 425));
                        groupAni->addAnimation(animation2);
                        QPropertyAnimation *animation3 = new QPropertyAnimation(ui->searchbyBtt, "geometry");
                        animation3->setDuration(100);
                        animation3->setStartValue(QRect(0, ui->searchbyBtt->geometry().y(), 251, 425));
                        animation3->setEndValue(QRect(0, 117, 251, 425));
                        groupAni->addAnimation(animation3);
                        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->extensionText, "geometry");
                        animation4->setDuration(100);
                        animation4->setStartValue(QRect(0, ui->extensionText->geometry().y(), 251, 425));
                        animation4->setEndValue(QRect(0, 162, 251, 425));
                        groupAni->addAnimation(animation4);
                        QPropertyAnimation *animation5 = new QPropertyAnimation(ui->favlistBtt, "geometry");
                        animation5->setDuration(100);
                        animation5->setStartValue(QRect(0, ui->favlistBtt->geometry().y(), 251, 425));
                        animation5->setEndValue(QRect(0, 192, 251, 425));
                        groupAni->addAnimation(animation5);
                        QPropertyAnimation *animation6 = new QPropertyAnimation(ui->historyBtt, "geometry");
                        animation6->setDuration(100);
                        animation6->setStartValue(QRect(0, ui->historyBtt->geometry().y(), 251, 425));
                        animation6->setEndValue(QRect(0, 237, 251, 425));
                        groupAni->addAnimation(animation6);
                        QPropertyAnimation *animation7 = new QPropertyAnimation(ui->quizBtt, "geometry");
                        animation7->setDuration(100);
                        animation7->setStartValue(QRect(0, ui->quizBtt->geometry().y(), 251, 425));
                        animation7->setEndValue(QRect(0, 282, 251, 425));
                        groupAni->addAnimation(animation7);
                        QPropertyAnimation *animation8 = new QPropertyAnimation(ui->groupText, "geometry");
                        animation8->setDuration(100);
                        animation8->setStartValue(QRect(0, ui->groupText->geometry().y(), 251, 425));
                        animation8->setEndValue(QRect(0, 427, 251, 425));
                        groupAni->addAnimation(animation8);
                        QPropertyAnimation *animation9 = new QPropertyAnimation(ui->courseText, "geometry");
                        animation9->setDuration(100);
                        animation9->setStartValue(QRect(0, ui->courseText->geometry().y(), 251, 425));
                        animation9->setEndValue(QRect(0, 452, 251, 425));
                        groupAni->addAnimation(animation9);
                        groupAni->start();
                        ui->recommendationBar->setVisible(false);
                        ui->horizontalSpacer->changeSize(15, 20);
                        ui->randomWord->setVisible(true);
                        this->animationEnded = true;
                        this->animationAlreadyDone = false;
                    }
                    getPrediction(temptext);
                }
                else {
                    QString temptext = k->text();
                    if (!this->animationAlreadyDone && temptext.length() > 0) {
                        QParallelAnimationGroup* groupAni = new QParallelAnimationGroup;
                        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->settingText, "geometry");
                        animation1->setDuration(100);
                        animation1->setStartValue(QRect(0, ui->settingText->geometry().y(), 251, 425));
                        animation1->setEndValue(QRect(0, 357, 251, 425));
                        groupAni->addAnimation(animation1);
                        QPropertyAnimation *animation2 = new QPropertyAnimation(ui->datasetBtt, "geometry");
                        animation2->setDuration(100);
                        animation2->setStartValue(QRect(0, ui->datasetBtt->geometry().y(), 251, 425));
                        animation2->setEndValue(QRect(0, 387, 251, 425));
                        groupAni->addAnimation(animation2);
                        QPropertyAnimation *animation3 = new QPropertyAnimation(ui->searchbyBtt, "geometry");
                        animation3->setDuration(100);
                        animation3->setStartValue(QRect(0, ui->searchbyBtt->geometry().y(), 251, 425));
                        animation3->setEndValue(QRect(0, 432, 251, 425));
                        groupAni->addAnimation(animation3);
                        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->extensionText, "geometry");
                        animation4->setDuration(100);
                        animation4->setStartValue(QRect(0, ui->extensionText->geometry().y(), 251, 425));
                        animation4->setEndValue(QRect(0, 477, 251, 425));
                        groupAni->addAnimation(animation4);
                        QPropertyAnimation *animation5 = new QPropertyAnimation(ui->favlistBtt, "geometry");
                        animation5->setDuration(100);
                        animation5->setStartValue(QRect(0, ui->favlistBtt->geometry().y(), 251, 425));
                        animation5->setEndValue(QRect(0, 507, 251, 425));
                        groupAni->addAnimation(animation5);
                        QPropertyAnimation *animation6 = new QPropertyAnimation(ui->historyBtt, "geometry");
                        animation6->setDuration(100);
                        animation6->setStartValue(QRect(0, ui->historyBtt->geometry().y(), 251, 425));
                        animation6->setEndValue(QRect(0, 552, 251, 425));
                        groupAni->addAnimation(animation6);
                        QPropertyAnimation *animation7 = new QPropertyAnimation(ui->quizBtt, "geometry");
                        animation7->setDuration(100);
                        animation7->setStartValue(QRect(0, ui->quizBtt->geometry().y(), 251, 425));
                        animation7->setEndValue(QRect(0, 597, 251, 425));
                        groupAni->addAnimation(animation7);
                        QPropertyAnimation *animation8 = new QPropertyAnimation(ui->groupText, "geometry");
                        animation8->setDuration(100);
                        animation8->setStartValue(QRect(0, ui->groupText->geometry().y(), 251, 425));
                        animation8->setEndValue(QRect(0, 742, 251, 425));
                        groupAni->addAnimation(animation8);
                        QPropertyAnimation *animation9 = new QPropertyAnimation(ui->courseText, "geometry");
                        animation9->setDuration(100);
                        animation9->setStartValue(QRect(0, ui->courseText->geometry().y(), 251, 425));
                        animation9->setEndValue(QRect(0, 767, 251, 425));
                        groupAni->addAnimation(animation9);
                        groupAni->start();
                        ui->recommendationBar->setVisible(true);
                        ui->randomWord->setVisible(false);
                        ui->horizontalSpacer->changeSize(15,20);
                        this->animationAlreadyDone = true;

                        this->animationEnded = false;
                    }

                    getPrediction(temptext);
                }
            }
        }

        else if (obj == (QObject*)ui->resetDataBtt) {
#ifdef __APPLE__
            auto movie1 = new QMovie(this);

            if (event->type() == QEvent::HoverEnter) {
                movie1->setFileName(path + "/resources/refresh_black.gif");
                connect(movie1, &QMovie::frameChanged, [=]{ ui->resetDataBtt->setIcon(movie1->currentPixmap()); });
                connect(movie1, &QMovie::frameChanged, this,  [movie1]() {
                    if(movie1->currentFrameNumber() == (movie1->frameCount() - 1)) {
                        movie1->stop();
                        if (movie1->state() == QMovie::NotRunning) {
                            emit movie1->finished();
                            if (movie1) delete movie1;
                        }
                    }
                }
               );
                movie1->start();
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie1) {
                    movie1->stop();
                    delete movie1;
                    movie1 = nullptr;
                }
                QPixmap pixmap1(path + "/resources/refresh_black.gif");
                QIcon ButtonIcon1(pixmap1);
                ui->resetDataBtt->setIcon(ButtonIcon1);
            }
#endif
        }
        else if (obj == (QObject*)ui->addWordBtt) {
#ifdef __APPLE__
            auto movie2 = new QMovie(this);
            if (event->type() == QEvent::HoverEnter) {
                movie2->setFileName(path + "/resources/add_black.gif");
                connect(movie2, &QMovie::frameChanged, [=]{
                  ui->addWordBtt->setIcon(movie2->currentPixmap());
                });
                connect(movie2, &QMovie::frameChanged, this,  [movie2]() {
                    if(movie2->currentFrameNumber() == (movie2->frameCount() - 1)) {
                        movie2->stop();
                        if (movie2->state() == QMovie::NotRunning) {
                            emit movie2->finished();
                            if (movie2) delete movie2;
                        }
                    }
                }
                );
               movie2->start();
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie2) {
                    movie2->stop();
                    delete movie2;
                    movie2 = nullptr;
                }
                QPixmap pixmap2(path + "/resources/add_black.gif");
                QIcon ButtonIcon2(pixmap2);
                ui->addWordBtt->setIcon(ButtonIcon2);
            }
#endif
        }
        else if (obj == (QObject*) ui->randomWord) {
#ifdef __APPLE__
            auto movie3 = new QMovie(this);
            if (event->type() == QEvent::HoverEnter) {
                movie3->setFileName(path + "/resources/shuffle_black.gif");
                connect(movie3, &QMovie::frameChanged, [=]{
                  ui->randomWord->setIcon(movie3->currentPixmap());
                });
                connect(movie3, &QMovie::frameChanged, this,  [movie3]() {
                    if(movie3->currentFrameNumber() == (movie3->frameCount() - 1)) {
                        movie3->stop();
                        if (movie3->state() == QMovie::NotRunning) {
                            emit movie3->finished();
                            if (movie3) delete movie3;
                        }
                    }
                }
               );
                movie3->start();
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie3) {
                    movie3->stop();
                    delete movie3;
                    movie3 = nullptr;
                }
                QPixmap pixmap3(path + "/resources/shuffle_black.gif");
                QIcon ButtonIcon3(pixmap3);
                ui->randomWord->setIcon(ButtonIcon3);
            }
#endif
        }
        else if (obj == (QObject*)ui->changeModeBtt) {
#ifdef __APPLE__
            auto movie4 = new QMovie(this);
            if (event->type() == QEvent::HoverEnter) {
                movie4->setFileName(path + "/resources/change_black.gif");
                connect(movie4, &QMovie::frameChanged, [=]{
                  ui->changeModeBtt->setIcon(movie4->currentPixmap());
                });
                connect(movie4, &QMovie::frameChanged, this,  [movie4]() {
                    if(movie4->currentFrameNumber() == (movie4->frameCount() - 1)) {
                        movie4->stop();
                        if (movie4->state() == QMovie::NotRunning) {
                            emit movie4->finished();
                            if (movie4) delete movie4;
                        }
                    }
                }
               );
                movie4->start();
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie4) {
                    movie4->stop();
                    delete movie4;
                    movie4 = nullptr;
                }
                QPixmap pixmap4(path + "/resources/change_black.gif");
                QIcon ButtonIcon4(pixmap4);
                ui->changeModeBtt->setIcon(ButtonIcon4);
            }
#endif
        }
        else if (obj == (QObject*)ui->generateQuesBtt) {
#ifdef __APPLE__
            auto movie5 = new QMovie(this);
            if (event->type() == QEvent::HoverEnter) {
                movie5->setFileName(path + "/resources/generate_black.gif");
                connect(movie5, &QMovie::frameChanged, [=]{
                ui->generateQuesBtt->setIcon(movie5->currentPixmap());
                });
                connect(movie5, &QMovie::frameChanged, this,  [movie5]() {
                    if(movie5->currentFrameNumber() == (movie5->frameCount() - 1)) {
                        movie5->stop();
                        if (movie5->state() == QMovie::NotRunning) {
                            emit movie5->finished();
                            if (movie5) delete movie5;
                        }
                    }
                }
               );
                movie5->start();
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie5) {
                    movie5->stop();
                    delete movie5;
                    movie5 = nullptr;
                }
                QPixmap pixmap5(path + "/resources/generate_black.gif");
                QIcon ButtonIcon5(pixmap5);
                ui->generateQuesBtt->setIcon(ButtonIcon5);
            }
#endif
        }
        else if (obj == (QObject*)ui->favFuncBtt) {
#ifdef __APPLE__
            auto movie6 = new QMovie(this);
            bool isInFav = isInFavList(ui->word->text());
            if (event->type() == QEvent::HoverEnter) {
                if (isInFav) {
                    movie6->setFileName(path + "/resources/fav_superblack.gif");
                    connect(movie6, &QMovie::frameChanged, [=]{
                        ui->favFuncBtt->setIcon(movie6->currentPixmap());
                    });
                    connect(movie6, &QMovie::frameChanged, this,  [movie6]() {
                        if(movie6->currentFrameNumber() == (movie6->frameCount() - 1)) {
                        movie6->stop();
                            if (movie6->state() == QMovie::NotRunning) {
                                emit movie6->finished();
                                if (movie6) delete movie6;
                            }
                        }
                    }
                    );
                    movie6->start();
                }
                else {
                    movie6->setFileName(path + "/resources/fav_black.gif");
                    connect(movie6, &QMovie::frameChanged, [=]{
                        ui->favFuncBtt->setIcon(movie6->currentPixmap());
                    });
                    connect(movie6, &QMovie::frameChanged, this,  [movie6]() {
                        if(movie6->currentFrameNumber() == (movie6->frameCount() - 1)) {
                            movie6->stop();
                            if (movie6->state() == QMovie::NotRunning) {
                                emit movie6->finished();
                                if (movie6) delete movie6;
                            }
                        }
                    }
                    );
                    movie6->start();
                }
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie6) {
                    movie6->stop();
                    delete movie6;
                }
                if (isInFav) {
                    QPixmap pixmap6(path + "/resources/fav_superblack.gif");
                    QIcon ButtonIcon6(pixmap6);
                    ui->favFuncBtt->setIcon(ButtonIcon6);
                }
                else {
                    QPixmap pixmap6(path + "/resources/fav_black.gif");
                    QIcon ButtonIcon6(pixmap6);
                    ui->favFuncBtt->setIcon(ButtonIcon6);
                }
             }
#endif
        }
        else if (obj == (QObject*)ui->editWordBtt) {
#ifdef __APPLE__
            auto movie7 = new QMovie(this);
            if (event->type() == QEvent::HoverEnter) {
                movie7->setFileName(path + "/resources/edit_black.gif");
                connect(movie7, &QMovie::frameChanged, [=]{
                  ui->editWordBtt->setIcon(movie7->currentPixmap());
                });
                connect(movie7, &QMovie::frameChanged, this,  [movie7]() {
                    if(movie7->currentFrameNumber() == (movie7->frameCount() - 1)) {
                        movie7->stop();
                        if (movie7->state() == QMovie::NotRunning) {
                            emit movie7->finished();
                            if (movie7) delete movie7;
                        }
                    }
                }
               );
                movie7->start();
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie7) {
                    movie7->stop();
                    delete movie7;
                    movie7 = nullptr;
                }
                QPixmap pixmap7(path + "/resources/edit_black.gif");
                QIcon ButtonIcon7(pixmap7);
                ui->editWordBtt->setIcon(ButtonIcon7);
            }
#endif
        }
        else if (obj == (QObject*)ui->deleteWordBtt) {
#ifdef __APPLE__
            auto movie8 = new QMovie(this);
            if (event->type() == QEvent::HoverEnter) {
                movie8->setFileName(path + "/resources/trash_black.gif");
                connect(movie8, &QMovie::frameChanged, [=]{
                  ui->deleteWordBtt->setIcon(movie8->currentPixmap());
                });
                connect(movie8, &QMovie::frameChanged, this,  [movie8]() {
                    if(movie8->currentFrameNumber() == (movie8->frameCount() - 1)) {
                        movie8->stop();
                        if (movie8->state() == QMovie::NotRunning) {
                            emit movie8->finished();
                            if (movie8) delete movie8;
                        }
                    }
                }
               );
                movie8->start();
            }
            else if (event->type() == QEvent::HoverLeave) {
                if (movie8) {
                    movie8->stop();
                    delete movie8;
                    movie8 = nullptr;
                }
                QPixmap pixmap8(path + "/resources/trash_black.gif");
                QIcon ButtonIcon8(pixmap8);
                ui->deleteWordBtt->setIcon(ButtonIcon8);
            }
#endif
        }
        obj = obj->parent();
    }

    return false;
}

mainpage::~mainpage()
{
    delete ui;
}

void mainpage::closeEvent(QCloseEvent* event)
{
    hide();

    load_thread.waitForFinished();

    QMainWindow::closeEvent(event);
}

void mainpage::on_datasetBtt_clicked()
{
    ui->searchBar->clearFocus();
    ui->stackedWidget->setCurrentWidget(ui->page_5);

    if (!load_thread.isFinished()) return;

    ui->dataDetectBtt->setText(QString("Currently using: ")
                               + QString("<span style=' font-weight: bold; color:#aa0000;'>")
                               + database.get().get_dataset_name() + QString("</span>"));
    ui->sizeData->setText("This dataset has a total of " + QString::number(database.get().get_dictionary_size()) + " words.");
}

bool mainpage::isInFavList(QString temp)
{
    for (const Word& str : database.get().get_favorite_list())
    {
        if (temp == str.get_word()) return true;
    }

    return false;
}

void mainpage::on_recommendationBar_itemClicked(QListWidgetItem *item)
{
    updateUIWord(item->text());
    if (!isInFavList(item->text()))
            ui->favFuncBtt->setText("♡");
        else
            ui->favFuncBtt->setText("❤");
}

void mainpage::on_quizBtt_clicked()
{
    if (!load_thread.isFinished()) return;

     word_or_def = 0;
     ui->stackedWidget->setCurrentWidget(ui->page_2);

     random_cache = database.get().random_words(4);

     index_random_cache = QRandomGenerator::global()->bounded(0, 3);

     ui->questionTable->setText("What is the definition of \"" + random_cache[index_random_cache].get_word() + "\" ?");
     ui->choice1Btt->setText(random_cache[0].get_definition());
     ui->choice2Btt->setText(random_cache[1].get_definition());
     ui->choice3Btt->setText(random_cache[2].get_definition());
     ui->choice4Btt->setText(random_cache[3].get_definition());

     unlockChoice();
}

bool mainpage::isTrue(QString def)
{
    QString ans = word_or_def ? random_cache[index_random_cache].get_word()
                              : random_cache[index_random_cache].get_definition();

    return (def == ans);
}

void mainpage::on_favlistBtt_clicked()
{
    if (!load_thread.isFinished()) return;

    ui->stackedWidget->setCurrentWidget(ui->page_3);
    ui->favTable->clear();

    for (const Word& str : database.get().get_favorite_list())
    {
        ui->favTable->addItem(str.get_word());
    }
}

void mainpage::on_historyBtt_clicked()
{
    if (!load_thread.isFinished()) return;

    ui->stackedWidget->setCurrentWidget(ui->page_4);
    ui->historyTable->clear();

    QString date_temp;

    for (const QString& line : database.get().get_history_list())
    {
        std::vector<QString> date;

        QString word;

        for (const QChar& ch : line)
        {
            if (ch == '~')
            {
                date.push_back(word);
                word.clear();
            }
            else
                word.append(ch);
        }

        QString date_str = date[2] + '/' + date[1] + '/' + date[0];

        if (date_str != date_temp)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem;
            item->setText(0, date_str);
            ui->historyTable->addTopLevelItem(item);
            date_temp = date_str;
        }

        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, word);

        for (int i = 0; i < ui->historyTable->topLevelItemCount(); i++)
        {
            if (ui->historyTable->topLevelItem(i)->text(0) == date_str)
            {
                ui->historyTable->topLevelItem(i)->addChild(item);
            }
        }
    }
}

void mainpage::on_choice1Btt_clicked()
{
    anschoice = ui->choice1Btt->text();
    ui->choice1Btt->setStyleSheet("background-color: rgb(66, 172, 255)");
    lockChoice();
}

void mainpage::on_choice2Btt_clicked()
{
    anschoice = ui->choice2Btt->text();
    ui->choice2Btt->setStyleSheet("background-color: rgb(66, 172, 255)");
    lockChoice();
}

void mainpage::on_choice3Btt_clicked()
{
    anschoice = ui->choice3Btt->text();
    ui->choice3Btt->setStyleSheet("background-color: rgb(66, 172, 255)");
    lockChoice();
}

void mainpage::on_choice4Btt_clicked()
{
    anschoice = ui->choice4Btt->text();
    ui->choice4Btt->setStyleSheet("background-color: rgb(66, 172, 255)");
    lockChoice();
}

void mainpage::on_changeModeBtt_clicked()
{

    unlockChoice();

    word_or_def = !word_or_def;

    random_cache = database.get().random_words(4);

    index_random_cache = QRandomGenerator::global()->bounded(0, 3);

    QString question = word_or_def ?
                "What is the word of \"" + random_cache[index_random_cache].get_definition() + "\" ?" :
                "What is the definition of \"" + random_cache[index_random_cache].get_word() + "\" ?";

    QString ans_1 = word_or_def ? random_cache[0].get_word() : random_cache[0].get_definition();
    QString ans_2 = word_or_def ? random_cache[1].get_word() : random_cache[1].get_definition();
    QString ans_3 = word_or_def ? random_cache[2].get_word() : random_cache[2].get_definition();
    QString ans_4 = word_or_def ? random_cache[3].get_word() : random_cache[3].get_definition();

    ui->questionTable->setText(question);
    ui->choice1Btt->setText(ans_1);
    ui->choice2Btt->setText(ans_2);
    ui->choice3Btt->setText(ans_3);
    ui->choice4Btt->setText(ans_4);
}

void mainpage::on_generateQuesBtt_clicked()
{

    unlockChoice();

    random_cache = database.get().random_words(4);

    index_random_cache = QRandomGenerator::global()->bounded(0, 3);

    QString question = word_or_def ?
                "What is the word of \"" + random_cache[index_random_cache].get_definition() + "\" ?" :
                "What is the definition of \"" + random_cache[index_random_cache].get_word() + "\" ?";

    QString ans_1 = word_or_def ? random_cache[0].get_word() : random_cache[0].get_definition();
    QString ans_2 = word_or_def ? random_cache[1].get_word() : random_cache[1].get_definition();
    QString ans_3 = word_or_def ? random_cache[2].get_word() : random_cache[2].get_definition();
    QString ans_4 = word_or_def ? random_cache[3].get_word() : random_cache[3].get_definition();

    ui->questionTable->setText(question);
    ui->choice1Btt->setText(ans_1);
    ui->choice2Btt->setText(ans_2);
    ui->choice3Btt->setText(ans_3);
    ui->choice4Btt->setText(ans_4);
}

void mainpage::on_favFuncBtt_clicked()
{
    if (ui->favFuncBtt->text()=="❤")
    {
        database.get().action_on_favorite_file(ui->word->text(), true);
        ui->favFuncBtt->setText("♡");
    }
    else
    {
        database.get().action_on_favorite_file(ui->word->text(), false);
        ui->favFuncBtt->setText("❤");
    }
}

void mainpage::on_editWordBtt_clicked()
{
    if (!load_thread.isFinished()) return;

    dialog_addnewword* newDialog_AddNewWord = new dialog_addnewword(this, this->ui->word->text(), this->ui->def->text());
    newDialog_AddNewWord->show();
    connect(newDialog_AddNewWord, &dialog_addnewword::buttonClicked, this, [&](QString word, QString def) {
        Word w = database.get().search_for_definition(word, 0);
        if (!w.get_word().isEmpty())
            database.get().edit_definition(word, def);
        else
            database.get().insert(word, def);
        updateUIWord(word);
    });
}

void mainpage::on_randomWord_clicked()
{
    if (!load_thread.isFinished()) return;

    Word random = database.get().random_words(1)[0];

    while (random.get_word() == ui->word->text())
        random = database.get().random_words(1)[0];

    ui->stackedWidget->setCurrentWidget(ui->page);
    ui->word->setText(random.get_word());
    ui->def->setText(random.get_definition());

    if (isInFavList(random.get_word()))
        ui->favFuncBtt->setText("❤");
    else ui->favFuncBtt->setText("♡");

    ui->favFuncBtt->setText(isInFavList(random.get_word()) ? "❤" : "♡");
}

void mainpage::on_deleteWordBtt_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_6);
    database.get().remove(ui->word->text());
    getPrediction(ui->searchBar->text());
}

void mainpage::on_searchbyBtt_clicked()
{
    if (!load_thread.isFinished()) return;

    ui->stackedWidget->setCurrentWidget(ui->page_7);
}

void mainpage::on_searchByWordBtt_clicked()
{
    turnOnButton(ui->searchByWordBtt);
    turnOffButton(ui->searchByDefBtt);
}

void mainpage::revealAnswer()
{
    lockChoice();
    if (isTrue(anschoice)) {
        if (anschoice == ui->choice1Btt->text()) {
            ui->choice1Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        } else if (anschoice == ui->choice2Btt->text()) {
            ui->choice2Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        } else if (anschoice == ui->choice3Btt->text()) {
            ui->choice3Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        } else {
            ui->choice4Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        }
    }
    else {
        if (anschoice != "") {
            if (anschoice == ui->choice1Btt->text()) {
                ui->choice1Btt->setStyleSheet("background-color: rgb(255, 116, 105)");
            } else if (anschoice == ui->choice2Btt->text()) {
                ui->choice2Btt->setStyleSheet("background-color: rgb(255, 116, 105)");
            } else if (anschoice == ui->choice3Btt->text()) {
                ui->choice3Btt->setStyleSheet("background-color: rgb(255, 116, 105)");
            } else {
                ui->choice4Btt->setStyleSheet("background-color: rgb(255, 116, 105)");
            }
        }

        if (isTrue(ui->choice1Btt->text())) {
            ui->choice1Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        } else if (isTrue(ui->choice2Btt->text())) {
            ui->choice2Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        } else if (isTrue(ui->choice3Btt->text())) {
            ui->choice3Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        } else {
            ui->choice4Btt->setStyleSheet("background-color: rgb(70, 204, 0)");
        }
    }
}

void mainpage::lockChoice()
{
    ui->choice1Btt->setEnabled(false);
    ui->choice2Btt->setEnabled(false);
    ui->choice3Btt->setEnabled(false);
    ui->choice4Btt->setEnabled(false);
}

void mainpage::unlockChoice()
{
    ui->choice1Btt->setEnabled(true);
    ui->choice2Btt->setEnabled(true);
    ui->choice3Btt->setEnabled(true);
    ui->choice4Btt->setEnabled(true);

    ui->choice1Btt->setStyleSheet("");
    ui->choice2Btt->setStyleSheet("");
    ui->choice3Btt->setStyleSheet("");
    ui->choice4Btt->setStyleSheet("");



    QPropertyAnimation *animationx = new QPropertyAnimation(ui->progressBar, "value");
    animationx->setDuration(5000);
    animationx->setStartValue(0);
    animationx->setEndValue(100);
    animationx->start();
    QObject::connect(animationx, &QAbstractAnimation::finished, this, &mainpage::revealAnswer);
}

void mainpage::on_searchByDefBtt_clicked()
{
    turnOffButton(ui->searchByWordBtt);
    turnOnButton(ui->searchByDefBtt);
}

void mainpage::on_resetDataBtt_clicked()
{
    if (!load_thread.isFinished()) return;

    load_thread = QtConcurrent::run([=]()
    {
        database.get().reset();
    }).then([=]()
    {
        ui->sizeData->setText("This dataset has a total of " + QString::number(database.get().get_dictionary_size()) + " words.");
    });
}

void mainpage::on_addWordBtt_clicked()
{
    if (!load_thread.isFinished()) return;

    dialog_addnewword* newDialog_AddNewWord = new dialog_addnewword(this, "", "");
    newDialog_AddNewWord->show();
    connect(newDialog_AddNewWord, &dialog_addnewword::buttonClicked, this, [&](QString word, QString def) {
        Word w = database.get().search_for_definition(word, 0);
        if (!w.get_word().isEmpty())
            database.get().edit_definition(word, def);
        else
            database.get().insert(word, def);
        updateUIWord(word);
    });
}

void mainpage::on_deleteHis_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Test", "Are you sure?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        database.get().clear_history();
        ui->historyTable->clear();
    }
}

void mainpage::on_historyTable_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    for (int i = 0; i < ui->historyTable->topLevelItemCount(); i++) {
        if (ui->historyTable->topLevelItem(i) == item) return;
    }
    QString word = item->text(column);

    updateUIWord(word);
}

void mainpage::on_favTable_itemDoubleClicked(QListWidgetItem *item)
{
    updateUIWord(item->text());
}

void mainpage::updateUIWord(QString word) {
    ui->stackedWidget->setCurrentWidget(ui->page);
    ui->word->setText(word);
    ui->def->setText(database.get().search_for_definition(word, 1).get_definition());
    if (isInFavList(word)) {
        QPixmap pixmap6("resources/fav_superblack.gif");
        QIcon ButtonIcon6(pixmap6);
        ui->favFuncBtt->setIcon(ButtonIcon6);
    }
    else {
        QPixmap pixmap6("resources/fav_black.gif");
        QIcon ButtonIcon6(pixmap6);
        ui->favFuncBtt->setIcon(ButtonIcon6);
    }
}

void mainpage::on_pushButton_3_clicked()
{
    QString str = ui->comboBox->currentText();

    if (str == database.get().get_dataset_name()
            || !ui->checkBox->isChecked()
            || !load_thread.isFinished()) return;

    ui->dataDetectBtt->setText(QString("Currently using: ")
                               + QString("<span style=' font-weight: bold; color:#aa0000;'>")
                               + str + QString("</span>"));

    ui->sizeData->setText("Please wait, the dataset is building");

    load_thread = QtConcurrent::run([=]()
    {
        database.change_dataset(str);
    }).then([=]()
    {
        ui->sizeData->setText("This dataset has a total of " + QString::number(database.get().get_dictionary_size()) + " words.");
        ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
    });
}

