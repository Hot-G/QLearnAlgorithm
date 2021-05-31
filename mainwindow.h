#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <vector>
#include <QApplication>
#include <QtWidgets>
#include "qlearn.h"
#include "chartform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_pushButton_clicked();

    void on_costGraphButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<QLabel*> allAreas;
    QLearn qlearn;
};
#endif // MAINWINDOW_H
