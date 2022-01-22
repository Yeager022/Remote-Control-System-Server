#include "the_resv.h"
#include "serverwidget.h"

The_Resv::The_Resv(QObject *parent) : QThread(parent)
{
    //初始化
    pServer = (ServerWidget*)parent;
    min = 2*sizeof(qint64);
    fileMin = 3*sizeof(qint64) + sizeof(QString);
    dataSize = 0;
    cmd = 0;
    buf = 0;
}

void The_Resv::run()
{
    connect(pServer->tcpSocket, &QTcpSocket::readyRead, [=](){
       stringSwitch();

    });
}

//将字符串恢复
void The_Resv::stringSwitch()
{
    //初始化
    dataSize = 0;
    buf = 0;
    cmd = 0;

    buf = pServer->tcpSocket->readAll();
    m_buffer.append(buf);
    unsigned int total = m_buffer.size();
    while(total)
    {
        QDataStream packet(m_buffer);
        packet.setVersion(QDataStream::Qt_5_14);
        if(total < min)
        {
            break;
        }
        packet >> dataSize >> cmd;
        if(total < dataSize)//数据长度不够
        {
            break;
        }
        qDebug() << "头部初始化完毕！";
        qDebug() << dataSize;
        qDebug() << cmd;

        switch (cmd)
        {
        case 0:
        {
            break;
        }
        case -1://心跳接收
        {
            qDebug() << "接收到心跳请求";
            emit heart_Start();
            break;
        }
        case 1://文件
        {
            name = m_buffer.mid(min, dataSize-min);
            unsigned int total = name.size();
            QDataStream packet(name);
            packet.setVersion(QDataStream::Qt_5_14);
            packet >> Size >> fileName;

            ////确认是否接收文件
            //QMessageBox msg;
            //QString prompt;//提示语
            //prompt = "接收文件：" + fileName + "，是否接收？";
            //msg.setText(tr("提示"));
            //msg.setInformativeText(prompt);
            //msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            //msg.setDefaultButton(QMessageBox::Yes);
            //int ret = msg.exec();
            //switch (ret)
            //{
            //case QMessageBox::Yes:
            //    break;
            //case QMessageBox::No:
            //    return;
            //}

            while(total)
            {
                if(total < Size)
                {
                    break;
                }

                file.setFileName(fileName);
                //打开文件
                bool isOK = file.open(QIODevice::WriteOnly);
                if(false == isOK)
                {
                    qDebug() << "打开错误";
                    break;
                }

                QByteArray data = m_buffer.mid(min+Size, dataSize-min-Size);
                file.write(data);
                file.close();
                qDebug() << "文件接收完成！";
                emit finish();
                break;
            }

            break;
        }
        case 2://鼠标单击
        {
            qDebug() << "接收鼠标事件";
            QByteArray data = m_buffer.mid(min,dataSize-min);
            str = QString(data);
            if(str == NULL)
            {
                qDebug() << "转换失败";
                return;
            }
            emit send_Signal();//发送鼠标事件
            break;
        }

        case 3: //聊天信息
        {
            qDebug() << "接收聊天信息";
            QByteArray data = m_buffer.mid(min, dataSize - min);
            QString str = QString(data);
            if (str == NULL)
            {
                qDebug() << "转换失败";
                return;
            }
            qDebug() << "转换成功";
            pServer->message = str;
            emit chat();
            break;
        }

        }

        //缓存多余数据
        buf = m_buffer.right(total - dataSize);
        total = buf.size();
        //更新多余数据
        m_buffer = buf;

    }

}
















