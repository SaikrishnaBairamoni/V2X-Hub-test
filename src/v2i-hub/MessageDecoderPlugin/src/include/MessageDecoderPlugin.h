/**
 * Copyright (C) 2025 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this plogFile except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#pragma once

#include "PluginClient.h"
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include <chrono>
#include <thread>

using namespace std;
using namespace tmx;
using namespace tmx::utils;
using namespace tmx::messages;


namespace MessageDecoderPlugin
{
class MessageDecoderPlugin: public PluginClient
{
public:
    /**
     * @brief Construct a new MessageDecoderPlugin with the given name.
     * @param name The name to give the plugin for identification purposes.
     */
    explicit MessageDecoderPlugin(const std::string &name);

    /**
     * @brief Main method for running the plugin.
     * @param argc number of arguments
     * @param argv array of arguments
     */
    int Main() override;

protected:
    void UpdateConfigSettings();

    // Virtual method overrides.
    /**
     * @brief Called when configuration is changed
     * @param key Key of the configuration value changed
     * @param value Changed value
     */
    void OnConfigChanged(const char *key, const char *value);

    /**
     * @brief Called on plugin state change
     * @param state New plugin state
     */
    void OnStateChange(IvpPluginState state);

    /** 
     * @brief 
     * @param msg 
     */
    void OnMessageReceived(IvpMessage *msg);
 
private:
    std::mutex _cfgLock;
    string remoteIP_;
    int remotePort_;
 
};
} /* namespace MessageDecoderPlugin */
