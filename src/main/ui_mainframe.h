/********************************************************************************
** Form generated from reading UI file 'mainframe.ui'
**
** Created: Fri Apr 15 23:05:58 2022
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRAME_H
#define UI_MAINFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QTextEdit *textEditPath;
    QPushButton *btnOpenPath;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *comboBoxTargetPath;
    QPushButton *btnOpenTarget;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnLink;
    QPushButton *btnRevert;
    QPushButton *btnDelete;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_3;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(1600, 400);
        Dialog->setMinimumSize(QSize(1600, 400));
        Dialog->setMaximumSize(QSize(1600, 400));
        verticalLayout_2 = new QVBoxLayout(Dialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));

        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 50));

        horizontalLayout_4->addWidget(label);

        textEditPath = new QTextEdit(Dialog);
        textEditPath->setObjectName(QString::fromUtf8("textEditPath"));
        textEditPath->setMinimumSize(QSize(0, 80));
        textEditPath->setMaximumSize(QSize(16777215, 80));
        textEditPath->setLineWrapMode(QTextEdit::WidgetWidth);
        textEditPath->setReadOnly(false);

        horizontalLayout_4->addWidget(textEditPath);

        btnOpenPath = new QPushButton(Dialog);
        btnOpenPath->setObjectName(QString::fromUtf8("btnOpenPath"));
        btnOpenPath->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_4->addWidget(btnOpenPath);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(45, 0));
        label_2->setMaximumSize(QSize(72, 50));

        horizontalLayout_3->addWidget(label_2);

        comboBoxTargetPath = new QComboBox(Dialog);
        comboBoxTargetPath->setObjectName(QString::fromUtf8("comboBoxTargetPath"));
        comboBoxTargetPath->setMinimumSize(QSize(0, 80));
        comboBoxTargetPath->setMaximumSize(QSize(16777215, 80));
        comboBoxTargetPath->setEditable(true);

        horizontalLayout_3->addWidget(comboBoxTargetPath);

        btnOpenTarget = new QPushButton(Dialog);
        btnOpenTarget->setObjectName(QString::fromUtf8("btnOpenTarget"));
        btnOpenTarget->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_3->addWidget(btnOpenTarget);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        btnLink = new QPushButton(Dialog);
        btnLink->setObjectName(QString::fromUtf8("btnLink"));

        horizontalLayout_5->addWidget(btnLink);

        btnRevert = new QPushButton(Dialog);
        btnRevert->setObjectName(QString::fromUtf8("btnRevert"));

        horizontalLayout_5->addWidget(btnRevert);

        btnDelete = new QPushButton(Dialog);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));

        horizontalLayout_5->addWidget(btnDelete);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));

        verticalLayout_4->addLayout(horizontalLayout_6);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));

        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "QtDemo", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "\344\275\215\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
        btnOpenPath->setText(QApplication::translate("Dialog", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "\347\233\256\346\240\207\357\274\232", 0, QApplication::UnicodeUTF8));
        btnOpenTarget->setText(QApplication::translate("Dialog", "\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        btnLink->setText(QApplication::translate("Dialog", "mklink", 0, QApplication::UnicodeUTF8));
        btnRevert->setText(QApplication::translate("Dialog", "\350\277\230\345\216\237", 0, QApplication::UnicodeUTF8));
        btnDelete->setText(QApplication::translate("Dialog", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRAME_H
