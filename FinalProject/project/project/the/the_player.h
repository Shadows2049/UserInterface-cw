//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileIconProvider>
#include <QListWidget>
#include "the_button.h"
#include <vector>
#include <QTimer>

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    std::vector<TheButtonInfo>* infos;
    std::vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 1000ms is one second between ...
        mTimer->start();
    }

    // all buttons have been setup, store pointers here
    void setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i);
    std::vector<QUrl> ulist;
private slots:

    // change the image and video for one button every one second

    void playStateChanged (QMediaPlayer::State ms);
    void volumnUp();
    void mute();
    void volumnDown();
    void start();
public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void addTo(TheButtonInfo* button);
    void playul ();
    void dl();
    void f();
    void s();
};

#endif //CW2_THE_PLAYER_H
