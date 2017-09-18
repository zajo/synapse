//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include <vector>

namespace
    {
#ifdef NDEBUG
    bool const debug=false;
#else
    bool const debug=true;
#endif
    }
namespace
boost
    {
    namespace
    synapse
        {
        namespace
        synapse_detail
            {
            interthread_interface * get_interthread_api();
            namespace
                {
                template <class T>
                bool
                is_empty( weak_ptr<T> const & a )
                    {
                    weak_ptr<T> b;
                    return !a.owner_before(b) && !b.owner_before(a);
                    }
                class
                conn_rec
                    {
                    void const * ep_;
                    weak_store e_;
                    weak_ptr<void const> connection_lifetime_;
                    shared_ptr<void const> fn_;
                    int next_;
                    bool translated_;
                    public:
                    conn_rec( weak_store const & e, weak_ptr<void const> const & connection_lifetime, shared_ptr<void const> const & fn, bool translated ):
                        ep_(e.maybe_lock<void const>().get()),
                        e_(e),
                        connection_lifetime_(connection_lifetime),
                        fn_(fn),
                        next_(-1),
                        translated_(translated)
                        {
                        BOOST_SYNAPSE_ASSERT(ep_!=0);
                        BOOST_SYNAPSE_ASSERT(fn);
                        }
                    weak_store const &
                    emitter() const
                        {
                        return e_;
                        }
                    bool
                    same_emitter( void const * e ) const
                        {
                        return e==ep_;
                        }
                    bool
                    is_free() const
                        {
                        bool fr=(ep_==0);
                        BOOST_SYNAPSE_ASSERT(e_.empty()==fr);
                        BOOST_SYNAPSE_ASSERT(!fn_==fr);
                        BOOST_SYNAPSE_ASSERT(connection_lifetime_.expired() || !fr);
                        return fr;
                        }
                    bool
                    has_expired() const
                        {
                        BOOST_SYNAPSE_ASSERT(!is_free());
                        return connection_lifetime_.expired();
                        }
                    void
                    clear( int first_free )
                        {
                        BOOST_SYNAPSE_ASSERT(first_free==-1 || first_free>=0);
                        BOOST_SYNAPSE_ASSERT(!is_free());
                        e_.clear();
                        ep_=0;
                        connection_lifetime_.reset();
                        fn_.reset();
                        next_=first_free;
                        BOOST_SYNAPSE_ASSERT(is_free());
                        }
                    int const &
                    next() const
                        {
                        BOOST_SYNAPSE_ASSERT(next_==-1 || next_>=0);
                        return next_;
                        }
                    int &
                    next()
                        {
                        BOOST_SYNAPSE_ASSERT(next_==-1 || next_>=0);
                        return next_;
                        }
                    int
                    emit( void const * e, args_binder_base const * args ) const
                        {
                        BOOST_SYNAPSE_ASSERT(!is_free());
                        BOOST_SYNAPSE_ASSERT(e!=0);
                        if( ep_==e && !e_.expired() )
                            {
                            BOOST_SYNAPSE_ASSERT(ep_==e_.maybe_lock<void const>().get());
                            if( shared_ptr<void const> lk=connection_lifetime_.lock() )
                                {
                                if( translated_ )
                                    if( args )
                                        return args->call_translated(fn_.get());
                                    else
                                        return (*static_cast<function<int()> const *>(fn_.get()))();
                                else
                                    if( args )
                                        args->call(fn_.get());
                                    else
                                        (*static_cast<function<void()> const *>(fn_.get()))();
                                return 1;
                                }
                            }
                        return false;
                        };
                    };
                }
            struct
            thread_local_signal_data::
            connection_list
                {
                private:
                connection_list( connection_list const & );
                connection_list & operator=( connection_list const & );
                shared_ptr<thread_local_signal_data> const tlsd_;
                std::vector<conn_rec> conn_;
                std::vector<conn_rec> * emit_conn_ptr_;
                int first_free_;
                int first_rec_;
                int * last_next_;
                void
                check_invariants() const
                    {
                    if( debug )
                        {
                        BOOST_SYNAPSE_ASSERT(*last_next_==-1);
                        int count1=0;
                        for( int i=first_free_; i!=-1; i=conn_[i].next() )
                            {
                            BOOST_SYNAPSE_ASSERT(conn_[i].is_free());
                            ++count1;
                            }
                        int count2=0;
                        bool found_last_next=false;
                        for( int const * i=&first_rec_; ; )
                            {
                            if( last_next_==i )
                                found_last_next=true;
                             if( *i==-1 )
                                 break;
                            conn_rec const & cr=conn_[*i];
                            BOOST_SYNAPSE_ASSERT(!cr.is_free());
                            ++count2;
                            i=&cr.next();
                            }
                        BOOST_SYNAPSE_ASSERT(found_last_next);
                        int count3=0;
                        for( std::vector<conn_rec>::const_iterator i=conn_.begin(),ie=conn_.end(); i!=ie; ++i )
                            count3 += i->is_free();
                        BOOST_SYNAPSE_ASSERT(count1==count3);
                        BOOST_SYNAPSE_ASSERT(count1+count2==conn_.size());
                        }
                    }
                bool
                idx_valid( int idx ) const
                    {
                    return idx>=0 && idx<conn_.size();
                    }
                static
                int
                emit_impl( thread_local_signal_data const & tlsd, void const * e, args_binder_base const * args )
                    {
                    BOOST_SYNAPSE_ASSERT(tlsd.emitter_blocked_!=0);
                    BOOST_SYNAPSE_ASSERT(e!=0);
                    BOOST_SYNAPSE_ASSERT(!tlsd.cl_.expired());
                    int n1=0;
                    if( interthread_interface * interthread=tlsd.interthread_.load() )
                        n1=interthread->emit(tlsd,e,args);
                    if( !tlsd.emitter_blocked_(tlsd,e) )
                        if( shared_ptr<thread_local_signal_data::connection_list> cl=tlsd.cl_.lock() )
                            {
                            int n2=cl->emit_from_emitter(e,args);
                            return n1+n2;
                            }
                        else
                            BOOST_SYNAPSE_ASSERT(0);
                    return n1;
                    }
                public:
                int (* const emit_meta_connected_)( connection &, unsigned );
                connection_list( shared_ptr<thread_local_signal_data> const & tlsd, int (*emit_meta_connected)( connection &, unsigned ) ):
                    tlsd_(tlsd),
                    emit_conn_ptr_(0),
                    first_free_(-1),
                    first_rec_(-1),
                    last_next_(&first_rec_),
                    emit_meta_connected_(emit_meta_connected)
                    {
                    BOOST_SYNAPSE_ASSERT(emit_meta_connected_!=0);
                    tlsd_->emit_=&emit_impl;
                    check_invariants();
                    int const n=++tlsd_->cl_count_;
                    BOOST_SYNAPSE_ASSERT(n>0);
                    }
                ~connection_list()
                    {
                    int const n=--tlsd_->cl_count_;
                    BOOST_SYNAPSE_ASSERT(n>=0);
                    tlsd_->cl_.reset();
                    tlsd_->emit_=&emit_stub;
                    }
                weak_store const &
                emitter( int index ) const
                    {
                    return conn_[index].emitter();
                    }
                int
                add( conn_rec const & r )
                    {
                    if( emit_conn_ptr_ && emit_conn_ptr_->empty() )
                        *emit_conn_ptr_=conn_;
                    (void) purge();
                    int idx;
                    if( first_free_!=-1 )
                        {
                        BOOST_SYNAPSE_ASSERT(idx_valid(first_free_));
                        idx=first_free_;
                        conn_rec & cr=conn_[idx];
                        BOOST_SYNAPSE_ASSERT(cr.is_free());
                        first_free_=cr.next();
                        cr=r;
                        *last_next_=idx;
                        }
                    else
                        {
                        idx=conn_.size();
                        *last_next_=idx;
                        conn_.push_back(r);
                        }
                    last_next_=&conn_[idx].next();
                    BOOST_SYNAPSE_ASSERT(first_free_!=idx);
                    BOOST_SYNAPSE_ASSERT(!conn_[idx].is_free());
                    check_invariants();
                    return idx;
                    }
                void
                remove( int idx )
                    {
                    check_invariants();
                    BOOST_SYNAPSE_ASSERT(idx_valid(idx));
                    BOOST_SYNAPSE_ASSERT(first_free_!=idx);
                    if( emit_conn_ptr_ && emit_conn_ptr_->empty() )
                        *emit_conn_ptr_=conn_;
                    conn_rec & cr=conn_[idx];
                    BOOST_SYNAPSE_ASSERT(!cr.is_free());
                    int * i;
                    for( i=&first_rec_; *i!=-1 && *i!=idx; i=&conn_[*i].next() )
                        { }
                    int const j=(*i=cr.next());
                    if( j==-1 )
                        {
                        BOOST_SYNAPSE_ASSERT(last_next_==&cr.next());
                        last_next_=i;
                        }
                    cr.clear(first_free_);
                    first_free_=idx;
                    check_invariants();
                    }
                template <class F>
                int
                enumerate_recs( F f )
                    {
                    check_invariants();
                    if( conn_.empty() )
                        return 0;
                    struct
                    restore_emit_conn_ptr
                        {
                        std::vector<conn_rec> * & emit_conn_ptr_;
                        std::vector<conn_rec> * const ptr_;
                        explicit restore_emit_conn_ptr( std::vector<conn_rec> * & emit_conn_ptr ): emit_conn_ptr_(emit_conn_ptr), ptr_(emit_conn_ptr_) { }
                        ~restore_emit_conn_ptr() { emit_conn_ptr_=ptr_; }
                        } restore(emit_conn_ptr_);
                    std::vector<conn_rec> emit_conn;
                    emit_conn_ptr_=&emit_conn;
                    int counter=0;
                    for( int i=first_rec_; i!=-1; )
                        {
                        conn_rec & cr = (emit_conn.empty()?conn_:emit_conn)[i];
                        i=cr.next();
                        counter += f(cr);
                        }
                    return counter;
                    }
                int
                emitter_connection_count( void const * e )
                    {
                    return e==0?0:enumerate_recs( [e](conn_rec const & r) { return r.same_emitter(e); } );
                    }
                int
                emit_from_emitter( void const * e, args_binder_base const * args )
                    {
                    BOOST_SYNAPSE_ASSERT(e!=0);
                    return enumerate_recs( [e,args](conn_rec const & r) { return r.emit(e,args); } );
                    }
                bool
                purge()
                    {
                    check_invariants();
                    int c=0;
                    int * i;
                    for( i=&first_rec_; *i!=-1; )
                        {
                        conn_rec & cr=conn_[*i];
                        BOOST_SYNAPSE_ASSERT(!cr.is_free());
                        if( cr.has_expired() )
                            {
                            int ff=first_free_;
                            first_free_=*i;
                            *i=cr.next();
                            cr.clear(ff);
                            }
                        else
                            {
                            ++c;
                            i=&cr.next();
                            }
                        }
                    BOOST_SYNAPSE_ASSERT(*i==-1);
                    last_next_=i;
                    BOOST_SYNAPSE_ASSERT(c<=conn_.size());
                    check_invariants();
                    return c==0;
                    }
                };
            int
            emit_from_emitter( thread_local_signal_data::connection_list & cl, void const * e, args_binder_base const * args )
                {
                return cl.emit_from_emitter(e,args);
                }
            namespace
                {
                shared_ptr<thread_local_signal_data::connection_list>
                get_connection_list_( shared_ptr<thread_local_signal_data> const & tlsd, int (*emit_meta_connected)(connection &,unsigned) )
                    {
                    shared_ptr<thread_local_signal_data::connection_list> cl=tlsd->cl_.lock();
                    if( !cl )
                        {
                        make_shared<thread_local_signal_data::connection_list>(tlsd,emit_meta_connected).swap(cl);
                        tlsd->cl_=cl;
                        if( interthread_interface * interthread=get_interthread_api() )
                            {
                            tlsd->interthread_.store(interthread);
                            interthread->notify_connection_list_created(tlsd);
                            }
                        }
                    return cl;
                    }
                struct
                connection_impl:
                    connection
                    {
                    private:
                    connection_impl( connection_impl const & );
                    connection_impl & operator=( connection_impl const & );
                    shared_ptr<thread_local_signal_data::connection_list> const cl_;
                    int position_;
                    weak_store const &
                    emitter_() const
                        {
                        return cl_->emitter(position_);
                        }
                    public:
                    explicit
                    connection_impl( shared_ptr<thread_local_signal_data::connection_list> const & cl ):
                        cl_(cl)
                        {
                        }
                    void
                    connect( conn_rec const & cr )
                        {
                        BOOST_SYNAPSE_ASSERT(cl_->emit_meta_connected_!=0);
                        position_=cl_->add(cr);
                        unsigned flags=meta::connect_flags::connecting;
                        if( cl_->emitter_connection_count(emitter_().maybe_lock<void const>().get())==1 )
                            flags |= meta::connect_flags::first_for_this_emitter;
                        cl_->emit_meta_connected_(*this,flags);
                        }
                    ~connection_impl()
                        {
                        BOOST_SYNAPSE_ASSERT(cl_->emit_meta_connected_!=0);
                        unsigned flags=0;
                        if( cl_->emitter_connection_count(emitter_().maybe_lock<void const>().get())==1 )
                            flags |= meta::connect_flags::last_for_this_emitter;
                        try
                            {
                            cl_->emit_meta_connected_(*this,flags);
                            }
                        catch(...)
                            {
                            }
                        cl_->remove(position_);
                        }
                    };
                shared_ptr<connection>
                connect_impl( shared_ptr<thread_local_signal_data> const & tlsd, weak_store const & e, shared_ptr<void const> const & fn, weak_ptr<void const> const & lifetime, int(*emit_meta_connected)(connection &,unsigned), bool translated )
                    {
                    BOOST_SYNAPSE_ASSERT(fn);
                    shared_ptr<connection_impl> c=make_shared<connection_impl>(get_connection_list_(tlsd,emit_meta_connected));
                    c->connect(conn_rec(e,is_empty(lifetime)?c:lifetime,fn,translated));
                    return c;
                    }
                }
            shared_ptr<connection>
            connect_( shared_ptr<thread_local_signal_data> const & tlsd, weak_store const & e, shared_ptr<void const> const & fn, weak_ptr<void const> const & lifetime, int(*emit_meta_connected)(connection &,unsigned) )
                {
                return connect_impl(tlsd,e,fn,lifetime,emit_meta_connected,false);
                }
            shared_ptr<connection>
            connect_translated_( shared_ptr<thread_local_signal_data> const & tlsd, weak_store const & e, shared_ptr<void const> const & fn, weak_ptr<void const> const & lifetime, int(*emit_meta_connected)(connection &,unsigned) )
                {
                return connect_impl(tlsd,e,fn,lifetime,emit_meta_connected,true);
                }
            shared_ptr<void const> &
            meta_emitter()
                {
                BOOST_SYNAPSE_STATIC_INIT(shared_ptr<void const>,me,(make_shared<int>(42)));
                return me;
                }
            }
        namespace
        meta
            {
            shared_ptr<void const>
            emitter()
                {
                return synapse_detail::meta_emitter();
                }
            }
        connection::
        ~connection()
            {
            }
        } 
    }
