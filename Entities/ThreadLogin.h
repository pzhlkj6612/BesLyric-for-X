﻿#ifndef ThreadLogin_H
#define ThreadLogin_H

#include "Define/Static.h"
#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStringList>
#include <QSysInfo>
#include "NetAccess.h"
#include "SettingManager.h"

class ThreadLogin :public QThread
{
    Q_OBJECT

public:
    ThreadLogin(){}

    bool SendLogin()
    {
       m_bAnonymity = SettingManager::GetInstance().data().loginAnonymously;

       start(NormalPriority);
       return true;
    }


protected:
    virtual void run()
    {
        SendLoginInfo(m_bAnonymity);
    }


    //发送登录信息（ip地址）
    void SendLoginInfo(bool bAnonymity)
    {
        //获得ip地址的网页源，2个备用
        const int SrcCount = 2;
        QString ipSrc[SrcCount]={
            "http://ip.qq.com/",
            "https://whatismyipaddress.com/",
        };

        //单个源最大检测ip的次数
        int nMaxSingleCheckCount = 5;

        QString strIP= "";
        QString tempBuffer;

        if(bAnonymity)
        {
            strIP = "Anonymity";
        }
        else
        {
            int nLeftCheckCount = nMaxSingleCheckCount * SrcCount; // 计算总共检测次数
            while(nLeftCheckCount-1)
            {
                bool bRet = NetworkAccess::SyncDownloadString(
                            ipSrc[(nLeftCheckCount-1) / nMaxSingleCheckCount],tempBuffer);

                if(bRet == false)
                {
                    //可能没网络，或网络异常
                    //等待5秒再检测
                    nLeftCheckCount--;
                    sleep(5);
                    continue;
                }

                // https://whatismyipaddress.com/ 和 http://ip.qq.com/ 的网页中IP都是 >ddd.ddd.ddd.ddd< 特征，查找该特征的字符串

                CatchIPStr(tempBuffer, strIP);

                if(strIP.size() != 0)
                    break;//已经获得ip，退出循环

                nLeftCheckCount--;
            }
        }

        if(strIP.size() == 0)
            strIP = "unknown";

        //访问链接，服务端负责记录登录信息
#if (QT_VERSION < QT_VERSION_CHECK(5, 13, 0))
        QUrlQuery loginInfoQuery{};
        loginInfoQuery.addQueryItem("ip", strIP);
        loginInfoQuery.addQueryItem("version", VERSION_NAME);
        loginInfoQuery.addQueryItem("vernum", VERSION_NUMBER);
        loginInfoQuery.addQueryItem("systemArchitecture",
             "[Build]" + QSysInfo::buildCpuArchitecture()
             + "[Current]" + QSysInfo::currentCpuArchitecture()
         );
#else
        QUrlQuery loginInfoQuery{
            QPair<QString, QString>{"ip", strIP},
            QPair<QString, QString>{"version", VERSION_NAME},
            QPair<QString, QString>{"vernum", VERSION_NUMBER},
            QPair<QString, QString>{"systemArchitecture",
                "[Build]" + QSysInfo::buildCpuArchitecture()
                + "[Current]" + QSysInfo::currentCpuArchitecture()
            }
        };
#endif
        NetworkAccess::SyncDownloadString(LINK_SEND_LOGIN, tempBuffer, loginInfoQuery, false);
    }


    //俘获满足需求的IP字符串
    bool CatchIPStr( QString &buffer, QString& ip)
    {
        // > . . . <

        QRegularExpression rx{">(\\d+\\.\\d+\\.\\d+\\.\\d+)<"};
        auto match = rx.match(buffer);

        if (!match.hasMatch()) {
            return false;
        } else {
            ip = match.captured(1);
            return true;
        }
    }

private:
    bool m_bAnonymity;
};


#endif // ThreadLogin_H
