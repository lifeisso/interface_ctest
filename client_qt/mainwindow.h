#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>

#include "../src/com.h"

namespace Ui {
    class MainWindow;
}

class Thread:public QThread
{
    Q_OBJECT

public:
    Thread();
    COM uart;
    unsigned long long packet_sum;
    unsigned int packet_number;
    int packet_db;

protected:
       void run();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton232_clicked();

    void on_pushButton485_clicked();

    void on_pushButton422_clicked();

    void on_pushButtonnet1_clicked();

    void on_pushButtonnet2_clicked();

    void on_pushButtonbeep_clicked();

    void on_pushButtonscreen_2_clicked();

    void timerDone();

    void on_pushButtonio_clicked();

    void on_pushButtonu9_clicked();

    void on_pushButtonu10_clicked();

    #define DATA_PATH "/mnt/Nand/VD/dat/test_data.txt"
    void save_test_zigbee_data();
private:
    Ui::MainWindow *ui;
    Thread T;

};

#endif // MAINWINDOW_H
