# ACE_Thread_Manager - youfangyuan - CSDN博客
2012年04月22日 17:05:47[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1640
```cpp
```
ACE_INLINE
ACE_At_Thread_Exit::ACE_At_Thread_Exit (void)
  : next_ (0),
    td_ (0),
    was_applied_ (false),
    is_owner_ (true)
{
}
ACE_INLINE bool
ACE_At_Thread_Exit::was_applied() const
{
   return was_applied_;
}
ACE_INLINE bool
ACE_At_Thread_Exit::was_applied (bool applied)
{
  was_applied_ = applied;
  if (was_applied_)
    td_ = 0;
  return was_applied_;
}
ACE_INLINE bool
ACE_At_Thread_Exit::is_owner() const
{
  return is_owner_;
}
ACE_INLINE bool
ACE_At_Thread_Exit::is_owner (bool owner)
{
  is_owner_ = owner;
  return is_owner_;
}
ACE_INLINE void
ACE_At_Thread_Exit::do_apply (void)
{
  if (!this->was_applied_ && this->is_owner_)
    td_->at_pop();
}
ACE_INLINE
ACE_At_Thread_Exit_Func::ACE_At_Thread_Exit_Func (void *object,
                                                  ACE_CLEANUP_FUNC func,
                                                  void *param)
  : object_(object),
    func_(func),
    param_(param)
{
}
ACE_INLINE
ACE_Thread_Descriptor_Base::ACE_Thread_Descriptor_Base (void)
  : ACE_OS_Thread_Descriptor (),
    thr_id_ (ACE_OS::NULL_thread),
    thr_handle_ (ACE_OS::NULL_hthread),
    grp_id_ (0),
    thr_state_ (ACE_Thread_Manager::ACE_THR_IDLE),
    task_ (0),
    next_ (0),
    prev_ (0)
{
}
ACE_INLINE
ACE_Thread_Descriptor_Base::~ACE_Thread_Descriptor_Base (void)
{
}
ACE_INLINE bool
ACE_Thread_Descriptor_Base::operator== (
  const ACE_Thread_Descriptor_Base &rhs) const
{
  return
    ACE_OS::thr_cmp (this->thr_handle_, rhs.thr_handle_)
    && ACE_OS::thr_equal (this->thr_id_, rhs.thr_id_);
}
ACE_INLINE bool
ACE_Thread_Descriptor_Base::operator!=(const ACE_Thread_Descriptor_Base &rhs) const
{
  return !(*this == rhs);
}
ACE_INLINE ACE_Task_Base *
ACE_Thread_Descriptor_Base::task (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::task");
  return this->task_;
}
// Group ID.
ACE_INLINE int
ACE_Thread_Descriptor_Base::grp_id (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::grp_id");
  return grp_id_;
}
// Current state of the thread.
ACE_INLINE ACE_UINT32
ACE_Thread_Descriptor_Base::state (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::state");
  return thr_state_;
}
// Reset this base descriptor.
ACE_INLINE void
ACE_Thread_Descriptor_Base::reset (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::reset");
  this->thr_id_ = ACE_OS::NULL_thread;
  this->thr_handle_ = ACE_OS::NULL_hthread;
  this->grp_id_ = 0;
  this->thr_state_ = ACE_Thread_Manager::ACE_THR_IDLE;
  this->task_ = 0;
  this->flags_ = 0;
}
// Unique thread id.
ACE_INLINE ACE_thread_t
ACE_Thread_Descriptor::self (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  return this->thr_id_;
}
// Unique kernel-level thread handle.
ACE_INLINE void
ACE_Thread_Descriptor::self (ACE_hthread_t &handle)
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  handle = this->thr_handle_;
}
ACE_INLINE void
ACE_Thread_Descriptor::log_msg_cleanup (ACE_Log_Msg* log_msg)
{
  log_msg_ = log_msg;
}
// Set the <next_> pointer
ACE_INLINE void
ACE_Thread_Descriptor::set_next (ACE_Thread_Descriptor *td)
{
  ACE_TRACE ("ACE_Thread_Descriptor::set_next");
  this->next_ = td;
}
// Get the <next_> pointer
ACE_INLINE ACE_Thread_Descriptor *
ACE_Thread_Descriptor::get_next (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor::get_next");
  return static_cast<ACE_Thread_Descriptor * ACE_CAST_CONST> (this->next_);
}
// Reset this thread descriptor
ACE_INLINE void
ACE_Thread_Descriptor::reset (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Descriptor::reset");
  this->ACE_Thread_Descriptor_Base::reset ();
  this->at_exit_list_ = 0;
    // Start the at_exit hook list.
  this->tm_ = tm;
    // Setup the Thread_Manager.
  this->log_msg_ = 0;
  this->terminated_ = false;
}
ACE_INLINE ACE_Thread_Descriptor *
ACE_Thread_Manager::thread_desc_self (void)
{
  // This method must be called with lock held.
  // Try to get it from cache.
  ACE_Thread_Descriptor *desc = ACE_LOG_MSG->thr_desc ();
#if 1
  //  ACE_ASSERT (desc != 0);
  // Thread descriptor should always get cached.
#else
  if (desc == 0)
    {
      ACE_thread_t id = ACE_OS::thr_self ();
      desc = this->find_thread (id);
      // Thread descriptor adapter might not have been put into the
      // list yet.
      if (desc != 0)
        // Update the TSS cache.
        ACE_LOG_MSG->thr_desc (desc);
    }
#endif
  return desc;
}
// Return the unique ID of the thread.
ACE_INLINE ACE_thread_t
ACE_Thread_Manager::thr_self (void)
{
  ACE_TRACE ("ACE_Thread_Manager::thr_self");
  return ACE_Thread::self ();
}
ACE_INLINE ACE_Task_Base *
ACE_Thread_Manager::task (void)
{
  ACE_TRACE ("ACE_Thread_Manager::task");
  ACE_Thread_Descriptor *td = this->thread_desc_self () ;
  if (td == 0)
    return 0;
  else
    return td->task ();
}
ACE_INLINE int
ACE_Thread_Manager::open (size_t)
{
  // Currently no-op.
  return 0;
}
ACE_INLINE int
ACE_Thread_Manager::at_exit (ACE_At_Thread_Exit* at)
{
  ACE_Thread_Descriptor *td = this->thread_desc_self ();
  if (td == 0)
    return -1;
  else
    return td->at_exit (at);
}
ACE_INLINE int
ACE_Thread_Manager::at_exit (ACE_At_Thread_Exit& at)
{
  ACE_Thread_Descriptor *td = this->thread_desc_self ();
  if (td == 0)
    return -1;
  else
    return td->at_exit (at);
}
ACE_INLINE int
ACE_Thread_Manager::at_exit (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param)
{
  ACE_Thread_Descriptor *td = this->thread_desc_self ();
  if (td == 0)
    return -1;
  else
    return td->at_exit (object, cleanup_hook, param);
}
ACE_INLINE void
ACE_Thread_Manager::wait_on_exit (int do_wait)
{
  this->automatic_wait_ = do_wait;
}
ACE_INLINE int
ACE_Thread_Manager::wait_on_exit (void)
{
  return this->automatic_wait_;
}
ACE_INLINE int
ACE_Thread_Manager::register_as_terminated (ACE_Thread_Descriptor *td)
{
#if defined (ACE_HAS_VXTHREADS)
  ACE_UNUSED_ARG (td);
#else  /* ! ACE_HAS_VXTHREADS */
  ACE_Thread_Descriptor_Base *tdb = 0;
  ACE_NEW_RETURN (tdb, ACE_Thread_Descriptor_Base (*td), -1);
  this->terminated_thr_list_.insert_tail (tdb);
#endif /* !ACE_HAS_VXTHREADS */
  return 0;
}
ACE_INLINE size_t
ACE_Thread_Manager::count_threads (void) const
{
  return this->thr_list_.size ();
}
```cpp
ACE_At_Thread_Exit::~ACE_At_Thread_Exit (void)
{
  this->do_apply ();
}
ACE_At_Thread_Exit_Func::~ACE_At_Thread_Exit_Func (void)
{
  this->do_apply ();
}
void
ACE_At_Thread_Exit_Func::apply (void)
{
  this->func_ (this->object_, this->param_);
}
ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Control)
ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Manager)
#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS)
// Process-wide Thread Manager.
ACE_Thread_Manager *ACE_Thread_Manager::thr_mgr_ = 0;
// Controls whether the Thread_Manager is deleted when we shut down
// (we can only delete it safely if we created it!)
bool ACE_Thread_Manager::delete_thr_mgr_ = false;
#endif /* ! defined (ACE_THREAD_MANAGER_LACKS_STATICS) */
ACE_TSS_TYPE (ACE_Thread_Exit) *ACE_Thread_Manager::thr_exit_ = 0;
int
ACE_Thread_Manager::set_thr_exit (ACE_TSS_TYPE (ACE_Thread_Exit) *ptr)
{
  if (ACE_Thread_Manager::thr_exit_ == 0)
    ACE_Thread_Manager::thr_exit_ = ptr;
  else
    return -1;
  return 0;
}
void
ACE_Thread_Manager::dump (void)
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Thread_Manager::dump");
  // Cast away const-ness of this in order to use its non-const lock_.
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon,
                     ((ACE_Thread_Manager *) this)->lock_));
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\ngrp_id_ = %d"), this->grp_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\ncurrent_count_ = %d"), this->thr_list_.size ()));
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      iter.next ()->dump ();
    }
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}
ACE_Thread_Descriptor::~ACE_Thread_Descriptor (void)
{
  delete this->sync_;
}
void
ACE_Thread_Descriptor::at_pop (int apply)
{
  ACE_TRACE ("ACE_Thread_Descriptor::at_pop");
  // Get first at from at_exit_list
  ACE_At_Thread_Exit* at = this->at_exit_list_;
  // Remove at from at_exit list
  this->at_exit_list_ = at->next_;
  // Apply if required
  if (apply)
   {
     at->apply ();
     // Do the apply method
     at->was_applied (true);
     // Mark at has been applied to avoid double apply from
     // at destructor
   }
  // If at is not owner delete at.
  if (!at->is_owner ())
   delete at;
}
void
ACE_Thread_Descriptor::at_push (ACE_At_Thread_Exit* cleanup, bool is_owner)
{
  ACE_TRACE ("ACE_Thread_Descriptor::at_push");
  cleanup->is_owner (is_owner);
  cleanup->td_ = this;
  cleanup->next_ = at_exit_list_;
  at_exit_list_ = cleanup;
}
int
ACE_Thread_Descriptor::at_exit (ACE_At_Thread_Exit& cleanup)
{
  ACE_TRACE ("ACE_Thread_Descriptor::at_exit");
  at_push (&cleanup, 1);
  return 0;
}
int
ACE_Thread_Descriptor::at_exit (ACE_At_Thread_Exit* cleanup)
{
  ACE_TRACE ("ACE_Thread_Descriptor::at_exit");
  if (cleanup==0)
   return -1;
  else
   {
     this->at_push (cleanup);
     return 0;
   }
}
void
ACE_Thread_Descriptor::do_at_exit ()
{
  ACE_TRACE ("ACE_Thread_Descriptor::do_at_exit");
  while (at_exit_list_!=0)
    this->at_pop ();
}
void
ACE_Thread_Descriptor::terminate ()
{
  ACE_TRACE ("ACE_Thread_Descriptor::terminate");
  if (!terminated_)
   {
     ACE_Log_Msg* log_msg = this->log_msg_;
     terminated_ = true;
     // Run at_exit hooks
     this->do_at_exit ();
     // We must remove Thread_Descriptor from Thread_Manager list
     if (this->tm_ != 0)
      {
         int close_handle = 0;
#if !defined (ACE_HAS_VXTHREADS)
         // Threads created with THR_DAEMON shouldn't exist here, but
         // just to be safe, let's put it here.
         if (ACE_BIT_DISABLED (this->thr_state_, ACE_Thread_Manager::ACE_THR_JOINING))
           {
             if (ACE_BIT_DISABLED (this->flags_, THR_DETACHED | THR_DAEMON)
                 || ACE_BIT_ENABLED (this->flags_, THR_JOINABLE))
               {
                 // Mark thread as terminated.
                 ACE_SET_BITS (this->thr_state_, ACE_Thread_Manager::ACE_THR_TERMINATED);
                 tm_->register_as_terminated (this);
                 // Must copy the information here because td will be
                 // "freed" below.
               }
#if defined (ACE_WIN32)
             else
               {
                 close_handle = 1;
               }
#endif /* ACE_WIN32 */
           }
#endif /* !ACE_HAS_VXTHREADS */
         // Remove thread descriptor from the table.
         if (this->tm_ != 0)
           tm_->remove_thr (this, close_handle);
      }
     // Check if we need delete ACE_Log_Msg instance
     // If ACE_TSS_cleanup was not executed first log_msg == 0
     if (log_msg == 0)
      {
        // Only inform to ACE_TSS_cleanup that it must delete the log instance
        // setting ACE_LOG_MSG thr_desc to 0.
        ACE_LOG_MSG->thr_desc (0);
      }
     else
      {
        // Thread_Descriptor is the owner of the Log_Msg instance!!
        // deleted.
        this->log_msg_ = 0;
        delete log_msg;
      }
   }
}
int
ACE_Thread_Descriptor::at_exit (void *object,
                                ACE_CLEANUP_FUNC cleanup_hook,
                                void *param)
{
  ACE_TRACE ("ACE_Thread_Descriptor::at_exit");
  // To keep compatibility, when cleanup_hook is null really is a at_pop
  // without apply.
  if (cleanup_hook == 0)
   {
     if (this->at_exit_list_!= 0)
      this->at_pop(0);
   }
  else
   {
     ACE_At_Thread_Exit* cleanup = 0;
     ACE_NEW_RETURN (cleanup,
                     ACE_At_Thread_Exit_Func (object,
                                              cleanup_hook,
                                              param),
                     -1);
     this->at_push (cleanup);
   }
  return 0;
}
void
ACE_Thread_Descriptor::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Thread_Descriptor::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nthr_id_ = %d"), this->thr_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nthr_handle_ = %d"), this->thr_handle_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\ngrp_id_ = %d"), this->grp_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nthr_state_ = %d"), this->thr_state_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nflags_ = %x\n"), this->flags_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}
ACE_Thread_Descriptor::ACE_Thread_Descriptor (void)
  : log_msg_ (0),
    at_exit_list_ (0),
    terminated_ (false)
{
  ACE_TRACE ("ACE_Thread_Descriptor::ACE_Thread_Descriptor");
  ACE_NEW (this->sync_,
           ACE_DEFAULT_THREAD_MANAGER_LOCK);
}
void
ACE_Thread_Descriptor::acquire_release (void)
{
  // Just try to acquire the lock then release it.
#if defined (ACE_THREAD_MANAGER_USES_SAFE_SPAWN)
  if (ACE_BIT_DISABLED (this->thr_state_, ACE_Thread_Manager::ACE_THR_SPAWNED))
#endif /* ACE_THREAD_MANAGER_USES_SAFE_SPAWN */
    {
      this->sync_->acquire ();
      // Acquire the lock before removing <td> from the thread table.  If
      // this thread is in the table already, it should simply acquire the
      // lock easily.
      // Once we get the lock, we must have registered.
      ACE_ASSERT (ACE_BIT_ENABLED (this->thr_state_, ACE_Thread_Manager::ACE_THR_SPAWNED));
      this->sync_->release ();
      // Release the lock before putting it back to freelist.
    }
}
void
ACE_Thread_Descriptor::acquire (void)
{
  // Just try to acquire the lock then release it.
#if defined (ACE_THREAD_MANAGER_USES_SAFE_SPAWN)
  if (ACE_BIT_DISABLED (this->thr_state_, ACE_Thread_Manager::ACE_THR_SPAWNED))
#endif /* ACE_THREAD_MANAGER_USES_SAFE_SPAWN */
    {
      this->sync_->acquire ();
    }
}
void
ACE_Thread_Descriptor::release (void)
{
  // Just try to acquire the lock then release it.
#if defined (ACE_THREAD_MANAGER_USES_SAFE_SPAWN)
  if (ACE_BIT_DISABLED (this->thr_state_, ACE_Thread_Manager::ACE_THR_SPAWNED))
#endif /* ACE_THREAD_MANAGER_USES_SAFE_SPAWN */
    {
      this->sync_->release ();
      // Release the lock before putting it back to freelist.
    }
}
// The following macro simplifies subsequence code.
#define ACE_FIND(OP,INDEX) \
  ACE_Thread_Descriptor *INDEX = OP; \
ACE_Thread_Descriptor *
ACE_Thread_Manager::thread_descriptor (ACE_thread_t thr_id)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));
  ACE_FIND (this->find_thread (thr_id), ptr);
  return ptr;
}
ACE_Thread_Descriptor *
ACE_Thread_Manager::hthread_descriptor (ACE_hthread_t thr_handle)
{
  ACE_TRACE ("ACE_Thread_Manager::hthread_descriptor");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));
  ACE_FIND (this->find_hthread (thr_handle), ptr);
  return ptr;
}
// Return the thread descriptor (indexed by ACE_hthread_t).
int
ACE_Thread_Manager::thr_self (ACE_hthread_t &self)
{
  ACE_TRACE ("ACE_Thread_Manager::thr_self");
  ACE_Thread_Descriptor *desc =
    this->thread_desc_self ();
  if (desc == 0)
    return -1;
  else
    desc->self (self);
  return 0;
}
// Initialize the synchronization variables.
ACE_Thread_Manager::ACE_Thread_Manager (size_t prealloc,
                                        size_t lwm,
                                        size_t inc,
                                        size_t hwm)
  : grp_id_ (1),
    automatic_wait_ (1)
#if defined (ACE_HAS_THREADS)
    , zero_cond_ (lock_)
#endif /* ACE_HAS_THREADS */
    , thread_desc_freelist_ (ACE_FREE_LIST_WITH_POOL,
                             prealloc, lwm, hwm, inc)
{
  ACE_TRACE ("ACE_Thread_Manager::ACE_Thread_Manager");
}
#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS)
ACE_Thread_Manager *
ACE_Thread_Manager::instance (void)
{
  ACE_TRACE ("ACE_Thread_Manager::instance");
  if (ACE_Thread_Manager::thr_mgr_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));
      if (ACE_Thread_Manager::thr_mgr_ == 0)
        {
          ACE_NEW_RETURN (ACE_Thread_Manager::thr_mgr_,
                          ACE_Thread_Manager,
                          0);
          ACE_Thread_Manager::delete_thr_mgr_ = true;
        }
    }
  return ACE_Thread_Manager::thr_mgr_;
}
ACE_Thread_Manager *
ACE_Thread_Manager::instance (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Manager::instance");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                            *ACE_Static_Object_Lock::instance (), 0));
  ACE_Thread_Manager *t = ACE_Thread_Manager::thr_mgr_;
  // We can't safely delete it since we don't know who created it!
  ACE_Thread_Manager::delete_thr_mgr_ = false;
  ACE_Thread_Manager::thr_mgr_ = tm;
  return t;
}
void
ACE_Thread_Manager::close_singleton (void)
{
  ACE_TRACE ("ACE_Thread_Manager::close_singleton");
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));
  if (ACE_Thread_Manager::delete_thr_mgr_)
    {
      // First, we clean up the thread descriptor list.
      ACE_Thread_Manager::thr_mgr_->close ();
      delete ACE_Thread_Manager::thr_mgr_;
      ACE_Thread_Manager::thr_mgr_ = 0;
      ACE_Thread_Manager::delete_thr_mgr_ = false;
    }
  ACE_Thread_Exit::cleanup (ACE_Thread_Manager::thr_exit_);
}
#endif /* ! defined (ACE_THREAD_MANAGER_LACKS_STATICS) */
// Close up and release all resources.
int
ACE_Thread_Manager::close ()
{
  ACE_TRACE ("ACE_Thread_Manager::close");
  // Clean up the thread descriptor list.
  if (this->automatic_wait_)
    this->wait (0, 1);
  else
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
      this->remove_thr_all ();
    }
  return 0;
}
ACE_Thread_Manager::~ACE_Thread_Manager (void)
{
  ACE_TRACE ("ACE_Thread_Manager::~ACE_Thread_Manager");
  this->close ();
}
// Run the entry point for thread spawned under the control of the
// <ACE_Thread_Manager>.  This must be an extern "C" to make certain
// compilers happy...
//
// The interaction with <ACE_Thread_Exit> and
// <ace_thread_manager_adapter> works like this, with
// ACE_HAS_THREAD_SPECIFIC_STORAGE or ACE_HAS_TSS_EMULATION:
//
// o Every thread in the <ACE_Thread_Manager> is run with
//   <ace_thread_manager_adapter>.
//
// o <ace_thread_manager_adapter> retrieves the singleton
//   <ACE_Thread_Exit> instance from <ACE_Thread_Exit::instance>.
//   The singleton gets created in thread-specific storage
//   in the first call to that function.  The key point is that the
//   instance is in thread-specific storage.
//
// o A thread can exit by various means, such as <ACE_Thread::exit>, C++
//   or Win32 exception, "falling off the end" of the thread entry
//   point function, etc.
//
// o If you follow this so far, now it gets really fun . . .
//   When the thread-specific storage (for the thread that
//   is being destroyed) is cleaned up, the OS threads package (or
//   the ACE emulation of thread-specific storage) will destroy any
//   objects that are in thread-specific storage.  It has a list of
//   them, and just walks down the list and destroys each one.
//
// o That's where the ACE_Thread_Exit destructor gets called.
#if defined(ACE_USE_THREAD_MANAGER_ADAPTER)
extern "C" void *
ace_thread_manager_adapter (void *args)
{
#if defined (ACE_HAS_TSS_EMULATION)
  // As early as we can in the execution of the new thread, allocate
  // its local TS storage.  Allocate it on the stack, to save dynamic
  // allocation/dealloction.
  void *ts_storage[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX];
  ACE_TSS_Emulation::tss_open (ts_storage);
#endif /* ACE_HAS_TSS_EMULATION */
  ACE_Thread_Adapter *thread_args = reinterpret_cast<ACE_Thread_Adapter *> (args);
  // NOTE: this preprocessor directive should match the one in above
  // ACE_Thread_Exit::instance ().  With the Xavier Pthreads package,
  // the exit_hook in TSS causes a seg fault.  So, this works around
  // that by creating exit_hook on the stack.
#if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)
  // Obtain our thread-specific exit hook and make sure that it knows
  // how to clean us up!  Note that we never use this pointer directly
  // (it's stored in thread-specific storage), so it's ok to
  // dereference it here and only store it as a reference.
  ACE_Thread_Exit &exit_hook = *ACE_Thread_Exit::instance ();
#else
  // Without TSS, create an <ACE_Thread_Exit> instance.  When this
  // function returns, its destructor will be called because the
  // object goes out of scope.  The drawback with this appraoch is
  // that the destructor _won't_ get called if <thr_exit> is called.
  // So, threads shouldn't exit that way.  Instead, they should return
  // from <svc>.
  ACE_Thread_Exit exit_hook;
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION */
  // Keep track of the <Thread_Manager> that's associated with this
  // <exit_hook>.
  exit_hook.thr_mgr (thread_args->thr_mgr ());
  // Invoke the user-supplied function with the args.
  void *status = thread_args->invoke ();
  delete static_cast<ACE_Base_Thread_Adapter *> (thread_args);
  return status;
}
#endif
// Call the appropriate OS routine to spawn a thread.  Should *not* be
// called with the lock_ held...
int
ACE_Thread_Manager::spawn_i (ACE_THR_FUNC func,
                             void *args,
                             long flags,
                             ACE_thread_t *t_id,
                             ACE_hthread_t *t_handle,
                             long priority,
                             int grp_id,
                             void *stack,
                             size_t stack_size,
                             ACE_Task_Base *task,
                             const char** thr_name)
{
  // First, threads created by Thread Manager should not be daemon threads.
  // Using assertion is probably a bit too strong.  However, it helps
  // finding this kind of error as early as possible.  Perhaps we can replace
  // assertion by returning error.
  ACE_ASSERT (ACE_BIT_DISABLED (flags, THR_DAEMON));
  // Create a new thread running <func>.  *Must* be called with the
  // <lock_> held...
  // Get a "new" Thread Descriptor from the freelist.
  auto_ptr<ACE_Thread_Descriptor> new_thr_desc (this->thread_desc_freelist_.remove ());
  // Reset thread descriptor status
  new_thr_desc->reset (this);
  ACE_Thread_Adapter *thread_args = 0;
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
  ACE_NEW_RETURN (thread_args,
                  ACE_Thread_Adapter (func,
                                      args,
                                      (ACE_THR_C_FUNC) ACE_THREAD_ADAPTER_NAME,
                                      this,
                                      new_thr_desc.get (),
                                      ACE_OS_Object_Manager::seh_except_selector(),
                                      ACE_OS_Object_Manager::seh_except_handler(),
                                      flags),
                  -1);
# else
  ACE_NEW_RETURN (thread_args,
                  ACE_Thread_Adapter (func,
                                      args,
                                      (ACE_THR_C_FUNC) ACE_THREAD_ADAPTER_NAME,
                                      this,
                                      new_thr_desc.get (),
                                      flags),
                  -1);
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
  auto_ptr <ACE_Base_Thread_Adapter> auto_thread_args (static_cast<ACE_Base_Thread_Adapter *> (thread_args));
  ACE_TRACE ("ACE_Thread_Manager::spawn_i");
  ACE_hthread_t thr_handle;
  ACE_thread_t thr_id;
  if (t_id == 0)
    t_id = &thr_id;
  // Acquire the <sync_> lock to block the spawned thread from
  // removing this Thread Descriptor before it gets put into our
  // thread table.
  new_thr_desc->sync_->acquire ();
  int const result = ACE_Thread::spawn (func,
                                        args,
                                        flags,
                                        t_id,
                                        &thr_handle,
                                        priority,
                                        stack,
                                        stack_size,
                                        thread_args,
                                        thr_name);
  if (result != 0)
    {
      // _Don't_ clobber errno here!  result is either 0 or -1, and
      // ACE_OS::thr_create () already set errno!  D. Levine 28 Mar 1997
      // errno = result;
      ACE_Errno_Guard guard (errno);     // Lock release may smash errno
      new_thr_desc->sync_->release ();
      return -1;
    }
  auto_thread_args.release ();
#if defined (ACE_HAS_WTHREADS)
  // Have to duplicate handle if client asks for it.
  // @@ How are thread handles implemented on AIX?  Do they
  // also need to be duplicated?
  if (t_handle != 0)
# if defined (ACE_LACKS_DUPLICATEHANDLE)
    *t_handle = thr_handle;
# else  /* ! ACE_LACKS_DUP */
  (void) ::DuplicateHandle (::GetCurrentProcess (),
                            thr_handle,
                            ::GetCurrentProcess (),
                            t_handle,
                            0,
                            TRUE,
                            DUPLICATE_SAME_ACCESS);
# endif /* ! ACE_LACKS_DUP */
#else  /* ! ACE_HAS_WTHREADS */
  if (t_handle != 0)
    *t_handle = thr_handle;
#endif /* ! ACE_HAS_WTHREADS */
  // append_thr also put the <new_thr_desc> into Thread_Manager's
  // double-linked list.  Only after this point, can we manipulate
  // double-linked list from a spawned thread's context.
  return this->append_thr (*t_id,
                           thr_handle,
                           ACE_THR_SPAWNED,
                           grp_id,
                           task,
                           flags,
                           new_thr_desc.release ());
}
int
ACE_Thread_Manager::spawn (ACE_THR_FUNC func,
                           void *args,
                           long flags,
                           ACE_thread_t *t_id,
                           ACE_hthread_t *t_handle,
                           long priority,
                           int grp_id,
                           void *stack,
                           size_t stack_size,
                           const char** thr_name)
{
  ACE_TRACE ("ACE_Thread_Manager::spawn");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  if (grp_id == -1)
    grp_id = this->grp_id_++; // Increment the group id.
  if (priority != ACE_DEFAULT_THREAD_PRIORITY)
    ACE_CLR_BITS (flags, THR_INHERIT_SCHED);
  if (this->spawn_i (func,
                     args,
                     flags,
                     t_id,
                     t_handle,
                     priority,
                     grp_id,
                     stack,
                     stack_size,
                     0,
                     thr_name) == -1)
    return -1;
  return grp_id;
}
// Create N new threads running FUNC.
int
ACE_Thread_Manager::spawn_n (size_t n,
                             ACE_THR_FUNC func,
                             void *args,
                             long flags,
                             long priority,
                             int grp_id,
                             ACE_Task_Base *task,
                             ACE_hthread_t thread_handles[],
                             void *stack[],
                             size_t stack_size[],
                             const char* thr_name[])
{
  ACE_TRACE ("ACE_Thread_Manager::spawn_n");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  if (grp_id == -1)
    grp_id = this->grp_id_++; // Increment the group id.
  for (size_t i = 0; i < n; i++)
    {
      // @@ What should happen if this fails?! e.g., should we try to
      // cancel the other threads that we've already spawned or what?
      if (this->spawn_i (func,
                         args,
                         flags,
                         0,
                         thread_handles == 0 ? 0 : &thread_handles[i],
                         priority,
                         grp_id,
                         stack == 0 ? 0 : stack[i],
                         stack_size == 0 ? ACE_DEFAULT_THREAD_STACKSIZE : stack_size[i],
                         task,
                         thr_name == 0 ? 0 : &thr_name [i]) == -1)
        return -1;
    }
  return grp_id;
}
// Create N new threads running FUNC.
int
ACE_Thread_Manager::spawn_n (ACE_thread_t thread_ids[],
                             size_t n,
                             ACE_THR_FUNC func,
                             void *args,
                             long flags,
                             long priority,
                             int grp_id,
                             void *stack[],
                             size_t stack_size[],
                             ACE_hthread_t thread_handles[],
                             ACE_Task_Base *task,
                             const char* thr_name[])
{
  ACE_TRACE ("ACE_Thread_Manager::spawn_n");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  if (grp_id == -1)
    grp_id = this->grp_id_++; // Increment the group id.
  for (size_t i = 0; i < n; i++)
    {
      // @@ What should happen if this fails?! e.g., should we try to
      // cancel the other threads that we've already spawned or what?
      if (this->spawn_i (func,
                         args,
                         flags,
                         thread_ids == 0 ? 0 : &thread_ids[i],
                         thread_handles == 0 ? 0 : &thread_handles[i],
                         priority,
                         grp_id,
                         stack == 0 ? 0 : stack[i],
                         stack_size == 0 ? ACE_DEFAULT_THREAD_STACKSIZE : stack_size[i],
                         task,
                         thr_name == 0 ? 0 : &thr_name [i]) == -1)
        return -1;
    }
  return grp_id;
}
// Append a thread into the pool (does not check for duplicates).
// Must be called with locks held.
int
ACE_Thread_Manager::append_thr (ACE_thread_t t_id,
                                ACE_hthread_t t_handle,
                                ACE_UINT32 thr_state,
                                int grp_id,
                                ACE_Task_Base *task,
                                long flags,
                                ACE_Thread_Descriptor *td)
{
  ACE_TRACE ("ACE_Thread_Manager::append_thr");
  ACE_Thread_Descriptor *thr_desc = 0;
  if (td == 0)
    {
      ACE_NEW_RETURN (thr_desc,
                      ACE_Thread_Descriptor,
                      -1);
      thr_desc->tm_ = this;
      // Setup the Thread_Manager.
    }
  else
    thr_desc = td;
  thr_desc->thr_id_ = t_id;
  thr_desc->thr_handle_ = t_handle;
  thr_desc->grp_id_ = grp_id;
  thr_desc->task_ = task;
  thr_desc->flags_ = flags;
  this->thr_list_.insert_head (thr_desc);
  ACE_SET_BITS (thr_desc->thr_state_, thr_state);
  thr_desc->sync_->release ();
  return 0;
}
// Return the thread descriptor (indexed by ACE_hthread_t).
ACE_Thread_Descriptor *
ACE_Thread_Manager::find_hthread (ACE_hthread_t h_id)
{
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (ACE_OS::thr_cmp (iter.next ()->thr_handle_, h_id))
        {
          return iter.next ();
        }
    }
  return 0;
}
// Locate the index in the table associated with <t_id>.  Must be
// called with the lock held.
ACE_Thread_Descriptor *
ACE_Thread_Manager::find_thread (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::find_thread");
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (ACE_OS::thr_equal (iter.next ()->thr_id_, t_id))
        {
          return iter.next ();
        }
    }
  return 0;
}
// Insert a thread into the pool (checks for duplicates and doesn't
// allow them to be inserted twice).
int
ACE_Thread_Manager::insert_thr (ACE_thread_t t_id,
                                ACE_hthread_t t_handle,
                                int grp_id,
                                long flags)
{
  ACE_TRACE ("ACE_Thread_Manager::insert_thr");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  // Check for duplicates and bail out if we're already registered...
  if (this->find_thread (t_id) != 0 )
    return -1;
  if (grp_id == -1)
    grp_id = this->grp_id_++;
  if (this->append_thr (t_id,
                        t_handle,
                        ACE_THR_SPAWNED,
                        grp_id,
                        0,
                        flags) == -1)
    return -1;
  return grp_id;
}
// Run the registered hooks when the thread exits.
void
ACE_Thread_Manager::run_thread_exit_hooks (int i)
{
#if 0 // currently unused!
  ACE_TRACE ("ACE_Thread_Manager::run_thread_exit_hooks");
  // @@ Currently, we have just one hook.  This should clearly be
  // generalized to support an arbitrary number of hooks.
  ACE_Thread_Descriptor *td = this->thread_desc_self ();
  for (ACE_Cleanup_Info_Node *iter = td->cleanup_info_->pop_front ();
       iter != 0;
       iter = cleanup_info_->pop_front ())
    {
      if (iter->cleanup_hook () != 0)
        {
          (*iter->cleanup_hook ()) (iter->object (), iter->param ());
        }
      delete iter;
    }
  ACE_UNUSED_ARG (i);
#else
  ACE_UNUSED_ARG (i);
#endif /* 0 */
}
// Remove a thread from the pool.  Must be called with locks held.
void
ACE_Thread_Manager::remove_thr (ACE_Thread_Descriptor *td,
                                int close_handler)
{
  ACE_TRACE ("ACE_Thread_Manager::remove_thr");
  td->tm_ = 0;
  this->thr_list_.remove (td);
#if defined (ACE_WIN32)
  if (close_handler != 0)
    ::CloseHandle (td->thr_handle_);
#else
  ACE_UNUSED_ARG (close_handler);
#endif /* ACE_WIN32 */
  this->thread_desc_freelist_.add (td);
#if defined (ACE_HAS_THREADS)
  // Tell all waiters when there are no more threads left in the pool.
  if (this->thr_list_.size () == 0)
    this->zero_cond_.broadcast ();
#endif /* ACE_HAS_THREADS */
}
// Repeatedly call remove_thr on all table entries until there
// is no thread left.   Must be called with lock held.
void
ACE_Thread_Manager::remove_thr_all (void)
{
  ACE_Thread_Descriptor *td = 0;
  while ((td = this->thr_list_.delete_head ()) != 0)
    {
      this->remove_thr (td, 1);
    }
}
// ------------------------------------------------------------------
// Factor out some common behavior to simplify the following methods.
#define ACE_THR_OP(OP,STATE) \
  int result = OP (td->thr_handle_); \
  if (result == -1) { \
    if (errno != ENOTSUP) \
      this->thr_to_be_removed_.enqueue_tail (td); \
    return -1; \
  } \
  else { \
    ACE_SET_BITS (td->thr_state_, STATE); \
    return 0; \
  }
int
ACE_Thread_Manager::join_thr (ACE_Thread_Descriptor *td, int)
{
  ACE_TRACE ("ACE_Thread_Manager::join_thr");
  int const result = ACE_Thread::join (td->thr_handle_);
  if (result != 0)
    {
      // Since the thread are being joined, we should
      // let it remove itself from the list.
      //      this->remove_thr (td);
      errno = result;
      return -1;
    }
  return 0;
}
int
ACE_Thread_Manager::suspend_thr (ACE_Thread_Descriptor *td, int)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_thr");
  int const result = ACE_Thread::suspend (td->thr_handle_);
  if (result == -1) {
    if (errno != ENOTSUP)
      this->thr_to_be_removed_.enqueue_tail (td);
    return -1;
  }
  else {
    ACE_SET_BITS (td->thr_state_, ACE_THR_SUSPENDED);
    return 0;
  }
}
int
ACE_Thread_Manager::resume_thr (ACE_Thread_Descriptor *td, int)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_thr");
  int const result = ACE_Thread::resume (td->thr_handle_);
  if (result == -1) {
    if (errno != ENOTSUP)
      this->thr_to_be_removed_.enqueue_tail (td);
    return -1;
  }
  else {
    ACE_CLR_BITS (td->thr_state_, ACE_THR_SUSPENDED);
    return 0;
  }
}
int
ACE_Thread_Manager::cancel_thr (ACE_Thread_Descriptor *td, int async_cancel)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_thr");
  // Must set the state first and then try to cancel the thread.
  ACE_SET_BITS (td->thr_state_, ACE_THR_CANCELLED);
  if (async_cancel != 0)
    // Note that this call only does something relevant if the OS
    // platform supports asynchronous thread cancellation.  Otherwise,
    // it's a no-op.
    return ACE_Thread::cancel (td->thr_id_);
  return 0;
}
int
ACE_Thread_Manager::kill_thr (ACE_Thread_Descriptor *td, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_thr");
  ACE_thread_t tid = td->thr_id_;
  int const result = ACE_Thread::kill (tid, signum);
  if (result != 0)
    {
      // Only remove a thread from us when there is a "real" error.
      if (errno != ENOTSUP)
        this->thr_to_be_removed_.enqueue_tail (td);
      return -1;
    }
    return 0;
}
// ------------------------------------------------------------------
// Factor out some common behavior to simplify the following methods.
#define ACE_EXECUTE_OP(OP, ARG) \
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1)); \
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ()); \
  ACE_FIND (this->find_thread (t_id), ptr); \
  if (ptr == 0) \
    { \
      errno = ENOENT; \
      return -1; \
    } \
  int const result = OP (ptr, ARG); \
  ACE_Errno_Guard error (errno); \
  while (! this->thr_to_be_removed_.is_empty ()) { \
    ACE_Thread_Descriptor * td = 0; \
    this->thr_to_be_removed_.dequeue_head (td); \
    this->remove_thr (td, 1); \
  } \
  return result
// Suspend a single thread.
int
ACE_Thread_Manager::suspend (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend");
  ACE_EXECUTE_OP (this->suspend_thr, 0);
}
// Resume a single thread.
int
ACE_Thread_Manager::resume (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::resume");
  ACE_EXECUTE_OP (this->resume_thr, 0);
}
// Cancel a single thread.
int
ACE_Thread_Manager::cancel (ACE_thread_t t_id, int async_cancel)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel");
  ACE_EXECUTE_OP (this->cancel_thr, async_cancel);
}
// Send a signal to a single thread.
int
ACE_Thread_Manager::kill (ACE_thread_t t_id, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill");
  ACE_EXECUTE_OP (this->kill_thr, signum);
}
int
ACE_Thread_Manager::check_state (ACE_UINT32 state,
                                 ACE_thread_t id,
                                 int enable)
{
  ACE_TRACE ("ACE_Thread_Manager::check_state");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_UINT32 thr_state;
  int self_check = ACE_OS::thr_equal (id, ACE_OS::thr_self ());
  // If we're checking the state of our thread, try to get the cached
  // value out of TSS to avoid lookup.
  if (self_check)
    {
      ACE_Thread_Descriptor *desc = ACE_LOG_MSG->thr_desc ();
      if (desc == 0)
        return 0;               // Always return false.
      thr_state = desc->thr_state_;
    }
  else
    {
      // Not calling from self, have to look it up from the list.
      ACE_FIND (this->find_thread (id), ptr);
      if (ptr == 0)
        return 0;
      thr_state = ptr->thr_state_;
    }
  if (enable)
    return ACE_BIT_ENABLED (thr_state, state);
  return ACE_BIT_DISABLED (thr_state, state);
}
// Test if a single thread has terminated.
int
ACE_Thread_Manager::testterminate (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::testterminate");
  return this->check_state (ACE_THR_TERMINATED, t_id);
}
// Test if a single thread is suspended.
int
ACE_Thread_Manager::testsuspend (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::testsuspend");
  return this->check_state (ACE_THR_SUSPENDED, t_id);
}
// Test if a single thread is active (i.e., resumed).
int
ACE_Thread_Manager::testresume (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::testresume");
  return this->check_state (ACE_THR_SUSPENDED, t_id, 0);
}
// Test if a single thread is cancelled.
int
ACE_Thread_Manager::testcancel (ACE_thread_t t_id)
{
  ACE_TRACE ("ACE_Thread_Manager::testcancel");
  return this->check_state (ACE_THR_CANCELLED, t_id);
}
// Thread information query functions.
int
ACE_Thread_Manager::hthread_within (ACE_hthread_t handle)
{
  ACE_TRACE ("ACE_Thread_Manager::hthread_within");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_monx, this->lock_, -1));
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (ACE_OS::thr_cmp(iter.next ()->thr_handle_, handle))
        {
          return 1;
        }
    }
  return 0;
}
int
ACE_Thread_Manager::thread_within (ACE_thread_t tid)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_within");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_monx, this->lock_, -1));
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (ACE_OS::thr_equal (iter.next ()->thr_id_, tid))
        {
          return 1;
        }
    }
  return 0;
}
// Get group ids for a particular thread id.
int
ACE_Thread_Manager::get_grp (ACE_thread_t t_id, int &grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::get_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_FIND (this->find_thread (t_id), ptr);
  if (ptr)
    grp_id = ptr->grp_id_;
  else
    return -1;
  return 0;
}
// Set group ids for a particular thread id.
int
ACE_Thread_Manager::set_grp (ACE_thread_t t_id, int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::set_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_FIND (this->find_thread (t_id), ptr);
  if (ptr)
    ptr->grp_id_ = grp_id;
  else
    return -1;
  return 0;
}
// Suspend a group of threads.
int
ACE_Thread_Manager::apply_grp (int grp_id,
                               ACE_THR_MEMBER_FUNC func,
                               int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_monx, this->lock_, -1));
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ());
  int result = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (iter.next ()->grp_id_ == grp_id)
        {
          if ((this->*func) (iter.next (), arg) == -1)
            {
              result = -1;
            }
        }
    }
  // Must remove threads after we have traversed the thr_list_ to
  // prevent clobber thr_list_'s integrity.
  if (! this->thr_to_be_removed_.is_empty ())
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      for (ACE_Thread_Descriptor *td;
           this->thr_to_be_removed_.dequeue_head (td) != -1;
           )
        this->remove_thr (td, 1);
    }
  return result;
}
int
ACE_Thread_Manager::suspend_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::suspend_thr));
}
// Resume a group of threads.
int
ACE_Thread_Manager::resume_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::resume_thr));
}
// Kill a group of threads.
int
ACE_Thread_Manager::kill_grp (int grp_id, int signum)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::kill_thr), signum);
}
// Cancel a group of threads.
int
ACE_Thread_Manager::cancel_grp (int grp_id, int async_cancel)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_grp");
  return this->apply_grp (grp_id,
                          ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::cancel_thr),
                          async_cancel);
}
int
ACE_Thread_Manager::apply_all (ACE_THR_MEMBER_FUNC func, int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_all");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ());
  int result = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if ((this->*func)(iter.next (), arg) == -1)
        {
          result = -1;
        }
    }
  // Must remove threads after we have traversed the thr_list_ to
  // prevent clobber thr_list_'s integrity.
  if (! this->thr_to_be_removed_.is_empty ())
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      for (ACE_Thread_Descriptor *td;
           this->thr_to_be_removed_.dequeue_head (td) != -1;
           )
        this->remove_thr (td, 1);
    }
  return result;
}
// Resume all threads that are suspended.
int
ACE_Thread_Manager::resume_all (void)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_all");
  return this->apply_all (ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::resume_thr));
}
int
ACE_Thread_Manager::suspend_all (void)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_all");
  return this->apply_all (ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::suspend_thr));
}
int
ACE_Thread_Manager::kill_all (int sig)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_all");
  return this->apply_all (&ACE_Thread_Manager::kill_thr, sig);
}
int
ACE_Thread_Manager::cancel_all (int async_cancel)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_all");
  return this->apply_all (ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::cancel_thr),
                          async_cancel);
}
int
ACE_Thread_Manager::join (ACE_thread_t tid, ACE_THR_FUNC_RETURN *status)
{
  ACE_TRACE ("ACE_Thread_Manager::join");
  bool found = false;
  ACE_Thread_Descriptor_Base tdb;
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
#if !defined (ACE_HAS_VXTHREADS)
    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor_Base> biter (this->terminated_thr_list_);
         !biter.done ();
         biter.advance ())
      {
        if (ACE_OS::thr_equal (biter.next ()->thr_id_, tid))
          {
            ACE_Thread_Descriptor_Base *tdb = biter.advance_and_remove (false);
            if (ACE_Thread::join (tdb->thr_handle_, status) == -1)
              {
                return -1;
              }
            delete tdb;
            // return immediately if we've found the thread we want to join.
            return 0;
          }
      }
#endif /* !ACE_HAS_VXTHREADS */
    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
         !iter.done ();
         iter.advance ())
      {
        // If threads are created as THR_DETACHED or THR_DAEMON, we
        // can't help much.
        if (ACE_OS::thr_equal (iter.next ()->thr_id_,tid) &&
            (ACE_BIT_DISABLED (iter.next ()->flags_, THR_DETACHED | THR_DAEMON)
             || ACE_BIT_ENABLED (iter.next ()->flags_, THR_JOINABLE)))
          {
            tdb = *iter.next ();
            ACE_SET_BITS (iter.next ()->thr_state_, ACE_THR_JOINING);
            found = 1;
            break;
          }
      }
    if (!found)
      return -1;
    // Didn't find the thread we want or the thread is not joinable.
  }
  if (ACE_Thread::join (tdb.thr_handle_, status) == -1)
    return -1;
  return 0;
}
// Wait for group of threads
int
ACE_Thread_Manager::wait_grp (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::wait_grp");
  int copy_count = 0;
  ACE_Thread_Descriptor_Base *copy_table = 0;
  // We have to make sure that while we wait for these threads to
  // exit, we do not have the lock.  Therefore we make a copy of all
  // interesting entries and let go of the lock.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
#if !defined (ACE_HAS_VXTHREADS)
    ACE_NEW_RETURN (copy_table,
                    ACE_Thread_Descriptor_Base [this->thr_list_.size ()
                                               + this->terminated_thr_list_.size ()],
                    -1);
#else
    ACE_NEW_RETURN (copy_table,
                    ACE_Thread_Descriptor_Base [this->thr_list_.size ()],
                    -1);
#endif /* !ACE_HAS_VXTHREADS */
    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
         !iter.done ();
         iter.advance ())
      {
        // If threads are created as THR_DETACHED or THR_DAEMON, we
        // can't help much.
        if (iter.next ()->grp_id_ == grp_id &&
            (ACE_BIT_DISABLED (iter.next ()->flags_, THR_DETACHED | THR_DAEMON)
             || ACE_BIT_ENABLED (iter.next ()->flags_, THR_JOINABLE)))
          {
            ACE_SET_BITS (iter.next ()->thr_state_, ACE_THR_JOINING);
            copy_table[copy_count++] = *iter.next ();
          }
      }
#if !defined (ACE_HAS_VXTHREADS)
    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor_Base> biter (this->terminated_thr_list_);
         !biter.done ();
         biter.advance ())
      {
        // If threads are created as THR_DETACHED or THR_DAEMON, we
        // can't help much.
        if (biter.next ()->grp_id_ == grp_id)
          {
            ACE_Thread_Descriptor_Base *tdb = biter.advance_and_remove (false);
            copy_table[copy_count++] = *tdb;
            delete tdb;
          }
      }
#endif /* !ACE_HAS_VXTHREADS */
  }
  // Now actually join() with all the threads in this group.
  int result = 0;
  for (int i = 0;
       i < copy_count && result != -1;
       i++)
    {
      if (ACE_Thread::join (copy_table[i].thr_handle_) == -1)
        result = -1;
    }
  delete [] copy_table;
  return result;
}
// Must be called when thread goes out of scope to clean up its table
// slot.
ACE_THR_FUNC_RETURN
ACE_Thread_Manager::exit (ACE_THR_FUNC_RETURN status, bool do_thread_exit)
{
  ACE_TRACE ("ACE_Thread_Manager::exit");
#if defined (ACE_WIN32)
  // Remove detached thread handle.
  if (do_thread_exit)
    {
#if 0
      // @@ This callback is now taken care of by TSS_Cleanup.  Do we
      //    need it anymore?
      // On Win32, if we really wants to exit from a thread, we must
      // first  clean up the thread specific storage.  By doing so,
      // ACE_Thread_Manager::exit will be called again with
      // do_thr_exit = 0 and cleaning up the ACE_Cleanup_Info (but not
      // exiting the thread.)  After the following call returns, we
      // are safe to exit this thread.
      delete ACE_Thread_Exit::instance ();
#endif /* 0 */
      ACE_Thread::exit (status);
    }
#endif /* ACE_WIN32 */
  // Just hold onto the guard while finding this thread's id and
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));
    // Find the thread id, but don't use the cache.  It might have been
    // deleted already.
    ACE_thread_t const id = ACE_OS::thr_self ();
    ACE_Thread_Descriptor* td = this->find_thread (id);
    if (td != 0)
     {
       // @@ We call Thread_Descriptor terminate this realize the cleanup
       // process itself.
       td->terminate();
     }
  }
  if (do_thread_exit)
    {
      ACE_Thread::exit (status);
      // On reasonable systems <ACE_Thread::exit> should not return.
      // However, due to horrible semantics with Win32 thread-specific
      // storage this call can return (don't ask...).
    }
  return 0;
}
// Wait for all the threads to exit.
int
ACE_Thread_Manager::wait (const ACE_Time_Value *timeout,
                          bool abandon_detached_threads,
                          bool use_absolute_time)
{
  ACE_TRACE ("ACE_Thread_Manager::wait");
  ACE_Time_Value local_timeout;
  // Check to see if we're using absolute time or not.
  if (use_absolute_time == false && timeout != 0)
    {
      local_timeout = *timeout;
      local_timeout += ACE_OS::gettimeofday ();
      timeout = &local_timeout;
    }
#if !defined (ACE_HAS_VXTHREADS)
  ACE_Double_Linked_List<ACE_Thread_Descriptor_Base> term_thr_list_copy;
#endif /* ACE_HAS_VXTHREADS */
#if defined (ACE_HAS_THREADS)
  {
    // Just hold onto the guard while waiting.
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
    if (ACE_Object_Manager::shutting_down () != 1)
      {
        // Program is not shutting down.  Perform a normal wait on threads.
        if (abandon_detached_threads != 0)
          {
            ACE_ASSERT (this->thr_to_be_removed_.is_empty ());
            for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>
                   iter (this->thr_list_);
                 !iter.done ();
                 iter.advance ())
              {
                if (ACE_BIT_ENABLED (iter.next ()->flags_,
                                     THR_DETACHED | THR_DAEMON)
                    && ACE_BIT_DISABLED (iter.next ()->flags_, THR_JOINABLE))
                  {
                    this->thr_to_be_removed_.enqueue_tail (iter.next ());
                    ACE_SET_BITS (iter.next ()->thr_state_, ACE_THR_JOINING);
                  }
              }
            if (! this->thr_to_be_removed_.is_empty ())
              {
                ACE_Thread_Descriptor *td = 0;
                while (this->thr_to_be_removed_.dequeue_head (td) != -1)
                  this->remove_thr (td, 1);
              }
          }
        while (this->thr_list_.size () > 0)
          if (this->zero_cond_.wait (timeout) == -1)
            return -1;
      }
    else
        // Program is shutting down, no chance to wait on threads.
        // Therefore, we'll just remove threads from the list.
        this->remove_thr_all ();
#if !defined (ACE_HAS_VXTHREADS)
  ACE_Thread_Descriptor_Base* item = 0;
  while ((item = this->terminated_thr_list_.delete_head ()) != 0)
    {
      term_thr_list_copy.insert_tail (item);
    }
#endif /* ACE_HAS_VXTHREADS */
    // Release the guard, giving other threads a chance to run.
  }
#if !defined (ACE_HAS_VXTHREADS)
    // @@ VxWorks doesn't support thr_join (yet.)  We are working
    // on our implementation.   Chorus'es thr_join seems broken.
    ACE_Thread_Descriptor_Base *item = 0;
    while ((item = term_thr_list_copy.delete_head ()) != 0)
      {
        if (ACE_BIT_DISABLED (item->flags_, THR_DETACHED | THR_DAEMON)
            || ACE_BIT_ENABLED (item->flags_, THR_JOINABLE))
          // Detached handles shouldn't reached here.
          (void) ACE_Thread::join (item->thr_handle_);
        delete item;
      }
#endif /* !ACE_HAS_VXTHREADS */
#else
  ACE_UNUSED_ARG (timeout);
  ACE_UNUSED_ARG (abandon_detached_threads);
#endif /* ACE_HAS_THREADS */
  return 0;
}
int
ACE_Thread_Manager::apply_task (ACE_Task_Base *task,
                                ACE_THR_MEMBER_FUNC func,
                                int arg)
{
  ACE_TRACE ("ACE_Thread_Manager::apply_task");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_ASSERT (this->thr_to_be_removed_.is_empty ());
  int result = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    if (iter.next ()->task_ == task
        && (this->*func) (iter.next (), arg) == -1)
      result = -1;
  // Must remove threads after we have traversed the thr_list_ to
  // prevent clobber thr_list_'s integrity.
  if (! this->thr_to_be_removed_.is_empty ())
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      for (ACE_Thread_Descriptor *td = 0;
           this->thr_to_be_removed_.dequeue_head (td) != -1;
           )
        this->remove_thr (td, 1);
    }
  return result;
}
// Wait for all threads to exit a task.
int
ACE_Thread_Manager::wait_task (ACE_Task_Base *task)
{
  int copy_count = 0;
  ACE_Thread_Descriptor_Base *copy_table = 0;
  // We have to make sure that while we wait for these threads to
  // exit, we do not have the lock.  Therefore we make a copy of all
  // interesting entries and let go of the lock.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
#if !defined (ACE_HAS_VXTHREADS)
    ACE_NEW_RETURN (copy_table,
                    ACE_Thread_Descriptor_Base [this->thr_list_.size ()
                                                + this->terminated_thr_list_.size ()],
                    -1);
#else
    ACE_NEW_RETURN (copy_table,
                    ACE_Thread_Descriptor_Base [this->thr_list_.size ()],
                    -1);
#endif /* !ACE_HAS_VXTHREADS */
    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
         !iter.done ();
         iter.advance ())
      {
        // If threads are created as THR_DETACHED or THR_DAEMON, we
        // can't wait on them here.
        if (iter.next ()->task_ == task &&
            (ACE_BIT_DISABLED (iter.next ()->flags_,
                               THR_DETACHED | THR_DAEMON)
             || ACE_BIT_ENABLED (iter.next ()->flags_,
                                 THR_JOINABLE)))
          {
            ACE_SET_BITS (iter.next ()->thr_state_,
                          ACE_THR_JOINING);
            copy_table[copy_count++] = *iter.next ();
          }
      }
#if !defined (ACE_HAS_VXTHREADS)
    for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor_Base> titer (this->terminated_thr_list_);
         !titer.done ();
         titer.advance ())
      {
        // If threads are created as THR_DETACHED or THR_DAEMON, we can't help much here.
        if (titer.next ()->task_ == task)
          {
            ACE_Thread_Descriptor_Base *tdb = titer.advance_and_remove (false);
            copy_table[copy_count++] = *tdb;
            delete tdb;
          }
      }
#endif /* !ACE_HAS_VXTHREADS */
  }
  // Now to do the actual work
  int result = 0;
  for (int i = 0;
       i < copy_count && result != -1;
       i++)
    {
      if (ACE_Thread::join (copy_table[i].thr_handle_) == -1)
        result = -1;
    }
  delete [] copy_table;
  return result;
}
// Suspend a task
int
ACE_Thread_Manager::suspend_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::suspend_task");
  return this->apply_task (task,
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::suspend_thr));
}
// Resume a task.
int
ACE_Thread_Manager::resume_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::resume_task");
  return this->apply_task (task,
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::resume_thr));
}
// Kill a task.
int
ACE_Thread_Manager::kill_task (ACE_Task_Base *task, int /* signum */)
{
  ACE_TRACE ("ACE_Thread_Manager::kill_task");
  return this->apply_task (task,
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::kill_thr));
}
// Cancel a task.
int
ACE_Thread_Manager::cancel_task (ACE_Task_Base *task,
                                 int async_cancel)
{
  ACE_TRACE ("ACE_Thread_Manager::cancel_task");
  return this->apply_task (task,
                           ACE_THR_MEMBER_FUNC (&ACE_Thread_Manager::cancel_thr),
                           async_cancel);
}
// Locate the index in the table associated with <task> from the
// beginning of the table up to an index.  Must be called with the
// lock held.
ACE_Thread_Descriptor *
ACE_Thread_Manager::find_task (ACE_Task_Base *task, size_t slot)
{
  ACE_TRACE ("ACE_Thread_Manager::find_task");
  size_t i = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (i >= slot)
        break;
      if (task == iter.next ()->task_)
        return iter.next ();
      ++i;
    }
  return 0;
}
// Returns the number of ACE_Task in a group.
int
ACE_Thread_Manager::num_tasks_in_group (int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::num_tasks_in_group");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int tasks_count = 0;
  size_t i = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (iter.next ()->grp_id_ == grp_id
          && this->find_task (iter.next ()->task_, i) == 0
          && iter.next ()->task_ != 0)
        {
          ++tasks_count;
        }
      ++i;
    }
  return tasks_count;
}
// Returns the number of threads in an ACE_Task.
int
ACE_Thread_Manager::num_threads_in_task (ACE_Task_Base *task)
{
  ACE_TRACE ("ACE_Thread_Manager::num_threads_in_task");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int threads_count = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (iter.next ()->task_ == task)
        {
          ++threads_count;
        }
    }
  return threads_count;
}
// Returns in task_list a list of ACE_Tasks registered with ACE_Thread_Manager.
ssize_t
ACE_Thread_Manager::task_all_list (ACE_Task_Base *task_list[],
                                   size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::task_all_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  size_t task_list_count = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (task_list_count >= n)
        {
          break;
        }
      ACE_Task_Base *task_p = iter.next ()->task_;
      if (0 != task_p)
        {
          // This thread has a task pointer; see if it's already in the
          // list. Don't add duplicates.
          size_t i = 0;
          for (; i < task_list_count; ++i)
            {
              if (task_list[i] == task_p)
                {
                  break;
                }
            }
          if (i == task_list_count)        // No match - add this one
            {
              task_list[task_list_count++] = task_p;
            }
        }
    }
  return ACE_Utils::truncate_cast<ssize_t> (task_list_count);
}
// Returns in thread_list a list of all thread ids
ssize_t
ACE_Thread_Manager::thread_all_list (ACE_thread_t thread_list[],
                                     size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_all_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  size_t thread_count = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (thread_count >= n)
        {
          break;
        }
      thread_list[thread_count] = iter.next ()->thr_id_;
      ++thread_count;
    }
  return ACE_Utils::truncate_cast<ssize_t> (thread_count);
}
int
ACE_Thread_Manager::thr_state (ACE_thread_t id,
                               ACE_UINT32& state)
{
  ACE_TRACE ("ACE_Thread_Manager::thr_state");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int const self_check = ACE_OS::thr_equal (id, ACE_OS::thr_self ());
  // If we're checking the state of our thread, try to get the cached
  // value out of TSS to avoid lookup.
  if (self_check)
    {
      ACE_Thread_Descriptor *desc = ACE_LOG_MSG->thr_desc ();
      if (desc == 0)
        {
          return 0;               // Always return false.
        }
      state = desc->thr_state_;
    }
  else
    {
      // Not calling from self, have to look it up from the list.
      ACE_FIND (this->find_thread (id), ptr);
      if (ptr == 0)
        {
          return 0;
        }
      state = ptr->thr_state_;
    }
  return 1;
}
// Returns in task_list a list of ACE_Tasks in a group.
ssize_t
ACE_Thread_Manager::task_list (int grp_id,
                               ACE_Task_Base *task_list[],
                               size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::task_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_Task_Base **task_list_iterator = task_list;
  size_t task_list_count = 0;
  size_t i = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (task_list_count >= n)
        {
          break;
        }
      if (iter.next ()->grp_id_ == grp_id
          && this->find_task (iter.next ()->task_, i) == 0)
        {
          task_list_iterator[task_list_count] = iter.next ()->task_;
          ++task_list_count;
        }
      ++i;
    }
  return ACE_Utils::truncate_cast<ssize_t> (task_list_count);
}
// Returns in thread_list a list of thread ids in an ACE_Task.
ssize_t
ACE_Thread_Manager::thread_list (ACE_Task_Base *task,
                                 ACE_thread_t thread_list[],
                                 size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  size_t thread_count = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (thread_count >= n)
        {
          break;
        }
      if (iter.next ()->task_ == task)
        {
          thread_list[thread_count] = iter.next ()->thr_id_;
          ++thread_count;
        }
    }
  return ACE_Utils::truncate_cast<ssize_t> (thread_count);
}
// Returns in thread_list a list of thread handles in an ACE_Task.
ssize_t
ACE_Thread_Manager::hthread_list (ACE_Task_Base *task,
                                  ACE_hthread_t hthread_list[],
                                  size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::hthread_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  size_t hthread_count = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (hthread_count >= n)
        {
          break;
        }
      if (iter.next ()->task_ == task)
        {
          hthread_list[hthread_count] = iter.next ()->thr_handle_;
          ++hthread_count;
        }
    }
  return ACE_Utils::truncate_cast<ssize_t> (hthread_count);
}
ssize_t
ACE_Thread_Manager::thread_grp_list (int grp_id,
                                     ACE_thread_t thread_list[],
                                     size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::thread_grp_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  size_t thread_count = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (thread_count >= n)
        {
          break;
        }
      if (iter.next ()->grp_id_ == grp_id)
        {
          thread_list[thread_count] = iter.next ()->thr_id_;
          thread_count++;
        }
    }
  return ACE_Utils::truncate_cast<ssize_t> (thread_count);
}
// Returns in thread_list a list of thread handles in an ACE_Task.
ssize_t
ACE_Thread_Manager::hthread_grp_list (int grp_id,
                                      ACE_hthread_t hthread_list[],
                                      size_t n)
{
  ACE_TRACE ("ACE_Thread_Manager::hthread_grp_list");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  size_t hthread_count = 0;
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (hthread_count >= n)
        {
          break;
        }
      if (iter.next ()->grp_id_ == grp_id)
        {
          hthread_list[hthread_count] = iter.next ()->thr_handle_;
          hthread_count++;
        }
    }
  return ACE_Utils::truncate_cast<ssize_t> (hthread_count);
}
int
ACE_Thread_Manager::set_grp (ACE_Task_Base *task, int grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::set_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  for (ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor> iter (this->thr_list_);
       !iter.done ();
       iter.advance ())
    {
      if (iter.next ()->task_ == task)
        {
          iter.next ()->grp_id_ = grp_id;
        }
    }
  return 0;
}
int
ACE_Thread_Manager::get_grp (ACE_Task_Base *task, int &grp_id)
{
  ACE_TRACE ("ACE_Thread_Manager::get_grp");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  ACE_FIND (this->find_task (task), ptr);
  grp_id = ptr->grp_id_;
  return 0;
}
```
/** * @class ACE_Thread_Manager * * @brief Manages a pool of threads. * * This class allows operations on groups of threads atomically. * The default behavior of thread manager is to wait on * all threads under it's management when it gets destructed. * Therefore,
 remember to remove a thread from thread manager if * you don't want it to wait for the thread. There are also * functions to disable this default wait-on-exit behavior. * However, if your program depends on turning this off to run * correctly, you are probably
 doing something wrong. Rule of * thumb, use ACE_Thread to manage your daemon threads. * Notice that if there're threads which live beyond the scope of * main(), you are sure to have resource leaks in your program. * Remember to wait on threads before exiting
 your main program if that * could happen in your programs. */class ACE_Export ACE_Thread_Manager{public: friend class ACE_Thread_Control; // Allow ACE_THread_Exit to register the global TSS instance object. friend class ACE_Thread_Exit; friend class ACE_Thread_Descriptor;#if
 !defined (__GNUG__) typedef int (ACE_Thread_Manager::*ACE_THR_MEMBER_FUNC)(ACE_Thread_Descriptor *, int);#endif /* !__GNUG__ */ /// These are the various states a thread managed by the /// ACE_Thread_Manager can be in. enum { /// Uninitialized. ACE_THR_IDLE
 = 0x00000000, /// Created but not yet running. ACE_THR_SPAWNED = 0x00000001, /// Thread is active (naturally, we don't know if it's actually /// *running* because we aren't the scheduler...). ACE_THR_RUNNING = 0x00000002, /// Thread is suspended. ACE_THR_SUSPENDED
 = 0x00000004, /// Thread has been cancelled (which is an indiction that it needs to /// terminate...). ACE_THR_CANCELLED = 0x00000008, /// Thread has shutdown, but the slot in the thread manager hasn't /// been reclaimed yet. ACE_THR_TERMINATED = 0x00000010,
 /// Join operation has been invoked on the thread by thread manager. ACE_THR_JOINING = 0x10000000 }; /** * @brief Initialization and termination methods. * * Internally, ACE_Thread_Manager keeps a freelist for caching * resources it uses to keep track of managed
 threads (not the * threads themselves.) @a prealloc, @a lwm, @a inc, @hwm * determine the initial size, the low water mark, increment step, * and high water mark of the freelist. * * @sa ACE_Free_List */ ACE_Thread_Manager (size_t preaolloc = ACE_DEFAULT_THREAD_MANAGER_PREALLOC,
 size_t lwm = ACE_DEFAULT_THREAD_MANAGER_LWM, size_t inc = ACE_DEFAULT_THREAD_MANAGER_INC, size_t hwm = ACE_DEFAULT_THREAD_MANAGER_HWM); ~ACE_Thread_Manager (void);#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS) /// Get pointer to a process-wide ACE_Thread_Manager.
 static ACE_Thread_Manager *instance (void); /// Set pointer to a process-wide ACE_Thread_Manager and return /// existing pointer. static ACE_Thread_Manager *instance (ACE_Thread_Manager *); /// Delete the dynamically allocated Singleton static void close_singleton
 (void);#endif /* ! defined (ACE_THREAD_MANAGER_LACKS_STATICS) */ /// No-op. Currently unused. int open (size_t size = 0); /** * Release all resources. * By default, this method will wait until all threads exit. * However, when called from close_singleton(),
 most global resources * are destroyed and thus, close() does not try to wait; it simply cleans * up internal thread records (the thread descriptor list). */ int close (void); /** * Create a new thread, which executes @a func with argument @a arg. * * @param
 func The function that is called in the spawned thread. * * @param arg The value passed to each spawned thread's @a func. * * @param flags Flags to control attributes of the spawned threads. * @sa ACE_OS::thr_create() for descriptions of the * possible flags
 values and their interactions. * * @param t_id Pointer to a location to receive the spawned thread's * ID. If 0, the ID is not returned. * * @param t_handle Pointer to a location to receive the spawned thread's * thread handle. If 0, the handle is not returned.
 * * @param priority The priority at which the thread is spawned. * * @param grp_id The thread group that the spawned thread is * added to. If -1 is specified, a new thread group is * created for the spawned thread. * * @param stack Pointers to the base of
 a pre-allocated stack space * for the thread's stack. If 0, the platform allocates * stack space for the thread. If a stack is specified, * it is recommended that @a stack_size also be supplied * to specify the size of the stack. * Not all platforms support
 pre-allocated stacks. If * @a stack is specified for a platform which does not * allow pre-allocated stack space this parameter is * ignored. * * @param stack_size Indicate how large the thread's stack should be, in * bytes. If a pre-allocated stack pointer
 is passed in * @a stack, @a stack_size indicates the size of that * stack area. If no pre-allocated stack is passed, * the stack size specified is passed to the * operating system to request that it allocate a stack * of the specified size. * * @param thr_name
 Pointer to a name to assign to the spawned thread. * This is only meaningful for platforms that have a * capacity to name threads (e.g., VxWorks and some * varieties of Pthreads). This argument is ignored if * specified as 0 and on platforms that do not have
 the * capability to name threads. * * @retval -1 on failure; @c errno contains an error value. * @retval The group id of the spawned thread. */ int spawn (ACE_THR_FUNC func, void *arg = 0, long flags = THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, ACE_thread_t
 *t_id = 0, ACE_hthread_t *t_handle = 0, long priority = ACE_DEFAULT_THREAD_PRIORITY, int grp_id = -1, void *stack = 0, size_t stack_size = ACE_DEFAULT_THREAD_STACKSIZE, const char** thr_name = 0); /** * Spawn a specified number of threads, all of which execute
 @a func * with argument @a arg. * * @param n The number of threads to spawn. * * @param func The function that is called in the spawned thread. * * @param arg The value passed to each spawned thread's @a func. * * @param flags Flags to control attributes of
 the spawned threads. * @sa ACE_OS::thr_create() for descriptions of the * possible flags values and their interactions. * * @param priority The priority at which the threads are spawned. * * @param grp_id The thread group that the spawned threads are * added
 to. If -1 is specified, a new thread group is * created for the spawned threads. * * @param task The ACE_Task that the spawned threads are associated * with. If 0, the threads are not associated with an * ACE_Task. This argument is usually assigned by the
 * ACE_Task_Base::activate() method to associate the * spawned threads with the spawning ACE_Task object. * * @param thread_handles An array of @a n entries which will receive * the thread handles of the spawned threads. * * @param stack An array of @a n pointers
 to pre-allocated stack space * for each thread's stack. If specified as 0, the * platform allocates stack space for each thread. If * a stack is specified, it is recommended that a * @a stack_size element also be supplied that specifies * the size of the stack.
 * Not all platforms support pre-allocated stacks. If * @a stack is specified for a platform which does not * allow pre-allocated stack space this parameter is * ignored. * * @param stack_size An array of @a n values which indicate how large * each thread's
 stack should be, in bytes. * If pre-allocated stacks are passed in @a stacks, these * sizes are for those stacks. If no pre-allocated stacks * are passed, the stack sizes are specified to the * operating system to request that it allocate stacks * of the specified
 sizes. If an array entry is 0, the * platform defaults are used for the corresponding thread. * If a 0 array pointer is specified, platform defaults * are used for all thread stack sizes. * * @param thr_name An array of names to assign to the spawned threads.
 * This is only meaningful for platforms that have a * capacity to name threads (e.g., VxWorks and some * varieties of Pthreads). This argument is ignored if * specified as 0 and on platforms that do not have the * capability to name threads. * * ACE_Thread_Manager
 can manipulate threads in groups based on * @a grp_id or @a task using functions such as kill_grp() or * cancel_task(). * * @retval -1 on failure; @c errno contains an error value. * @retval The group id of the threads. */ int spawn_n (size_t n, ACE_THR_FUNC
 func, void *arg = 0, long flags = THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, long priority = ACE_DEFAULT_THREAD_PRIORITY, int grp_id = -1, ACE_Task_Base *task = 0, ACE_hthread_t thread_handles[] = 0, void *stack[] = 0, size_t stack_size[] = 0, const char*
 thr_name[] = 0); /** * Spawn a specified number of threads, all of which execute @a func * with argument @a arg. * * @param thread_ids An array to receive the thread IDs of successfully * spawned buffer. If 0, the thread IDs are not returned. * If specified,
 the array must be at least @a n entries. * * @param n The number of threads to spawn. * * @param func The function that is called in the spawned thread. * * @param arg The value passed to each spawned thread's @a func. * * @param flags Flags to control attributes
 of the spawned threads. * @sa ACE_OS::thr_create() for descriptions of the * possible flags values and their interactions. * * @param priority The priority at which the threads are spawned. * * @param grp_id The thread group that the spawned threads are *
 added to. If -1 is specified, a new thread group is * created for the spawned threads. * * @param stack An array of @a n pointers to pre-allocated stack space * for each thread's stack. If specified as 0, the * platform allocates stack space for each thread.
 If * a stack is specified, it is recommended that a * @a stack_size element also be supplied that specifies * the size of the stack. * Not all platforms support pre-allocated stacks. If * @a stack is specified for a platform which does not * allow pre-allocated
 stack space this parameter is * ignored. * * @param stack_size An array of @a n values which indicate how large * each thread's stack should be, in bytes. * If pre-allocated stacks are passed in @a stacks, these * sizes are for those stacks. If no pre-allocated
 stacks * are passed, the stack sizes are specified to the * operating system to request that it allocate stacks * of the specified sizes. If an array entry is 0, the * platform defaults are used for the corresponding thread. * If a 0 array pointer is specified,
 platform defaults * are used for all thread stack sizes. * * @param thread_handles An array of @a n entries which will receive * the thread handles of the spawned threads. * * @param task The ACE_Task that the spawned threads are associated * with. If 0, the
 threads are not associated with an * ACE_Task. This argument is usually assigned by the * ACE_Task_Base::activate() method to associate the * spawned threads with the spawning ACE_Task object. * * @param thr_name An array of names to assign to the spawned
 threads. * This is only meaningful for platforms that have a * capacity to name threads (e.g., VxWorks and some * varieties of Pthreads). This argument is ignored if * specified as 0 and on platforms that do not have the * capability to name threads. * * ACE_Thread_Manager
 can manipulate threads in groups based on * @a grp_id or @a task using functions such as kill_grp() or * cancel_task(). * * @retval -1 on failure; @c errno contains an error value. * @retval The group id of the threads. */ int spawn_n (ACE_thread_t thread_ids[],
 size_t n, ACE_THR_FUNC func, void *arg, long flags, long priority = ACE_DEFAULT_THREAD_PRIORITY, int grp_id = -1, void *stack[] = 0, size_t stack_size[] = 0, ACE_hthread_t thread_handles[] = 0, ACE_Task_Base *task = 0, const char* thr_name[] = 0); /** * Called
 to clean up when a thread exits. * * @param do_thread_exit If non-0 then ACE_Thread::exit is called to * exit the thread * @param status If ACE_Thread_Exit is called, this is passed as * the exit value of the thread. * Should _not_ be called by main thread.
 */ ACE_THR_FUNC_RETURN exit (ACE_THR_FUNC_RETURN status = 0, bool do_thread_exit = true); /** * Block until there are no more threads running in this thread * manager or @c timeout expires. * * @param timeout is treated as "absolute" time by default, but this
 * can be changed to "relative" time by setting the @c * use_absolute_time to false. * @param abandon_detached_threads If true, @c wait() will first * check thru its thread list for * threads with THR_DETACHED or * THR_DAEMON flags set and remove * these threads.
 Notice that * unlike other @c wait_*() methods, * by default, @c wait() does wait on * all thread spawned by this * thread manager no matter the detached * flags are set or not unless it is * called with @c * abandon_detached_threads flag set. * @param use_absolute_time
 If true then treat @c timeout as * absolute time, else relative time. * @return 0 on success * and -1 on failure. * * @note If this function is called while the @c * ACE_Object_Manager is shutting down (as a result of program * rundown via @c ACE::fini()),
 it will not wait for any threads to * complete. If you must wait for threads spawned by this thread * manager to complete and you are in a ACE rundown situation (such * as your object is being destroyed by the @c ACE_Object_Manager) * you can use @c wait_grp()
 instead. */ int wait (const ACE_Time_Value *timeout = 0, bool abandon_detached_threads = false, bool use_absolute_time = true); /// Join a thread specified by @a tid. Do not wait on a detached thread. int join (ACE_thread_t tid, ACE_THR_FUNC_RETURN *status
 = 0); /** * Block until there are no more threads running in a group. * Returns 0 on success and -1 on failure. Notice that wait_grp * will not wait on detached threads. */ int wait_grp (int grp_id); /** * Return the "real" handle to the calling thread, caching
 it if * necessary in TSS to speed up subsequent lookups. This is * necessary since on some platforms (e.g., Windows) we can't get this * handle via direct method calls. Notice that you should *not* * close the handle passed back from this method. It is used
 * internally by Thread Manager. On the other hand, you *have to* * use this internal thread handle when working on Thread_Manager. * Return -1 if fail. */ int thr_self (ACE_hthread_t &); /** * Return the unique ID of the calling thread. * Same as calling ACE_Thread::self().
 */ ACE_thread_t thr_self (void); /** * Returns a pointer to the current ACE_Task_Base we're executing * in if this thread is indeed running in an ACE_Task_Base, else * return 0. */ ACE_Task_Base *task (void); /** * @name Suspend and resume methods * * Suspend/resume
 is not supported on all platforms. For example, Pthreads * does not support these functions. */ //@{ /// Suspend all threads int suspend_all (void); /// Suspend a single thread. int suspend (ACE_thread_t); /// Suspend a group of threads. int suspend_grp (int
 grp_id); /** * True if @a t_id is inactive (i.e., suspended), else false. Always * return false if @a t_id is not managed by the Thread_Manager. */ int testsuspend (ACE_thread_t t_id); /// Resume all stopped threads int resume_all (void); /// Resume a single
 thread. int resume (ACE_thread_t); /// Resume a group of threads. int resume_grp (int grp_id); /** * True if @a t_id is active (i.e., resumed), else false. Always * return false if @a t_id is not managed by the Thread_Manager. */ int testresume (ACE_thread_t
 t_id); //@} // = Send signals to one or more threads without blocking. /** * Send @a signum to all stopped threads. Not supported on platforms * that do not have advanced signal support, such as Win32. */ int kill_all (int signum); /** * Send the @a signum
 to a single thread. Not supported on platforms * that do not have advanced signal support, such as Win32. */ int kill (ACE_thread_t, int signum); /** * Send @a signum to a group of threads, not supported on platforms * that do not have advanced signal support,
 such as Win32. */ int kill_grp (int grp_id, int signum); // = Cancel methods, which provides a cooperative thread-termination mechanism (will not block). /** * Cancel's all the threads. */ int cancel_all (int async_cancel = 0); /** * Cancel a single thread.
 */ int cancel (ACE_thread_t, int async_cancel = 0); /** * Cancel a group of threads. */ int cancel_grp (int grp_id, int async_cancel = 0); /** * True if @a t_id is cancelled, else false. Always return false if * @a t_id is not managed by the Thread_Manager.
 */ int testcancel (ACE_thread_t t_id); /** * True if @a t_id has terminated (i.e., is no longer running), * but the slot in the thread manager hasn't been reclaimed yet, * else false. Always return false if @a t_id is not managed by the * Thread_Manager. */
 int testterminate (ACE_thread_t t_id); /// Set group ids for a particular thread id. int set_grp (ACE_thread_t, int grp_id); /// Get group ids for a particular thread id. int get_grp (ACE_thread_t, int &grp_id); /** * @name Task-related operations */ //@{
 /** * Block until there are no more threads running in a specified task. * This method will not wait for either detached or daemon threads; * the threads must have been spawned with the @c THR_JOINABLE flag. * Upon successful completion, the threads have been
 joined, so further * attempts to join with any of the waited-for threads will fail. * * @param task The ACE_Task_Base object whose threads are to waited for. * * @retval 0 Success. * @retval -1 Failure (consult errno for further information). */ int wait_task
 (ACE_Task_Base *task); /** * Suspend all threads in an ACE_Task. */ int suspend_task (ACE_Task_Base *task); /** * Resume all threads in an ACE_Task. */ int resume_task (ACE_Task_Base *task); /** * Send a signal @a signum to all threads in an ACE_Task. */ int
 kill_task (ACE_Task_Base *task, int signum); /** * Cancel all threads in an ACE_Task. If <async_cancel> is non-0, * then asynchronously cancel these threads if the OS platform * supports cancellation. Otherwise, perform a "cooperative" * cancellation. */ int
 cancel_task (ACE_Task_Base *task, int async_cancel = 0); //@} // = Collect thread handles in the thread manager. Notice that // the collected information is just a snapshot. /// Check if the thread is managed by the thread manager. Return true if /// the thread
 is found, false otherwise. int hthread_within (ACE_hthread_t handle); int thread_within (ACE_thread_t tid); /// Returns the number of ACE_Task_Base in a group. int num_tasks_in_group (int grp_id); /// Returns the number of threads in an ACE_Task_Base. int
 num_threads_in_task (ACE_Task_Base *task); /** * Returns a list of ACE_Task_Base pointers corresponding to the tasks * that have active threads in a specified thread group. * * @param grp_id The thread group ID to obtain task pointers for. * * @param task_list
 is a pointer to an array to receive the list of pointers. * The caller is responsible for supplying an array with at * least @arg n entries. * * @param n The maximum number of ACE_Task_Base pointers to write * in @arg task_list. * * @retval If successful,
 the number of pointers returned, which will be * no greater than @arg n. Returns -1 on error. * * @note This method has no way to indicate if there are more than * @arg n ACE_Task_Base pointers available. Therefore, it may be * wise to guess a larger value
 of @arg n than one thinks in cases * where the exact number of tasks is not known. * * @sa num_tasks_in_group(), task_all_list() */ ssize_t task_list (int grp_id, ACE_Task_Base *task_list[], size_t n); /** * Returns in @a thread_list a list of up to @a n thread
 ids in an * ACE_Task_Base. The caller must allocate the memory for * @a thread_list. In case of an error, -1 is returned. If no * requested values are found, 0 is returned, otherwise correct * number of retrieved values are returned. */ ssize_t thread_list
 (ACE_Task_Base *task, ACE_thread_t thread_list[], size_t n); /** * Returns in @a hthread_list a list of up to @a n thread handles in * an ACE_Task_Base. The caller must allocate memory for * @a hthread_list. In case of an error, -1 is returned. If no * requested
 values are found, 0 is returned, otherwise correct * number of retrieved values are returned. */ ssize_t hthread_list (ACE_Task_Base *task, ACE_hthread_t hthread_list[], size_t n); /** * Returns in @a thread_list a list of up to @a n thread ids in a * group
 @a grp_id. The caller must allocate the memory for * @a thread_list. In case of an error, -1 is returned. If no * requested values are found, 0 is returned, otherwise correct * number of retrieved values are returned. */ ssize_t thread_grp_list (int grp_id,
 ACE_thread_t thread_list[], size_t n); /** * Returns in @a hthread_list a list of up to @a n thread handles in * a group @a grp_id. The caller must allocate memory for * @a hthread_list. */ ssize_t hthread_grp_list (int grp_id, ACE_hthread_t hthread_list[],
 size_t n); /** * Returns a list of ACE_Task_Base pointers corresponding to the tasks * that have active threads managed by this instance. * * @param task_list is a pointer to an array to receive the list of pointers. * The caller is responsible for supplying
 an array with at * least @arg n entries. * * @param n The maximum number of ACE_Task_Base pointers to write * in @arg task_list. * * @retval If successful, the number of pointers returned, which will be * no greater than @arg n. Returns -1 on error. * * @note
 This method has no way to indicate if there are more than * @arg n ACE_Task_Base pointers available. Therefore, it may be * wise to guess a larger value of @arg n than one thinks in cases * where the exact number of tasks is not known. * * @sa count_threads()
 */ ssize_t task_all_list (ACE_Task_Base *task_list[], size_t n); /** * Returns in @a thread_list a list of up to @a n thread ids. The * caller must allocate the memory for @a thread_list. In case of an * error, -1 is returned. If no requested values are found,
 0 is * returned, otherwise correct number of retrieved values are * returned. */ ssize_t thread_all_list (ACE_thread_t thread_list[], size_t n); /// Set group ids for a particular task. int set_grp (ACE_Task_Base *task, int grp_id); /// Get group ids for a
 particular task. int get_grp (ACE_Task_Base *task, int &grp_id); /// Return a count of the current number of threads active in the /// <Thread_Manager>. size_t count_threads (void) const; /// Get the state of the thread. Returns false if the thread is not
 /// managed by this thread manager. int thr_state (ACE_thread_t id, ACE_UINT32& state); /** * Register an At_Thread_Exit hook and the ownership is acquire by * Thread_Descriptor, this is the usual case when the AT is dynamically * allocated. */ int at_exit
 (ACE_At_Thread_Exit* cleanup); /// Register an At_Thread_Exit hook and the ownership is retained for the /// caller. Normally used when the at_exit hook is created in stack. int at_exit (ACE_At_Thread_Exit& cleanup); /** * ***** * @deprecated This function
 is deprecated. Please use the previous two * at_exit method. Notice that you should avoid mixing this method * with the previous two at_exit methods. ***** * * Register an object (or array) for cleanup at * thread termination. "cleanup_hook" points to a (global,
 or * static member) function that is called for the object or array * when it to be destroyed. It may perform any necessary cleanup * specific for that object or its class. "param" is passed as the * second parameter to the "cleanup_hook" function; the first
 * parameter is the object (or array) to be destroyed. * "cleanup_hook", for example, may delete the object (or array). * If <cleanup_hook> == 0, the <object> will _NOT_ get cleanup at * thread exit. You can use this to cancel the previously added * at_exit.
 */ int at_exit (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param); /// Access function to determine whether the Thread_Manager will /// wait for its thread to exit or not when being closing down. void wait_on_exit (int dowait); int wait_on_exit (void);
 /// Dump the state of an object. void dump (void); /// Declare the dynamic allocation hooks. ACE_ALLOC_HOOK_DECLARE;protected: // = Accessors for ACE_Thread_Descriptors. /** * Get a pointer to the calling thread's own thread_descriptor. * This must be called
 from a spawn thread. This function will * fetch the info from TSS. */ ACE_Thread_Descriptor *thread_desc_self (void); /// Return a pointer to the thread's Thread_Descriptor, /// 0 if fail. ACE_Thread_Descriptor *thread_descriptor (ACE_thread_t); /// Return
 a pointer to the thread's Thread_Descriptor, /// 0 if fail. ACE_Thread_Descriptor *hthread_descriptor (ACE_hthread_t); /// Create a new thread (must be called with locks held). int spawn_i (ACE_THR_FUNC func, void *arg, long flags, ACE_thread_t * = 0, ACE_hthread_t
 *t_handle = 0, long priority = ACE_DEFAULT_THREAD_PRIORITY, int grp_id = -1, void *stack = 0, size_t stack_size = 0, ACE_Task_Base *task = 0, const char** thr_name = 0); /// Run the registered hooks when the thread exits. void run_thread_exit_hooks (int i);
 /// Locate the index of the table slot occupied by <t_id>. Returns /// -1 if <t_id> is not in the table doesn't contain <t_id>. ACE_Thread_Descriptor *find_thread (ACE_thread_t t_id); /// Locate the index of the table slot occupied by <h_id>. Returns /// -1
 if <h_id> is not in the table doesn't contain <h_id>. ACE_Thread_Descriptor *find_hthread (ACE_hthread_t h_id); /** * Locate the thread descriptor address of the list occupied by * @a task. Returns 0 if @a task is not in the table doesn't contain * @a task.
 */ ACE_Thread_Descriptor *find_task (ACE_Task_Base *task, size_t slot = 0); /// Insert a thread in the table (checks for duplicates). int insert_thr (ACE_thread_t t_id, ACE_hthread_t, int grp_id = -1, long flags = 0); /// Append a thread in the table (adds
 at the end, growing the table /// if necessary). int append_thr (ACE_thread_t t_id, ACE_hthread_t, ACE_UINT32, int grp_id, ACE_Task_Base *task = 0, long flags = 0, ACE_Thread_Descriptor *td = 0); /// Remove thread from the table. void remove_thr (ACE_Thread_Descriptor
 *td, int close_handler); /// Remove all threads from the table. void remove_thr_all (void); // = The following four methods implement a simple scheme for // operating on a collection of threads atomically. /** * Efficiently check whether @a thread is in a
 particular @a state. * This call updates the TSS cache if possible to speed up * subsequent searches. */ int check_state (ACE_UINT32 state, ACE_thread_t thread, int enable = 1); /// Apply @a func to all members of the table that match the @a task int apply_task
 (ACE_Task_Base *task, ACE_THR_MEMBER_FUNC func, int = 0); /// Apply @a func to all members of the table that match the @a grp_id. int apply_grp (int grp_id, ACE_THR_MEMBER_FUNC func, int arg = 0); /// Apply @a func to all members of the table. int apply_all
 (ACE_THR_MEMBER_FUNC, int = 0); /// Join the thread described in @a td. int join_thr (ACE_Thread_Descriptor *td, int = 0); /// Resume the thread described in @a td. int resume_thr (ACE_Thread_Descriptor *td, int = 0); /// Suspend the thread described in @a
 td. int suspend_thr (ACE_Thread_Descriptor *td, int = 0); /// Send signal @a signum to the thread described in @a td. int kill_thr (ACE_Thread_Descriptor *td, int signum); /// Set the cancellation flag for the thread described in @a td. int cancel_thr (ACE_Thread_Descriptor
 *td, int async_cancel = 0); /// Register a thread as terminated and put it into the <terminated_thr_list_>. int register_as_terminated (ACE_Thread_Descriptor *td); /// Setting the static ACE_TSS_TYPE (ACE_Thread_Exit) *thr_exit_ pointer. static int set_thr_exit
 (ACE_TSS_TYPE (ACE_Thread_Exit) *ptr); /** * Keeping a list of thread descriptors within the thread manager. * Double-linked list enables us to cache the entries in TSS * and adding/removing thread descriptor entries without * affecting other thread's descriptor
 entries. */ ACE_Double_Linked_List<ACE_Thread_Descriptor> thr_list_;#if !defined (ACE_HAS_VXTHREADS) /// Collect terminated but not yet joined thread entries. ACE_Double_Linked_List<ACE_Thread_Descriptor_Base> terminated_thr_list_;#endif /* !ACE_HAS_VXTHREADS
 */ /// Collect pointers to thread descriptors of threads to be removed later. ACE_Unbounded_Queue<ACE_Thread_Descriptor*> thr_to_be_removed_; /// Keeps track of the next group id to assign. int grp_id_; /// Set if we want the Thread_Manager to wait on all
 threads before /// being closed, reset otherwise. int automatic_wait_; // = ACE_Thread_Mutex and condition variable for synchronizing termination.#if defined (ACE_HAS_THREADS) /// Serialize access to the <zero_cond_>. ACE_Thread_Mutex lock_; /// Keep track
 of when there are no more threads. ACE_Condition_Thread_Mutex zero_cond_;#endif /* ACE_HAS_THREADS */ ACE_Locked_Free_List<ACE_Thread_Descriptor, ACE_SYNCH_MUTEX> thread_desc_freelist_;private:#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS) /// Pointer to
 a process-wide ACE_Thread_Manager. static ACE_Thread_Manager *thr_mgr_; /// Must delete the thr_mgr_ if true. static bool delete_thr_mgr_; /// Global ACE_TSS (ACE_Thread_Exit) object ptr. static ACE_TSS_TYPE (ACE_Thread_Exit) *thr_exit_;#endif /* ! defined
 (ACE_THREAD_MANAGER_LACKS_STATICS) */};
