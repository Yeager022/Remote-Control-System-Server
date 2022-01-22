#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QTimer>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QDateTime>
#include <QDataStream>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QScreen>
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QFileDialog>
#include <Windows.h>
#include "the_switch.h"
#include "the_resv.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWidget; }
QT_END_NAMESPACE

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    ServerWidget(QWidget *parent = nullptr);
    qint64 i = 0;
    qint64 num = 1;

    //图片
    QPixmap pixmap;
    QString res;

    //文件
    QFile file;
    QStringList dir;
    QString filePath;//文件路径
    QString fileName; //文件名
    qint64 fileSize; //文件大小
    qint64 sendSize; //已发送文件大小
    qint64 dataSize;
    qint64 cmd;//类型
    QByteArray temp;//用于暂存
    QByteArray temp2;
    QByteArray temp3;//屏幕
    qint64 Size;

    //聊天信息
    QString message;

    //鼠标
    bool isStart = true;
    QString direction;//左右键
    int x;//横坐标
    int y;//纵坐标
    float screenX;//分辨率
    float screenY;//分辨率
    QString xy;

    QTcpSocket *tcpSocket; //通信指针
    The_Switch *thread;//线程对象
    The_Resv *thread2;//线程对象


    void closeEvent(QCloseEvent *event);//关闭窗口

    void initialize();//初始化

    void newConnection();//产生新连接时

    void heart_Start();//发送信息触发心跳

public slots:
    void stopThread();//停止线程槽函数

    void create_Mouse();//创建鼠标事件

    void finished();//提示文件接收

    void heart_Resv();//心跳处理

    void chatMessage();//接收聊天信息

private slots:
    void on_btn_Send_clicked();

    void on_btn_Message_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpServer;


};
#endif // SERVERWIDGET_H
