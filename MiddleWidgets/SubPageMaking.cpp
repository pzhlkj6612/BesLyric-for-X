﻿#include "SubPageMaking.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QFileDialog>
#include <BesMessageBox.h>
#include <BesShadowDialog.h>
#include <QDesktopServices>
#include "SettingManager.h"
#include "BesScaleUtil.h"

SubPageMaking::SubPageMaking(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initLayout();
    initEntity();
    initConnection();
}

SubPageMaking::~SubPageMaking()
{

}

void SubPageMaking::initLayout()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(
        30 * BesScaleUtil::scale(),
        30 * BesScaleUtil::scale(),
        30 * BesScaleUtil::scale(),
        30 * BesScaleUtil::scale());

    widgetMusicPath = new QWidget(this);
    widgetLyricPath = new QWidget(this);;

    labelSelectMusic = new QLabel(widgetMusicPath);
    labelSelectLyric= new QLabel(widgetLyricPath);
    labelSelectOutputDir= new QLabel(this);
    labelSelectMusic->setText(tr("选择音乐："));
    labelSelectLyric->setText(tr("选择歌词："));
    labelSelectOutputDir->setText(tr("输出目录："));
    labelSelectMusic->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    labelSelectLyric->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    labelSelectOutputDir->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    labelSelectMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSelectLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSelectOutputDir->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    editSelectMusic = new BesFileLineEdit(BesEditFileType::BesFileTypeMusic, widgetMusicPath);
    editSelectLyric = new BesFileLineEdit(BesEditFileType::BesFileTypeTxt, widgetLyricPath);
    editSelectOutputDir = new BesFileLineEdit(BesEditFileType::BesFileTypeFloder,this);
    editSelectMusic->setReadOnly(true);
    editSelectLyric->setReadOnly(true);
    editSelectOutputDir->setReadOnly(true);
    editSelectMusic->setMinimumHeight(28* BesScaleUtil::scale());
    editSelectLyric->setMinimumHeight(28* BesScaleUtil::scale());
    editSelectOutputDir->setMinimumHeight(28* BesScaleUtil::scale());
    editSelectMusic->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSelectLyric->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSelectOutputDir->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSelectMusic->setPlaceholderText(tr("点击选择文件 或 拖放文件到这里"));
    editSelectLyric->setPlaceholderText(tr("点击选择文件 或 拖放文件到这里"));
    editSelectOutputDir->setPlaceholderText(tr("点击选择文件夹 或 拖放文件夹到这里"));

    btnSelectMusic = new BesButton(widgetMusicPath);
    btnSelectLyric = new BesButton(widgetLyricPath);
    btnSelectOutputDir = new BesButton(this);
    btnSelectMusic->setText(tr("选择"));
    btnSelectLyric->setText(tr("选择"));
    btnSelectOutputDir->setText(tr("选择"));
    btnSelectMusic->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnSelectLyric->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnSelectOutputDir->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnSelectMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectOutputDir->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnGuessLyricInfo = new BesButton(widgetMusicPath);
    btnGuessLyricInfo->setText(tr("搜词"));
    btnGuessLyricInfo->setToolTip(tr("尽可能智能地得到歌手和歌曲名，在歌词下载页面进行搜索"));
    btnGuessLyricInfo->setMinimumSize(60* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnGuessLyricInfo->setMaximumSize(60* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnGuessLyricInfo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	btnGuessLyricInfo->setVisible(false);

    btnDownloadMp3= new BesButton(widgetMusicPath);
    btnDownloadMp3->setText(tr("mp3"));
    btnDownloadMp3->setToolTip(tr("本软件无法直接播放 ncm 文件，可点此尝试下载其 mp3"));
    btnDownloadMp3->setMinimumSize(60* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnDownloadMp3->setMaximumSize(60* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnDownloadMp3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnDownloadMp3->setVisible(false);

    btnEditTxtLyric= new BesButton(widgetLyricPath);
    btnEditTxtLyric->setText(tr("编辑"));
    btnEditTxtLyric->setToolTip(tr("发现歌词出错时，可点击此直接编辑，然后“载入最新”"));
    btnEditTxtLyric->setMinimumSize(60* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnEditTxtLyric->setMaximumSize(60* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnEditTxtLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnEditTxtLyric->setVisible(false);


    QHBoxLayout* hLayout1 = new QHBoxLayout(widgetMusicPath);
    QHBoxLayout* hLayout2 = new QHBoxLayout(widgetLyricPath);
    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout1->setContentsMargins(0, 0, 0, 0);
    hLayout2->setContentsMargins(0, 0, 0, 0);
    hLayout1->addWidget(labelSelectMusic);
    hLayout1->addWidget(editSelectMusic);
    hLayout1->addWidget(btnDownloadMp3);
    hLayout1->addWidget(btnGuessLyricInfo);
    hLayout1->addWidget(btnSelectMusic);
    hLayout2->addWidget(labelSelectLyric);
    hLayout2->addWidget(editSelectLyric);
    hLayout2->addWidget(btnEditTxtLyric);
    hLayout2->addWidget(btnSelectLyric);
    hLayout3->addWidget(labelSelectOutputDir);
    hLayout3->addWidget(editSelectOutputDir);
    hLayout3->addWidget(btnSelectOutputDir);

    vLayout->addWidget(widgetMusicPath);
    vLayout->addWidget(widgetLyricPath);
    vLayout->addLayout(hLayout3);
    vLayout->addSpacerItem(new QSpacerItem(20,30* BesScaleUtil::scale(),
                                           QSizePolicy::Minimum,QSizePolicy::Minimum));

    labelTip= new QLabel(this);
    labelTipUp= new QLabel(this);
    labelTipEmpty =new QLabel(this);
    labelTipSpace= new QLabel(this);
    labelTipBack= new QLabel(this);
    labelTipDown = new QLabel(this);
    labelTipReturn= new QLabel(this);
    labelTip->setText(tr("制作提示："));
    labelTipUp->setText(tr("推上一行"));
    labelTipEmpty->setText(tr("空出一行"));
    labelTipBack->setText(tr("回退5秒"));
    labelTipDown->setText(tr("回退1行"));
    labelTipSpace->setText(tr("暂停"));
    labelTipReturn->setText(tr("结束"));

    labelTip->setMinimumHeight(28* BesScaleUtil::scale());
    labelTipUp->setMinimumHeight(28* BesScaleUtil::scale());
    labelTipEmpty->setMinimumHeight(28* BesScaleUtil::scale());
    labelTipSpace->setMinimumHeight(28* BesScaleUtil::scale());
    labelTipBack->setMinimumHeight(28* BesScaleUtil::scale());
    labelTipDown->setMinimumHeight(28* BesScaleUtil::scale());
    labelTipReturn->setMinimumHeight(28* BesScaleUtil::scale());

    labelTip->setMaximumWidth(150* BesScaleUtil::scale());
    labelTipUp->setMaximumWidth(150* BesScaleUtil::scale());
    labelTipEmpty->setMaximumWidth(150* BesScaleUtil::scale());
    labelTipSpace->setMaximumWidth(150* BesScaleUtil::scale());
    labelTipBack->setMaximumWidth(150* BesScaleUtil::scale());
    labelTipDown->setMaximumWidth(150* BesScaleUtil::scale());
    labelTipReturn->setMaximumWidth(150* BesScaleUtil::scale());

    labelTip->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipUp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipSpace->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipBack->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipDown->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipReturn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    btnLoadLastFiles = new BesButton(this);
    btnLoadLastFiles->setText(tr("载入最新"));
    btnLoadLastFiles->setMinimumHeight(28* BesScaleUtil::scale());
    btnLoadLastFiles->setMinimumWidth(100* BesScaleUtil::scale());
    btnLoadLastFiles->setMaximumWidth(200* BesScaleUtil::scale());
    btnLoadLastFiles->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QLabel *imageLabel1 = new QLabel(this);
    QLabel *imageLabel2 = new QLabel(this);
    QLabel *imageLabel3 = new QLabel(this);
    QLabel *imageLabel4 = new QLabel(this);
    QLabel *imageLabel5 = new QLabel(this);
    QLabel *imageLabel6 = new QLabel(this);
    imageLabel1->setMinimumHeight(28* BesScaleUtil::scale());
    imageLabel2->setMinimumHeight(28* BesScaleUtil::scale());
    imageLabel3->setMinimumHeight(28* BesScaleUtil::scale());
    imageLabel4->setMinimumHeight(28* BesScaleUtil::scale());
    imageLabel5->setMinimumHeight(28* BesScaleUtil::scale());
    imageLabel6->setMinimumHeight(28* BesScaleUtil::scale());

    imageLabel1->setMaximumWidth(30);
    imageLabel2->setMaximumWidth(30);
    imageLabel3->setMaximumWidth(30);
    imageLabel4->setMaximumWidth(30);
    imageLabel5->setMaximumWidth(50);
    imageLabel6->setMaximumWidth(30);

    imageLabel1->setPixmap(QPixmap(":/resource/image/key_up.png"));
    imageLabel2->setPixmap(QPixmap(":/resource/image/key_right.png"));
    imageLabel3->setPixmap(QPixmap(":/resource/image/key_back.png"));
    imageLabel4->setPixmap(QPixmap(":/resource/image/key_down.png"));
    imageLabel5->setPixmap(QPixmap(":/resource/image/key_space.png"));
    imageLabel6->setPixmap(QPixmap(":/resource/image/key_return.png"));

    imageLabel1->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel3->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel4->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel5->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel6->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QHBoxLayout* hLayout4 = new QHBoxLayout();
    QHBoxLayout* hLayout4_5 = new QHBoxLayout();

    hLayout4->addWidget(labelTip);
    hLayout4->addSpacerItem(new QSpacerItem(10* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel1);
    hLayout4->addWidget(labelTipUp);
    hLayout4->addSpacerItem(new QSpacerItem(10* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel2);
    hLayout4->addWidget(labelTipEmpty);
    hLayout4->addSpacerItem(new QSpacerItem(10* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel3);
    hLayout4->addWidget(labelTipBack);
    hLayout4->addSpacerItem(new QSpacerItem(10* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel4);
    hLayout4->addWidget(labelTipDown);
    hLayout4->addSpacerItem(new QSpacerItem(10* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel5);
    hLayout4->addWidget(labelTipSpace);
    hLayout4->addSpacerItem(new QSpacerItem(10* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel6);
    hLayout4->addWidget(labelTipReturn);
    hLayout4->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4_5->addSpacerItem(new QSpacerItem(10* BesScaleUtil::scale(),28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4_5->addWidget(btnLoadLastFiles);
    hLayout4->addLayout(hLayout4_5);

    vLayout->addLayout(hLayout4);
    vLayout->addSpacerItem(new QSpacerItem(20,15* BesScaleUtil::scale(),QSizePolicy::Minimum,QSizePolicy::Minimum));

    widgetCurrentMusicAndLyric = new QWidget(this);  //歌曲和歌词显示控件

    labelCurrentMusicTip= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentMusic= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentLyricTip= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentLyric= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentMusicTip->setText(tr("当前音乐："));
    labelCurrentLyricTip->setText(tr("当前歌词："));
    labelCurrentMusicTip->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    labelCurrentLyricTip->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    labelCurrentMusic->setMinimumHeight(28* BesScaleUtil::scale());
    labelCurrentLyric->setMinimumHeight(28* BesScaleUtil::scale());
    labelCurrentMusicTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrentLyricTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrentMusic->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelCurrentLyric->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    QHBoxLayout* hLayout5 = new QHBoxLayout();  //注意：布局定义属于整个父窗口
    QHBoxLayout* hLayout6 = new QHBoxLayout();
    hLayout5->addWidget(labelCurrentMusicTip);
    hLayout5->addWidget(labelCurrentMusic);
    hLayout6->addWidget(labelCurrentLyricTip);
    hLayout6->addWidget(labelCurrentLyric);

    QVBoxLayout* vBoxLayout1 = new QVBoxLayout(widgetCurrentMusicAndLyric);
    vBoxLayout1->addLayout(hLayout5);
    vBoxLayout1->addLayout(hLayout6);

    vLayout->addWidget(widgetCurrentMusicAndLyric);

    widgetLyricBoard = new QWidget(this);   //具体歌词显示板
    widgetLyricBoard->setObjectName("widgetLyricBoard");

    labelTimeTip= new QLabel(widgetLyricBoard);
    labelCurrenLineTip= new QLabel(widgetLyricBoard);
    labelNextLineTip= new QLabel(widgetLyricBoard);
    labelTimeTip->setText(tr("00:00.000"));
    labelCurrenLineTip->setText(tr("当前行："));
    labelNextLineTip->setText(tr("下一行："));
    labelTimeTip->setMinimumSize(110,28* BesScaleUtil::scale());
    labelCurrenLineTip->setMinimumSize(110,28* BesScaleUtil::scale());
    labelNextLineTip->setMinimumSize(110,28* BesScaleUtil::scale());
    labelTimeTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrenLineTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelNextLineTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    widgetLine0 = new QWidget(widgetLyricBoard);
    widgetMiddleLine = new QWidget(widgetLyricBoard);
    widgetLine4 = new QWidget(widgetLyricBoard);

    widgetLine0->setMinimumHeight(30* BesScaleUtil::scale());
    widgetLine0->setMaximumHeight(30* BesScaleUtil::scale());
    widgetLine0->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    labelLine0= new QLabel(widgetLine0);
    labelLine1= new QLabel(widgetLyricBoard);
    labelLine2= new QLabel(widgetMiddleLine);
    labelLine3= new QLabel(widgetLyricBoard);
    labelLine4= new QLabel(widgetLine4);
    labelLine0->setMinimumHeight(28* BesScaleUtil::scale());
    labelLine1->setMinimumHeight(28* BesScaleUtil::scale());
    labelLine2->setMinimumHeight(28* BesScaleUtil::scale());
    labelLine3->setMinimumHeight(28* BesScaleUtil::scale());
    labelLine4->setMinimumHeight(28* BesScaleUtil::scale());
    labelLine0->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelLine1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelLine2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelLine3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelLine4->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    labelLine0->setObjectName("labelLine0");
    labelLine1->setObjectName("labelLine1");
    labelLine2->setObjectName("labelLine2");
    labelLine3->setObjectName("labelLine3");
    labelLine4->setObjectName("labelLine4");

    labelDoneMark0 = new QLabel(widgetLine0);
    labelDoneMark0->setMinimumSize(30,30);
    labelDoneMark0->setMaximumSize(30,30);
    labelDoneMark0->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    labelDoneMark0->setText("♪");
    labelDoneMark0->setObjectName("labelDoneMark");

    labelDoneMark1 = new QLabel(widgetLyricBoard);
    labelDoneMark1->setMinimumSize(30,30);
    labelDoneMark1->setMaximumSize(30,30);
    labelDoneMark1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    labelDoneMark1->setText("♪");
    labelDoneMark1->setObjectName("labelDoneMark");

    labelDoneMark2 = new QLabel(widgetLyricBoard);
    labelDoneMark2->setMinimumSize(30,30);
    labelDoneMark2->setMaximumSize(30,30);
    labelDoneMark2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    labelDoneMark2->setText("♪");
    labelDoneMark2->setObjectName("labelDoneMark");

    labelDoneMark0->setVisible(false);
    labelDoneMark1->setVisible(false);
    labelDoneMark2->setVisible(false);

    editMiddleLine = new QLineEdit(widgetMiddleLine);
    editMiddleLine->setMinimumHeight(28* BesScaleUtil::scale());
    editMiddleLine->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editMiddleLine->setObjectName("editMiddleLine");
    editMiddleLine->setVisible(false);

    labelCurrenLineEmptyTip = new QLabel(widgetMiddleLine);
    labelCurrenLineEmptyTip->setText(tr("空行"));
    labelCurrenLineEmptyTip->setMinimumSize(80,25* BesScaleUtil::scale());
    labelCurrenLineEmptyTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrenLineEmptyTip->setObjectName("labelCurrenLineEmptyTip");
    labelCurrenLineEmptyTip->setAlignment(Qt::AlignCenter);
    labelCurrenLineEmptyTip->setVisible(false);

    middleSpacer = new QSpacerItem(1,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    //middleSpacer->changeSize(1,25,QSizePolicy::Ignored,QSizePolicy::Fixed);

    QHBoxLayout* hLayout7 = new QHBoxLayout(widgetLyricBoard);
    hLayout7->setContentsMargins(0, 0, 0, 0);
    hLayout7->addWidget(labelLine2);
    hLayout7->addWidget(labelDoneMark2);
    hLayout7->addWidget(editMiddleLine);
    hLayout7->addSpacerItem(middleSpacer);
    hLayout7->addWidget(labelCurrenLineEmptyTip);
    widgetMiddleLine->setLayout(hLayout7);

    btnEditLyricCurrent = new BesButton(widgetLine0);
    btnEditLyricCurrent->setText(tr("单行编辑"));
    btnEditLyricCurrent->setMinimumSize(85,28* BesScaleUtil::scale());
    btnEditLyricCurrent->setMaximumSize(85,28* BesScaleUtil::scale());
    btnEditLyricCurrent->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout* hLayoutLine0 = new QHBoxLayout();
    hLayoutLine0->addWidget(btnEditLyricCurrent);
    hLayoutLine0->addSpacerItem(new QSpacerItem(25,25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayoutLine0->addWidget(labelLine0);
    hLayoutLine0->addWidget(labelDoneMark0);
    hLayoutLine0->addSpacerItem(new QSpacerItem(1,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayoutLine0->setContentsMargins(0, 0, 0, 0);
    widgetLine0->setLayout(hLayoutLine0);

    btnEditBatchLyric = new BesButton(widgetLine4);
    btnEditBatchLyric->setText(tr("批量编辑"));
    btnEditBatchLyric->setMinimumSize(85,28* BesScaleUtil::scale());
    btnEditBatchLyric->setMaximumSize(85,28* BesScaleUtil::scale());
    btnEditBatchLyric->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout* hLayoutLine4 = new QHBoxLayout();
    hLayoutLine4->addWidget(btnEditBatchLyric);
    hLayoutLine4->addSpacerItem(new QSpacerItem(25,25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayoutLine4->addWidget(labelLine4);
    hLayoutLine4->setContentsMargins(0, 0, 0, 0);
    widgetLine4->setLayout(hLayoutLine4);


    QHBoxLayout* hLayout8 = new QHBoxLayout();  //注意：布局定义属于整个父窗口
    QHBoxLayout* hLayout9 = new QHBoxLayout();
    QHBoxLayout* hLayout10 = new QHBoxLayout();
    hLayout8->addWidget(labelTimeTip);
    hLayout8->addWidget(labelLine1);
    hLayout8->addWidget(labelDoneMark1);
    hLayout8->addSpacerItem(new QSpacerItem(1,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout9->addWidget(labelCurrenLineTip);
    hLayout9->addWidget(widgetMiddleLine);
    hLayout10->addWidget(labelNextLineTip);
    hLayout10->addWidget(labelLine3);

    QVBoxLayout* vBoxLayout2 = new QVBoxLayout(widgetLyricBoard);
    vBoxLayout2->addSpacerItem(new QSpacerItem(20,6* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::Fixed));
    vBoxLayout2->addWidget(widgetLine0);
    vBoxLayout2->addSpacerItem(new QSpacerItem(10,6,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vBoxLayout2->addLayout(hLayout8);
    vBoxLayout2->addLayout(hLayout9);
    vBoxLayout2->addLayout(hLayout10);
    vBoxLayout2->addSpacerItem(new QSpacerItem(10,6,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vBoxLayout2->addWidget(widgetLine4);
    vBoxLayout2->addSpacerItem(new QSpacerItem(20,6* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::Fixed));

    vLayout->addWidget(widgetLyricBoard);

    btnPreviewResult = new BesButton(this);
    btnOpenResult = new BesButton(this);
    btnToRemaking = new BesButton(this);
    btnStartMaking = new BesButton(this);
    btnPreviewResult->setText(tr("预览效果"));
    btnOpenResult->setText(tr("打开生成文件"));
    btnToRemaking->setText(tr("我要重制"));
    btnStartMaking->setText(tr("开始制作"));
    btnPreviewResult->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnOpenResult->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnToRemaking->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    btnStartMaking->setMinimumSize(100* BesScaleUtil::scale(),28* BesScaleUtil::scale());
    QHBoxLayout* hLayout11 = new QHBoxLayout();
    hLayout11->addWidget(btnPreviewResult);
    hLayout11->addWidget(btnOpenResult);
    hLayout11->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),28* BesScaleUtil::scale(),QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout11->addWidget(btnToRemaking);
    hLayout11->addWidget(btnStartMaking);
    vLayout->addSpacerItem(new QSpacerItem(20,15* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout11);
    vLayout->addSpacerItem(new QSpacerItem(20,15* BesScaleUtil::scale(),QSizePolicy::Minimum,QSizePolicy::MinimumExpanding));

    //初始状态,展开选择音乐和歌词路径
    toggleMusicAndLyricPath(true);
}

void SubPageMaking::initEntity()
{
    pathLoaded = false;
    pathResultLrcLyric = "";
    isMaking = false;
    isEditing = false;
    isBatchEditing =false;

    btnPreviewResult->setEnabled(false);
    btnOpenResult->setEnabled(false);
    btnToRemaking->setEnabled(false);
    btnStartMaking->setEnabled(false);

    editSelectOutputDir->setText(SettingManager::GetInstance().data().defaultOutputPath);
    pathOutputDir = SettingManager::GetInstance().data().defaultOutputPath;
}

void SubPageMaking::initConnection()
{
    connect(btnSelectMusic, &BesButton::clicked, this, QOverload<>::of(&SubPageMaking::selectMusicPath));
    connect(btnSelectLyric, &BesButton::clicked, this, QOverload<>::of(&SubPageMaking::selectLyricPath));
    connect(btnSelectOutputDir, &BesButton::clicked, this, &SubPageMaking::selectOutputDir);
    connect(btnLoadLastFiles, &BesButton::clicked, this, &SubPageMaking::loadCurrentPath);

    connect(btnEditLyricCurrent, &BesButton::clicked, this, &SubPageMaking::onEditCurrentLine);
    connect(btnEditBatchLyric, &BesButton::clicked, this, &SubPageMaking::onEditBatchLyric);

    connect(btnStartMaking, &BesButton::clicked, this, &SubPageMaking::startMaking);
    connect(btnToRemaking, &BesButton::clicked, this, &SubPageMaking::remaking);

    connect(btnPreviewResult, &BesButton::clicked, this, &SubPageMaking::previewResult);
    connect(btnOpenResult, &BesButton::clicked, this, &SubPageMaking::openResult);

    connect(btnGuessLyricInfo, &BesButton::clicked, this, &SubPageMaking::onGuessLyricInfo);
    connect(btnDownloadMp3, &BesButton::clicked, this, &SubPageMaking::onGuessNcmInfo);

    connect(btnEditTxtLyric, &BesButton::clicked, this, &SubPageMaking::onEditCurrentRawLyric);

    connect(editSelectMusic, &BesFileLineEdit::sig_filesHaveBeenDrop,
            [=](QList<QString> list){selectMusicPath(list.at(0));});
    connect(editSelectLyric, &BesFileLineEdit::sig_filesHaveBeenDrop,
            [=](QList<QString> list){selectLyricPath(list.at(0));});
    connect(editSelectOutputDir, &BesFileLineEdit::sig_filesHaveBeenDrop,
            [=](QList<QString> list){selectOutputPath(list.at(0));});
}

//推上一行
void SubPageMaking::markOneLine()
{
    if(lyricMaker.hasNextRawLine())
        lyricMaker.markCurrentRawLine(curAudioPos);

    updateLinesText();
}

//空出一行
void SubPageMaking::markEmptyLine()
{
    lyricMaker.markEmptyLine(curAudioPos);
    updateLinesText();
}

//回退5秒
void SubPageMaking::backBy5Second()
{
    emit onSeekBackward(5000);

    qint64 to = 0;
    if(curAudioPos > 5000)
        to = curAudioPos - 5000;

    lyricMaker.stepBackToTime(to);
    updateLinesText();
}

//回退到上一行
void SubPageMaking::backOneLine()
{
    //尝试获取上一行已经制作的歌词
    if(!lyricMaker.hasPreLrcLine())
        return;

    qint64 time = lyricMaker.getLastLrcLineTime();

    qint64 back = curAudioPos-time;
    if(back < 0)
        back = 0;

    emit onSeekBackward(back);
    lyricMaker.stepBackToTime(time);
    updateLinesText();
}

//暂停或开始
void SubPageMaking::playOrPause()
{
    emit onPlayOrPauseMusic();
}

//暂停
void SubPageMaking::pause()
{
    emit onPauseMusic();
}

//结束制作
void SubPageMaking::finishMaking()
{
    if(isMaking == true)
    {
        isMaking = false;

        lyricMaker.finishMaking();

        if(lyricMaker.isResultLrcEmpty())
        {
            BesMessageBox::information( tr("提示"),tr("制作结果为空，请重新按提示制作"));

            btnLoadLastFiles->setEnabled(true);
            btnPreviewResult->setEnabled(false);
            btnOpenResult->setEnabled(false);
            btnToRemaking->setEnabled(false);
        }
        else
        {
            QFileInfo musicFile(pathMusic);
            QString outputFile = pathOutputDir + "/" + musicFile.completeBaseName() + ".lrc";

            //保存 lrc 文件，如果保存失败则不断尝试，直到保存成功或用户取消为止
            bool saved = false;
            do{
                if(lyricMaker.saveLyrc(outputFile))
                {
                    pathResultLrcLyric = outputFile;
                    BesMessageBox::information(tr("提示"),tr("成功保存到：")+outputFile);

                    emit sig_addToMakingHistory(pathMusicLoaded, pathResultLrcLyric);

                    btnPreviewResult->setEnabled(true);
                    btnOpenResult->setEnabled(true);

                    saved = true;
                    break;//成功保存，可退出循环
                }
                else
                {
                    //提示保存失败，并且为了保留用户制作歌词的成果，让用户选择其他路径保存
                    QString tip = tr("保存失败：")+outputFile;
                    tip += "\n\n";
                    tip += tr("保存失败的原因可能有：文件名包含非法字符、目标文件被占用、目标文件为只读文件、本程序没有写入目标文件的权限等");
                    tip += "\n\n";
                    tip += tr("按“确定”后可选择另一个保存文件的目录 :)");
                    int nRet = BesMessageBox::information(tr("提示"),tip, true);
                    if(QMessageBox::StandardButton::Ok == nRet)
                    {
                        QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), outputFile, tr("Lrc 文件 (*.lrc)"));
                        if(!fileName.isEmpty())
                            outputFile = fileName;
                        else
                            break;//未选择文件而取消
                    }
                    else
                        break;//关闭提示，取消保存
                }
            }while(true);

            if(!saved)
            {
                btnPreviewResult->setEnabled(false);
                btnOpenResult->setEnabled(false);
            }

            btnLoadLastFiles->setEnabled(true);
            btnToRemaking->setEnabled(false);
        }

        emit onStopMusic();

        emit onExitMakingMode();

        //退出制作歌词状态,展开选择音乐和歌词路径
        toggleMusicAndLyricPath(true);

        //检测歌词是否发生改变，并提示是否保存回源歌词文件
        if(lyricMaker.isLyricModified())
        {
            if(QMessageBox::StandardButton::Ok ==
                    BesMessageBox::question( tr("提示"),tr("检测到原歌词可能已发生更改，是否覆盖到：")+lyricMaker.getRawLyricPath())
                    )
            {
                if(lyricMaker.saveToRawLyric())
                    BesMessageBox::information( tr("提示"),tr("歌词更改已保存到：")+lyricMaker.getRawLyricPath());
                else
                    BesMessageBox::information( tr("提示"),tr("保存失败，无法保存到：")+lyricMaker.getRawLyricPath());
            }
        }
    }
}

//点击了预览效果
void SubPageMaking::previewResult()
{
    if(pathResultLrcLyric == "")
        return;

    if(!pathLoaded) //路径都没有载入
        return;

    //载入lrc歌词，并且播放当前歌曲来预览
    emit loadLrcLyricAndSwitchToPreview(pathResultLrcLyric);

}

void SubPageMaking::openResult()
{
    QFileInfo fileInfo(pathResultLrcLyric);
    QString dirPath = fileInfo.dir().absolutePath();
    QDesktopServices::openUrl(QUrl("file:///"+dirPath, QUrl::TolerantMode));
    QDesktopServices::openUrl(QUrl("file:///"+pathResultLrcLyric, QUrl::TolerantMode));
}

//更新当前行内容的显示
void SubPageMaking::updateLinesText()
{
    QString line0,line1, line2, line3, line4;
    lyricMaker.getPPreLrcLineText(line0);
    lyricMaker.getPreLrcLineText(line1);

    bool bRet = lyricMaker.getCurrentLrcLineText(line2);    //为了能够显示空行，先尝试获取lrc
    if(!bRet)
    {
        //一开始是空行，不能编辑
        btnEditLyricCurrent->setEnabled(false);

        lyricMaker.getCurrentRawLineText(line2);  //一开始未制作时，lrc为空，获取失败，获取raw
    }
    else
    {
        //获取成功，看是否为空，为空显示空行提示
        if(line2.size() == 0)
        {
            labelCurrenLineEmptyTip->setVisible(true);

            //空行不能编辑
            btnEditLyricCurrent->setEnabled(false);
        }
        else
        {
            labelCurrenLineEmptyTip->setVisible(false);

            //非空行能编辑
            btnEditLyricCurrent->setEnabled(true);
        }
    }

    lyricMaker.getNextRawLineText(line3);
    lyricMaker.getNNextRawLineText(line4);

    labelDoneMark0->setVisible(line0.size()!=0);
    labelDoneMark1->setVisible(line1.size()!=0);
    labelDoneMark2->setVisible(line2.size()!=0);

    labelLine0->setText(line0);
    labelLine1->setText(line1);
    labelLine2->setText(line2);
    labelLine3->setText(line3);
    labelLine4->setText(line4);
}



void SubPageMaking::selectMusicPath()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开音频文件"),
                                                      SettingManager::GetInstance().data().defaultMusicPath,
                                                      tr("音频 (*.mp3 *.wav *.ncm);;视频 (*.mp4);;其他 (*.*)"));

    selectMusicPath(fileName);
}

void SubPageMaking::selectLyricPath()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("打开原歌词文件"),
                                                     SettingManager::GetInstance().data().defaultLyricPath,
                                                      tr("文本 (*.txt);;其他 (*.*)"));
    selectLyricPath(fileName);
}

void SubPageMaking::selectOutputDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("打开 LRC 歌词输出目录"),
                                     SettingManager::GetInstance().data().defaultOutputPath,
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    selectOutputPath(dir);
}

void SubPageMaking::loadCurrentPath()
{
    //检测路径是否都选择完毕
    if(pathMusic.size() == 0)
    {
        BesMessageBox::information( tr("提示"),tr("请先选择音乐"));
        return;
    }

    if(pathLyric.size() == 0)
    {
        BesMessageBox::information( tr("提示"),tr("请先选择歌词"));
        return;
    }

    if(pathOutputDir.size() == 0)
    {
        BesMessageBox::information( tr("提示"),tr("请先选择输出目录"));
        return;
    }

    if("ncm" == pathMusic.right(pathMusic.size() - pathMusic.lastIndexOf('.') - 1).trimmed().toLower())
    {
        BesMessageBox::information(tr("提示"),tr("本软件无法直接播放 ncm 文件，可点击【mp3】按钮，尝试下载其 mp3"));
        return;
    }


    //载入歌词到歌词制作器
    if(!lyricMaker.loadRawLyric(pathLyric))
    {
        BesMessageBox::information( tr("提示"),tr("读取歌词失败"));
        return;
    }

    if(lyricMaker.isRawLyricEmpty())
    {
        BesMessageBox::information( tr("提示"),tr("歌词数据为空，请重新载入歌词文件"));
        return;
    }

    pathLoaded = true;

    emit onReloadMusic(pathMusic); 

    pathMusicLoaded = pathMusic;

    //初始化歌词制作过程
    initMakingProcess(true);

    btnStartMaking->setEnabled(true);
    btnPreviewResult->setEnabled(false);
    btnOpenResult->setEnabled(false);
}


void SubPageMaking::startMaking()
{
    if(!pathLoaded)
        return; //路径没有成功加载，不响应

    if(isMaking == false)  //没有在制作时才允许响应制作的操作
    {
        isMaking = true;

        emit onStartMaking();

        emit onEnterMakingMode();

        btnStartMaking->setEnabled(false);
        btnLoadLastFiles->setEnabled(false);
        btnToRemaking->setEnabled(true);

        //进入制作歌词状态,不展开选择音乐和歌词路径
        toggleMusicAndLyricPath(false);
    }
}


void SubPageMaking::remaking()
{
    if(!pathLoaded)
        return; //路径没有成功加载，不响应

    if(isMaking == true)  //在制作时才允许响应制作的操作
    {
        isMaking = false;  //必须先置为 false ，因为音乐的停止会触发 finishMaking 操作

        emit onStopMusic();

        emit onExitMakingMode();

        //初始化歌词制作过程
        initMakingProcess(false);

        btnToRemaking->setEnabled(false);
        btnStartMaking->setEnabled(true);
        btnLoadLastFiles->setEnabled(true);

        //退出制作歌词状态,展开选择音乐和歌词路径
        toggleMusicAndLyricPath(true);
    }
}

void SubPageMaking::updatePos(qint64 pos)
{
	if (isMaking)
	{
		curAudioPos = pos;

		//计算时间字符串

		int ms = pos % 1000;
		pos = pos / 1000;
		int s = pos % 60;
		int m = pos / 60;

		auto timeLabel = QString{"%1:%2.%3"}
			.arg(m, 2, 10, QLatin1Char{'0'})
			.arg(s, 2, 10, QLatin1Char{'0'})
			.arg(ms, 3, 10, QLatin1Char{'0'});

		labelTimeTip->setText(timeLabel);
    }
}

void SubPageMaking::onGuessNcmInfo()
{
    if(pathMusic.size() != 0)
    {
        btnDownloadMp3->setEnabled(false);
        if(!threadGuessLyricInfo.StartGuess(pathMusic, true))
            btnDownloadMp3->setEnabled(true);
    }
}

void SubPageMaking::onGuessLyricInfo()
{
	if (pathMusic.size() != 0)
	{
		btnGuessLyricInfo->setEnabled(false);
        if(!threadGuessLyricInfo.StartGuess(pathMusic, false))
            btnGuessLyricInfo->setEnabled(true);
    }
}

void SubPageMaking::onEditCurrentRawLyric()
{
    QDesktopServices::openUrl(QUrl("file:///"+pathLyric, QUrl::TolerantMode));
}

//编辑当前行
void SubPageMaking::onEditCurrentLine()
{
    toggleMiddleLineEdit(true);
}

//批量编辑歌词
void SubPageMaking::onEditBatchLyric()
{
    isBatchEditing = true;
    pause();

    LyricEditorBox* lyricEditor = new LyricEditorBox(nullptr);

    QVector<QString>rawLines;
    QVector<QPair<qint64, QString>> lrcLines;
    lyricMaker.getLyricData(rawLines,lrcLines);
    lyricEditor->setLyricData(rawLines,lrcLines);

    lyricEditor->exec();

    if(lyricEditor->isResultOK())
    {
        //取出结果，更新当前歌词
        lyricEditor->getLyricData(rawLines,lrcLines);
        lyricMaker.updateLyricData(rawLines,lrcLines);
        updateLinesText();
    }

    delete lyricEditor;

    isBatchEditing = false;
}

//默认输出路径发生改变
void SubPageMaking::OnDefaultPathOutputChanged(QString path)
{
    pathOutputDir = SettingManager::GetInstance().data().defaultOutputPath;
    editSelectOutputDir->setText(SettingManager::GetInstance().data().defaultOutputPath); //LRC 歌词存放目录
}

void SubPageMaking::selectMusicPath(const QString& musicPath)
{
    if(musicPath.size() !=0)
    {
        editSelectMusic->setText(musicPath);
        pathMusic = musicPath;
		btnGuessLyricInfo->setVisible(true);

        QString extension = musicPath.right( musicPath.size() - musicPath.lastIndexOf('.') -1);
        btnDownloadMp3->setVisible(extension.toLower() == "ncm");       //ncm需要显示下载mp3按钮
    }

}

void SubPageMaking::selectLyricPath(const QString& lyricPath)
{
    if(lyricPath.size() !=0)
    {
        editSelectLyric->setText(lyricPath);
        pathLyric = lyricPath;

        btnEditTxtLyric->setVisible(true);
    }
}

void SubPageMaking::selectOutputPath(const QString& outputPath)
{
    if(outputPath.size() !=0)
    {
        editSelectOutputDir->setText(outputPath);
        pathOutputDir = outputPath;
    }
}

//初始化制作歌词的过程
void SubPageMaking::initMakingProcess(bool updateCurrentSongLyric)
{
    if(pathLoaded)
    {
        labelTimeTip->setText("00:00.000");

        if(updateCurrentSongLyric)
        {
            //更新当前选中的路径提示
            labelCurrentMusic->setText(pathMusic);
            labelCurrentLyric->setText(pathLyric);
        }

        //获取数据填充到行上
        lyricMaker.startMaking();

        QString firstLine;
        QString secondLine;
        lyricMaker.getNextRawLineText(firstLine);
        lyricMaker.getNNextRawLineText(secondLine);

        labelLine0->setText("");
        labelLine1->setText("");
        labelLine2->setText("");
        labelLine3->setText(firstLine);
        labelLine4->setText(secondLine);
        labelCurrenLineEmptyTip->setVisible(false);

        labelDoneMark0->setVisible(false);
        labelDoneMark1->setVisible(false);
        labelDoneMark2->setVisible(false);

        //一开始是空行，不能编辑
        btnEditLyricCurrent->setEnabled(false);
    }
}

//切换是否显示路径
void SubPageMaking::toggleMusicAndLyricPath(bool bExtend)
{
    widgetMusicPath->setVisible(bExtend);
    widgetLyricPath->setVisible(bExtend);
    widgetLine0->setVisible(!bExtend);
    widgetLine4->setVisible(!bExtend);
}

//是否切换出中间行编辑
void SubPageMaking::toggleMiddleLineEdit(bool showEdit)
{
    static QString lineBackup;
    if(showEdit)
    {
        pause();

        labelLine2->setVisible(false);
        labelDoneMark2->setVisible(false);
        middleSpacer->changeSize(1,25,QSizePolicy::Ignored,QSizePolicy::Fixed);
        editMiddleLine->setVisible(true);

        lineBackup = labelLine2->text();
        editMiddleLine->setText(lineBackup);

        btnEditLyricCurrent->setEnabled(false);
        isEditing = true;
    }
    else
    {
        labelLine2->setVisible(true);
        labelDoneMark2->setVisible(true);
        middleSpacer->changeSize(1,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
        editMiddleLine->setVisible(false);

        QString newLine = editMiddleLine->text().trimmed();
        if(newLine.isEmpty())
        {
            labelLine2->setText(lineBackup);    //编辑单行时，编辑后歌词不能为空
        }
        else
        {
            if(newLine != lineBackup)
            {
                labelLine2->setText(newLine);       //保存更新当前的歌词
                lyricMaker.updateCurrentLineText(newLine);
            }
        }

        btnEditLyricCurrent->setEnabled(true);
        isEditing = false;
    }
}


