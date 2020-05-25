#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include <QTime>
#include <QVBoxLayout>
#include "images.h"
#include "model.h"
#include "logdatamodel.h"
#include "controller.h"
#include "definitions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event);

private slots:
    void redraw();
    void showGameResult(GameResult result);
    void showGameError(GameErrorMessage message);
    void changeGameOpponent(const QString& name);

    void on_actionStart_triggered();
    void on_actionQuit_triggered();
    void on_actionClear_triggered();
    void on_actionRandom_triggered();
    void on_actionLeave_triggered();

private:
    void setStatus(const QString& status);

    QImage myFieldImage();
    QImage enemyFieldImage();
    QImage getFieldImage(char);

private:
    Ui::MainWindow *ui;
    State state;
    Model* model;
    LogDataModel *gameLogModel;
    Controller* controller;
};

#endif // MAINWINDOW_H
