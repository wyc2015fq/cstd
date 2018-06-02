
#ifndef vtkCommand_h
#define vtkCommand_h

// Define all types of events here.
// Using this macro makes it possible to avoid mismatches between the event
// enums and their string counterparts.
#define vtkAllEventsMacro() \
    _vtk_add_event(AnyEvent)\
    _vtk_add_event(DeleteEvent)\
    _vtk_add_event(StartEvent)\
    _vtk_add_event(EndEvent)\
    _vtk_add_event(RenderEvent)\
    _vtk_add_event(ProgressEvent)\
    _vtk_add_event(PickEvent)\
    _vtk_add_event(StartPickEvent)\
    _vtk_add_event(EndPickEvent)\
    _vtk_add_event(AbortCheckEvent)\
    _vtk_add_event(ExitEvent)\
    _vtk_add_event(LeftButtonPressEvent)\
    _vtk_add_event(LeftButtonReleaseEvent)\
    _vtk_add_event(MiddleButtonPressEvent)\
    _vtk_add_event(MiddleButtonReleaseEvent)\
    _vtk_add_event(RightButtonPressEvent)\
    _vtk_add_event(RightButtonReleaseEvent)\
    _vtk_add_event(EnterEvent)\
    _vtk_add_event(LeaveEvent)\
    _vtk_add_event(KeyPressEvent)\
    _vtk_add_event(KeyReleaseEvent)\
    _vtk_add_event(CharEvent)\
    _vtk_add_event(ExposeEvent)\
    _vtk_add_event(ConfigureEvent)\
    _vtk_add_event(TimerEvent)\
    _vtk_add_event(MouseMoveEvent)\
    _vtk_add_event(MouseWheelForwardEvent)\
    _vtk_add_event(MouseWheelBackwardEvent)\
    _vtk_add_event(ActiveCameraEvent)\
    _vtk_add_event(CreateCameraEvent)\
    _vtk_add_event(ResetCameraEvent)\
    _vtk_add_event(ResetCameraClippingRangeEvent)\
    _vtk_add_event(ModifiedEvent)\
    _vtk_add_event(WindowLevelEvent)\
    _vtk_add_event(StartWindowLevelEvent)\
    _vtk_add_event(EndWindowLevelEvent)\
    _vtk_add_event(ResetWindowLevelEvent)\
    _vtk_add_event(SetOutputEvent)\
    _vtk_add_event(ErrorEvent)\
    _vtk_add_event(WarningEvent)\
    _vtk_add_event(StartInteractionEvent)\
        /*^ mainly used by vtkInteractorObservers*/\
    _vtk_add_event(InteractionEvent)\
    _vtk_add_event(EndInteractionEvent)\
    _vtk_add_event(EnableEvent)\
    _vtk_add_event(DisableEvent)\
    _vtk_add_event(CreateTimerEvent)\
    _vtk_add_event(DestroyTimerEvent)\
    _vtk_add_event(PlacePointEvent)\
    _vtk_add_event(PlaceWidgetEvent)\
    _vtk_add_event(CursorChangedEvent)\
    _vtk_add_event(ExecuteInformationEvent)\
    _vtk_add_event(RenderWindowMessageEvent)\
    _vtk_add_event(WrongTagEvent)\
    _vtk_add_event(StartAnimationCueEvent)\
        /*^ used by vtkAnimationCue*/ \
    _vtk_add_event(AnimationCueTickEvent)\
    _vtk_add_event(EndAnimationCueEvent)\
    _vtk_add_event(VolumeMapperRenderEndEvent)\
    _vtk_add_event(VolumeMapperRenderProgressEvent)\
    _vtk_add_event(VolumeMapperRenderStartEvent)\
    _vtk_add_event(VolumeMapperComputeGradientsEndEvent)\
    _vtk_add_event(VolumeMapperComputeGradientsProgressEvent)\
    _vtk_add_event(VolumeMapperComputeGradientsStartEvent)\
    _vtk_add_event(WidgetModifiedEvent)\
    _vtk_add_event(WidgetValueChangedEvent)\
    _vtk_add_event(WidgetActivateEvent)\
    _vtk_add_event(ConnectionCreatedEvent)\
    _vtk_add_event(ConnectionClosedEvent)\
    _vtk_add_event(DomainModifiedEvent)\
    _vtk_add_event(PropertyModifiedEvent)\
    _vtk_add_event(UpdateEvent)\
    _vtk_add_event(RegisterEvent)\
    _vtk_add_event(UnRegisterEvent)\
    _vtk_add_event(UpdateInformationEvent)\
    _vtk_add_event(AnnotationChangedEvent)\
    _vtk_add_event(SelectionChangedEvent)\
    _vtk_add_event(UpdatePropertyEvent)\
    _vtk_add_event(ViewProgressEvent)\
    _vtk_add_event(UpdateDataEvent)\
    _vtk_add_event(CurrentChangedEvent)\
    _vtk_add_event(ComputeVisiblePropBoundsEvent)\
    _vtk_add_event(TDxMotionEvent)\
      /*^ 3D Connexion device event */\
    _vtk_add_event(TDxButtonPressEvent)\
      /*^ 3D Connexion device event */\
    _vtk_add_event(TDxButtonReleaseEvent)\
      /* 3D Connexion device event */\
    _vtk_add_event(HoverEvent)\
    _vtk_add_event(LoadStateEvent)\
    _vtk_add_event(SaveStateEvent)\
    _vtk_add_event(StateChangedEvent)\
    _vtk_add_event(WindowMakeCurrentEvent)\
    _vtk_add_event(WindowIsCurrentEvent)\
    _vtk_add_event(WindowFrameEvent)\
    _vtk_add_event(HighlightEvent)\
    _vtk_add_event(WindowSupportsOpenGLEvent)\
    _vtk_add_event(WindowIsDirectEvent)\
    _vtk_add_event(UncheckedPropertyModifiedEvent)\
    _vtk_add_event(UpdateShaderEvent)\
    _vtk_add_event(MessageEvent)\
    _vtk_add_event(StartPinchEvent)\
    _vtk_add_event(PinchEvent)\
    _vtk_add_event(EndPinchEvent)\
    _vtk_add_event(StartRotateEvent)\
    _vtk_add_event(RotateEvent)\
    _vtk_add_event(EndRotateEvent)\
    _vtk_add_event(StartPanEvent)\
    _vtk_add_event(PanEvent)\
    _vtk_add_event(EndPanEvent)\
    _vtk_add_event(TapEvent)\
    _vtk_add_event(LongTapEvent)\
    _vtk_add_event(SwipeEvent)

