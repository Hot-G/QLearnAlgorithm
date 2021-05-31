#include <QtGui>
#include <QtCore>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QMainWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->hide(); ui->costGraphButton->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    qlearn.SetStartLocation(ui->startXTxt->toPlainText().toInt(), ui->startYTxt->toPlainText().toInt());
    qlearn.SetEndLocation(ui->finishXTxt->toPlainText().toInt(), ui->finishYTxt->toPlainText().toInt());
    //GENERATE MAP
    qlearn.GenerateMap(ui->widthTxt->toPlainText().toInt(), ui->heightTxt->toPlainText().toInt());
    qlearn.DrawMap();
    //CLOSE SETTINGS PANEL
    ui->settingsWidget->hide();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->layout()->setSpacing(2);

    //SHOW MAP

    for(int i = 0;i < qlearn.width;i++){
        for(int j = 0;j < qlearn.heigth;j++){
            QLabel *newLbl = new QLabel("");

            switch (qlearn.GetMap()[i][j]) {
            case 0:
                newLbl->setStyleSheet("QLabel { background-color : gray; }");
                break;
            case 1:
                newLbl->setStyleSheet("QLabel { background-color : yellow; color : blue; }");
                break;
            case 2:
                newLbl->setStyleSheet("QLabel { background-color : red; color : blue; }");
                break;
            case 3:
                newLbl->setStyleSheet("QLabel { background-color : blue; color : blue; }");
                break;
            }


            mainLayout->addWidget(newLbl, i, j);

            allAreas.push_back(newLbl);
        }
    }

    ui->mainVerticalLayout->addLayout(mainLayout);

    //START Q LEARNING
    qlearn.Init(0.9, 500);

    vector<int> shortPath = qlearn.GetShortestPath();

    int playerX = qlearn.startXpoint, playerY = qlearn.startYpoint;

    for(int i = 0;i < (int)shortPath.size() - 1;i++){
        qlearn.MovePlayer(shortPath.at(i), &playerX, &playerY);

        allAreas.at(playerX * qlearn.width + playerY)->setStyleSheet("QLabel { background-color : green; }");
    }

    ui->pushButton->show(); ui->costGraphButton->show();
}

void MainWindow::on_pushButton_clicked()
{
    ChartForm form;
    form.SetTitle("Episode via Step");
    form.SetData(qlearn.episodeviastep);
    //form.setModal(true);
    //form.showFullScreen();
    form.exec();
}


void MainWindow::on_costGraphButton_clicked()
{
    ChartForm form;
    form.SetTitle("Episode via Cost");
    form.SetData(qlearn.episodeviacost);
    //form.setModal(true);
    //form.showFullScreen();
    form.exec();
}

