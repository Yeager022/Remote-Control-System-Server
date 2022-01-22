#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
{
    //初始化
    initialize();
    //连接和断开时
    newConnection();    

}

//初始化
void ServerWidget::initialize()
{
    ui->setupUi(this);

    tcpServer = NULL;
    tcpSocket = NULL;

    //监听套接字
    tcpServer = new QTcpServer(this);//指定父对象，自动回收空间
    tcpSocket = new QTcpSocket(this);

    //QHostAddress::Any 绑定当前所有ip地址
    tcpServer->listen(QHostAddress::Any,8888);

    setWindowTitle("服务端");
    ui->btn_Send->setEnabled(false);
    ui->btn_Message->setEnabled(false);

}

//产生新连接时
void ServerWidget::newConnection()
{
    connect(tcpServer, &QTcpServer::newConnection, [=](){

        //确认是否链接
        QMessageBox msg;
        QString prompt;//提示语
        prompt = "接收到远程控制申请，想控制你的电脑，是否同意？";
        msg.setText(tr("提示"));
        msg.setInformativeText(prompt);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::Yes);
        int ret = msg.exec();
        switch (ret)
        {
        case QMessageBox::Yes:

            break;
        case QMessageBox::No:
            return;
        }

        //取出建立好连接的套接字
        tcpSocket = tcpServer->nextPendingConnection();

        
        QString temp = QString("成功与客户端进行连接");
        ui->textEdit->setText(temp);

        //发送信息触发心跳
        heart_Start();

        //按钮指针设置为可用
        ui->btn_Send->setEnabled(true);
        ui->btn_Message->setEnabled(true);

        //分配线程空间
        thread = new The_Switch(this);
        thread2 = new The_Resv(this);

        //启动线程
        thread->start();
        thread2->start();

        connect(thread2, SIGNAL(send_Signal()), this, SLOT(create_Mouse()));
        connect(thread2, SIGNAL(finish()), this, SLOT(finished()));
        connect(thread2, SIGNAL(heart_Start()), this, SLOT(heart_Resv()));
        connect(thread2, SIGNAL(chat()), this, SLOT(chatMessage()));

        //断开连接
        connect(tcpSocket, &QTcpSocket::disconnected, [=](){
            ui->textEdit->append("与服务器断开连接！");

            thread->timer->stop();
            thread->timer->deleteLater();
            delete thread->timer;

            //关闭线程，下次连接需要重新开启
            stopThread();

            //主动断开连接
            tcpSocket->disconnectFromHost();
            tcpSocket->close();

            ui->btn_Send->setEnabled(false);
            ui->btn_Message->setEnabled(false);
        });

    });
}

void ServerWidget::heart_Start()
{
    //初始化
    dataSize = 0;
    cmd = 0;
    temp = 0;

    QDataStream output(&temp,QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_14);
    if(isStart == true)
    {
        screenX = GetSystemMetrics(SM_CXSCREEN);
        screenY = GetSystemMetrics(SM_CYSCREEN);
        xy = QString("%1##%2").arg(screenX).arg(screenY);
        isStart = false;
        qDebug() << xy;
    }
    QByteArray buf = xy.toUtf8();
    dataSize = buf.size();
    qDebug() << "心跳请求发送";
    cmd = -1;//-1说明为心跳消息

    output<< qint64(dataSize)<< qint64(cmd);//以流方式写入
    dataSize += temp.size();//上一行的长度
    output.device()->seek(0);//回到数据流的开始位置
    output << dataSize;
    qint64 len = tcpSocket->write(temp);
    if(len > 0)//头部信息发送成功
    {
        for(int i = 0; i< 10000; i++);
        temp.resize(0);
    }
    else
    {
        QMessageBox::critical(NULL, "错误","头部信息发送失败！ line: serverwidget 111");
        return;
    }

    temp = buf;
    tcpSocket->write(temp);
    temp.resize(0);
}

void ServerWidget::stopThread()
{
    if(thread->isRunning())
    {
        //停止线程
        thread->quit();
        //等待线程处理完手头工作
        thread->wait();
    }
    if(thread2->isRunning())
    {
        //停止线程
        thread2->quit();
        //等待线程处理完手头工作
        thread2->wait();
    }

}

//发送聊天信息
void ServerWidget::on_btn_Message_clicked()
{
    //初始化
    dataSize = 0;
    cmd = 0;
    temp = 0;
    QString message = ui->textEdit_2->toPlainText();
    QByteArray buf = message.toUtf8();

    QDataStream output(&temp, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_14);
    dataSize = buf.size();
    qDebug() << dataSize;
    cmd = 3;//3标明为聊天信息

    output << qint64(dataSize) << qint64(cmd);
    dataSize += temp.size();
    output.device()->seek(0);
    output << dataSize;
    qint64 len = tcpSocket->write(temp);
    if (len > 0)//头部信息发送成功
    {
        for (int i = 0; i < 10000; i++);
        ui->textEdit->append("我：");
        ui->textEdit->append(message);
        temp.resize(0);
    }
    else
    {
        QMessageBox::critical(NULL, "错误", "头部信息发送失败！");
        return;
    }
    temp = buf;//放入数据
    tcpSocket->write(temp);
    temp.resize(0);//清空

    ui->textEdit_2->setText(NULL);
}

