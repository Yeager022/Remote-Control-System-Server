#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#ifndef MYRESV_H
#define MYRESV_H

#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

class ServerWidget;
class The_Resv : public QThread
{
    Q_OBJECT
public:
    explicit The_Resv(QObject *parent = nullptr);

    QByteArray m_buffer;
    qint64 isStart;
    qint64 dataSize;
    qint64 cmd;
    qint64 min;
    qint64 Size;//文件名包头长度
    qint64 fileMin;
    QByteArray buf;
    QByteArray name;
    QString str;

    QFile file;
    QFileInfo fileInfo;
    qint64 fileSize;
    QString fileName;
    QString fileSuffix;



public slots:
    void stringSwitch();

protected:
    void run();

private:
    ServerWidget *pServer;

signals:
    void send_Signal();
    void finish();
    void heart_Start();
    void chat();

};

#endif // MYRESV_H
