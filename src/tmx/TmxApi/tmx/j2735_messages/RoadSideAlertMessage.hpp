/*
 * @file RoadSideAlertMessage.hpp
 *
 *  Created on: May 8, 2016
 *      Author: Gregory M. Baumgardner
 */

#ifndef TMX_J2735_MESSAGES_ROADSIDEALERTMESSAGE_HPP_
#define TMX_J2735_MESSAGES_ROADSIDEALERTMESSAGE_HPP_

#include <RoadSideAlert.h>
#include <tmx/j2735_messages/J2735MessageTemplate.hpp>
#include <tmx/messages/TmxJ2735.hpp>

#if SAEJ2735_SPEC < 63
TMX_J2735_DECLARE(Rsa, RoadSideAlert, api::roadSideAlert_D, api::MSGSUBTYPE_ROADSIDEALERT_STRING)
#else
TMX_J2735_DECLARE(Rsa, RoadSideAlert, api::roadSideAlert, api::MSGSUBTYPE_ROADSIDEALERT_STRING)
#endif

// Specialize the unique key function
TMX_J2735_NAMESPACE_START(tmx)
TMX_J2735_NAMESPACE_START(messages)
TMX_J2735_NAMESPACE_START(j2735)

template <>
inline int get_j2735_message_key<tmx::messages::RsaMessage>(std::shared_ptr<RoadSideAlert> message) {
	return 1;
}

TMX_J2735_NAMESPACE_END(j2735)
TMX_J2735_NAMESPACE_END(messages)
TMX_J2735_NAMESPACE_END(tmx)

#endif /* TMX_J2735_MESSAGES_ROADSIDEALERTMESSAGE_HPP_ */