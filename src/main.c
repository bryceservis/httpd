#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include "../include/sockets.h"

void *pocip(void *args) {

    tls_conf_t *conf = (tls_conf_t *) malloc(sizeof(tls_conf_t));
    tls_cache_t *cache = (tls_cache_t *) malloc(sizeof(tls_cache_t));
    conf->tls_cache = cache;
    conf->minimum_version = TLS1_2_VERSION;
    conf->maximum_version = TLS1_3_VERSION;
    cache->cache_size = 8192;
    cache->timeout_duration = 3600;
    cache->cache_id = "cache";
    SSL_CTX *context = create_tls_context("../keys/cert.pem", "../keys/key.pem", conf);
    tls_socket_listen(context, (int) args, 4096);
    free(cache);

}

void *pocip2(void *args) {

    socket_listen((int) args, 4096);

}

int main(void) {
    struct sockaddr_in *a = create_sockaddr_in("0.0.0.0", 443);
    struct sockaddr_in6 *b = create_sockaddr_in6("::1", 443);
    struct sockaddr_in *c = create_sockaddr_in("0.0.0.0", 80);
    struct sockaddr_in6 *d = create_sockaddr_in6("::1", 80);
    int a2 = create_socket_ipv4(a);
    int b2 = create_socket_ipv6(b);
    int c2 = create_socket_ipv4(c);
    int d2 = create_socket_ipv6(d);
    pthread_t tid;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    pthread_create(&tid, NULL, pocip, (void *) a2);
    pthread_detach(tid);
    pthread_create(&tid2, NULL, pocip, (void *) b2);
    pthread_detach(tid2);
    pthread_create(&tid3, NULL, pocip2, (void *) c2);
    pthread_detach(tid3);
    pthread_create(&tid4, NULL, pocip2, (void *) d2);
    pthread_detach(tid4);
    getchar();
    return 0;
}