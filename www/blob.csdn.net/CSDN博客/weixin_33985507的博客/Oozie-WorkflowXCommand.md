# Oozie-WorkflowXCommand - weixin_33985507的博客 - CSDN博客
2016年07月01日 17:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
WorkflowXCommand是对任务流wf操作命令的封装类，对wf的命令我们整理为：
![1884216-7fd8966128c4c5a8.png](https://upload-images.jianshu.io/upload_images/1884216-7fd8966128c4c5a8.png)
diagram2.png
> 
WorkflowActionInfoXCommand
```
@Override
protected WorkflowActionBean execute() throws CommandException {
   JPAService jpaService = Services.get().get(JPAService.class);
   if (jpaService != null) {
       WorkflowActionBean action;
       try {
           action = jpaService.execute(new WorkflowActionGetJPAExecutor(this.id));
       }
       catch (JPAExecutorException ex) { 
          throw new CommandException(ex);
       }
       return action;    } 
  else {
       LOG.error(ErrorCode.E0610);
       return null;
   }
}
```
查询wf action信息的命令；
> 
WfEndXCommand
```
@Override
protected Void execute() throws CommandException {
    LOG.debug("STARTED WFEndXCommand " + job.getId());
    deleteWFDir();
    LOG.debug("ENDED WFEndXCommand " + job.getId());
    return null;
}
```
任务直接结束后调用这个结束命令，用来清理临时文件；
> 
SuspendXCommand
```
@Override
protected Void execute() throws CommandException {
    InstrumentUtils.incrJobCounter(getName(), 1, getInstrumentation());
    try {
        suspendJob(this.jpaService, this.wfJobBean, this.wfid, null, updateList);
        this.wfJobBean.setLastModifiedTime(new Date());
        updateList.add(new UpdateEntry<WorkflowJobQuery>(WorkflowJobQuery.UPDATE_WORKFLOW_STATUS_INSTANCE_MODIFIED,
                this.wfJobBean));
        BatchQueryExecutor.getInstance().executeBatchInsertUpdateDelete(null, updateList, null);
        queue(new WorkflowNotificationXCommand(this.wfJobBean));
        //Calling suspend recursively to handle parent workflow
        suspendParentWorkFlow();
    }
    catch (WorkflowException e) {
        throw new CommandException(e);
    }
    catch (JPAExecutorException je) {
        throw new CommandException(je);
    }
    finally {
        updateParentIfNecessary(wfJobBean);
    }
    return null;
}
```
任务流挂起命令，将任务流的状态置为挂起，将任务流下面要跑还没跑的action设置为不pending标识，更新数据库，同时发生 **WorkflowNotificationXCommand** 命令，这个在后面文章中会专门讲通知命令，如果该任务流是其他任务流的子任务流，挂起上层任务流。
> 
SubmitXCommand
```
@Override
protected String execute() throws CommandException {
    InstrumentUtils.incrJobCounter(getName(), 1, getInstrumentation());    WorkflowAppService wps = Services.get().get(WorkflowAppService.class);
    try {
        XLog.Info.get().setParameter(DagXLogInfoService.TOKEN, conf.get(OozieClient.LOG_TOKEN));
        String user = conf.get(OozieClient.USER_NAME);
        URI uri = new URI(conf.get(OozieClient.APP_PATH));
        HadoopAccessorService has = Services.get().get(HadoopAccessorService.class);
        Configuration fsConf = has.createJobConf(uri.getAuthority());        FileSystem fs = has.createFileSystem(user, uri, fsConf);
        Path configDefault = null;
        Configuration defaultConf = null;
        // app path could be a directory
        Path path = new Path(uri.getPath());
        if (!fs.isFile(path)) {
            configDefault = new Path(path, CONFIG_DEFAULT);
        } else {
            configDefault = new Path(path.getParent(), CONFIG_DEFAULT);
        }
        if (fs.exists(configDefault)) {
            try {
                defaultConf = new XConfiguration(fs.open(configDefault));
                PropertiesUtils.checkDisallowedProperties(defaultConf, DISALLOWED_DEFAULT_PROPERTIES);
                XConfiguration.injectDefaults(defaultConf, conf);
            }
            catch (IOException ex) {
                throw new IOException("default configuration file, " + ex.getMessage(), ex);
            } 
       }
        WorkflowApp app = wps.parseDef(conf, defaultConf);
        XConfiguration protoActionConf = wps.createProtoActionConf(conf, true);
        WorkflowLib workflowLib = Services.get().get(WorkflowStoreService.class).getWorkflowLibWithNoDB();
        PropertiesUtils.checkDisallowedProperties(conf, DISALLOWED_USER_PROPERTIES);
        // Resolving all variables in the job properties.
        // This ensures the Hadoop Configuration semantics is preserved.
        XConfiguration resolvedVarsConf = new XConfiguration();
        for (Map.Entry<String, String> entry : conf) { 
           resolvedVarsConf.set(entry.getKey(), conf.get(entry.getKey()));
        }
        conf = resolvedVarsConf;
        WorkflowInstance wfInstance;
        try {
            wfInstance = workflowLib.createInstance(app, conf);
        }
        catch (WorkflowException e) {
            throw new StoreException(e);
       }
        Configuration conf = wfInstance.getConf();
        // System.out.println("WF INSTANCE CONF:");
        // System.out.println(XmlUtils.prettyPrint(conf).toString());
        WorkflowJobBean workflow = new WorkflowJobBean(); 
       workflow.setId(wfInstance.getId()); 
       workflow.setAppName(ELUtils.resolveAppName(app.getName(), conf));
        workflow.setAppPath(conf.get(OozieClient.APP_PATH));
        workflow.setConf(XmlUtils.prettyPrint(conf).toString());
        workflow.setProtoActionConf(protoActionConf.toXmlString());
        workflow.setCreatedTime(new Date());
        workflow.setLastModifiedTime(new Date());
        workflow.setLogToken(conf.get(OozieClient.LOG_TOKEN, ""));
        workflow.setStatus(WorkflowJob.Status.PREP);
        workflow.setRun(0);
        workflow.setUser(conf.get(OozieClient.USER_NAME));
        workflow.setGroup(conf.get(OozieClient.GROUP_NAME));
        workflow.setWorkflowInstance(wfInstance);
        workflow.setExternalId(conf.get(OozieClient.EXTERNAL_ID));
        // Set parent id if it doesn't already have one (for subworkflows)
        if (workflow.getParentId() == null) {
            workflow.setParentId(conf.get(SubWorkflowActionExecutor.PARENT_ID));        }
        // Set to coord action Id if workflow submitted through coordinator
        if (workflow.getParentId() == null) {
            workflow.setParentId(parentId);
        }
        LogUtils.setLogInfo(workflow);
        LOG.debug("Workflow record created, Status [{0}]", workflow.getStatus());
        Element wfElem = XmlUtils.parseXml(app.getDefinition());
        ELEvaluator evalSla = createELEvaluatorForGroup(conf, "wf-sla-submit");
        String jobSlaXml = verifySlaElements(wfElem, evalSla);
        if (!dryrun) {
            writeSLARegistration(wfElem, jobSlaXml, workflow.getId(), workflow.getParentId(), workflow.getUser(), 
                   workflow.getGroup(), workflow.getAppName(), LOG, evalSla);
            workflow.setSlaXml(jobSlaXml);
            // System.out.println("SlaXml :"+ slaXml);
            //store.insertWorkflow(workflow);
            insertList.add(workflow);
            JPAService jpaService = Services.get().get(JPAService.class);
            if (jpaService != null) {
                try {
                    BatchQueryExecutor.getInstance().executeBatchInsertUpdateDelete(insertList, null, null);
                } 
               catch (JPAExecutorException je) {
                    throw new CommandException(je);
                }
            }
            else {
                LOG.error(ErrorCode.E0610);
                return null;
            }
            return workflow.getId();
        }
        else {
            // Checking variable substitution for dryrun
            ActionExecutorContext context = new ActionXCommand.ActionExecutorContext(workflow, null, false, false);
            Element workflowXml = XmlUtils.parseXml(app.getDefinition());
            removeSlaElements(workflowXml);
            String workflowXmlString = XmlUtils.removeComments(XmlUtils.prettyPrint(workflowXml).toString());
            workflowXmlString = context.getELEvaluator().evaluate(workflowXmlString, String.class);
            workflowXml = XmlUtils.parseXml(workflowXmlString);
            Iterator<Element> it = workflowXml.getDescendants(new ElementFilter("job-xml"));
            // Checking all variable substitutions in job-xml files
            while (it.hasNext()) {
                Element e = it.next();
                String jobXml = e.getTextTrim();
                Path xmlPath = new Path(workflow.getAppPath(), jobXml);
                Configuration jobXmlConf = new XConfiguration(fs.open(xmlPath));
                String jobXmlConfString = XmlUtils.prettyPrint(jobXmlConf).toString();
                jobXmlConfString = XmlUtils.removeComments(jobXmlConfString);
                context.getELEvaluator().evaluate(jobXmlConfString, String.class);
            }
            return "OK";
        }
    }    catch (WorkflowException ex) {
        throw new CommandException(ex);
    }    catch (HadoopAccessorException ex) {
        throw new CommandException(ex);
    }    catch (Exception ex) {
        throw new CommandException(ErrorCode.E0803, ex.getMessage(), ex);
    }
}
```
wf提交命令，将用户提交的任务解析后更新到数据库；
> 
SignalXCommand
```
@Override
protected Void execute() throws CommandException {
    LOG.debug("STARTED SignalCommand for jobid=" + jobId + ", actionId=" + actionId);
    WorkflowInstance workflowInstance = wfJob.getWorkflowInstance();
    workflowInstance.setTransientVar(WorkflowStoreService.WORKFLOW_BEAN, wfJob);
    WorkflowJob.Status prevStatus = wfJob.getStatus();
    boolean completed = false, skipAction = false;
    WorkflowActionBean syncAction = null;
    if (wfAction == null) {
        if (wfJob.getStatus() == WorkflowJob.Status.PREP) {
            try {
                completed = workflowInstance.start();
            }
            catch (WorkflowException e) {
                throw new CommandException(e);
            }
            wfJob.setStatus(WorkflowJob.Status.RUNNING);
            wfJob.setStartTime(new Date());
            wfJob.setWorkflowInstance(workflowInstance);
            generateEvent = true;
            // 1. Add SLA status event for WF-JOB with status STARTED
            SLAEventBean slaEvent = SLADbXOperations.createStatusEvent(wfJob.getSlaXml(), jobId, Status.STARTED,
                    SlaAppType.WORKFLOW_JOB);
            if (slaEvent != null) {
                insertList.add(slaEvent);
            }
            // 2. Add SLA registration events for all WF_ACTIONS
            createSLARegistrationForAllActions(workflowInstance.getApp().getDefinition(), wfJob.getUser(),
                    wfJob.getGroup(), wfJob.getConf());
            queue(new WorkflowNotificationXCommand(wfJob));
        }
        else {
            throw new CommandException(ErrorCode.E0801, wfJob.getId()); 
       }
    }
    else {
        WorkflowInstance.Status initialStatus = workflowInstance.getStatus();
        String skipVar = workflowInstance.getVar(wfAction.getName() + WorkflowInstance.NODE_VAR_SEPARATOR
                + ReRunXCommand.TO_SKIP);
        if (skipVar != null) {
            skipAction = skipVar.equals("true");
        }
        try {
            completed = workflowInstance.signal(wfAction.getExecutionPath(), wfAction.getSignalValue());
        }
        catch (WorkflowException e) {
           LOG.error("Workflow action failed : " + e.getMessage(), e);
            wfJob.setStatus(WorkflowJob.Status.valueOf(workflowInstance.getStatus().toString()));
            completed = true;
        }
       wfJob.setWorkflowInstance(workflowInstance);
        wfAction.resetPending();
        if (!skipAction) { 
           wfAction.setTransition(workflowInstance.getTransition(wfAction.getName()));
            queue(new WorkflowNotificationXCommand(wfJob, wfAction));
        }
        updateList.add(new UpdateEntry<WorkflowActionQuery>(WorkflowActionQuery.UPDATE_ACTION_PENDING_TRANS,                wfAction));
        WorkflowInstance.Status endStatus = workflowInstance.getStatus();
        if (endStatus != initialStatus) {
            generateEvent = true;
        }
    }
    if (completed) {
        try {
            for (String actionToKillId : WorkflowStoreService.getActionsToKill(workflowInstance)) {
                WorkflowActionBean actionToKill;
                actionToKill = WorkflowActionQueryExecutor.getInstance().get(
                        WorkflowActionQuery.GET_ACTION_ID_TYPE_LASTCHECK, actionToKillId);
                actionToKill.setPending();
                actionToKill.setStatus(WorkflowActionBean.Status.KILLED);
                updateList.add(new UpdateEntry<WorkflowActionQuery>(
                        WorkflowActionQuery.UPDATE_ACTION_STATUS_PENDING, actionToKill));
                queue(new ActionKillXCommand(actionToKill.getId(), actionToKill.getType()));
            }
            for (String actionToFailId : WorkflowStoreService.getActionsToFail(workflowInstance)) {
                WorkflowActionBean actionToFail = WorkflowActionQueryExecutor.getInstance().get(
                        WorkflowActionQuery.GET_ACTION_FAIL, actionToFailId);
                actionToFail.resetPending();
                actionToFail.setStatus(WorkflowActionBean.Status.FAILED); 
               if (wfJobErrorCode != null) {
                    wfJobErrorCode = actionToFail.getErrorCode();
                    wfJobErrorMsg = actionToFail.getErrorMessage();
                }
                queue(new WorkflowNotificationXCommand(wfJob, actionToFail));
                SLAEventBean slaEvent = SLADbXOperations.createStatusEvent(wfAction.getSlaXml(), wfAction.getId(),
                        Status.FAILED, SlaAppType.WORKFLOW_ACTION);
                if (slaEvent != null) {
                    insertList.add(slaEvent);
                }
                updateList.add(new UpdateEntry<WorkflowActionQuery>(
                        WorkflowActionQuery.UPDATE_ACTION_STATUS_PENDING, actionToFail));
            }
        }
        catch (JPAExecutorException je) {
            throw new CommandException(je);
        }
        wfJob.setStatus(WorkflowJob.Status.valueOf(workflowInstance.getStatus().toString()));
        wfJob.setEndTime(new Date());
        wfJob.setWorkflowInstance(workflowInstance);
        Status slaStatus = Status.SUCCEEDED;
        switch (wfJob.getStatus()) {
            case SUCCEEDED:
                slaStatus = Status.SUCCEEDED;
                break;
            case KILLED:
                slaStatus = Status.KILLED;
                break;
            case FAILED:
                slaStatus = Status.FAILED;
                break;
            default: // TODO SUSPENDED
                break;
        }
        SLAEventBean slaEvent = SLADbXOperations.createStatusEvent(wfJob.getSlaXml(), jobId, slaStatus,
                SlaAppType.WORKFLOW_JOB);
        if (slaEvent != null) {
            insertList.add(slaEvent);
        }
        queue(new WorkflowNotificationXCommand(wfJob));
        if (wfJob.getStatus() == WorkflowJob.Status.SUCCEEDED) {
            InstrumentUtils.incrJobCounter(INSTR_SUCCEEDED_JOBS_COUNTER_NAME, 1, getInstrumentation());
        }
        // output message for Kill node
        if (wfAction != null) { // wfAction could be a no-op job
            NodeDef nodeDef = workflowInstance.getNodeDef(wfAction.getExecutionPath());
            if (nodeDef != null && nodeDef instanceof KillNodeDef) { 
                boolean isRetry = false;
                boolean isUserRetry = false;
                ActionExecutorContext context = new ActionXCommand.ActionExecutorContext(wfJob, wfAction, isRetry,                        isUserRetry);
                InstrumentUtils.incrJobCounter(INSTR_KILLED_JOBS_COUNTER_NAME, 1, getInstrumentation());
                try {
                    String tmpNodeConf = nodeDef.getConf();
                    String actionConf = context.getELEvaluator().evaluate(tmpNodeConf, String.class);
                    LOG.debug(
                            "Try to resolve KillNode message for jobid [{0}], actionId [{1}], before resolve [{2}], "
                                    + "after resolve [{3}]", jobId, actionId, tmpNodeConf, actionConf);
                    if (wfAction.getErrorCode() != null) {
                        wfAction.setErrorInfo(wfAction.getErrorCode(), actionConf);
                    }
                    else {
                        wfAction.setErrorInfo(ErrorCode.E0729.toString(), actionConf);
                    }
                    updateList.add(new UpdateEntry<WorkflowActionQuery>(
                            WorkflowActionQuery.UPDATE_ACTION_PENDING_TRANS_ERROR, wfAction));
                }
                catch (Exception ex) {
                    LOG.warn("Exception in SignalXCommand ", ex.getMessage(), ex);
                    throw new CommandException(ErrorCode.E0729, wfAction.getName(), ex);
                }
            }
        }
    }
    else {
        for (WorkflowActionBean newAction : WorkflowStoreService.getActionsToStart(workflowInstance)) {
            boolean isOldWFAction = false;
            // In case of subworkflow rerun when failed option have been provided, rerun command do not delete
            // old action. To avoid twice entry for same action, Checking in Db if the workflow action already exist. 
           if(SubWorkflowActionExecutor.ACTION_TYPE.equals(newAction.getType())) {
                try {
                    WorkflowActionBean oldAction = WorkflowActionQueryExecutor.getInstance()
                            .get(WorkflowActionQuery.GET_ACTION_CHECK,
                                    newAction.getId());
                    newAction.setExternalId(oldAction.getExternalId());
                    newAction.setCreatedTime(oldAction.getCreatedTime());
                    isOldWFAction = true;
                } catch (JPAExecutorException e) {
                    if(e.getErrorCode() != ErrorCode.E0605) {
                        throw new CommandException(e);
                    }
                }
            }
            String skipVar = workflowInstance.getVar(newAction.getName() + WorkflowInstance.NODE_VAR_SEPARATOR
                    + ReRunXCommand.TO_SKIP);
            boolean skipNewAction = false, suspendNewAction = false;
            if (skipVar != null) {
                skipNewAction = skipVar.equals("true");
            }
            if (skipNewAction) {
                WorkflowActionBean oldAction = new WorkflowActionBean();
                oldAction.setId(newAction.getId());
                oldAction.setPending();
                oldAction.setExecutionPath(newAction.getExecutionPath());
                updateList.add(new UpdateEntry<WorkflowActionQuery>(WorkflowActionQuery.UPDATE_ACTION_PENDING,
                        oldAction));
                queue(new SignalXCommand(jobId, oldAction.getId()));
            }
            else {
                if(!skipAction) {
                    try {
                        // Make sure that transition node for a forked action
                        // is inserted only once
                        WorkflowActionQueryExecutor.getInstance().get(WorkflowActionQuery.GET_ACTION_ID_TYPE_LASTCHECK,
                                newAction.getId());
                        continue;
                    } catch (JPAExecutorException jee) {
                    } 
               }
                suspendNewAction = checkForSuspendNode(newAction);                newAction.setPending();
                String actionSlaXml = getActionSLAXml(newAction.getName(), workflowInstance.getApp()
                        .getDefinition(), wfJob.getConf());
                newAction.setSlaXml(actionSlaXml);
                if(!isOldWFAction) {
                    newAction.setCreatedTime(new Date());
                    insertList.add(newAction);
                } else {
                    updateList.add(new UpdateEntry<WorkflowActionQuery>(WorkflowActionQuery.UPDATE_ACTION_START, 
                           newAction));
                }
                LOG.debug("SignalXCommand: Name: " + newAction.getName() + ", Id: " + newAction.getId()
                        + ", Authcode:" + newAction.getCred());
                if (wfAction != null) {// null during wf job submit
                    ActionService as = Services.get().get(ActionService.class);
                    ActionExecutor current = as.getExecutor(wfAction.getType());
                    LOG.trace("Current Action Type:" + current.getClass());
                    if (!suspendNewAction) {
                        if (!(current instanceof ForkActionExecutor) && !(current instanceof StartActionExecutor)) {
                            // Excluding :start: here from executing first action synchronously since it 
                           // blocks the consumer thread till the action is submitted to Hadoop,
                            // in turn reducing the number of new submissions the threads can accept.
                            // Would also be susceptible to longer delays in case Hadoop cluster is busy.
                            syncAction = newAction;
                        }
                        else {
                            queue(new ActionStartXCommand(newAction.getId(), newAction.getType()));
                        }
                    }
                }
                else {
                    syncAction = newAction; // first action after wf submit should always be sync
                }
            }
        }
    }
    try {
        wfJob.setLastModifiedTime(new Date());
        updateList.add(new UpdateEntry<WorkflowJobQuery>(
                WorkflowJobQuery.UPDATE_WORKFLOW_STATUS_INSTANCE_MOD_START_END, wfJob));
        // call JPAExecutor to do the bulk writes
        BatchQueryExecutor.getInstance().executeBatchInsertUpdateDelete(insertList, updateList, null);
        if (prevStatus != wfJob.getStatus()) {
            LOG.debug("Updated the workflow status to " + wfJob.getId() + "  status =" + wfJob.getStatusStr());
        }
        if (generateEvent && EventHandlerService.isEnabled()) {
            generateEvent(wfJob, wfJobErrorCode, wfJobErrorMsg);
        }
    }
    catch (JPAExecutorException je) {
        throw new CommandException(je);
    }
    // Changing to synchronous call from asynchronous queuing to prevent
    // undue delay from between end of previous and start of next action
    if (wfJob.getStatus() != WorkflowJob.Status.RUNNING
            && wfJob.getStatus() != WorkflowJob.Status.SUSPENDED) {
        // only for asynchronous actions, parent coord action's external id will
        // persisted and following update will succeed. 
       updateParentIfNecessary(wfJob);
        new WfEndXCommand(wfJob).call();
 // To delete the WF temp dir
    }
    else if (syncAction != null) {
        new ActionStartXCommand(wfJob, syncAction.getId(), syncAction.getType()).call(getEntityKey());
    }
    LOG.debug("ENDED SignalCommand for jobid=" + jobId + ", actionId=" + actionId);
    return null;
}
```
通过此命令发起执行一个任务流wf实例，或者一个任务流wf实例中的action；
里面涉及到的 **WorkflowInstance** 任务流实例 后面会专门讲解，大概的关系是一个wf 任务实例就是一个 **WorkflowInstance** 一个实例中有多个node节点，node节点可能是控制节点也可能是任务节点，对应于wf中的action 对象。wf 任务中有多个 action对象，action对象之间没有依赖关系，他们的依赖关系在node组织结构中。后面会特别讲到。
> 
ResumeXCommand
```
@Override
protected Void execute() throws CommandException {
    try {
        if (workflow.getStatus() == WorkflowJob.Status.SUSPENDED) {
            InstrumentUtils.incrJobCounter(getName(), 1, getInstrumentation());
            workflow.getWorkflowInstance().resume();
            WorkflowInstance wfInstance = workflow.getWorkflowInstance();
            ((LiteWorkflowInstance) wfInstance).setStatus(WorkflowInstance.Status.RUNNING);
            workflow.setWorkflowInstance(wfInstance);
            workflow.setStatus(WorkflowJob.Status.RUNNING);
            //for (WorkflowActionBean action : store.getActionsForWorkflow(id, false)) {
            for (WorkflowActionBean action : jpaService.execute(new WorkflowJobGetActionsJPAExecutor(id))) {
                // Set pending flag to true for the actions that are START_RETRY or
                // START_MANUAL or END_RETRY or END_MANUAL
                if (action.isRetryOrManual()) {
                    action.setPendingOnly();
                    updateList.add(new UpdateEntry<WorkflowActionQuery>(
                            WorkflowActionQuery.UPDATE_ACTION_STATUS_PENDING, action));                }
                if (action.isPending()) {
                    if (action.getStatus() == WorkflowActionBean.Status.PREP
                            || action.getStatus() == WorkflowActionBean.Status.START_MANUAL) {
                        // When resuming a workflow that was programatically suspended (via ActionCheckXCommand) because of
                        // a repeated transient error, we have to clean up the action dir
                        if (!action.getType().equals(StartActionExecutor.TYPE) &&
       // The control actions have invalid
                            !action.getType().equals(ForkActionExecutor.TYPE) &&
        // action dir paths because they
                            !action.getType().equals(JoinActionExecutor.TYPE) &&
        // contain ":" (colons)
                            !action.getType().equals(KillActionExecutor.TYPE) &&
                            !action.getType().equals(EndActionExecutor.TYPE)) {
                            ActionExecutorContext context =
                                    new ActionXCommand.ActionExecutorContext(workflow, action, false, false);
                            if (context.getAppFileSystem().exists(context.getActionDir())) {
                                context.getAppFileSystem().delete(context.getActionDir(), true);
                            }
                        }
                        queue(new ActionStartXCommand(action.getId(), action.getType()));
                    }
                    else {
                        if (action.getStatus() == WorkflowActionBean.Status.START_RETRY) {
                            Date nextRunTime = action.getPendingAge();
                            queue(new ActionStartXCommand(action.getId(), action.getType()),
                                          nextRunTime.getTime() - System.currentTimeMillis());
                        }
                        else {
                            if (action.getStatus() == WorkflowActionBean.Status.DONE
                                    || action.getStatus() == WorkflowActionBean.Status.END_MANUAL) {
                                queue(new ActionEndXCommand(action.getId(), action.getType()));
                            }
                            else {
                                if (action.getStatus() == WorkflowActionBean.Status.END_RETRY) {
                                    Date nextRunTime = action.getPendingAge();
                                    queue(new ActionEndXCommand(action.getId(), action.getType()),
                                                  nextRunTime.getTime() - System.currentTimeMillis());
                                }
                            }
                        }
                    }
                }
            }
            workflow.setLastModifiedTime(new Date());
            updateList.add(new UpdateEntry<WorkflowJobQuery>(
                    WorkflowJobQuery.UPDATE_WORKFLOW_STATUS_INSTANCE_MODIFIED, workflow));
            BatchQueryExecutor.getInstance().executeBatchInsertUpdateDelete(null, updateList, null);
            if (EventHandlerService.isEnabled()) {
                generateEvent(workflow);
            }
            queue(new WorkflowNotificationXCommand(workflow));
        }
        return null;
    }    catch (WorkflowException ex) {
        throw new CommandException(ex);
    }    catch (JPAExecutorException e) {
        throw new CommandException(e);
    }    catch (HadoopAccessorException e) {
        throw new CommandException(e);
    }    catch (IOException e) {
        throw new CommandException(ErrorCode.E0902, e.getMessage(), e);
    }    catch (URISyntaxException e) {
        throw new CommandException(ErrorCode.E0902, e.getMessage(), e);
    }    finally {
        updateParentIfNecessary(workflow);
    }
}
```
任务流恢复命令，将处于挂起状态的任务流恢复，讲wf 任务对象状态置为 RUNNING，同时也将 wf 任务所关联的 WorkflowInstance 的状态置为 RUNNING，查询数据库，找到这个任务流wf 包含的action对象。对挂起的action的不同状态做不同的恢复操作。
> 
ReRunXCommand
```
/* (non-Javadoc)
 * @see org.apache.oozie.command.XCommand#execute()
 */
@Overrideprotected Void execute() throws CommandException {
    setupReRun();
    startWorkflow(jobId);
    return null;
}
```
重跑任务流，操作的最小单元虽然是任务流，但是在重跑的时候可以对不需要跑的节点进行skip。
> 
KillXCommand
```
@Override
protected Void execute() throws CommandException {
    LOG.info("STARTED WorkflowKillXCommand for jobId=" + wfId);
    wfJob.setEndTime(new Date());
    if (wfJob.getStatus() != WorkflowJob.Status.FAILED) {
        InstrumentUtils.incrJobCounter(getName(), 1, getInstrumentation());
        wfJob.setStatus(WorkflowJob.Status.KILLED);
        SLAEventBean slaEvent = SLADbXOperations.createStatusEvent(wfJob.getSlaXml(), wfJob.getId(),
                Status.KILLED, SlaAppType.WORKFLOW_JOB);
        if(slaEvent != null) {
            insertList.add(slaEvent);
        }
        try {
            wfJob.getWorkflowInstance().kill();
        }
        catch (WorkflowException e) {
            throw new CommandException(ErrorCode.E0725, e.getMessage(), e);
        }
        WorkflowInstance wfInstance = wfJob.getWorkflowInstance();
        ((LiteWorkflowInstance) wfInstance).setStatus(WorkflowInstance.Status.KILLED);
        wfJob.setWorkflowInstance(wfInstance);
    }
    try {
        for (WorkflowActionBean action : actionList) {
            if (action.getStatus() == WorkflowActionBean.Status.RUNNING
                    || action.getStatus() == WorkflowActionBean.Status.DONE) {
                if (!(actionService.getExecutor(action.getType()) instanceof ControlNodeActionExecutor)) {
                    action.setPending();
                }
                action.setStatus(WorkflowActionBean.Status.KILLED);
                updateList.add(new UpdateEntry<WorkflowActionQuery>(WorkflowActionQuery.UPDATE_ACTION_STATUS_PENDING, action));
                queue(new ActionKillXCommand(action.getId(), action.getType()));
            }
            else if (action.getStatus() == WorkflowActionBean.Status.PREP
                    || action.getStatus() == WorkflowActionBean.Status.START_RETRY
                    || action.getStatus() == WorkflowActionBean.Status.START_MANUAL
                    || action.getStatus() == WorkflowActionBean.Status.END_RETRY
                    || action.getStatus() == WorkflowActionBean.Status.END_MANUAL
                    || action.getStatus() == WorkflowActionBean.Status.USER_RETRY) {
                action.setStatus(WorkflowActionBean.Status.KILLED);
                action.resetPending();
                SLAEventBean slaEvent = SLADbXOperations.createStatusEvent(action.getSlaXml(), action.getId(),
                        Status.KILLED, SlaAppType.WORKFLOW_ACTION);
                if(slaEvent != null) {
                    insertList.add(slaEvent);
                }
                updateList.add(new UpdateEntry<WorkflowActionQuery>(WorkflowActionQuery.UPDATE_ACTION_STATUS_PENDING, action));
                if (EventHandlerService.isEnabled()
                        && !(actionService.getExecutor(action.getType()) instanceof ControlNodeActionExecutor)) {
                    generateEvent(action, wfJob.getUser());
                }
            }
        }
        wfJob.setLastModifiedTime(new Date());
        updateList.add(new UpdateEntry<WorkflowJobQuery>(WorkflowJobQuery.UPDATE_WORKFLOW_STATUS_INSTANCE_MOD_END, wfJob));
        BatchQueryExecutor.getInstance().executeBatchInsertUpdateDelete(insertList, updateList, null);
        if (EventHandlerService.isEnabled()) {
            generateEvent(wfJob);
        }
        queue(new WorkflowNotificationXCommand(wfJob));
    }
    catch (JPAExecutorException e) {
        throw new CommandException(e);
    }
    finally {
        if(wfJob.getStatus() == WorkflowJob.Status.KILLED) {
             new WfEndXCommand(wfJob).call();
 //To delete the WF temp dir
        }
        updateParentIfNecessary(wfJob);
    } 
   LOG.info("ENDED WorkflowKillXCommand for jobId=" + wfId);
    return null;
}
```
任务流杀死命令，将任务流状态置为杀死状态，将 **WorkflowInstance** node节点置为杀死状态，针对action 发送杀死命令，进行深度杀死。
> 
JobsXCommand
```
@Override
protected WorkflowsInfo execute() throws CommandException {
    try {
        JPAService jpaService = Services.get().get(JPAService.class);
        if (jpaService != null) {
            this.workflows = jpaService.execute(new WorkflowsJobGetJPAExecutor(this.filter, this.start, this.len));
        }
        else {
            throw new CommandException(ErrorCode.E0610);
        }
        for (WorkflowJobBean workflow : this.workflows.getWorkflows()) {
            workflow.setConsoleUrl(JobXCommand.getJobConsoleUrl(workflow.getId()));
        }
        return this.workflows;
    }
    catch (Exception ex) {
        throw new CommandException(ErrorCode.E0603, ex.getMessage(), ex);
    }
}
```
查看多个任务流信息命令，只要注意一点 ConsoleUrl 并没有在数据库中写死；
> 
JobXCommand
```
@Override
protected WorkflowJobBean execute() throws CommandException {
    try {
        JPAService jpaService = Services.get().get(JPAService.class);
        if (jpaService != null) {
            this.workflow = jpaService.execute(new WorkflowInfoWithActionsSubsetGetJPAExecutor(this.id, this.start,
                    this.len));
        }
        else {
            throw new CommandException(ErrorCode.E0610, this.id);
        }
        this.workflow.setConsoleUrl(getJobConsoleUrl(id));
    }
    catch (JPAExecutorException ex) {
        throw new CommandException(ex);
    }
    catch (Exception ex) {
        throw new CommandException(ErrorCode.E0603, ex.getMessage(), ex);
    }
    return this.workflow;
}
```
查看单个任务流信息命令，这里有两个入参，**start** 和 **len** 用来展示任务流中特定的action集。
> 
ExternalIdXCommand
```
@Override
protected String execute() throws CommandException {
    try {
        JPAService jpaService = Services.get().get(JPAService.class);
        String wfId = null;
        if (jpaService != null) {
            wfId = jpaService.execute(new WorkflowIdGetForExternalIdJPAExecutor(externalId));
        }
        else {
            LOG.error(ErrorCode.E0610);
        }
        return wfId;
    }    catch (XException ex) {
        throw new CommandException(ex);
    }
}
```
根据 externalId 查询任务流的 id；
> 
DefinitionXCommand
```
@Override
protected String execute() throws CommandException {
    if (wfJob != null) {
        return wfJob.getWorkflowInstance().getApp().getDefinition();
    }
    else {
        throw new CommandException(ErrorCode.E0604, "null");
    }
}
```
查看任务流定义命令；
> 
CompletedActionXCommand
```
@Override
protected Void execute() throws CommandException {
    // If the action is still in PREP, we probably received a callback before Oozie was able to update from PREP to RUNNING; 
   // we'll requeue this command a few times and hope that it switches to RUNNING before giving up
    if (this.wfactionBean.getStatus() == WorkflowActionBean.Status.PREP) {
        int maxEarlyRequeueCount = Services.get().get(CallbackService.class).getEarlyRequeueMaxRetries();
        if (this.earlyRequeueCount < maxEarlyRequeueCount) {
            long delay = getRequeueDelay();
            LOG.warn("Received early callback for action still in PREP state; will wait [{0}]ms and requeue up to [{1}] more"
                    + " times", delay, (maxEarlyRequeueCount - earlyRequeueCount));
            queue(new CompletedActionXCommand(this.actionId, this.externalStatus, null, this.getPriority(),
                    this.earlyRequeueCount + 1), delay);
       } else {
            throw new CommandException(ErrorCode.E0822, actionId);
        }
    } else {
    // RUNNING
        ActionExecutor executor = Services.get().get(ActionService.class).getExecutor(this.wfactionBean.getType());
        // this is done because oozie notifications (of sub-wfs) is send
        // every status change, not only on completion.
        if (executor.isCompleted(externalStatus)) {
            queue(new ActionCheckXCommand(this.wfactionBean.getId(), getPriority(), -1));
        }
    }
    return null;
}
```
action完成后的回调命令，并不是说这个命令掉了 这个action就完成了，因为还要考虑子任务流的情况，所以还要用 **ActionCheckXCommand**  检查。
> 
BulkWorkflowXCommand
```
@Override
protected WorkflowsInfo execute() throws CommandException {
    try {
        List<WorkflowJobBean> workflows = this.workflowsInfo.getWorkflows();
        for (WorkflowJobBean job : workflows) {
            switch (operation) {
                case Kill:
                    if (job.getStatus() == WorkflowJob.Status.PREP
                            || job.getStatus() == WorkflowJob.Status.RUNNING
                            || job.getStatus() == WorkflowJob.Status.SUSPENDED
                            || job.getStatus() == WorkflowJob.Status.FAILED) {
                        new KillXCommand(job.getId()).call();
                    }
                    break;
                case Suspend:
                    if (job.getStatus() == WorkflowJob.Status.RUNNING) { 
                       new SuspendXCommand(job.getId()).call();
                    }
                    break;
                case Resume:
                    if (job.getStatus() == WorkflowJob.Status.SUSPENDED) { 
                       new ResumeXCommand(job.getId()).call();
                    }
                    break;
                default:
                    throw new CommandException(ErrorCode.E1102, operation);
            }
        }
        loadJobs();
        return this.workflowsInfo;
    }    catch (Exception ex) {
        throw new CommandException(ErrorCode.E0725, ex.getMessage(), ex);
    }
}
```
批量处理任务流的命令;
> 
ActionXCommand
对任务流内的action的操作的抽象类。
> 
SubmitHttpXCommand
```
@Override
protected String execute() throws CommandException {
    InstrumentUtils.incrJobCounter(getName(), 1, getInstrumentation());
    WorkflowAppService wps = Services.get().get(WorkflowAppService.class);
    try {
        XLog.Info.get().setParameter(DagXLogInfoService.TOKEN, conf.get(OozieClient.LOG_TOKEN));
        String wfXml = getWorkflowXml(conf);
        LOG.debug("workflow xml created on the server side is :\n");
        LOG.debug(wfXml);
        WorkflowApp app = wps.parseDef(wfXml, conf);
        XConfiguration protoActionConf = wps.createProtoActionConf(conf, false);
        WorkflowLib workflowLib = Services.get().get(WorkflowStoreService.class).getWorkflowLibWithNoDB();        PropertiesUtils.checkDisallowedProperties(conf, DISALLOWED_USER_PROPERTIES);
        // Resolving all variables in the job properties.
        // This ensures the Hadoop Configuration semantics is preserved.
        XConfiguration resolvedVarsConf = new XConfiguration();
        for (Map.Entry<String, String> entry : conf) {
            resolvedVarsConf.set(entry.getKey(), conf.get(entry.getKey()));
        }
        conf = resolvedVarsConf;
        WorkflowInstance wfInstance;
        try {
            wfInstance = workflowLib.createInstance(app, conf);
        }
        catch (WorkflowException e) {
            throw new StoreException(e);
        }
        Configuration conf = wfInstance.getConf();
        WorkflowJobBean workflow = new WorkflowJobBean();
        workflow.setId(wfInstance.getId());
        workflow.setAppName(app.getName());
        workflow.setAppPath(conf.get(OozieClient.APP_PATH));
        workflow.setConf(XmlUtils.prettyPrint(conf).toString());
        workflow.setProtoActionConf(protoActionConf.toXmlString());
        workflow.setCreatedTime(new Date());
        workflow.setLastModifiedTime(new Date());
        workflow.setLogToken(conf.get(OozieClient.LOG_TOKEN, ""));
        workflow.setStatus(WorkflowJob.Status.PREP);
        workflow.setRun(0);
        workflow.setUser(conf.get(OozieClient.USER_NAME));
        workflow.setGroup(conf.get(OozieClient.GROUP_NAME));
        workflow.setWorkflowInstance(wfInstance);
        workflow.setExternalId(conf.get(OozieClient.EXTERNAL_ID));
        LogUtils.setLogInfo(workflow);
        JPAService jpaService = Services.get().get(JPAService.class);
        if (jpaService != null) {
            jpaService.execute(new WorkflowJobInsertJPAExecutor(workflow));
        }
        else {
            LOG.error(ErrorCode.E0610);
            return null;
        }
        return workflow.getId();
    }
    catch (WorkflowException ex) {
        throw new CommandException(ex);
    }
    catch (Exception ex) {
        throw new CommandException(ErrorCode.E0803, ex.getMessage(), ex);
    }
}
```
通过http方式来提交任务流；