#define vtkEventDeclarationMacro(_enum_name)\
  enum _enum_name{\
    NoEvent = 0,\
    vtkAllEventsMacro() \
    UserEvent = 1000\
  };


// The superclass that all commands should be subclasses of
class vtkCommand
{
public:

  /**
   * All derived classes of vtkCommand must implement this
   * method. This is the method that actually does the work of the
   * callback. The caller argument is the object invoking the event,
   * the eventId parameter is the id of the event, and callData
   * parameter is data that can be passed into the execute
   * method. (Note: vtkObject::InvokeEvent() takes two parameters: the
   * event id (or name) and call data. Typically call data is NULL,
   * but the user can package data and pass it this
   * way. Alternatively, a derived class of vtkCommand can be used to
   * pass data.)
   */
  // virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData) = 0;

  //@{
  /**
   * Convenience methods for translating between event names and event
   * ids.
   */
  static const char *GetStringFromEventId(unsigned long event)
  {
    switch (event)
    {
#define _vtk_add_event(Enum)\
    case Enum: return #Enum;
      
      vtkAllEventsMacro()
        
#undef _vtk_add_event
        
    case UserEvent:
      return "UserEvent";
      
    case NoEvent:
      return "NoEvent";
    }
    
    // Unknown event. Original code was returning NoEvent, so I'll stick with
    // that.
    return "NoEvent";
}
  static unsigned long GetEventIdFromString(const char *event) 
  {
    if (event)
    {
#define _vtk_add_event(Enum)\
      if (strcmp(event, #Enum) == 0) {return Enum;}
      vtkAllEventsMacro()
#undef _vtk_add_event
        
        if (strcmp("UserEvent",event) == 0)
        {
          return vtkCommand::UserEvent;
        }
    }
    
    return vtkCommand::NoEvent;
}
  //@}

  /**
   * Set/Get the abort flag. If this is set to true no further
   * commands are executed.
   */
  void SetAbortFlag(int f)
    { this->AbortFlag = f; }
  int GetAbortFlag()
    { return this->AbortFlag; }
  void AbortFlagOn()
    { this->SetAbortFlag(1); }
  void AbortFlagOff()
    { this->SetAbortFlag(0); }

  /**
   * Set/Get the passive observer flag. If this is set to true, this
   * indicates that this command does not change the state of the
   * system in any way. Passive observers are processed first, and
   * are not called even when another command has focus.
   */
  void SetPassiveObserver(int f)
    { this->PassiveObserver = f; }
  int GetPassiveObserver()
    { return this->PassiveObserver; }
  void PassiveObserverOn()
    { this->SetPassiveObserver(1); }
  void PassiveObserverOff()
    { this->SetPassiveObserver(0); }

  /**
   * All the currently defined events are listed here.  Developers can
   * use -- vtkCommand::UserEvent + int to specify their own event
   * ids.
   * Add new events by updating vtkAllEventsMacro.
   */
#define _vtk_add_event(Enum)  Enum,
  vtkEventDeclarationMacro(EventIds)
#undef _vtk_add_event

public:
  int AbortFlag;
  int PassiveObserver;

};

#endif /* vtkCommand_h */

// VTK-HeaderTest-Exclude: vtkCommand.h
