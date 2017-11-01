#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../src/LinuxMsg.h"

#include <QTimer>

int result;
#define BIT232      0
#define BIT485      1
#define BIT422      2
#define BITNET1     3
#define BITNET2     4
#define BITU9       5
#define BITU10      6
#define BITIO       7
#define BITSCREEN   8
#define BITBEEP     9

#define SET_BIT(val, bit)     (val |= 0x1 << bit)
#define CLEAR_BIT(val, bit)     (val &= ~(0x1 << bit))
#define TEST_BIT(val, bit)      (0x1 & (val >> bit))
#define GET_BIT(val, bit)       (((val >> bit) & 0x1) << bit)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->lineEditNumber->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    QTimer *timer = new QTimer( this );

    connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );

    timer->start( 10); // 1秒单触发定时器

    //T.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//232接口测试命令
void MainWindow::on_pushButton232_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BIT232);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//485接口测试命令
void MainWindow::on_pushButton485_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BIT485);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//422接口测试命令
void MainWindow::on_pushButton422_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BIT422);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//网络1接口测试命令
void MainWindow::on_pushButtonnet1_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BITNET1);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//网络2接口测试命令
void MainWindow::on_pushButtonnet2_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BITNET2);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//蜂鸣器测试命令
void MainWindow::on_pushButtonbeep_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BITBEEP);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//屏幕测试命令
void MainWindow::on_pushButtonscreen_2_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BITSCREEN);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//IO测试命令
void MainWindow::on_pushButtonio_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BITIO);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//内部zigbee1接口测试命令
void MainWindow::on_pushButtonu9_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BITU9);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

//内部zigbee2接口测试命令
void MainWindow::on_pushButtonu10_clicked()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
    int cmd = 0;

    SET_BIT(cmd, BITU10);
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
}

void MainWindow::timerDone()
{
    CMsg msg(0x5000);
    struct MsgBuf mb;
#if 0
    int cmd = 0x1;
    mb.mtype = 2;
    msg.SendMsg(mb.mtype, &cmd, sizeof(cmd));
#endif
        mb.mtype = 1;
         if(msg.RecvMsg(mb.mtype, &result, sizeof(result))){

            qDebug("recv %x\n", result);
            if(TEST_BIT(result, BIT232)){
                ui->lineEdit232->setText("ok");
            }
            if(TEST_BIT(result, BIT485)){
                ui->lineEdit485->setText("ok");
            }
            if(TEST_BIT(result, BIT422)){
                ui->lineEdit422->setText("ok");
            }
            if(TEST_BIT(result, BITNET1)){
                ui->lineEditnet1->setText("ok");
            }
            if(TEST_BIT(result, BITNET2)){
                ui->lineEditnet2->setText("ok");
            }
            if(TEST_BIT(result, BITBEEP)){
                ui->lineEditbeep->setText("ok");
            }
            if(TEST_BIT(result, BITSCREEN)){
                ui->lineEditscreen->setText("ok");
            }
            if(TEST_BIT(result, BITU9)){
                ui->lineEditu9->setText("ok");
            }
            if(TEST_BIT(result, BITU10)){
                ui->lineEditu10->setText("ok");
            }
            if(TEST_BIT(result, BITIO)){
                ui->lineEditio->setText("ok");
             }
        }

//         ui->lineEditDB->setText(QString::number(T.packet_db, 10));
//         ui->lineEditNumber->setText(QString::number(T.packet_number, 10));
//         ui->lineEditSum->setText(QString::number(T.packet_sum, 10));
}
#if 0
//测试新zigbee
void Thread::save_test_zigbee_data()
{
    FILE *fp = fopen(DATA_PATH, "w");
    if(fp != NULL){
        fprintf(fp, "sum:%lld now:%d db:%d", T.packet_sum, T.packet_number, T.packet_db);
        fclose(fp);
    }
}

Thread::Thread()
{
    packet_sum = 0;
    packet_number = 0;
    packet_db = 0;
}

void Thread::run()
{
    char buf[1024];
    int len;
    static unsigned n = 0;

    qDebug("thread\n");
#ifdef IMX6
    uart.COMOpen((char *)"/dev/ttymxc3", 115200);
#else
    uart.COMOpen((char *)"/dev/ttymxc5", 115200);
#endif
    while(true){
        memset(buf, 0, sizeof(buf));
        len = uart.COMRead(buf, 3);

        if(buf[0] != 0x7e)continue;
        len = buf[1]*256 + buf[2] + 1;
        len = uart.COMRead(buf, len);

        //总包号增加
        packet_sum++;
        //包号
        memcpy(&packet_number, buf+12, 4);
        if(packet_number != n+1){
            qDebug("*********%d**%d*************************lose******************\n", packet_number, n);
        }
        n = packet_number;
        //信号强度
        memcpy(&packet_db, buf+16, 4);
        qDebug("sum:%lld %d %d", packet_sum, packet_number, packet_db);
    }
}
#endif
