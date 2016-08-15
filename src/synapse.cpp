//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/emit.hpp>
#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <vector>

namespace
boost
    {
    namespace
    synapse
        {
        using synapse_detail::connection_list;
        using synapse_detail::weak_store;
        namespace
            {
#ifdef NDEBUG
            bool const debug=false;
#else
            bool const debug=true;
#endif
            template <class T>
            bool
            is_empty( weak_ptr<T> const & a )
                {
                weak_ptr<T> b;
                return !(a<b) && !(b<a);
                }
            class
            conn_rec
                {
                weak_store e_;
                void const * ep_;
                weak_ptr<void const> connection_lifetime_;
                shared_ptr<void const> fn_;
                int next_;
                public:
                conn_rec( weak_store const & e, weak_ptr<void const> const & connection_lifetime, shared_ptr<void const> const & fn ):
                    e_(e),
                    ep_(e_.maybe_lock<void const>().get()),
                    connection_lifetime_(connection_lifetime),
                    fn_(fn),
                    next_(-1)
                    {
                    BOOST_ASSERT(ep_!=0);
                    BOOST_ASSERT(fn);
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
                    BOOST_ASSERT(e_.empty()==fr);
                    BOOST_ASSERT(!fn_==fr);
                    BOOST_ASSERT(connection_lifetime_.expired() || !fr);
                    return fr;
                    }
                bool
                has_expired() const
                    {
                    BOOST_ASSERT(!is_free());
                    return connection_lifetime_.expired();
                    }
                void
                clear( int first_free )
                    {
                    BOOST_ASSERT(first_free==-1 || first_free>=0);
                    BOOST_ASSERT(!is_free());
                    e_.clear();
                    ep_=0;
                    connection_lifetime_.reset();
                    fn_.reset();
                    next_=first_free;
                    BOOST_ASSERT(is_free());
                    }
                int const &
                next() const
                    {
                    BOOST_ASSERT(next_==-1 || next_>=0);
                    return next_;
                    }
                int &
                next()
                    {
                    BOOST_ASSERT(next_==-1 || next_>=0);
                    return next_;
                    }
                bool
                emit( void const * e, synapse_detail::emit_binder_base const & b ) const
                    {
                    BOOST_ASSERT(!is_free());
                    BOOST_ASSERT(e!=0);
                    if( ep_==e && !e_.expired() )
                        {
                        BOOST_ASSERT(ep_==e_.maybe_lock<void const>().get());
                        if( shared_ptr<void const> lk=connection_lifetime_.lock() )
                            {
                            b.call(fn_.get());
                            return true;
                            }
                        }
                    return false;
                    };
                };
            }
        namespace
        synapse_detail
            {
            struct
            connection_list
                {
                private:
                connection_list( connection_list const & );
                connection_list & operator=( connection_list const & );
                void
                check_invariants() const
                    {
                    if( debug )
                        {
                        BOOST_ASSERT(*last_next_==-1);
                        int count1=0;
                        for( int i=first_free_; i!=-1; i=conn_[i].next() )
                            {
                            BOOST_ASSERT(conn_[i].is_free());
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
                            BOOST_ASSERT(!cr.is_free());
                            ++count2;
                            i=&cr.next();
                            }
                        BOOST_ASSERT(found_last_next);
                        int count3=0;
                        for( std::vector<conn_rec>::const_iterator i=conn_.begin(),ie=conn_.end(); i!=ie; ++i )
                            count3 += i->is_free();
                        BOOST_ASSERT(count1==count3);
                        BOOST_ASSERT(count1+count2==conn_.size());
                        }
                    }
                bool
                idx_valid( int idx ) const
                    {
                    return idx>=0 && idx<conn_.size();
                    }
                std::vector<conn_rec> conn_;
                std::vector<conn_rec> * emit_conn_ptr_;
                int first_free_;
                int first_rec_;
                int * last_next_;
                public:
                connection_list():
                    first_free_(-1),
                    first_rec_(-1),
                    last_next_(&first_rec_),
                    emit_conn_ptr_(0)
                    {
                    check_invariants();
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
                        BOOST_ASSERT(idx_valid(first_free_));
                        idx=first_free_;
                        conn_rec & cr=conn_[idx];
                        BOOST_ASSERT(cr.is_free());
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
                    BOOST_ASSERT(first_free_!=idx);
                    BOOST_ASSERT(!conn_[idx].is_free());
                    check_invariants();
                    return idx;
                    }
                void
                remove( int idx )
                    {
                    check_invariants();
                    BOOST_ASSERT(idx_valid(idx));
                    BOOST_ASSERT(first_free_!=idx);
                    if( emit_conn_ptr_ && emit_conn_ptr_->empty() )
                        *emit_conn_ptr_=conn_;
                    conn_rec & cr=conn_[idx];
                    BOOST_ASSERT(!cr.is_free());
                    int * i;
                    for( i=&first_rec_; *i!=-1 && *i!=idx; i=&conn_[*i].next() )
                        { }
                    int const j=(*i=cr.next());
                    if( j==-1 )
                        {
                        BOOST_ASSERT(last_next_==&cr.next());
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
                    return e==0?0:enumerate_recs(boost::bind(&conn_rec::same_emitter,_1,e));
                    }
                int
                emit_from_emitter( void const * e, synapse_detail::emit_binder_base const & b )
                    {
                    BOOST_ASSERT(e!=0);
                    return enumerate_recs(boost::bind(&conn_rec::emit,_1,e,boost::ref(b)));
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
                        BOOST_ASSERT(!cr.is_free());
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
                    BOOST_ASSERT(*i==-1);
                    last_next_=i;
                    BOOST_ASSERT(c<=conn_.size());
                    check_invariants();
                    return c==0;
                    }
                };
            }
        namespace
            {
            shared_ptr<connection_list>
            get_connection_list_( weak_ptr<connection_list> & wcl )
                {
                shared_ptr<connection_list> cl=wcl.lock();
                if( !cl )
                    {
                    shared_ptr<connection_list>(new connection_list).swap(cl);
                    wcl=cl;
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
                any user_data_;
                int (* const emit_meta_connected_)(connection &,unsigned);
                weak_ptr<connection_list> & wcl_;
                shared_ptr<connection_list> const cl_;
                int position_;
                synapse_detail::weak_store const &
                emitter_() const
                    {
                    return cl_->emitter(position_);
                    }
                public:
                connection_impl( weak_ptr<connection_list> & wcl, shared_ptr<connection_list> const & cl, int (*emit_meta_connected)(connection &,unsigned) ):
                    connection(user_data_),
                    emit_meta_connected_(emit_meta_connected),
                    wcl_(wcl),
                    cl_(cl)
                    {
                    BOOST_ASSERT(emit_meta_connected_!=0);
                    }
                void
                connect( conn_rec const & cr )
                    {
                    position_=cl_->add(cr);
                    unsigned flags=meta::connect_flags::connecting;
                    if( cl_->emitter_connection_count(emitter_().maybe_lock<void const>().get())==1 )
                        flags |= meta::connect_flags::first_for_this_emitter;
                    emit_meta_connected_(*this,flags);
                    }
                ~connection_impl()
                    {
                    unsigned flags=0;
                    if( cl_->emitter_connection_count(emitter_().maybe_lock<void const>().get())==1 )
                        flags |= meta::connect_flags::last_for_this_emitter;
                    emit_meta_connected_(*this,flags);
                    cl_->remove(position_);
                    if( cl_.unique() && cl_->purge() )
                        wcl_.reset();
                    }
                };
            int
            emit_impl( weak_ptr<connection_list> const & wcl, weak_ptr<synapse_detail::blocked_list> const & wbl, synapse_detail::emitter_blocked_t * check_blocked, void const * e, synapse_detail::emit_binder_base const & b )
                {
				BOOST_ASSERT(check_blocked!=0);
                BOOST_ASSERT(e!=0);
                if( !check_blocked(wbl,e) )
                    if( shared_ptr<connection_list> cl=wcl.lock() )
                        return cl->emit_from_emitter(e,b);
                return 0;
                }
            }
        namespace
        synapse_detail
            {
            shared_ptr<connection>
            connect_( emit_t * & emit_ptr, weak_ptr<connection_list> & wcl, weak_store const & e, shared_ptr<void const> const & fn, weak_ptr<void const> const & lifetime, int(*emit_meta_connected)(connection &,unsigned) )
                {
                BOOST_ASSERT(fn);
				emit_ptr=&emit_impl;
                shared_ptr<connection_impl> c(new connection_impl(wcl,get_connection_list_(wcl),emit_meta_connected));
                c->connect(conn_rec(e,is_empty(lifetime)?c:lifetime,fn));
                return c;
                }
            shared_ptr<void const> &
            meta_emitter()
                {
                static shared_ptr<void const> meta;
                return meta;
                }
            }
        namespace
        meta
            {
            shared_ptr<void const>
            emitter()
                {
                shared_ptr<void const> & me=synapse_detail::meta_emitter();
                if( !me )
                    me.reset(new int(42));
                return me;
                }
            }
        connection::
        connection( any & user_data ):
            user_data(user_data)
            {
            }
        connection::
        ~connection()
            {
            }
        } 
    }
