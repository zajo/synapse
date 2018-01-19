#include <boost/synapse/connect.hpp>

namespace
boost
	{
	namespace
	synapse
		{
		namespace
		synapse_detail
			{
			template <class Property>
			struct
			access_property
				{
				typedef access_property<Property>(*type)( typename Property::type * * );
				};
            template <class Signal>
            struct
            signal_traits<access_property<Signal> >:
                signal_traits<typename access_property<Signal>::type>
                {
                };
			}
		template <class Tag,class T>
		struct
		property
			{
			typedef T type;
			};
		template <class Property,class Object>
		shared_ptr<connection>
		set( Object * o, typename Property::type x )
			{
			return connect<synapse_detail::access_property<Property> >(o,[x]( typename Property::type * * out ) mutable { *out=&x; });
			}
		template <class Property,class Object>
		shared_ptr<connection>
		set( shared_ptr<Object> const & o, typename Property::type x )
			{
			return connect<synapse_detail::access_property<Property> >(o,[x]( typename Property::type * * out ) mutable { *out=&x; });
			}
		template <class Property,class Object>
		shared_ptr<connection>
		set( weak_ptr<Object> const & o, typename Property::type x )
			{
			return connect<synapse_detail::access_property<Property> >(o,[x]( typename Property::type * * out ) mutable { *out=&x; });
			}
		template <class Property,class Object>
		weak_ptr<pconnection>
		pset( shared_ptr<Object> const & o, typename Property::type x )
			{
			return pconnect<synapse_detail::access_property<Property> >(o,[x]( typename Property::type * * out ) mutable { *out=&x; });
			}
		template <class Property,class Object>
		weak_ptr<pconnection>
		pset( weak_ptr<Object> const & o, typename Property::type x )
			{
			return pconnect<synapse_detail::access_property<Property> >(o,[x]( typename Property::type * * out ) mutable { *out=&x; });
			}
		template <class Property,class Object>
		typename Property::type *
		get( Object * o )
			{
			typename Property::type * p;
			int n=emit<synapse_detail::access_property<Property> >(o,&p);
			BOOST_SYNAPSE_ASSERT(n>=0);
			BOOST_SYNAPSE_ASSERT(n<=1);
			return n?p:0;
			}
		}
	}
