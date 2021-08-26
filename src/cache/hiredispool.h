/* Author:   Huanghao
 * Date:     2017-2
 * Revision: 0.1
 * Function: Simple lightweight connection pool for hiredis
 *     + Connection pooling
 *     + Auto-reconnect and retry
 *     + Multiple server endpoints
 * Usage:
 */

#ifndef HIREDISPOOL_H
#define HIREDISPOOL_H

#include <stdarg.h>

typedef struct redis_endpoint_t {
    char host[256];
    int port;
} redis_endpoint_t;

typedef struct redis_config_t {
    redis_endpoint_t* endpoints;
    int num_endpoints;
    int connect_timeout;
    int net_readwrite_timeout;
    int num_redis_socks;
    int connect_failure_retry_delay;
} redis_config_t;

typedef struct redis_socket_t {
    int id;
    int backup;
    pthread_mutex_t mutex;
    int inuse;
    struct redis_socket_t* next;
    enum { sockunconnected, sockconnected } state;
    void* conn;
} redis_socket_t;

typedef struct redis_instance_t {
    time_t connect_after;
    redis_socket_t* redis_pool;
    redis_socket_t* last_used;
    redis_config_t* config;
} redis_instance_t;

/* Functions */
int redis_pool_create(const redis_config_t* config, redis_instance_t** instance);
int redis_pool_destroy(redis_instance_t* instance);

redis_socket_t* redis_get_socket(redis_instance_t* instance);
int redis_release_socket(redis_instance_t* instance, redis_socket_t* redisocket);

void* redis_command(redis_socket_t* redisocket, redis_instance_t* instance, const char* format, ...);
void* redis_vcommand(redis_socket_t* redisocket, redis_instance_t* instance, const char* format, va_list ap);

#endif/*HIREDISPOOL_H*/
