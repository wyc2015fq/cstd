
#include "wstd/logging.hpp"

#include <signal.h>
#include <csignal>

#include "signal_handler.h"

namespace
{

}  // namespace

namespace caffe
{

  SignalHandler::~SignalHandler()
  {
    UnhookHandler();
  }

  SolverAction::Enum SignalHandler::CheckForSignals() const
  {
    if (GotSIGHUP()) {
      return SIGHUP_action_;
    }
    if (GotSIGINT()) {
      return SIGINT_action_;
    }
    return SolverAction::NONE;
  }

// Return the function that the solver can use to find out if a snapshot or
// early exit is being requested.
  ActionCallback SignalHandler::GetActionFunction()
  {
    return std::bind(&SignalHandler::CheckForSignals, this);
  }

}  // namespace caffe
