//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This header defines synapse signaals for use with GLFW.

#ifndef UUID_DF9B5300F10111E79DCEF6AC7105BEE2
#define UUID_DF9B5300F10111E79DCEF6AC7105BEE2

#include <openssl/ssl.h>

namespace glfw_signals
{
    template <class T>
    result_combiner
        {
        T value;

        };

int res;
emit<Sig>([&res]( int x ) { res=x; },e,a1,a2);

    typedef signal<struct SSL_CTX_msg_callback_,int()> SSL_CTX_msg_callback;

    typedef SSL_CTX_msg_callback_(*SSL_CTX_msg_callback)(int write_p, int version, int content_type, const void *buf, size_t len, SSL *ssl, void *arg);
    typedef SSL_msg_callback_(*SSL_msg_callback)(int write_p, int version, int content_type, const void *buf, size_t len, SSL *ssl, void *arg);

void SSL_CTX_sess_set_new_cb(SSL_CTX *ctx,
                             int (*new_session_cb) (struct ssl_st *ssl,
                                                    SSL_SESSION *sess));
void SSL_CTX_sess_set_remove_cb(SSL_CTX *ctx,
                                void (*remove_session_cb) (struct ssl_ctx_st
                                                           *ctx,
                                                           SSL_SESSION
                                                           *sess));
void SSL_CTX_sess_set_get_cb(SSL_CTX *ctx,
                             SSL_SESSION *(*get_session_cb) (struct ssl_st
                                                             *ssl,
                                                             unsigned char
                                                             *data, int len,
                                                             int *copy));
void SSL_CTX_set_info_callback(SSL_CTX *ctx,
                               void (*cb) (const SSL *ssl, int type,
                                           int val));
void SSL_CTX_set_client_cert_cb(SSL_CTX *ctx,
                                int (*client_cert_cb) (SSL *ssl, X509 **x509,
                                                       EVP_PKEY **pkey));
void SSL_CTX_set_cookie_generate_cb(SSL_CTX *ctx,
                                    int (*app_gen_cookie_cb) (SSL *ssl,
                                                              unsigned char
                                                              *cookie,
                                                              unsigned int
                                                              *cookie_len));
void SSL_CTX_set_cookie_verify_cb(SSL_CTX *ctx,
                                  int (*app_verify_cookie_cb) (SSL *ssl,
                                                               unsigned char
                                                               *cookie,
                                                               unsigned int
                                                               cookie_len));
void SSL_CTX_set_next_protos_advertised_cb(SSL_CTX *s,
                                           int (*cb) (SSL *ssl,
                                                      const unsigned char
                                                      **out,
                                                      unsigned int *outlen,
                                                      void *arg), void *arg);
void SSL_CTX_set_next_proto_select_cb(SSL_CTX *s,
                                      int (*cb) (SSL *ssl,
                                                 unsigned char **out,
                                                 unsigned char *outlen,
                                                 const unsigned char *in,
                                                 unsigned int inlen,
                                                 void *arg), void *arg);
void SSL_CTX_set_alpn_select_cb(SSL_CTX *ctx,
                                int (*cb) (SSL *ssl,
                                           const unsigned char **out,
                                           unsigned char *outlen,
                                           const unsigned char *in,
                                           unsigned int inlen,
                                           void *arg), void *arg);
void SSL_CTX_set_psk_client_callback(SSL_CTX *ctx,
                                     unsigned int (*psk_client_callback) (SSL
                                                                          *ssl,
                                                                          const
                                                                          char
                                                                          *hint,
                                                                          char
                                                                          *identity,
                                                                          unsigned
                                                                          int
                                                                          max_identity_len,
                                                                          unsigned
                                                                          char
                                                                          *psk,
                                                                          unsigned
                                                                          int
                                                                          max_psk_len));
void SSL_set_psk_client_callback(SSL *ssl,
                                 unsigned int (*psk_client_callback) (SSL
                                                                      *ssl,
                                                                      const
                                                                      char
                                                                      *hint,
                                                                      char
                                                                      *identity,
                                                                      unsigned
                                                                      int
                                                                      max_identity_len,
                                                                      unsigned
                                                                      char
                                                                      *psk,
                                                                      unsigned
                                                                      int
                                                                      max_psk_len));
void SSL_CTX_set_psk_server_callback(SSL_CTX *ctx,
                                     unsigned int (*psk_server_callback) (SSL
                                                                          *ssl,
                                                                          const
                                                                          char
                                                                          *identity,
                                                                          unsigned
                                                                          char
                                                                          *psk,
                                                                          unsigned
                                                                          int
                                                                          max_psk_len));
void SSL_set_psk_server_callback(SSL *ssl,
                                 unsigned int (*psk_server_callback) (SSL
                                                                      *ssl,
                                                                      const
                                                                      char
                                                                      *identity,
                                                                      unsigned
                                                                      char
                                                                      *psk,
                                                                      unsigned
                                                                      int
                                                                      max_psk_len));
void SSL_set_verify(SSL *s, int mode,
                    int (*callback) (int ok, X509_STORE_CTX *ctx));
void SSL_set_cert_cb(SSL *s, int (*cb) (SSL *ssl, void *arg), void *arg);
void SSL_CTX_set_verify(SSL_CTX *ctx, int mode,
                        int (*callback) (int, X509_STORE_CTX *));
void SSL_CTX_set_cert_verify_callback(SSL_CTX *ctx,
                                      int (*cb) (X509_STORE_CTX *, void *),
                                      void *arg);
void SSL_CTX_set_cert_cb(SSL_CTX *c, int (*cb) (SSL *ssl, void *arg),
                         void *arg);
int SSL_CTX_set_srp_client_pwd_callback(SSL_CTX *ctx,
                                        char *(*cb) (SSL *, void *));
int SSL_CTX_set_srp_verify_param_callback(SSL_CTX *ctx,
                                          int (*cb) (SSL *, void *));
int SSL_CTX_set_srp_username_callback(SSL_CTX *ctx,
                                      int (*cb) (SSL *, int *, void *));
long SSL_callback_ctrl(SSL *, int, void (*)(void));
long SSL_CTX_callback_ctrl(SSL_CTX *, int, void (*)(void));
void SSL_set_info_callback(SSL *ssl,
                           void (*cb) (const SSL *ssl, int type, int val));
void SSL_CTX_set_tmp_rsa_callback(SSL_CTX *ctx,
                                  RSA *(*cb) (SSL *ssl, int is_export,
                                              int keylength));

void SSL_set_tmp_rsa_callback(SSL *ssl,
                              RSA *(*cb) (SSL *ssl, int is_export,
                                          int keylength));
void SSL_CTX_set_tmp_dh_callback(SSL_CTX *ctx,
                                 DH *(*dh) (SSL *ssl, int is_export,
                                            int keylength));
void SSL_set_tmp_dh_callback(SSL *ssl,
                             DH *(*dh) (SSL *ssl, int is_export,
                                        int keylength));
void SSL_CTX_set_tmp_ecdh_callback(SSL_CTX *ctx,
                                   EC_KEY *(*ecdh) (SSL *ssl, int is_export,
                                                    int keylength));
void SSL_set_tmp_ecdh_callback(SSL *ssl,
                               EC_KEY *(*ecdh) (SSL *ssl, int is_export,
                                                int keylength));


    //This is emitted from the GLFWwindow object to report exceptions from connected signal handlers
    typedef struct exception_caught_(*exception_caught)( GLFWwindow * );
}

#endif
