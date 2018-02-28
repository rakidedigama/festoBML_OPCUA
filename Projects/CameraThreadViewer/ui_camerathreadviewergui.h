/********************************************************************************
** Form generated from reading UI file 'camerathreadviewergui.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERATHREADVIEWERGUI_H
#define UI_CAMERATHREADVIEWERGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <imageviewerwidget.h>
#include "camerathreadviewergui.h"

QT_BEGIN_NAMESPACE

class Ui_CameraThreadViewerGUI
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    ImageViewerWidget *imageViewer;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_6;
    QPushButton *pbLights;
    QPushButton *pbLightsTest;
    QVBoxLayout *verticalLayout_2;
    QPushButton *startButton;
    QPushButton *stopButton;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QRadioButton *bRadioButton;
    QLabel *label;
    QLabel *label_2;
    QRadioButton *rawRadioButton;
    QLabel *Rgblabel;
    QLabel *lblDark;
    QRadioButton *rgbRadioButton;
    QRadioButton *rRadioButton;
    QLabel *FPSlabel;
    QRadioButton *gRadioButton;
    QLabel *lblImfSize;
    QCheckBox *cbTrigger;
    QPushButton *pauseButton;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QLabel *label_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *saveFolderLineEdit;
    QToolButton *btnSaveFolder;
    ClickableLabel *lblSaveFolder;
    QToolButton *btnStartSave;
    QToolButton *btnEndSave;
    QToolButton *btnSnap;
    QLabel *lblCounter;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout;
    QPushButton *pbFocus;
    QPushButton *btnFocusTest;
    QSpinBox *sbFirgelliVal;
    QPushButton *zaxisMsgBtn;
    QLabel *lblError;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CameraThreadViewerGUI)
    {
        if (CameraThreadViewerGUI->objectName().isEmpty())
            CameraThreadViewerGUI->setObjectName(QStringLiteral("CameraThreadViewerGUI"));
        CameraThreadViewerGUI->resize(980, 910);
        centralWidget = new QWidget(CameraThreadViewerGUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        imageViewer = new ImageViewerWidget(centralWidget);
        imageViewer->setObjectName(QStringLiteral("imageViewer"));

        horizontalLayout->addWidget(imageViewer);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(250, 16777215));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox_5 = new QGroupBox(groupBox_3);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        gridLayout_6 = new QGridLayout(groupBox_5);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        pbLights = new QPushButton(groupBox_5);
        pbLights->setObjectName(QStringLiteral("pbLights"));

        gridLayout_6->addWidget(pbLights, 0, 0, 1, 1);

        pbLightsTest = new QPushButton(groupBox_5);
        pbLightsTest->setObjectName(QStringLiteral("pbLightsTest"));

        gridLayout_6->addWidget(pbLightsTest, 1, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_5, 3, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        startButton = new QPushButton(groupBox_3);
        startButton->setObjectName(QStringLiteral("startButton"));

        verticalLayout_2->addWidget(startButton);

        stopButton = new QPushButton(groupBox_3);
        stopButton->setObjectName(QStringLiteral("stopButton"));

        verticalLayout_2->addWidget(stopButton);

        groupBox = new QGroupBox(groupBox_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(300, 0));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        bRadioButton = new QRadioButton(groupBox);
        bRadioButton->setObjectName(QStringLiteral("bRadioButton"));

        gridLayout_4->addWidget(bRadioButton, 4, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_4->addWidget(label, 5, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_4->addWidget(label_2, 7, 0, 1, 1);

        rawRadioButton = new QRadioButton(groupBox);
        rawRadioButton->setObjectName(QStringLiteral("rawRadioButton"));

        gridLayout_4->addWidget(rawRadioButton, 0, 0, 1, 1);

        Rgblabel = new QLabel(groupBox);
        Rgblabel->setObjectName(QStringLiteral("Rgblabel"));

        gridLayout_4->addWidget(Rgblabel, 8, 0, 1, 1);

        lblDark = new QLabel(groupBox);
        lblDark->setObjectName(QStringLiteral("lblDark"));

        gridLayout_4->addWidget(lblDark, 10, 0, 1, 1);

        rgbRadioButton = new QRadioButton(groupBox);
        rgbRadioButton->setObjectName(QStringLiteral("rgbRadioButton"));

        gridLayout_4->addWidget(rgbRadioButton, 1, 0, 1, 1);

        rRadioButton = new QRadioButton(groupBox);
        rRadioButton->setObjectName(QStringLiteral("rRadioButton"));

        gridLayout_4->addWidget(rRadioButton, 2, 0, 1, 1);

        FPSlabel = new QLabel(groupBox);
        FPSlabel->setObjectName(QStringLiteral("FPSlabel"));

        gridLayout_4->addWidget(FPSlabel, 6, 0, 1, 1);

        gRadioButton = new QRadioButton(groupBox);
        gRadioButton->setObjectName(QStringLiteral("gRadioButton"));

        gridLayout_4->addWidget(gRadioButton, 3, 0, 1, 1);

        lblImfSize = new QLabel(groupBox);
        lblImfSize->setObjectName(QStringLiteral("lblImfSize"));

        gridLayout_4->addWidget(lblImfSize, 9, 0, 1, 1);

        cbTrigger = new QCheckBox(groupBox);
        cbTrigger->setObjectName(QStringLiteral("cbTrigger"));

        gridLayout_4->addWidget(cbTrigger, 11, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        pauseButton = new QPushButton(groupBox_3);
        pauseButton->setObjectName(QStringLiteral("pauseButton"));

        verticalLayout_2->addWidget(pauseButton);

        groupBox_2 = new QGroupBox(groupBox_3);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_5->addWidget(label_3, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        saveFolderLineEdit = new QLineEdit(groupBox_2);
        saveFolderLineEdit->setObjectName(QStringLiteral("saveFolderLineEdit"));

        horizontalLayout_2->addWidget(saveFolderLineEdit);

        btnSaveFolder = new QToolButton(groupBox_2);
        btnSaveFolder->setObjectName(QStringLiteral("btnSaveFolder"));

        horizontalLayout_2->addWidget(btnSaveFolder);


        verticalLayout->addLayout(horizontalLayout_2);

        lblSaveFolder = new ClickableLabel(groupBox_2);
        lblSaveFolder->setObjectName(QStringLiteral("lblSaveFolder"));
        lblSaveFolder->setWordWrap(true);

        verticalLayout->addWidget(lblSaveFolder);


        gridLayout_5->addLayout(verticalLayout, 1, 0, 1, 1);

        btnStartSave = new QToolButton(groupBox_2);
        btnStartSave->setObjectName(QStringLiteral("btnStartSave"));
        btnStartSave->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout_5->addWidget(btnStartSave, 2, 0, 1, 1);

        btnEndSave = new QToolButton(groupBox_2);
        btnEndSave->setObjectName(QStringLiteral("btnEndSave"));

        gridLayout_5->addWidget(btnEndSave, 3, 0, 1, 1);

        btnSnap = new QToolButton(groupBox_2);
        btnSnap->setObjectName(QStringLiteral("btnSnap"));

        gridLayout_5->addWidget(btnSnap, 4, 0, 1, 1);

        lblCounter = new QLabel(groupBox_2);
        lblCounter->setObjectName(QStringLiteral("lblCounter"));

        gridLayout_5->addWidget(lblCounter, 5, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_3);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout = new QGridLayout(groupBox_4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pbFocus = new QPushButton(groupBox_4);
        pbFocus->setObjectName(QStringLiteral("pbFocus"));

        gridLayout->addWidget(pbFocus, 1, 0, 1, 1);

        btnFocusTest = new QPushButton(groupBox_4);
        btnFocusTest->setObjectName(QStringLiteral("btnFocusTest"));

        gridLayout->addWidget(btnFocusTest, 2, 0, 1, 1);

        sbFirgelliVal = new QSpinBox(groupBox_4);
        sbFirgelliVal->setObjectName(QStringLiteral("sbFirgelliVal"));
        sbFirgelliVal->setKeyboardTracking(false);
        sbFirgelliVal->setMinimum(4000);
        sbFirgelliVal->setMaximum(32767);
        sbFirgelliVal->setSingleStep(250);

        gridLayout->addWidget(sbFirgelliVal, 3, 0, 1, 1);

        zaxisMsgBtn = new QPushButton(groupBox_4);
        zaxisMsgBtn->setObjectName(QStringLiteral("zaxisMsgBtn"));

        gridLayout->addWidget(zaxisMsgBtn, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_4, 1, 0, 1, 1);


        horizontalLayout->addWidget(groupBox_3);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);

        lblError = new QLabel(centralWidget);
        lblError->setObjectName(QStringLiteral("lblError"));

        gridLayout_3->addWidget(lblError, 1, 0, 1, 1);

        CameraThreadViewerGUI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CameraThreadViewerGUI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 980, 21));
        CameraThreadViewerGUI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CameraThreadViewerGUI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CameraThreadViewerGUI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CameraThreadViewerGUI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CameraThreadViewerGUI->setStatusBar(statusBar);

        retranslateUi(CameraThreadViewerGUI);

        QMetaObject::connectSlotsByName(CameraThreadViewerGUI);
    } // setupUi

    void retranslateUi(QMainWindow *CameraThreadViewerGUI)
    {
        CameraThreadViewerGUI->setWindowTitle(QApplication::translate("CameraThreadViewerGUI", "CameraThreadViewerGUI", Q_NULLPTR));
        groupBox_3->setTitle(QString());
        groupBox_5->setTitle(QApplication::translate("CameraThreadViewerGUI", "Lights", Q_NULLPTR));
        pbLights->setText(QApplication::translate("CameraThreadViewerGUI", "Calibrate", Q_NULLPTR));
        pbLightsTest->setText(QApplication::translate("CameraThreadViewerGUI", "Test", Q_NULLPTR));
        startButton->setText(QApplication::translate("CameraThreadViewerGUI", "Start", Q_NULLPTR));
        stopButton->setText(QApplication::translate("CameraThreadViewerGUI", "Stop", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("CameraThreadViewerGUI", "Image type", Q_NULLPTR));
        bRadioButton->setText(QApplication::translate("CameraThreadViewerGUI", "B", Q_NULLPTR));
        label->setText(QApplication::translate("CameraThreadViewerGUI", "0", Q_NULLPTR));
        label_2->setText(QApplication::translate("CameraThreadViewerGUI", "0", Q_NULLPTR));
        rawRadioButton->setText(QApplication::translate("CameraThreadViewerGUI", "Raw", Q_NULLPTR));
        Rgblabel->setText(QApplication::translate("CameraThreadViewerGUI", "0", Q_NULLPTR));
        lblDark->setText(QString());
        rgbRadioButton->setText(QApplication::translate("CameraThreadViewerGUI", "RGB", Q_NULLPTR));
        rRadioButton->setText(QApplication::translate("CameraThreadViewerGUI", "R", Q_NULLPTR));
        FPSlabel->setText(QApplication::translate("CameraThreadViewerGUI", "0", Q_NULLPTR));
        gRadioButton->setText(QApplication::translate("CameraThreadViewerGUI", "G", Q_NULLPTR));
        lblImfSize->setText(QString());
        cbTrigger->setText(QApplication::translate("CameraThreadViewerGUI", "Trigger", Q_NULLPTR));
        pauseButton->setText(QApplication::translate("CameraThreadViewerGUI", "Pause", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("CameraThreadViewerGUI", "Saving", Q_NULLPTR));
        label_3->setText(QApplication::translate("CameraThreadViewerGUI", "Save folder", Q_NULLPTR));
        btnSaveFolder->setText(QApplication::translate("CameraThreadViewerGUI", "...", Q_NULLPTR));
        lblSaveFolder->setText(QString());
        btnStartSave->setText(QApplication::translate("CameraThreadViewerGUI", "...", Q_NULLPTR));
        btnEndSave->setText(QApplication::translate("CameraThreadViewerGUI", "...", Q_NULLPTR));
        btnSnap->setText(QApplication::translate("CameraThreadViewerGUI", "...", Q_NULLPTR));
        lblCounter->setText(QApplication::translate("CameraThreadViewerGUI", "0", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("CameraThreadViewerGUI", "Focusing", Q_NULLPTR));
        pbFocus->setText(QApplication::translate("CameraThreadViewerGUI", "Focus", Q_NULLPTR));
        btnFocusTest->setText(QApplication::translate("CameraThreadViewerGUI", "Focus test", Q_NULLPTR));
        zaxisMsgBtn->setText(QApplication::translate("CameraThreadViewerGUI", "Focus Msg", Q_NULLPTR));
        lblError->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CameraThreadViewerGUI: public Ui_CameraThreadViewerGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERATHREADVIEWERGUI_H
