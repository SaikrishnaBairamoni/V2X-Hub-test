//==========================================================================
// Name        : RsaPlugin.cpp
// Author      : FHWA Saxton Transportation Operations Laboratory  
// Version     :
// Copyright   : Copyright (c) 2023 FHWA Saxton Transportation Operations Laboratory. All rights reserved.
// Description : RSA Plugin
//==========================================================================
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <map>
#include <chrono>
#include <RoadSideAlert.h>
#include <tmx/j2735_messages/RoadSideAlertMessage.hpp>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "PluginClient.h"
#include "PluginDataMonitor.h"

using namespace std;
using namespace tmx;
using namespace tmx::messages;
using namespace tmx::utils;

namespace RsaPlugin {

	class RsaPluginWorker {
		public:
			// struct RSA {
			// 	RoadSideAlert rsa;
			// 	RSA(int anInt, double aDouble) : rsa.TemporaryID_t(anInt), number(aDouble) { }

			// };
			
	};



};

