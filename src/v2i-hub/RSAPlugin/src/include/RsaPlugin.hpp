//==========================================================================
// Name        : RsaPlugin.cpp
// Author      : FHWA Saxton Transportation Operations Laboratory  
// Version     :
// Copyright   : Copyright (c) 2023 FHWA Saxton Transportation Operations Laboratory. All rights reserved.
// Description : RSA Plugin
//==========================================================================
#pragma once
#include <string.h>

#include "PluginClient.h"
#include "PluginDataMonitor.h"

#include <atomic>
#include <thread>
#include <RoadSideAlert.h>
#include <tmx/j2735_messages/J2735MessageFactory.hpp>
#include <tmx/j2735_messages/RoadSideAlertMessage.hpp>

#include <UdpClient.h>
#include <tmx/messages/auto_message.hpp>
#include "RsaPluginWorker.hpp"


#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QHostAddress>
#include <QRegExp>
#include <QStringList>
#include <QSharedPointer>
#include <QObject>

#ifdef __linux__
#include <signal.h>
#include <unistd.h>
#endif
#include <qhttpengine/server.h>
#include <qserverPedestrian/OAIApiRouter.h>
#include <qserverPedestrian/OAIPSM.h>
#include <queue>


using namespace std;
using namespace tmx;
using namespace tmx::messages;
using namespace tmx::utils;
using namespace OpenAPI;

namespace RsaPlugin
{

/**
 * This plugin is an example to demonstrate the capabilities of a TMX plugin.
 */
class RsaPlugin: public PluginClient
{
public:
	RsaPlugin(std::string);
	virtual ~RsaPlugin();
	int Main();

protected:
	void UpdateConfigSettings();

	// Virtual method overrides.
	void OnConfigChanged(const char *key, const char *value);
	void OnStateChange(IvpPluginState state);

	void HandleRoadSideAlertMessage(RsaMessage &msg, routeable_message &routeableMsg);
	void BroadcastRsa(char *rsaJson);
	
	int  StartWebService();
	void RsaRequestHandler(QHttpEngine::Socket *socket);
	void writeResponse(int responseCode , QHttpEngine::Socket *socket);

private:
	tmx::utils::UdpClient *_signSimClient = NULL;
	J2735MessageFactory factory;
	
	uint16_t webport;
	string webip;

};
mutex _cfgLock;

}
