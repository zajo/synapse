//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/block.hpp>
#include <boost/synapse/blocker.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>
#include <vector>
#include <algorithm>

namespace boost { namespace synapse {

    namespace synapse_detail
    {
        namespace
        {
            struct blocker_impl:
                blocker
            {
            private:
                blocker_impl( blocker_impl const & );
                blocker_impl & operator=( blocker_impl const & );
                shared_ptr<thread_local_signal_data::blocked_emitters_list> const bl_;

                weak_store const &  emitter_() const
                {
                    return e_;
                }

            public:

                weak_store e_;
                blocker_impl( weak_store &&, shared_ptr<thread_local_signal_data::blocked_emitters_list> const & );
                ~blocker_impl();
            };
        } //namespace

        struct thread_local_signal_data::blocked_emitters_list
        {
        private:
            blocked_emitters_list( blocked_emitters_list const & );
            blocked_emitters_list & operator=( blocked_emitters_list const & );

            struct bl_rec
            {
                void const * ep_;
                weak_ptr<blocker_impl> eb_;
                blocker_impl const * ebp_;

                bl_rec( void const * ep, shared_ptr<blocker_impl> const & eb ):
                    ep_(ep),
                    eb_(eb),
                    ebp_(eb.get())
                {
                    BOOST_SYNAPSE_ASSERT(ep_!=0);
                    BOOST_SYNAPSE_ASSERT(eb);
                    BOOST_SYNAPSE_ASSERT(ebp_!=0);
                }

                bool same_emitter( void const * e ) const
                {
                    if( e==ep_ && !ebp_->e_.expired() )
                    {
                        BOOST_SYNAPSE_ASSERT(ebp_->e_.maybe_lock<void const>().get()==ep_);
                        return true;
                    }
                    else
                        return false;
                }

                bool same_blocker( blocker * eb ) const
                {
                    BOOST_SYNAPSE_ASSERT(eb!=0);
                    return ebp_==eb;
                }
            };

            static bool emitter_blocked_impl( thread_local_signal_data const & tlsd, void const * e )
            {
                if( shared_ptr<blocked_emitters_list> bl=tlsd.bl_.lock() )
                    return bl->is_blocked(e);
                else
                {
                    BOOST_SYNAPSE_ASSERT(0);
                    return false;
                }
            }

            shared_ptr<thread_local_signal_data> const tlsd_;
            std::vector<bl_rec> bl_;

        public:

            int (* const emit_meta_blocked_)(blocker &,bool);

            blocked_emitters_list( shared_ptr<thread_local_signal_data> const & tlsd, int (*emit_meta_blocked)(blocker &,bool) ):
                tlsd_(tlsd),
                emit_meta_blocked_(emit_meta_blocked)
            {
                tlsd_->emitter_blocked_=&emitter_blocked_impl;
                BOOST_SYNAPSE_ASSERT(emit_meta_blocked_!=0);
            }

            ~blocked_emitters_list()
            {
                tlsd_->bl_.reset();
                tlsd_->emitter_blocked_=&emitter_blocked_stub;
            }

            shared_ptr<blocker> block( shared_ptr<blocked_emitters_list> const & bl, weak_store && e, shared_ptr<void const> const & sp )
            {
                BOOST_SYNAPSE_ASSERT(bl);
                BOOST_SYNAPSE_ASSERT(sp);
                BOOST_SYNAPSE_ASSERT(sp==e.maybe_lock<void const>());
                std::vector<bl_rec>::const_iterator i=std::find_if(bl_.begin(),bl_.end(),[&sp](bl_rec const & r) { return r.same_emitter(sp.get()); });
                if( i!=bl_.end() )
                {
                    shared_ptr<blocker> bb=i->eb_.lock();
                    BOOST_SYNAPSE_ASSERT(bb);
                    return bb;
                }
                else
                {
                    shared_ptr<blocker_impl> bb=make_shared<blocker_impl>(std::move(e),bl);
                    bl_.push_back(bl_rec(sp.get(),bb));
                    return bb;
                }
            }

            void unblock( blocker * eb )
            {
                BOOST_SYNAPSE_ASSERT(eb!=0);
                std::vector<bl_rec>::iterator i=std::find_if(bl_.begin(),bl_.end(),[eb](bl_rec const & r) { return r.same_blocker(eb); });
                BOOST_SYNAPSE_ASSERT(i!=bl_.end());
                bl_.erase(i);
            }

            bool is_blocked( void const * e ) const
            {
                return std::find_if(bl_.begin(),bl_.end(),[e](bl_rec const & r) { return r.same_emitter(e); })!=bl_.end();
            }
        };

        namespace
        {
            blocker_impl::blocker_impl( weak_store && e, shared_ptr<thread_local_signal_data::blocked_emitters_list> const & bl ):
                bl_(bl),
                e_(std::move(e))
            {
                BOOST_SYNAPSE_ASSERT(bl_);
                BOOST_SYNAPSE_ASSERT(bl_->emit_meta_blocked_!=0);
                bl_->emit_meta_blocked_(*this,true);
            }

            blocker_impl::~blocker_impl()
            {
                BOOST_SYNAPSE_ASSERT(bl_->emit_meta_blocked_!=0);
                bl_->unblock(this);
                bl_->emit_meta_blocked_(*this,false);
            }

            shared_ptr<thread_local_signal_data::blocked_emitters_list> get_blocked_list_( shared_ptr<thread_local_signal_data> const & tlsd, int (*emit_meta_blocked)(blocker &,bool) )
            {
                shared_ptr<thread_local_signal_data::blocked_emitters_list> bl=tlsd->bl_.lock();
                if( !bl )
                {
                    make_shared<thread_local_signal_data::blocked_emitters_list>(tlsd,emit_meta_blocked).swap(bl);
                    tlsd->bl_=bl;
                }
                return bl;
            }
        } //namespace

        shared_ptr<blocker> block_( shared_ptr<thread_local_signal_data> const & tlsd, weak_store && e, int(*emit_meta_blocked)(blocker &,bool) )
        {
            if( shared_ptr<void const> sp=e.maybe_lock<void const>() )
            {
                shared_ptr<thread_local_signal_data::blocked_emitters_list> bl=get_blocked_list_(tlsd,emit_meta_blocked);
                return bl->block(bl,std::move(e),sp);
            }
            else
                return shared_ptr<blocker>();
        }

    } //namespace synapse_detail

    blocker::blocker()
    {
    }

    blocker::~blocker()
    {
    }

} }
