//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//  2811 cw3 : twak 11/11/2021
//

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QListWidget>
#include "the_player.h"
#include "the_button.h"

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}

int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( std::string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;
    videoWidget->setStyleSheet("background-color:black;");
    //videoWidget->setFixedSize(1,1);
    //videoWidget->setVisible(true);
    //videoWidget->setMaximumHeight(1200);
    //videoWidget->setMaximumWidth(800);
    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);
    player->setVolume(50);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);

    QListWidget *listWidget = new QListWidget();
//    int num = player->ulist.size();
    listWidget->addItem("a.wmv");
    listWidget->addItem("b.wmv");
    listWidget->addItem("c.wmv");
    listWidget->addItem("d.wmv");
    listWidget->addItem("e.wmv");


    // create the four buttons
    for ( int i = 0; i < 5; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        //button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
        button->connect(button, SIGNAL(addTo(TheButtonInfo*)), player, SLOT (addTo(TheButtonInfo*)));
    }

    //style
    QStringList list;
        list.append("background-color:white");
        list.append("border-style:outset");
        list.append("border-width:5px");
        list.append("border-color:rgb(10,45,110)");
        list.append("border-radius:10px");
        list.append("padding:2px");


    //Customized buttons
    QPushButton quit("Quit");
    quit.setIconSize(QSize(60,60));
    quit.setIcon(QIcon("../image/qu.jpg"));
    quit.setMinimumSize(100,100);
    quit.setStyleSheet(list.join(';'));
    quit.setFont(QFont("Showcard Gothic", 20));
    QObject::connect(&quit,SIGNAL(clicked()),&app,SLOT(quit()));

    QWidget *buttonWidget2 = new QWidget();
    QHBoxLayout *layout2 = new QHBoxLayout();
    buttonWidget2->setLayout(layout2);

    QPushButton slow("");
    QObject::connect(&slow,SIGNAL(clicked()),player,SLOT(s()));
    slow.setIconSize(QSize(50,50));
    slow.setIcon(QIcon("../image/s.jpg"));
    slow.setFont(QFont("Showcard Gothic", 12));
    slow.setStyleSheet("background-color: white;");
    layout2->addWidget(&slow,0,Qt::AlignBottom);

    QPushButton Pause("Pause/Play");
    QObject::connect(&Pause,SIGNAL(clicked()),player,SLOT(start()));
    Pause.setIconSize(QSize(50,50));
    Pause.setIcon(QIcon("../image/p.jpg"));
    Pause.setFont(QFont("Showcard Gothic", 12));
    Pause.setStyleSheet("background-color: white;");
    layout2->addWidget(&Pause,0,Qt::AlignBottom);

    QPushButton fast("");
    QObject::connect(&fast,SIGNAL(clicked()),player,SLOT(f()));
    fast.setIconSize(QSize(50,50));
    fast.setIcon(QIcon("../image/f.jpg"));
    fast.setFont(QFont("Showcard Gothic", 12));
    fast.setStyleSheet("background-color: white;");
    layout2->addWidget(&fast,0,Qt::AlignBottom);

    QWidget *buttonWidget3 = new QWidget();
    QHBoxLayout *layout3 = new QHBoxLayout();
    buttonWidget3->setLayout(layout3);
    QPushButton vd("");
    QObject::connect(&vd,SIGNAL(clicked()),player,SLOT(volumnDown()));
    vd.setIconSize(QSize(120,30));
    vd.setIcon(QIcon("../image/vd.jpg"));
    vd.setFont(QFont("Showcard Gothic", 10));
    vd.setStyleSheet("background-color: white;");
    layout3->addWidget(&vd,0,Qt::AlignBottom);

    QPushButton mu("");
    QObject::connect(&mu,SIGNAL(clicked()),player,SLOT(mute()));
    mu.setIconSize(QSize(30,30));
    mu.setIcon(QIcon("../image/vm.jpg"));
    mu.setFont(QFont("Showcard Gothic", 10));
    mu.setStyleSheet("background-color: white;");
    layout3->addWidget(&mu,0,Qt::AlignBottom);

    QPushButton vu("");
    QObject::connect(&vu,SIGNAL(clicked()),player,SLOT(volumnUp()));
    vu.setIconSize(QSize(120,30));
    vu.setIcon(QIcon("../image/vu.jpg"));
    vu.setFont(QFont("Showcard Gothic", 10));
    vu.setStyleSheet("background-color: white;");
    layout3->addWidget(&vu,0,Qt::AlignBottom);

    QWidget *buttonWidget4 = new QWidget();
    QVBoxLayout *layout4 = new QVBoxLayout();
    buttonWidget4->setLayout(layout4);

    QPushButton sv("Start");
    QObject::connect(&sv,SIGNAL(clicked()),player,SLOT(playul()));
    sv.setFont(QFont("Showcard Gothic", 10));
    sv.setStyleSheet("background-color: white;");
    layout4->addWidget(&sv,0,Qt::AlignBottom);

    QPushButton dl("Delete");
    QObject::connect(&dl,SIGNAL(clicked()),player,SLOT(dl()));
    dl.setFont(QFont("Showcard Gothic", 10));
    dl.setStyleSheet("background-color: white;");
    layout4->addWidget(&dl,0,Qt::AlignBottom);



    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QWidget window;
    //QVBoxLayout *top = new QVBoxLayout();
    QGridLayout *top = new QGridLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget,0,0,7,1);
    //top->addWidget(buttonWidget1,1,1);
    top->addWidget(buttonWidget,7,0,1,1);
    top->addWidget(&quit,7,1,1,1);
    top->addWidget(buttonWidget2,6,1,1,1);
    top->addWidget(buttonWidget3,5,1,1,1);
    top->addWidget(buttonWidget4,4,1,1,1);
    top->addWidget(listWidget,1,1,1,1);
    window.setStyleSheet("background-color: #CAFF70;");
    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
