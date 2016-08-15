//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/block.hpp>
#include <boost/synapse/blocker.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <algorithm>

namespace
boost
    {
    namespace
    synapse
        {
        using synapse_detail::weak_store;
        using synapse_detail::blocked_list;
        namespace
            {
            struct
            blocker_impl:
                blocker
                {
                private:
                blocker_impl( blocker_impl const & );
                blocker_impl & operator=( blocker_impl const & );
                int (* const emit_meta_blocked_)(blocker &,bool);
                weak_ptr<blocked_list> & wbl_;
                shared_ptr<blocked_list> const bl_;
                synapse_detail::weak_store const & 
                emitter_() const
                    {
                    return e_;
                    }
                public:
                weak_store e_;
                blocker_impl( weak_store e, weak_ptr<blocked_list> & wbl, shared_ptr<blocked_list> const & bl, int (*emit_meta_blocked)(blocker &,bool) ):
                    emit_meta_blocked_(emit_meta_blocked),
                    wbl_(wbl),
                    bl_(bl),
                    e_(e)
                    {
                    BOOST_ASSERT(emit_meta_blocked_!=0);
                    emit_meta_blocked_(*this,true);
                    }
                ~blocker_impl();
                };
            }
        namespace
        synapse_detail
            {
            struct
            blocked_list
                {
                private:
                blocked_list( blocked_list const & );
                blocked_list & operator=( blocked_list const & );
                struct
                bl_rec
                    {
                    void const * ep_;
                    weak_ptr<blocker_impl> eb_;
                    blocker_impl const * ebp_;
                    bl_rec( void const * ep, shared_ptr<blocker_impl> const & eb ):
                        ep_(ep),
                        eb_(eb),
                        ebp_(eb.get())
                        {
                        BOOST_ASSERT(ep_!=0);
                        BOOST_ASSERT(eb);
                        BOOST_ASSERT(ebp_!=0);
                        }
                    bool
                    same_emitter( void const * e ) const
                        {
                        if( e==ep_ && !ebp_->e_.expired() )
                            {
                            BOOST_ASSERT(ebp_->e_.maybe_lock<void const>().get()==ep_);
                            return true;
                            }
                        else
                            return false;
                        }
                    bool
                    same_blocker( blocker * eb ) const
                        {
                        BOOST_ASSERT(eb!=0);
                        return ebp_==eb;
                        }
                    };
                std::vector<bl_rec> bl_;
                public:
                blocked_list()
                    {
                    }
                shared_ptr<blocker>
                block( weak_store const & e, shared_ptr<void const> const & sp, weak_ptr<blocked_list> & wbl, shared_ptr<blocked_list> const & bl, int (*emit_meta_blocked)(blocker &,bool) )
                    {
                    BOOST_ASSERT(bl);
                    BOOST_ASSERT(wbl.lock()==bl);
                    BOOST_ASSERT(emit_meta_blocked!=0);
                    BOOST_ASSERT(sp);
                    BOOST_ASSERT(sp==e.maybe_lock<void const>());
                    std::vector<bl_rec>::const_iterator i=std::find_if(bl_.begin(),bl_.end(),boost::bind(&bl_rec::same_emitter,_1,sp.get()));
                    if( i!=bl_.end() )
                        {
                        shared_ptr<blocker> bb=i->eb_.lock();
                        BOOST_ASSERT(bb);
                        return bb;
                        }
                    else
                        {
                        shared_ptr<blocker_impl> bb(new blocker_impl(e,wbl,bl,emit_meta_blocked));
                        bl_.push_back(bl_rec(sp.get(),bb));
                        return bb;
                        }
                    }
                void
                unblock( blocker * eb )
                    {
                    BOOST_ASSERT(eb!=0);
                    std::vector<bl_rec>::iterator i=std::find_if(bl_.begin(),bl_.end(),boost::bind(&bl_rec::same_blocker,_1,eb));
                    BOOST_ASSERT(i!=bl_.end());
                    bl_.erase(i);
                    }
                bool
                is_blocked( void const * e ) const
                    {
                    return std::find_if(bl_.begin(),bl_.end(),boost::bind(&bl_rec::same_emitter,_1,e))!=bl_.end();
                    }
                };
            }
        namespace
            {
            blocker_impl::
            ~blocker_impl()
                {
                bl_->unblock(this);
                if( bl_.unique() )
                    wbl_.reset();
                emit_meta_blocked_(*this,false);
                }
            shared_ptr<blocked_list>
            get_blocked_list_( weak_ptr<blocked_list> & wbl )
                {
                shared_ptr<blocked_list> bl=wbl.lock();
                if( !bl )
                    {
                    shared_ptr<blocked_list>(new blocked_list).swap(bl);
                    wbl=bl;
                    }
                return bl;
                }
            bool
            emitter_blocked_impl( weak_ptr<blocked_list> const & wbl, void const * e )
                {
                if( boost::shared_ptr<blocked_list> bl=wbl.lock() )
                    return bl->is_blocked(e);
                else
                    return false;
                }
            }
        namespace
        synapse_detail
            {
            shared_ptr<blocker>
            block_( emitter_blocked_t * & blocked_ptr, weak_ptr<blocked_list> & wbl, weak_store const & e, int(*emit_meta_block)(blocker &,bool) )
                {
                if( shared_ptr<void const> sp=e.maybe_lock<void const>() )
                    {
					blocked_ptr=&emitter_blocked_impl;
                    shared_ptr<blocked_list> bl=get_blocked_list_(wbl);
                    return bl->block(e,sp,wbl,bl,emit_meta_block);
                    }
                else
                    return shared_ptr<blocker>();
                }
            }
        blocker::
        blocker()
            {
            }
        blocker::
        ~blocker()
            {
            }
        } 
    }
