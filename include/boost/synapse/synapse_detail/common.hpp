//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_A098543F9C6D4BEC8CE41B576AA69BA6
#define UUID_A098543F9C6D4BEC8CE41B576AA69BA6

#include <boost/synapse/signal_traits.hpp>
#include <boost/synapse/dep/thread_local.hpp>
#include <boost/synapse/dep/smart_ptr.hpp>
#include <boost/synapse/dep/assert.hpp>
#include <atomic>

namespace boost { namespace synapse {

    namespace meta
    {
        template <class Signal> struct connected;
        template <class Signal> struct blocked;
    }

    namespace synapse_detail
    {

        class connection_list_list;
        struct thread_local_signal_data;
        class args_binder_base;

        typedef int emit_fn( thread_local_signal_data const &, void const *, args_binder_base const * );
        typedef void cleanup_fn( thread_local_signal_data const & );

        typedef bool emitter_blocked_fn( thread_local_signal_data const &, void const * );
        inline bool emitter_blocked_stub( thread_local_signal_data const &, void const * ) { return false; }

        int emit_stub( thread_local_signal_data const &, void const *, args_binder_base const * );
        void cleanup_stub( thread_local_signal_data const & );

        class interthread_interface
        {
        public:
            virtual void notify_connection_list_created( shared_ptr<thread_local_signal_data> const & )=0;
            virtual int emit( thread_local_signal_data const &, void const *, args_binder_base const * )=0;
        };

        struct thread_local_signal_data
        {
        private:

            thread_local_signal_data( thread_local_signal_data const & );
            thread_local_signal_data & operator=( thread_local_signal_data const & );
            cleanup_fn * cleanup_;
            std::atomic<int> * const cl_count_;

        public:

            emit_fn * emit_;
            emitter_blocked_fn * emitter_blocked_;

            struct connection_list; weak_ptr<connection_list> cl_; friend struct connection_list;
            struct blocked_emitters_list; weak_ptr<blocked_emitters_list> bl_;
            struct posted_signals; shared_ptr<posted_signals> ps_;

            shared_ptr<thread_local_signal_data const> keep_meta_connected_tlsd_afloat_;
            shared_ptr<thread_local_signal_data const> keep_meta_blocked_tlsd_afloat_;

            shared_ptr<connection_list_list> const & (* const get_cll_)( shared_ptr<connection_list_list> (*)() );
            std::atomic<interthread_interface *> * const interthread_;

            thread_local_signal_data():
                cleanup_(&cleanup_stub),
                cl_count_(0),
                emit_(&emit_stub),
                emitter_blocked_(&emitter_blocked_stub),
                get_cll_(0),
                interthread_(0)
            {
            }

            thread_local_signal_data( shared_ptr<connection_list_list> const & (*get_cll)( shared_ptr<connection_list_list> (*)() ), std::atomic<int> & cl_count, std::atomic<interthread_interface *> & interthread ):
                cleanup_(&cleanup_stub),
                cl_count_(&cl_count),
                emit_(&emit_stub),
                emitter_blocked_(&emitter_blocked_stub),
                get_cll_(get_cll),
                interthread_(&interthread)
            {
            }

            ~thread_local_signal_data()
            {
                cleanup_(*this);
            }

            static inline int emit_stub( thread_local_signal_data const & tlsd, void const * e, args_binder_base const * args )
            {
                if( tlsd.cl_count_ && *tlsd.cl_count_ )
                    if( interthread_interface * interthread=tlsd.interthread_->load() )
                        return interthread->emit(tlsd,e,args);
                return 0;
            }

            static inline void cleanup_stub( thread_local_signal_data const & )
            {
            }
        }; //thread_local_signal_data

        template <class Signal>
        shared_ptr<connection_list_list> const & get_connection_list_list( shared_ptr<connection_list_list> (*create_connection_list_list)() )
        {
            BOOST_SYNAPSE_STATIC_INIT(shared_ptr<connection_list_list>,obj,(create_connection_list_list()));
            return obj;
        }

        template <class Signal>
        shared_ptr<thread_local_signal_data> const & get_thread_local_signal_data( bool allocate );

        template <class Signal>
        struct register_with_non_meta
        {
            static void keep_afloat( shared_ptr<thread_local_signal_data const> const & )
            {
            }
        };

        template <class Signal>
        struct register_with_non_meta<meta::connected<Signal> >
        {
            static void keep_afloat( shared_ptr<thread_local_signal_data const> const & meta )
            {
                auto main_tlsd = get_thread_local_signal_data<Signal>(true);
                BOOST_SYNAPSE_ASSERT(!main_tlsd->keep_meta_connected_tlsd_afloat_);
                main_tlsd->keep_meta_connected_tlsd_afloat_ = meta;
            }
        };

        template <class Signal>
        struct register_with_non_meta<meta::blocked<Signal> >
        {
            static void keep_afloat( shared_ptr<thread_local_signal_data const> const & meta )
            {
                auto main_tlsd = get_thread_local_signal_data<Signal>(true);
                BOOST_SYNAPSE_ASSERT(!main_tlsd->keep_meta_blocked_tlsd_afloat_);
                main_tlsd->keep_meta_blocked_tlsd_afloat_ = meta;
            }
        };

        template <class Signal,bool SignalIsThreadLocal=signal_traits<Signal>::is_thread_local>
        struct thread_local_signal_data_;

        template <class Signal>
        struct thread_local_signal_data_<Signal,false>
        {
            static shared_ptr<thread_local_signal_data> const & get( bool allocate )
            {
                BOOST_SYNAPSE_STATIC(std::atomic<int>,count);
                BOOST_SYNAPSE_STATIC(std::atomic<interthread_interface *>,interthread);
                BOOST_SYNAPSE_THREAD_LOCAL(shared_ptr<thread_local_signal_data>,obj);
                if( !obj && (allocate || interthread.load()) )
                {
                    obj=synapse::make_shared<thread_local_signal_data>(&get_connection_list_list<Signal>,count,interthread);
                    register_with_non_meta<Signal>::keep_afloat(obj);
                }
                return obj;
            }
        };

        template <class Signal>
        struct thread_local_signal_data_<Signal,true>
        {
            static shared_ptr<thread_local_signal_data> const & get( bool allocate )
            {
                BOOST_SYNAPSE_THREAD_LOCAL(shared_ptr<thread_local_signal_data>,obj);
                if( !obj && allocate )
                {
                    obj=synapse::make_shared<thread_local_signal_data>();
                    register_with_non_meta<Signal>::keep_afloat(obj);
                }
                return obj;
            }
        };

        template <class Signal>
        shared_ptr<thread_local_signal_data> const & get_thread_local_signal_data( bool allocate )
        {
            return thread_local_signal_data_<Signal>::get(allocate);
        }

    } //namespace synapse_detail

} }

#endif
