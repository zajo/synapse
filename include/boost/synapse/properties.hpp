#ifndef UUID_A4F73A40022511E8AF2945EB4EA1BEAD
#define UUID_A4F73A40022511E8AF2945EB4EA1BEAD

#include <boost/synapse/connect.hpp>

namespace boost { namespace synapse {

    namespace synapse_detail
    {
        template <class Property>
        struct access_property
        {
            typedef access_property<Property>(*type)( typename Property::type *, bool update_value );
        };

        template <class Property,class ObjectRef,class Object>
        shared_ptr<connection> set_( ObjectRef const & o, Object * op, typename Property::type x )
        {
            BOOST_SYNAPSE_ASSERT(op!=0);
            if( int n=emit<access_property<Property> >(op,&x,true) )
            {
                BOOST_SYNAPSE_ASSERT(n==1);
                return shared_ptr<connection>();
            }
            else
                return connect<access_property<Property> >(o,
                    [x]( typename Property::type * y, bool update_value ) mutable
                        {
                        if( update_value )
                            x=*y;
                        else
                            *y=x;
                        });
        }

        template <class Property,class ObjectRef,class Object>
        weak_ptr<pconnection> pset_( ObjectRef const & o, Object * op, typename Property::type x )
        {
            BOOST_SYNAPSE_ASSERT(op!=0);
            if( int n=emit<access_property<Property> >(op,&x,true) )
            {
                BOOST_SYNAPSE_ASSERT(n==1);
                return weak_ptr<pconnection>();
            }
            else
                return connect<access_property<Property> >(o,
                    [x]( typename Property::type * y, bool update_value ) mutable
                        {
                        if( update_value )
                            x=*y;
                        else
                            *y=x;
                        });
        }
    } //namespace synapse_detail

    template <class Signal>
    struct signal_traits<synapse_detail::access_property<Signal> >:
        signal_traits<typename synapse_detail::access_property<Signal>::type>
    {
        static bool const is_thread_local=true;
    };

    template <class Tag,class T>
    struct property
    {
        typedef T type;
    };

    template <class Property,class Object>
    shared_ptr<connection> set( Object * o, typename Property::type const & x )
    {
        return synapse_detail::set_<Property>(o,o,x);
    }

    template <class Property,class Object>
    weak_ptr<pconnection> set( shared_ptr<Object> const & o, typename Property::type x )
    {
        return synapse_detail::pset_<Property>(o,o.get(),x);
    }

    template <class Property,class Object>
    weak_ptr<pconnection> set( weak_ptr<Object> const & o, typename Property::type x )
    {
        if( shared_ptr<Object> so=o.lock() )
            return set<Property>(o,x);
        else
            return weak_ptr<pconnection>();
    }

    template <class Property,class Object>
    typename Property::type get( Object * o, typename Property::type p=typename Property::type() )
    {
        int n=emit<synapse_detail::access_property<Property> >(o,&p,false);
        BOOST_SYNAPSE_ASSERT(n>=0);
        BOOST_SYNAPSE_ASSERT(n<=1);
        return p;
    }

} }

#endif
