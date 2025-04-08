
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


#include "MessageDecoderPlugin.h"

namespace MessageDecoderPlugin
{
MessageDecoderPlugin::MessageDecoderPlugin(const std::string &name) : PluginClient(name)
{
}

void MessageDecoderPlugin::UpdateConfigSettings()
{
    // Configuration settings are retrieved from the API using the GetConfigValue template class.
    // This method does NOT execute in the main thread, so variables must be protected
    // (e.g. using std::atomic, std::mutex, etc.).
    std::lock_guard<mutex> lock(_cfgLock);
    GetConfigValue("Remote IP", remoteIP_);
    GetConfigValue("Remote Port", remotePort_);
}

void MessageDecoderPlugin::OnConfigChanged(const char *key, const char *value)
{
    PluginClient::OnConfigChanged(key, value);
    UpdateConfigSettings();
}

void MessageDecoderPlugin::OnStateChange(IvpPluginState state)
{
    PluginClient::OnStateChange(state);

    if (state == IvpPluginState_registered)
    {
        UpdateConfigSettings();
    }
}

// Override of main method of the plugin that should not return until the plugin exits.
// This method does not need to be overridden if the plugin does not want to use the main thread.
int MessageDecoderPlugin::Main()
{
    PLOG(logINFO) << "Starting MessageDecoderPlugin...";

    while (_plugin->state != IvpPluginState_error)
    {
        PLOG(logDEBUG4) <<"MessageDecoderPlugin: Sleeping 5 minutes";
        this_thread::sleep_for(chrono::milliseconds(300000));
    }

    PLOG(logINFO) << "MessageDecoderPlugin terminating gracefully.";
    return EXIT_SUCCESS;
}

} /* namespace MessageDecoderPlugin */

int main(int argc, char *argv[])
{
    return run_plugin<MessageDecoderPlugin::MessageDecoderPlugin>("MessageDecoderPlugin", argc, argv);
}

