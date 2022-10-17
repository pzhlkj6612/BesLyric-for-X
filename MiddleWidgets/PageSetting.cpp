﻿#include <Define/Define.h>
#include "PageSetting.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include "BesScaleUtil.h"

PageSetting::PageSetting(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initLayout();
    initConnection();
}

PageSetting::~PageSetting()
{

}

void PageSetting::initLayout()
{
    QGridLayout* mainLayout = new QGridLayout(this);
    pageSettingContainer = new QWidget(this);
    pageSettingContainer->setObjectName("pageSettingContainer");
    pageSettingContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(pageSettingContainer);


    QVBoxLayout* vMainLayout = new QVBoxLayout(pageSettingContainer);

    settingTopPanel = new QWidget(pageSettingContainer);
    settingTopPanel->setMaximumHeight(65 *BesScaleUtil::scale());
    settingTopPanel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    settingTopPanel->setObjectName("settingTopPanel");
    settingTopPanel->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    settingTitle = new QLabel(settingTopPanel);
    btnPackupSetting = new BesButton(settingTopPanel);
    settingTitle->setObjectName("settingTitle");
    btnPackupSetting->setObjectName("btnPackupSetting");
    settingTitle->setText(tr("设置"));

    QHBoxLayout* hSettingLayout = new QHBoxLayout(settingTopPanel);
    hSettingLayout->setContentsMargins(
        10 * BesScaleUtil::scale(),
        10 * BesScaleUtil::scale(),
        10 * BesScaleUtil::scale(),
        10 * BesScaleUtil::scale());
    hSettingLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hSettingLayout->addWidget(settingTitle);
    hSettingLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    hSettingLayout->addWidget(btnPackupSetting);
    hSettingLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));

    settingWidget = new SettingWidget(pageSettingContainer);
    settingWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    vMainLayout->setContentsMargins(0, 0, 0, 0);
    vMainLayout->setSpacing(0);
    vMainLayout->addWidget(settingTopPanel);
    vMainLayout->addWidget(settingWidget);

}

void PageSetting::initConnection()
{

}