//接收聊天信息
void ServerWidget::chatMessage()
{
    ui->textEdit->append("客户端:");
    ui->textEdit->append(message);
}

void ServerWidget::on_btn_Send_clicked()
{
    dir = QFileDialog::getOpenFileNames(this,"open","../");
    if(false == dir.isEmpty())//如果选择文件路径有效
    {
        for(int i = 0; i < dir.size(); ++i)
        {
            filePath = dir.at(i);
            fileName.clear();
            fileSize = 0;
            //获取文件信息
            QFileInfo info(filePath);
            fileName = info.fileName();
            fileSize = info.size();

            sendSize = 0;

            //只读方式打开文件
            //指定文件名
            file.setFileName(filePath);
            //打开文件
            bool isOK = file.open(QIODevice::ReadOnly);
            if(false == isOK)
            {
                QMessageBox::critical(NULL, "错误","只读方式打开失败！ line: serverwidget 147");
                return;
            }

            //提示打开文件的路径
            ui->textEdit->append(filePath);

            //发送文件初始化
            dataSize = 0;
            cmd = 0;
            temp = 0;
            temp2 = 0;
            Size = 0;

            QDataStream output(&temp,QIODevice::WriteOnly);
            output.setVersion(QDataStream::Qt_5_14);
            dataSize = fileSize;
            cmd = 2;

            QDataStream name(&temp2,QIODevice::WriteOnly);
            name.setVersion(QDataStream::Qt_5_14);
            name << qint64(Size) << QString(fileName);
            Size += temp2.size();
            name.device()->seek(0);
            name << Size;
            qDebug() << "文件名长度为：" << Size;

            output<< qint64(dataSize)<< qint64(cmd);//以流方式写入
            dataSize += temp.size();//上一行的长度
            dataSize += Size;
            qDebug() << dataSize;
            output.device()->seek(0);//回到数据流的开始位置
            output << dataSize;
            qint64 len = tcpSocket->write(temp);
            if(len > 0)//头部信息发送成功
            {
                for(int i = 0; i< 10000; i++);
                ui->textEdit->append("文件正在发送...");
                temp.resize(0);
            }
            else
            {
                QMessageBox::critical(NULL, "错误","头部信息发送失败！");
                file.close();
                return;
            }


            qint64 len2 = 0;
            len2 = tcpSocket->write(temp2);
            if(len2 > 0)
            {
                for(int i = 0; i< 10000; i++);
                temp2.resize(0);
            }
            else
            {
                QMessageBox::critical(NULL, "错误", "文件名发送失败！");
                file.close();
                return;
            }

            //发送数据
            temp = file.readAll();
            tcpSocket->write(temp);
            tcpSocket->write(temp2);
            temp.resize(0);
            temp2.resize(0);

        }
    }
    ui->textEdit->append("文件发送完毕！");
    file.close();
}

//提示文件接收
void ServerWidget::finished()
{
    QString str1 = QString("'%1'接收完成!").arg(thread2->fileName);
    QString str2 = QString("大小为：%1KB").arg(thread2->dataSize/1024);
    ui->textEdit->append(str1);
    ui->textEdit->append(str2);
}

//心跳处理
void ServerWidget::heart_Resv()
{
    //初始化
    dataSize = 0;
    cmd = 0;
    temp = 0;

    QDataStream output(&temp,QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_14);
    qDebug() << dataSize;
    cmd = -2;//-2说明为心跳接收

    output<< qint64(dataSize)<< qint64(cmd);//以流方式写入
    dataSize += temp.size();//上一行的长度
    output.device()->seek(0);//回到数据流的开始位置
    output << dataSize;
    qint64 len = tcpSocket->write(temp);
    if(len > 0)//头部信息发送成功
    {
        for(int i = 0; i< 10000; i++);
        temp.resize(0);
    }
    else
    {
        QMessageBox::critical(NULL, "错误","头部信息发送失败！");
        return;
    }
}

void ServerWidget::create_Mouse()
{
    qDebug() << "接收到鼠标事件";
    //取出传来的属性
    direction = QString(thread2->str).section("##", 0, 0);
    x = QString(thread2->str).section("##", 1, 1).toInt();
    y = QString(thread2->str).section("##", 2, 2).toInt();
    qDebug() << direction;
    qDebug() << x;
    qDebug() << y;

    //创建鼠标事件，发送给系统
    if(direction == "Left")
    {
        ::mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x*65535/screenX, y*65535/screenY, 0, 0);//鼠标移动到(x,y)处
        ::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//左键按下
        ::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//左键抬起
        qDebug() << "发送鼠标左键事件";
    }
    if(direction == "Right")
    {
        ::mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x*65535/screenX, y*65535/screenY, 0, 0);//鼠标移动到(x,y)处
        ::mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);//右键按下
        ::mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);//右键抬起
        qDebug() << "发送鼠标右键事件";
    }


}


//关闭窗口
void ServerWidget::closeEvent(QCloseEvent *event)
{
    if(tcpSocket->isOpen())
    {
        QMessageBox msg;
        msg.setText(tr("警告"));
        msg.setInformativeText(tr("确定关闭程序吗？如果有文件正在传输可能会被中断"));
        msg.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Cancel);
        int ret = msg.exec();
        switch (ret)
        {
        case QMessageBox::Yes:
            //主动断开连接
            tcpSocket->disconnectFromHost();
            tcpSocket->close();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }
    }

}






















