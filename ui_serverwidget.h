/********************************************************************************
** Form generated from reading UI file 'serverwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWIDGET_H
#define UI_SERVERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerWidget
{
public:
    QPushButton *btn_Send;
    QTextEdit *textEdit;
    QLabel *label;
    QTextEdit *textEdit_2;
    QPushButton *btn_Message;

    void setupUi(QWidget *ServerWidget)
    {
        if (ServerWidget->objectName().isEmpty())
            ServerWidget->setObjectName(QString::fromUtf8("ServerWidget"));
        ServerWidget->resize(600, 450);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ServerWidget->sizePolicy().hasHeightForWidth());
        ServerWidget->setSizePolicy(sizePolicy);
        ServerWidget->setMinimumSize(QSize(600, 450));
        ServerWidget->setMaximumSize(QSize(600, 450));
        btn_Send = new QPushButton(ServerWidget);
        btn_Send->setObjectName(QString::fromUtf8("btn_Send"));
        btn_Send->setGeometry(QRect(500, 110, 93, 30));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setPointSize(12);
        btn_Send->setFont(font);
        textEdit = new QTextEdit(ServerWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 110, 481, 221));
        textEdit->setReadOnly(true);
        label = new QLabel(ServerWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(true);
        label->setGeometry(QRect(230, 30, 150, 52));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\215\216\346\226\207\350\241\214\346\245\267"));
        font1.setPointSize(30);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);
        textEdit_2 = new QTextEdit(ServerWidget);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setGeometry(QRect(10, 340, 481, 101));
        textEdit_2->setFont(font);
        textEdit_2->setReadOnly(false);
        btn_Message = new QPushButton(ServerWidget);
        btn_Message->setObjectName(QString::fromUtf8("btn_Message"));
        btn_Message->setGeometry(QRect(500, 340, 93, 30));
        btn_Message->setFont(font);

        retranslateUi(ServerWidget);

        QMetaObject::connectSlotsByName(ServerWidget);
    } // setupUi

    void retranslateUi(QWidget *ServerWidget)
    {
        ServerWidget->setWindowTitle(QCoreApplication::translate("ServerWidget", "ClientWidget", nullptr));
        btn_Send->setText(QCoreApplication::translate("ServerWidget", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
        label->setText(QCoreApplication::translate("ServerWidget", "\346\234\215\345\212\241\347\253\257", nullptr));
        btn_Message->setText(QCoreApplication::translate("ServerWidget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerWidget: public Ui_ServerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWIDGET_H
