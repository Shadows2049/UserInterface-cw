//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include <QMediaPlaylist>
#include <iostream>
#include <QString>
using namespace std;
int i = 50;
int countt = 0;
int flag = 0;
std::vector<QUrl> ulist;
std::vector<TheButton*>* button11;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    //jumpTo(buttons -> at(0) -> info);
}

// change the image and video for one button every one second
/*
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}*/

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::volumnUp(){
    if(i<=90){
        i = i+10;
        setVolume(i);
    }
}

void ThePlayer::mute(){
        setVolume(0);
}


void ThePlayer::volumnDown(){
    if(i>=10){
    i = i-10;
    setVolume(i);
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}

void ThePlayer::addTo (TheButtonInfo* button) {
    ulist.push_back(* button->url);
    countt += 1;
}

void ThePlayer::dl() {
    ulist.pop_back();
    countt -= 1;
}

void ThePlayer::playul () {
    QMediaPlaylist *playlist = new QMediaPlaylist();
    for (int i = 0;i < countt;i++)
    {
        if(ulist.at(i).isEmpty() == false)
            {
                playlist->addMedia(QMediaContent(ulist.at(i)));
                cout << i << endl;
            }
    }
    setPlaylist(playlist);
    play();
}

void ThePlayer::f(){
    QUrl u;
    u.setUrl("../videos/g.wmv");
    setMedia(u);
    play();
}

void ThePlayer::s(){
    QUrl u;
    u.setUrl("../videos/g1.wmv");
    setMedia(u);
    play();
}


void ThePlayer::start(){
    if (flag == 0){
        if (mTimer->isActive()){
            mTimer->stop();
            pause();
            flag = 1;
        }
    }
    else{
        if(!mTimer->isActive()){
            mTimer->start();
            play();
            flag = 0;
        }
    }
}


