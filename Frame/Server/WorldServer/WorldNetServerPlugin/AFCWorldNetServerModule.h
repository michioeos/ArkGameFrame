/*
* This source file is part of ArkGameFrame
* For the latest info, see https://github.com/ArkGame
*
* Copyright (c) 2013-2017 ArkGame authors.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#pragma once

#include "SDK/Proto/AFMsgDefine.h"
#include "SDK/Interface/AFIKernelModule.h"
#include "SDK/Interface/AFIClassModule.h"
#include "SDK/Interface/AFIElementModule.h"
#include "SDK/Interface/AFILogModule.h"
#include "Server/Interface/AFIWorldToMasterModule.h"
#include "Server/Interface/AFIWorldLogicModule.h"
#include "Server/Interface/AFINetServerModule.h"
#include "Server/Interface/AFIWorldNetServerModule.h"
#include "Server/Interface/AFILoginNetServerModule.h"

class AFCWorldNetServerModule
    : public AFIWorldNetServerModule
{
public:
    AFCWorldNetServerModule(AFIPluginManager* p)
        : m_pElementModule(nullptr)
        , m_pClassModule(nullptr)
        , m_pWorldLogicModule(nullptr)
        , m_pKernelModule(nullptr)
        , m_pLogModule(nullptr)
        , m_pNetModule(nullptr)
    {
        pPluginManager = p;
        mnLastCheckTime = pPluginManager->GetNowTime();
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}

    virtual bool SendMsgToGame(const int nGameID, const AFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const AFGUID nPlayer = AFGUID());
    virtual bool SendMsgToGame(const AFIDataList& argObjectVar, const AFIDataList& argGameID,  const AFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData);
    virtual bool SendMsgToPlayer(const AFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const AFGUID nPlayer);

    virtual int OnObjectListEnter(const AFIDataList& self, const AFIDataList& argVar);
    virtual int OnObjectListLeave(const AFIDataList& self, const AFIDataList& argVar);
    virtual int OnPropertyEnter(const AFIDataList& argVar, const AFIDataList& argGameID, const AFGUID& self);
    virtual int OnRecordEnter(const AFIDataList& argVar, const AFIDataList& argGameID, const AFGUID& self);
    virtual bool OnRecordEnterPack(AFRecord* pRecord, AFMsg::ObjectRecordBase* pObjectRecordBase);

    virtual ARK_SHARE_PTR<ServerData> GetSuitProxyForEnter();
    virtual AFINetServerModule* GetNetModule();

    virtual int GetPlayerGameID(const AFGUID self);

protected:
    void OnSocketEvent(const NetEventType eEvent, const AFGUID& xClientID, const int nServerID);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const AFGUID& xClientID);
    //有连接
    void OnClientConnected(const AFGUID& xClientID);

protected:
    void OnGameServerRegisteredProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);
    void OnGameServerUnRegisteredProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);
    void OnRefreshGameServerInfoProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);

    void OnProxyServerRegisteredProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);
    void OnProxyServerUnRegisteredProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);
    void OnRefreshProxyServerInfoProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);

    int OnLeaveGameProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);
    //////////////////////////////////////////////////////////////////////////

    void SynGameToProxy();
    void SynGameToProxy(const AFGUID& xClientID);

    //////////////////////////////////////////////////////////////////////////
    void LogGameServer();

protected:
    void OnOnlineProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);
    void OnOfflineProcess(const AFIMsgHead& xHead, const int nMsgID, const char* msg, const uint32_t nLen, const AFGUID& xClientID);


private:
    int64_t mnLastCheckTime;

    //serverid,data
    AFMapEx<int, ServerData> mGameMap;
    AFMapEx<int, ServerData> mProxyMap;

    AFIElementModule* m_pElementModule;
    AFIClassModule* m_pClassModule;
    AFIWorldLogicModule* m_pWorldLogicModule;
    AFIKernelModule* m_pKernelModule;
    AFILogModule* m_pLogModule;
    AFINetServerModule* m_pNetModule;
};