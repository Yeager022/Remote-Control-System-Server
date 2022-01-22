#include "the_switch.h"
#include "serverwidget.h"
#include <QDateTime>

The_Switch::The_Switch(QObject *parent) : QThread(parent)
{
    pServer = (ServerWidget*)parent;
    timer = new QTimer(this);
    timer->start(445);

}

void The_Switch::run()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(Send_Picture()),Qt::UniqueConnection);
}

//图片
void The_Switch::Send_Picture()
{
    //初始化
    //截图一张要440ms左右
    //发一张图基本上1ms之内就可以完成
    headSize = 0;
    cmd = 0;
    temp = 0;
    buf = 0;
    QString res = myScreen();
    QByteArray buf = res.toUtf8();

    QDataStream output(&temp,QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_14);
    headSize = buf.size();
    qDebug() << headSize;
    //1标明为图片
    cmd = 1;

    output << qint64(headSize) << qint64(cmd);
    headSize += temp.size();
    output.device()->seek(0);
    output << headSize;
    qint64 len = pServer->tcpSocket->write(temp);
    if(len > 0)//头部信息发送成功
    {
        qDebug() << "头部信息发送成功！";
        for(int i = 0; i < 10000; i++);
    }
    temp.resize(0);//清空临时存储的包头


    temp = buf;//放入数据
    pServer->tcpSocket->write(temp);
    temp.resize(0);//清空
    qDebug() << i++;
}

QString The_Switch::myScreen()
{
//    QDateTime time = QDateTime::currentDateTime();
//    QString timeNow = time.toString("yyyy.MM.dd hh:mm:ss.zzz");
//    qDebug() << timeNow;
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    QByteArray byte_array;
    QDataStream data_stream(&byte_array,QIODevice::WriteOnly);
    data_stream << pixmap;
    QString res = QString::fromLocal8Bit(byte_array.toBase64());

//    QDateTime time2 = QDateTime::currentDateTime();
//    timeNow = time2.toString("yyyy.MM.dd hh:mm:ss.zzz");
//    qDebug() << timeNow;

    if(res == NULL)
    {
        QMessageBox::critical(NULL, "错误","图片转换错误！ line: the_switch 71");
    }
    return res;
}









