#ifndef LIBWEBSOCKETS_SERVER_EVENTLOOP_HHH
#define LIBWEBSOCKETS_SERVER_EVENTLOOP_HHH

#include "libwebsockets_server_module_defines.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum lws_protocol_group
{
    PROTOCOL_GROUP_BASED_ON_WEBSOCKETS,
    PROTOCOL_GROUP_BASED_ON_RAW_SOCKET,
}lws_protocol_group_t;

typedef struct lws_server_evloop_info
{
    callback_base *p_callback;
    lws_server_wt_t working_thread;
    lws_protocol_group_t protocol_group;
	const char *p_iface;            // NULL to bind the listen socket to all interfaces
    uint16_t port;
    uint16_t hint_max_sending_size;
    //uint16_t dead_timeout;          // If this is 0, 300 seconds is default.
    //uint8_t ssl_flags;
}lws_server_evloop_info_t;

typedef struct lws_server_evloop lws_server_evloop_t;

lws_server_evloop_t *create_server_evloop(lws_server_evloop_info_t *p_evloop_info);
void destroy_server_evloop(lws_server_evloop_t *p_evloop);
// The unit of 'blocking_timeout' value is millisecond.
void launch_server_evloop(lws_server_evloop_t *p_evloop, int32_t blocking_timeout);

// This function makes sense only if it is called in a thread which does not run the 
// event loop. Otherwise, this function does not make sense.
// It can be called in working thread.
void stop_server_evloop(lws_server_evloop_t *p_evloop, bool is_by_force);

#endif
