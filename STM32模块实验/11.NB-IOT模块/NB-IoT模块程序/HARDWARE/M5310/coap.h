/**
 * Copyright (c) 2017 China Mobile IOT.
 * All rights reserved.
**/

#ifndef ONENET_COAP_H_
#define ONENET_COAP_H_
#include "stm32f1xx.h"
#ifdef __cplusplus
extern "C" {
#endif

#define COAP_NO_ERROR                     0x00
#define COAP_IGNORE                       0x01
#define COAP_CREATED_201                  0x41 /* CREATED */
#define COAP_DELETED_202                  0x42 /* DELETED */
#define COAP_VALID_203                    0x43 /* NOT_MODIFIED */
#define COAP_CHANGED_204                  0x44 /* CHANGED */
#define COAP_CONTENT_205                  0x45 /* OK */
#define COAP_BAD_REQUEST_400              0x80 /* BAD_REQUEST */
#define COAP_UNAUTHORIZED_401             0x81 /* UNAUTHORIZED */
#define COAP_BAD_OPTION_402               0x82 /* BAD_OPTION */
#define COAP_FORBIDDEN_403                0x83 /* FORBIDDEN */
#define COAP_NOT_FOUND_404                0x84 /* NOT_FOUND */
#define COAP_METHOD_NOT_ALLOWED_405       0x85 /* METHOD_NOT_ALLOWED */
#define COAP_NOT_ACCEPTABLE_406           0x86 /* NOT_ACCEPTABLE */
#define COAP_PRECONDITION_FAILED_412      0x8c /* BAD_REQUEST */
#define COAP_REQUEST_ENTITY_TOO_LARGE_413 0x8d /* REQUEST_ENTITY_TOO_LARGE */
#define COAP_UNSUPPORTED_MEDIA_TYPE_415   0x8f /* UNSUPPORTED_MEDIA_TYPE */
#define COAP_INTERNAL_SERVER_ERROR_500    0xa0 /* INTERNAL_SERVER_ERROR */
#define COAP_NOT_IMPLEMENTED_501          0xa1 /* NOT_IMPLEMENTED */
#define COAP_BAD_GATEWAY_502              0xa2 /* BAD_GATEWAY */
#define COAP_SERVICE_UNAVAILABLE_503      0xa3 /* SERVICE_UNAVAILABLE */
#define COAP_GATEWAY_TIMEOUT_504          0xa4 /* GATEWAY_TIMEOUT */
#define COAP_PROXYING_NOT_SUPPORTED_505   0xa5 /* PROXYING_NOT_SUPPORTED */

#define COAP_READ                          1
#define COAP_WRITE                         2
#define COAP_EXECUTE                       3
#define COAP_OBSERVE                       4
#define COAP_ACK                           5
#define COAP_REG_RSP                       6
#define COAP_CLOSE_RSP                     7
#define UPDATE_RSP                         8
#define DISCOVER                           9

#define COAP_RESPONSE_TIMEOUT              30
#define COAP_MAX_RETRANSMIT                3
//#define COAP_ACK_RANDOM_FACTOR            1.5
//#define COAP_MAX_TRANSMIT_WAIT            ((COAP_RESPONSE_TIMEOUT*((1 <<(COAP_MAX_RETRANSMIT+1))-1)*COAP_ACK_RANDOM_FACTOR))	
	
	
#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* ONENET_COAP_H_ */
