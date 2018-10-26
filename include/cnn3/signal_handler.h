#ifndef INCLUDE_CAFFE_UTIL_SIGNAL_HANDLER_H_
#define INCLUDE_CAFFE_UTIL_SIGNAL_HANDLER_H_

#include "proto.h"
#include <signal.h>
#include <csignal>


static volatile sig_atomic_t got_sigint = false;
static volatile sig_atomic_t got_sighup = false;
static bool already_hooked_up = false;

void handle_signal(int signal)
{
  switch (signal) {
    //case SIGHUP://there is no SIGHUP in windows
    //  got_sighup = true;
    //  break;
  case SIGBREAK:
    got_sighup = true;
    break;
  case SIGINT:
    got_sigint = true;
    break;
  }
}

void HookupHandler()
{
  if (already_hooked_up) {
    LOG(FATAL) << "Tried to hookup signal handlers more than once.";
  }
  already_hooked_up = true;
  //struct sigaction sa;
  //// Setup the handler
  //sa.sa_handler = &handle_signal;
  //// Restart the system call, if at all possible
  //sa.sa_flags = SA_RESTART;
  //// Block every signal during the handler
  //sigfillset(&sa.sa_mask);
  //// Intercept SIGHUP and SIGINT
  //if (sigaction(SIGHUP, &sa, NULL) == -1) {
  //  LOG(FATAL) << "Cannot install SIGHUP handler.";
  //}
  //if (sigaction(SIGINT, &sa, NULL) == -1) {
  //  LOG(FATAL) << "Cannot install SIGINT handler.";
  //}
  if (signal(SIGBREAK, handle_signal) == SIG_ERR) {
    LOG(FATAL) << "Cannot install SIGBREAK handler.";
  }
  if (signal(SIGINT, handle_signal) == SIG_ERR) {
    LOG(FATAL) << "Cannot install SIGINT handler.";
  }
}

// Set the signal handlers to the default.
void UnhookHandler()
{
  if (already_hooked_up) {
    //struct sigaction sa;
    //// Setup the sighub handler
    //sa.sa_handler = SIG_DFL;
    //// Restart the system call, if at all possible
    //sa.sa_flags = SA_RESTART;
    //// Block every signal during the handler
    //sigfillset(&sa.sa_mask);
    //// Intercept SIGHUP and SIGINT
    //if (sigaction(SIGHUP, &sa, NULL) == -1) {
    //  LOG(FATAL) << "Cannot uninstall SIGHUP handler.";
    //}
    //if (sigaction(SIGINT, &sa, NULL) == -1) {
    //  LOG(FATAL) << "Cannot uninstall SIGINT handler.";
    //}
    if (signal(SIGBREAK, SIG_DFL) == SIG_ERR) {
      LOG(FATAL) << "Cannot uninstall SIGHUP handler.";
    }
    if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
      LOG(FATAL) << "Cannot uninstall SIGINT handler.";
    }
    already_hooked_up = false;
  }
}

// Return true iff a SIGINT has been received since the last time this
// function was called.
bool GotSIGINT()
{
  bool result = got_sigint != 0;
  got_sigint = false;
  return result;
}

// Return true iff a SIGHUP has been received since the last time this
// function was called.
bool GotSIGHUP()
{
  bool result = got_sighup != 0;
  got_sighup = false;
  return result;
}

class SignalHandler
{
public:
  // Contructor. Specify what action to take when a signal is received.
  SignalHandler(SolverAction SIGINT_action, SolverAction SIGHUP_action) :
    SIGINT_action_(SIGINT_action),
    SIGHUP_action_(SIGHUP_action)
  {
    HookupHandler();
  }
  ~SignalHandler()
  {
    UnhookHandler();
  }
  ActionCallback GetActionFunction()
  {
    return std::bind(&SignalHandler::CheckForSignals, this);
  }
private:
  SolverAction CheckForSignals() const
  {
    if (GotSIGHUP()) {
      return SIGHUP_action_;
    }
    if (GotSIGINT()) {
      return SIGINT_action_;
    }
    return SolverAction::NONE;
  }
  SolverAction::Enum SIGINT_action_;
  SolverAction::Enum SIGHUP_action_;
};


#endif  // INCLUDE_CAFFE_UTIL_SIGNAL_HANDLER_H_
