//==========================================================================
// Name        : RsaPlugin.cpp
// Author      : FHWA Saxton Transportation Operations Laboratory  
// Version     :
// Copyright   : Copyright (c) 2019 FHWA Saxton Transportation Operations Laboratory. All rights reserved.
// Description : Rsa Plugin
//==========================================================================

#include "include/RsaPlugin.hpp"

namespace RsaPlugin
{
/**
 * Construct a new RsaPlugin with the given name.
 *
 * @param name The name to give the plugin for identification purposes.
 */
RsaPlugin::RsaPlugin(string name): PluginClient(name)
{
	// The log level can be changed from the default here.
	FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

	// Subscribe to all messages specified by the filters above.
	SubscribeToMessages();
}

void RsaPlugin::RsaRequestHandler(QHttpEngine::Socket *socket)
{
	auto router = QSharedPointer<OpenAPI::OAIApiRouter>::create();
	QString st; 
	while(socket->bytesAvailable()>0)
	{	
		st.append(socket->readAll());
	}
	QByteArray array = st.toLocal8Bit();

	char* rsaMsgdef = array.data();	
	// Catch parse exceptions
    try {
	    BroadcastRsa(rsaMsgdef);
		writeResponse(QHttpEngine::Socket::Created, socket);
	}
	catch(const J2735Exception &e) {
        PLOG(logERROR) << "Error parsing file: " << e.what() << std::endl;
		writeResponse(QHttpEngine::Socket::BadRequest, socket);
	}
}


int RsaPlugin::StartWebService()
{
	//Web services 
	char *placeholderX[1]={0};
	int placeholderC=1;
	QCoreApplication a(placeholderC,placeholderX);

 	QHostAddress address = QHostAddress(QString::fromStdString (webip));
    quint16 port = static_cast<quint16>(webport);


	QSharedPointer<OpenAPI::OAIApiRequestHandler> handler(new OpenAPI::OAIApiRequestHandler());
	handler = QSharedPointer<OpenAPI::OAIApiRequestHandler> (new OpenAPI::OAIApiRequestHandler());

    QObject::connect(handler.data(), &OpenAPI::OAIApiRequestHandler::requestReceived, [&](QHttpEngine::Socket *socket) {

		this->RsaRequestHandler(socket);
    });

    QHttpEngine::Server server(handler.data());

    if (!server.listen(address, port)) {
        qCritical("Unable to listen on the specified port.");
        return 1;
    }
	return a.exec();

}

RsaPlugin::~RsaPlugin()
{
	if (_signSimClient != NULL)
		delete _signSimClient;
}

void RsaPlugin::UpdateConfigSettings()
{
	// Configuration settings are retrieved from the API using the GetConfigValue template class.
	// This method does NOT execute in the main thread, so variables must be protected
	// (e.g. using std::atomic, std::mutex, etc.).

	int instance;
	std::lock_guard<mutex> lock(_cfgLock);

	GetConfigValue<string>("WebServiceIP",webip);
	GetConfigValue<uint16_t>("WebServicePort",webport);
	GetConfigValue<int>("Instance", instance);

	std::thread webthread(&RsaPlugin::StartWebService,this);
	webthread.detach(); // wait for the thread to finish 
}

void RsaPlugin::OnConfigChanged(const char *key, const char *value)
{
	PluginClient::OnConfigChanged(key, value);
	UpdateConfigSettings();
}

void RsaPlugin::OnStateChange(IvpPluginState state)
{
	PluginClient::OnStateChange(state);

	if (state == IvpPluginState_registered)
	{
		UpdateConfigSettings();
	}
}


void RsaPlugin::BroadcastRsa(char * rsaJson) 
{  //overloaded 

	RsaMessage rsamessage;
	RsaEncodedMessage rsaENC;
	tmx::message_container_type container;
	std::unique_ptr<RsaEncodedMessage> msg;

	try
	{
		std::stringstream ss;
		ss << rsaJson;

		container.load<XML>(ss);
		rsamessage.set_contents(container.get_storage().get_tree());

		const std::string rsaString(rsaJson);

		rsaENC.encode_j2735_message(rsamessage);

		msg.reset();
		msg.reset(dynamic_cast<RsaEncodedMessage*>(factory.NewMessage(api::MSGSUBTYPE_ROADSIDEALERT_STRING)));

		string enc = rsaENC.get_encoding();
		msg->refresh_timestamp();
		msg->set_payload(rsaENC.get_payload_str());
		msg->set_encoding(enc);
		msg->set_flags(IvpMsgFlags_RouteDSRC);
		msg->addDsrcMetadata(0x8003);
		msg->refresh_timestamp();

		routeable_message *rMsg = dynamic_cast<routeable_message *>(msg.get());
		BroadcastMessage(*rMsg);

		PLOG(logINFO) << " RSA Plugin :: Broadcast RSA:: " << rsaENC.get_payload_str();	
	}
	catch(const std::exception& e)
	{
		PLOG(logWARNING) << "Error: " << e.what() << " broadcasting RSA for xml: " << rsaJson << std::endl;
	}
	
	

}

/**
 * Write HTTP response. 
 */
void RsaPlugin::writeResponse(int responseCode , QHttpEngine::Socket *socket) {
	socket->setStatusCode(responseCode);
    socket->writeHeaders();
    if(socket->isOpen()){
        socket->close();
    }

}


int RsaPlugin::Main()
{
	PLOG(logINFO) << "Starting plugin.";

	uint msCount = 0;
	while (_plugin->state != IvpPluginState_error)
	{

		msCount += 10;

		if (_plugin->state == IvpPluginState_registered)
		{
			RoadSideAlert rsa_1;
			RoadSideAlert &rsa = rsa_1;

			this_thread::sleep_for(chrono::milliseconds(100));

			msCount = 0;
		}
	}

	PLOG(logINFO) << "Plugin terminating gracefully.";
	return EXIT_SUCCESS;
}

} /* namespace RsaPlugin */

int main(int argc, char *argv[])
{
	return run_plugin<RsaPlugin::RsaPlugin>("RsaPlugin", argc, argv);
}
