#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#ifndef MYSEND_H
#define MYSEND_H

#include <QThread>
#include <QFile>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include <QTcpSocket>
#include <QDataStream>
#include <QMessageBox>

class ServerWidget;
class The_Switch : public QThread
{
    Q_OBJECT
public:
    explicit The_Switch(QObject *parent = nullptr);

    QString myScreen();

    QTimer *timer;

    qint64 i = 0;

    QString res;//图片QString

    QByteArray buf;//图片QByteArray

//    qint64 headName;//数据名

    qint64 headSize = 0;//数据头长度

    qint64 cmd;//类型

    QByteArray temp;//用于暂存

    qint64 isStart;//判断头部发送

public slots:
    void Send_Picture();

protected:
    void run();

private:
    ServerWidget *pServer;


signals:
//    void isDone();

};

#endif // MYSEND_H
