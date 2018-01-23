#include "boost/synapse/connect.hpp"
#include "boost/synapse/connection.hpp"
#include <openssl/ssl.h>

namespace synapse=boost::synapse;

typedef struct SSL_info_callback_(*SSL_info_callback)( const SSL *ssl, int type, int val );

namespace {

    void emit_fwd( SSL const * ssl, int type, int val ) {
        synapse::emit<SSL_info_callback>(ssl,ssl,type,val);
    }

}

int main( int argc, char const * argv[ ] ) {

    synapse::connect<synapse::meta::connected<SSL_info_callback> >( synapse::meta::emitter(),
        [ ]( synapse::connection & c, unsigned flags ) noexcept {

            if( flags&synapse::meta::connect_flags::connecting ) {

                if( flags&synapse::meta::connect_flags::first_for_this_emitter )
                    SSL_set_info_callback(c.emitter<SSL>().get(),&emit_fwd);

            } else {

                if( flags&synapse::meta::connect_flags::last_for_this_emitter )
                    if( auto ssl = c.emitter<SSL>() )
                        SSL_set_info_callback(ssl.get(),0);

            }
        } );


}
