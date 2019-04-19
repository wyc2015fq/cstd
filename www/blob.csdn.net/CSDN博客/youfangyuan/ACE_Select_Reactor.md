# ACE_Select_Reactor - youfangyuan - CSDN博客
2012年04月17日 00:18:26[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1245标签：[reference																[iterator																[binding																[token																[descriptor																[initialization](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=descriptor&t=blog)](https://so.csdn.net/so/search/s.do?q=token&t=blog)](https://so.csdn.net/so/search/s.do?q=binding&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)
个人分类：[ACE](https://blog.csdn.net/youfangyuan/article/category/1125933)
```cpp
template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler
  (ACE_Event_Handler *handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->register_handler_i (handler->get_handle (), handler, mask);
}
```
```cpp
template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler_i
  (ACE_HANDLE handle,
   ACE_Event_Handler *event_handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler_i");
  // Insert the <handle, event_handle> tuple into the Handler
  // Repository.
  return this->handler_rep_.bind (handle, event_handler, mask);
}
```
```cpp
class ACE_Select_Reactor_Impl
{
...
  /// Table that maps <ACE_HANDLEs> to <ACE_Event_Handler *>'s.
  ACE_Select_Reactor_Handler_Repository handler_rep_;
...
};
```
```cpp
class ACE_Export ACE_Select_Reactor_Handler_Repository
{
public:
  friend class ACE_Select_Reactor_Handler_Repository_Iterator;
  typedef ACE_HANDLE          key_type;
  typedef ACE_Event_Handler * value_type;
  // = The mapping from <HANDLES> to <Event_Handlers>.
#ifdef ACE_WIN32
  /**
   * The NT version implements this via a hash map
   * @c ACE_Event_Handler*.  Since NT implements @c ACE_HANDLE
   * as a void * we can't directly index into this array.  Therefore,
   * we must explicitly map @c ACE_HANDLE to @c ACE_Event_Handler.
   */
  typedef ACE_Hash_Map_Manager_Ex<key_type,
                                  value_type,
                                  ACE_Hash<key_type>,
                                  std::equal_to<key_type>,
                                  ACE_Null_Mutex> map_type;
  typedef map_type::size_type max_handlep1_type;
#else
  /**
   * The UNIX version implements this via a dynamically allocated
   * array of @c ACE_Event_Handler* that is indexed directly using
   * the @c ACE_HANDLE value.
   */
  typedef ACE_Array_Base<value_type> map_type;
  typedef ACE_HANDLE max_handlep1_type;
#endif  /* ACE_WIN32 */
  typedef map_type::size_type size_type;
  // = Initialization and termination methods.
  /// Default "do-nothing" constructor.
  ACE_Select_Reactor_Handler_Repository (ACE_Select_Reactor_Impl &);
  /// Initialize a repository of the appropriate @a size.
  /**
   * On Unix platforms, the size parameter should be as large as the
   * maximum number of file descriptors allowed for a given process.
   * This is necessary since a file descriptor is used to directly
   * index the array of event handlers maintained by the Reactor's
   * handler repository.  Direct indexing is used for efficiency
   * reasons.
   */
  int open (size_type size);
  /// Close down the repository.
  int close (void);
  // = Search structure operations.
  /**
   * Return the @c ACE_Event_Handler* associated with @c ACE_HANDLE.
   */
  ACE_Event_Handler * find (ACE_HANDLE handle);
  /// Bind the ACE_Event_Handler * to the ACE_HANDLE with the
  /// appropriate ACE_Reactor_Mask settings.
  int bind (ACE_HANDLE,
            ACE_Event_Handler *,
            ACE_Reactor_Mask);
  /// Remove the binding of ACE_HANDLE in accordance with the @a mask.
  int unbind (ACE_HANDLE,
              ACE_Reactor_Mask mask);
  /// Remove all the <ACE_HANDLE, ACE_Event_Handler> tuples.
  int unbind_all (void);
  // = Sanity checking.
  // Check the @a handle to make sure it's a valid @c ACE_HANDLE that
  // is within the range of legal handles (i.e., >= 0 && < max_size_).
  bool invalid_handle (ACE_HANDLE handle);
  // Check the @a handle to make sure it's a valid @c ACE_HANDLE that
  // within the range of currently registered handles (i.e., >= 0 && <
  // @c max_handlep1_).
  bool handle_in_range (ACE_HANDLE handle);
  // = Accessors.
  /// Returns the current table size.
  size_type size (void) const;
  /// Maximum ACE_HANDLE value, plus 1.
  max_handlep1_type max_handlep1 (void) const;
  /// Dump the state of an object.
  void dump (void) const;
  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
private:
  /// Remove the binding of @a handle corresponding to position @a pos
  /// in accordance with the @a mask.
  int unbind (ACE_HANDLE handle,
              map_type::iterator pos,
              ACE_Reactor_Mask mask);
  /**
   * @return @c iterator corresponding @c ACE_Event_Handler*
   *         associated with @c ACE_HANDLE.
   */
  map_type::iterator find_eh (ACE_HANDLE handle);
private:
  /// Reference to our @c Select_Reactor.
  ACE_Select_Reactor_Impl &select_reactor_;
#ifndef ACE_WIN32
  /// The highest currently active handle, plus 1 (ranges between 0 and
  /// @c max_size_.
  max_handlep1_type max_handlep1_;
#endif  /* !ACE_WIN32 */
  /// Underlying table of event handlers.
  map_type event_handlers_;
};
```
```cpp
// Bind the <ACE_Event_Handler *> to the <ACE_HANDLE>.
int
ACE_Select_Reactor_Handler_Repository::bind (ACE_HANDLE handle,
                                             ACE_Event_Handler *event_handler,
                                             ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::bind");
  if (event_handler == 0)
    return -1;
  if (handle == ACE_INVALID_HANDLE)
    handle = event_handler->get_handle ();
  if (this->invalid_handle (handle))
    return -1;
  // Is this handle already in the Reactor?
  bool existing_handle = false;
#if defined (ACE_WIN32)
  map_type::ENTRY * entry = 0;
  int const result =
    this->event_handlers_.bind (handle, event_handler, entry);
  if (result == -1)
    {
      return -1;
    }
  else if (result == 1)  // Entry already exists.
    {
      // Cannot use a different handler for an existing handle.
      if (event_handler != entry->item ())
        {
          return -1;
        }
      else
        {
          // Remember that this handle is already registered in the
          // Reactor.
          existing_handle = true;
        }
    }
#else
  // Check if this handle is already registered.
  ACE_Event_Handler * const current_handler =
    this->event_handlers_[handle];
  if (current_handler)
    {
      // Cannot use a different handler for an existing handle.
      if (current_handler != event_handler)
        return -1;
      // Remember that this handle is already registered in the
      // Reactor.
      existing_handle = true;
    }
  this->event_handlers_[handle] = event_handler;
  if (this->max_handlep1_ < handle + 1)
    this->max_handlep1_ = handle + 1;
#endif /* ACE_WIN32 */
  if (this->select_reactor_.is_suspended_i (handle))
    {
      this->select_reactor_.bit_ops (handle,
                                     mask,
                                     this->select_reactor_.suspend_set_,
                                     ACE_Reactor::ADD_MASK);
    }
  else
    {
      this->select_reactor_.bit_ops (handle,
                                     mask,
                                     this->select_reactor_.wait_set_,
                                     ACE_Reactor::ADD_MASK);
      // Note the fact that we've changed the state of the <wait_set_>,
      // which is used by the dispatching loop to determine whether it can
      // keep going or if it needs to reconsult select().
      // this->select_reactor_.state_changed_ = 1;
    }
  // If new entry, call add_reference() if needed.
  if (!existing_handle)
    event_handler->add_reference ();
  return 0;
}
// Remove the binding of <ACE_HANDLE>.
int
ACE_Select_Reactor_Handler_Repository::unbind (
  ACE_HANDLE handle,
  map_type::iterator pos,
  ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::unbind");
  // Retrieve event handler before unbinding it from the map.  The
  // iterator pointing to it will no longer be valid once the handler
  // is unbound.
  ACE_Event_Handler * const event_handler =
    (pos == this->event_handlers_.end ()
     ? 0
     : ACE_SELECT_REACTOR_EVENT_HANDLER (pos));
  // Clear out the <mask> bits in the Select_Reactor's wait_set.
  this->select_reactor_.bit_ops (handle,
                                 mask,
                                 this->select_reactor_.wait_set_,
                                 ACE_Reactor::CLR_MASK);
  // And suspend_set.
  this->select_reactor_.bit_ops (handle,
                                 mask,
                                 this->select_reactor_.suspend_set_,
                                 ACE_Reactor::CLR_MASK);
  // Note the fact that we've changed the state of the <wait_set_>,
  // which is used by the dispatching loop to determine whether it can
  // keep going or if it needs to reconsult select().
  // this->select_reactor_.state_changed_ = 1;
  // If there are no longer any outstanding events on this <handle>
  // then we can totally shut down the Event_Handler.
  bool const has_any_wait_mask =
    (this->select_reactor_.wait_set_.rd_mask_.is_set (handle)
     || this->select_reactor_.wait_set_.wr_mask_.is_set (handle)
     || this->select_reactor_.wait_set_.ex_mask_.is_set (handle));
  bool const has_any_suspend_mask =
    (this->select_reactor_.suspend_set_.rd_mask_.is_set (handle)
     || this->select_reactor_.suspend_set_.wr_mask_.is_set (handle)
     || this->select_reactor_.suspend_set_.ex_mask_.is_set (handle));
  bool complete_removal = false;
  if (!has_any_wait_mask && !has_any_suspend_mask)
    {
#if defined (ACE_WIN32)
      if (event_handler != 0 && this->event_handlers_.unbind (pos) == -1)
        return -1;  // Should not happen!
#else
      this->event_handlers_[handle] = 0;
      if (this->max_handlep1_ == handle + 1)
        {
          // We've deleted the last entry, so we need to figure out
          // the last valid place in the array that is worth looking
          // at.
          ACE_HANDLE const wait_rd_max =
            this->select_reactor_.wait_set_.rd_mask_.max_set ();
          ACE_HANDLE const wait_wr_max =
            this->select_reactor_.wait_set_.wr_mask_.max_set ();
          ACE_HANDLE const wait_ex_max =
            this->select_reactor_.wait_set_.ex_mask_.max_set ();
          ACE_HANDLE const suspend_rd_max =
            this->select_reactor_.suspend_set_.rd_mask_.max_set ();
          ACE_HANDLE const suspend_wr_max =
            this->select_reactor_.suspend_set_.wr_mask_.max_set ();
          ACE_HANDLE const suspend_ex_max =
            this->select_reactor_.suspend_set_.ex_mask_.max_set ();
          // Compute the maximum of six values.
          this->max_handlep1_ = wait_rd_max;
          if (this->max_handlep1_ < wait_wr_max)
            this->max_handlep1_ = wait_wr_max;
          if (this->max_handlep1_ < wait_ex_max)
            this->max_handlep1_ = wait_ex_max;
          if (this->max_handlep1_ < suspend_rd_max)
            this->max_handlep1_ = suspend_rd_max;
          if (this->max_handlep1_ < suspend_wr_max)
            this->max_handlep1_ = suspend_wr_max;
          if (this->max_handlep1_ < suspend_ex_max)
            this->max_handlep1_ = suspend_ex_max;
          ++this->max_handlep1_;
        }
#endif /* ACE_WIN32 */
      // The handle has been completely removed.
      complete_removal = true;
    }
  if (event_handler == 0)
    return -1;
  bool const requires_reference_counting =
    event_handler->reference_counting_policy ().value () ==
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED;
  // Close down the <Event_Handler> unless we've been instructed not
  // to.
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0)
    (void) event_handler->handle_close (handle, mask);
  // Call remove_reference() if the removal is complete and reference
  // counting is needed.
  if (complete_removal && requires_reference_counting)
    {
      (void) event_handler->remove_reference ();
    }
  return 0;
}
```
