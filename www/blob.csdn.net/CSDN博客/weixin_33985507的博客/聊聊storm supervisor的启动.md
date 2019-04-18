# 聊聊storm supervisor的启动 - weixin_33985507的博客 - CSDN博客
2018年10月12日 16:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
## 序
本文主要研究一下storm supervisor的启动
## Supervisor.launch
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/Supervisor.java
```
/**
     * Launch the supervisor
     */
    public void launch() throws Exception {
        LOG.info("Starting Supervisor with conf {}", conf);
        String path = ConfigUtils.supervisorTmpDir(conf);
        FileUtils.cleanDirectory(new File(path));
        Localizer localizer = getLocalizer();
        SupervisorHeartbeat hb = new SupervisorHeartbeat(conf, this);
        hb.run();
        // should synchronize supervisor so it doesn't launch anything after being down (optimization)
        Integer heartbeatFrequency = Utils.getInt(conf.get(Config.SUPERVISOR_HEARTBEAT_FREQUENCY_SECS));
        heartbeatTimer.scheduleRecurring(0, heartbeatFrequency, hb);
        this.eventManager = new EventManagerImp(false);
        this.readState = new ReadClusterState(this);
        
        Set<String> downloadedTopoIds = SupervisorUtils.readDownloadedTopologyIds(conf);
        Map<Integer, LocalAssignment> portToAssignments = localState.getLocalAssignmentsMap();
        if (portToAssignments != null) {
            Map<String, LocalAssignment> assignments = new HashMap<>();
            for (LocalAssignment la : localState.getLocalAssignmentsMap().values()) {
                assignments.put(la.get_topology_id(), la);
            }
            for (String topoId : downloadedTopoIds) {
                LocalAssignment la = assignments.get(topoId);
                if (la != null) {
                    SupervisorUtils.addBlobReferences(localizer, topoId, conf, la.get_owner());
                } else {
                    LOG.warn("Could not find an owner for topo {}", topoId);
                }
            }
        }
        // do this after adding the references so we don't try to clean things being used
        localizer.startCleaner();
        UpdateBlobs updateBlobsThread = new UpdateBlobs(this);
        if ((Boolean) conf.get(Config.SUPERVISOR_ENABLE)) {
            // This isn't strictly necessary, but it doesn't hurt and ensures that the machine stays up
            // to date even if callbacks don't all work exactly right
            eventTimer.scheduleRecurring(0, 10, new EventManagerPushCallback(readState, eventManager));
            // Blob update thread. Starts with 30 seconds delay, every 30 seconds
            blobUpdateTimer.scheduleRecurring(30, 30, new EventManagerPushCallback(updateBlobsThread, eventManager));
            // supervisor health check
            eventTimer.scheduleRecurring(300, 300, new SupervisorHealthCheck(this));
        }
        LOG.info("Starting supervisor with id {} at host {}.", getId(), getHostName());
    }
```
- supervisor launch的时候new了一个ReadClusterState
### ReadClusterState
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/ReadClusterState.java
```
public ReadClusterState(Supervisor supervisor) throws Exception {
        this.superConf = supervisor.getConf();
        this.stormClusterState = supervisor.getStormClusterState();
        this.syncSupEventManager = supervisor.getEventManger();
        this.assignmentVersions = new AtomicReference<Map<String, VersionedData<Assignment>>>(new HashMap<String, VersionedData<Assignment>>());
        this.assignmentId = supervisor.getAssignmentId();
        this.iSuper = supervisor.getiSupervisor();
        this.localizer = supervisor.getAsyncLocalizer();
        this.host = supervisor.getHostName();
        this.localState = supervisor.getLocalState();
        this.clusterState = supervisor.getStormClusterState();
        this.cachedAssignments = supervisor.getCurrAssignment();
        
        this.launcher = ContainerLauncher.make(superConf, assignmentId, supervisor.getSharedContext());
        
        @SuppressWarnings("unchecked")
        List<Number> ports = (List<Number>)superConf.get(Config.SUPERVISOR_SLOTS_PORTS);
        for (Number port: ports) {
            slots.put(port.intValue(), mkSlot(port.intValue()));
        }
        
        try {
            Collection<String> workers = SupervisorUtils.supervisorWorkerIds(superConf);
            for (Slot slot: slots.values()) {
                String workerId = slot.getWorkerId();
                if (workerId != null) {
                    workers.remove(workerId);
                }
            }
            if (!workers.isEmpty()) {
                supervisor.killWorkers(workers, launcher);
            }
        } catch (Exception e) {
            LOG.warn("Error trying to clean up old workers", e);
        }
        //All the slots/assignments should be recovered now, so we can clean up anything that we don't expect to be here
        try {
            localizer.cleanupUnusedTopologies();
        } catch (Exception e) {
            LOG.warn("Error trying to clean up old topologies", e);
        }
        
        for (Slot slot: slots.values()) {
            slot.start();
        }
    }
    private Slot mkSlot(int port) throws Exception {
        return new Slot(localizer, superConf, launcher, host, port,
                localState, clusterState, iSuper, cachedAssignments);
    }
```
- 这里读取SUPERVISOR_SLOTS_PORTS(`supervisor.slots.ports`)，默认是[6700,6701,6702,6703]
- 通过ContainerLauncher.make(superConf, assignmentId, supervisor.getSharedContext())创建ContainerLauncher
- 根据slots的port配置调用mkSlot创建slot，最后挨个调用slot的start，启动slot线程
### ContainerLauncher.make
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/ContainerLauncher.java
```
/**
     * Factory to create the right container launcher 
     * for the config and the environment.
     * @param conf the config
     * @param supervisorId the ID of the supervisor
     * @param sharedContext Used in local mode to let workers talk together without netty
     * @return the proper container launcher
     * @throws IOException on any error
     */
    public static ContainerLauncher make(Map<String, Object> conf, String supervisorId, IContext sharedContext) throws IOException {
        if (ConfigUtils.isLocalMode(conf)) {
            return new LocalContainerLauncher(conf, supervisorId, sharedContext);
        }
        
        if (Utils.getBoolean(conf.get(Config.SUPERVISOR_RUN_WORKER_AS_USER), false)) {
            return new RunAsUserContainerLauncher(conf, supervisorId);
        }
        return new BasicContainerLauncher(conf, supervisorId);
    }
```
- 这里根据配置来创建ContainerLauncher的不同子类，local模式的创建的是LocalContainerLauncher；要求runAsUser的创建的是RunAsUserContainerLauncher；其他的创建的是BasicContainerLauncher
## Slot
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/Slot.java
```
public void run() {
        try {
            while(!done) {
                Set<TopoProfileAction> origProfileActions = new HashSet<>(profiling.get());
                Set<TopoProfileAction> removed = new HashSet<>(origProfileActions);
                
                DynamicState nextState = 
                        stateMachineStep(dynamicState.withNewAssignment(newAssignment.get())
                                .withProfileActions(origProfileActions, dynamicState.pendingStopProfileActions), staticState);
                if (LOG.isDebugEnabled() || dynamicState.state != nextState.state) {
                    LOG.info("STATE {} -> {}", dynamicState, nextState);
                }
                //Save the current state for recovery
                if (!equivalent(nextState.currentAssignment, dynamicState.currentAssignment)) {
                    LOG.info("SLOT {}: Changing current assignment from {} to {}", staticState.port, dynamicState.currentAssignment, nextState.currentAssignment);
                    saveNewAssignment(nextState.currentAssignment);
                }
                if (equivalent(nextState.newAssignment, nextState.currentAssignment) &&
                    nextState.currentAssignment != null && nextState.currentAssignment.get_owner() == null &&
                    nextState.newAssignment != null && nextState.newAssignment.get_owner() != null) {
                    //This is an odd case for a rolling upgrade where the user on the old assignment may be null,
                    // but not on the new one.  Although in all other ways they are the same.
                    // If this happens we want to use the assignment with the owner.
                    LOG.info("Updating assignment to save owner {}", nextState.newAssignment.get_owner());
                    saveNewAssignment(nextState.newAssignment);
                    nextState = nextState.withCurrentAssignment(nextState.container, nextState.newAssignment);
                }
                
                // clean up the profiler actions that are not being processed
                removed.removeAll(dynamicState.profileActions);
                removed.removeAll(dynamicState.pendingStopProfileActions);
                for (TopoProfileAction action: removed) {
                    try {
                        clusterState.deleteTopologyProfileRequests(action.topoId, action.request);
                    } catch (Exception e) {
                        LOG.error("Error trying to remove profiling request, it will be retried", e);
                    }
                }
                Set<TopoProfileAction> orig, copy;
                do {
                    orig = profiling.get();
                    copy = new HashSet<>(orig);
                    copy.removeAll(removed);
                } while (!profiling.compareAndSet(orig, copy));
                dynamicState = nextState;
            }
        } catch (Throwable e) {
            if (!Utils.exceptionCauseIsInstanceOf(InterruptedException.class, e)) {
                LOG.error("Error when processing event", e);
                Utils.exitProcess(20, "Error when processing an event");
            }
        }
    }
    private void saveNewAssignment(LocalAssignment assignment) {
        synchronized(staticState.localState) {
            Map<Integer, LocalAssignment> assignments = staticState.localState.getLocalAssignmentsMap();
            if (assignments == null) {
                assignments = new HashMap<>();
            }
            if (assignment == null) {
                assignments.remove(staticState.port);
            } else {
                assignments.put(staticState.port, assignment);
            }
            staticState.localState.setLocalAssignmentsMap(assignments);
        }
        Map<Long, LocalAssignment> update = null;
        Map<Long, LocalAssignment> orig = null;
        do {
            Long lport = new Long(staticState.port);
            orig = cachedCurrentAssignments.get();
            update = new HashMap<>(orig);
            if (assignment == null) {
                update.remove(lport);
            } else {
                update.put(lport, assignment);
            }
        } while (!cachedCurrentAssignments.compareAndSet(orig, update));
    }
    static DynamicState stateMachineStep(DynamicState dynamicState, StaticState staticState) throws Exception {
        LOG.debug("STATE {}", dynamicState.state);
        switch (dynamicState.state) {
            case EMPTY:
                return handleEmpty(dynamicState, staticState);
            case RUNNING:
                return handleRunning(dynamicState, staticState);
            case WAITING_FOR_WORKER_START:
                return handleWaitingForWorkerStart(dynamicState, staticState);
            case KILL_AND_RELAUNCH:
                return handleKillAndRelaunch(dynamicState, staticState);
            case KILL:
                return handleKill(dynamicState, staticState);
            case WAITING_FOR_BASIC_LOCALIZATION:
                return handleWaitingForBasicLocalization(dynamicState, staticState);
            case WAITING_FOR_BLOB_LOCALIZATION:
                return handleWaitingForBlobLocalization(dynamicState, staticState);
            default:
                throw new IllegalStateException("Code not ready to handle a state of "+dynamicState.state);
        }
    }
```
- 不断循环stateMachineStep方法切换state
- 当state是WAITING_FOR_BLOB_LOCALIZATION时，会触发handleWaitingForBlobLocalization
### handleWaitingForBlobLocalization
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/Slot.java
```
/**
     * State Transitions for WAITING_FOR_BLOB_LOCALIZATION state.
     * PRECONDITION: neither pendingLocalization nor pendingDownload is null.
     * PRECONDITION: The slot should be empty
     * @param dynamicState current state
     * @param staticState static data
     * @return the next state
     * @throws Exception on any error
     */
    static DynamicState handleWaitingForBlobLocalization(DynamicState dynamicState, StaticState staticState) throws Exception {
        assert(dynamicState.pendingLocalization != null);
        assert(dynamicState.pendingDownload != null);
        assert(dynamicState.container == null);
        
        //Ignore changes to scheduling while downloading the topology blobs
        // We don't support canceling the download through the future yet,
        // so to keep everything in sync, just wait
        try {
            dynamicState.pendingDownload.get(1000, TimeUnit.MILLISECONDS);
            //Downloading of all blobs finished.
            if (!equivalent(dynamicState.newAssignment, dynamicState.pendingLocalization)) {
                //Scheduling changed
                staticState.localizer.releaseSlotFor(dynamicState.pendingLocalization, staticState.port);
                return prepareForNewAssignmentNoWorkersRunning(dynamicState, staticState);
            }
            Container c = staticState.containerLauncher.launchContainer(staticState.port, dynamicState.pendingLocalization, staticState.localState);
            return dynamicState.withCurrentAssignment(c, dynamicState.pendingLocalization).withState(MachineState.WAITING_FOR_WORKER_START).withPendingLocalization(null, null);
        } catch (TimeoutException e) {
            //We waited for 1 second loop around and try again....
            return dynamicState;
        }
    }
```
- 这里通过staticState.containerLauncher.launchContainer去启动container
## BasicContainerLauncher.launchContainer
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/BasicContainerLauncher.java
```
@Override
    public Container launchContainer(int port, LocalAssignment assignment, LocalState state) throws IOException {
        LocalContainer ret = new LocalContainer(_conf, _supervisorId, port, assignment, _sharedContext);
        ret.setup();
        ret.launch();
        return ret;
    }
```
- launchContainer的时候，先调用setup，再调用launch方法
### Container.setup
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/Container.java
```
/**
     * Setup the container to run.  By default this creates the needed directories/links in the
     * local file system
     * PREREQUISITE: All needed blobs and topology, jars/configs have been downloaded and
     * placed in the appropriate locations
     * @throws IOException on any error
     */
    protected void setup() throws IOException {
        _type.assertFull();
        if (!_ops.doRequiredTopoFilesExist(_conf, _topologyId)) {
            LOG.info("Missing topology storm code, so can't launch  worker with assignment {} for this supervisor {} on port {} with id {}", _assignment,
                    _supervisorId, _port, _workerId);
            throw new IllegalStateException("Not all needed files are here!!!!");
        } 
        LOG.info("Setting up {}:{}", _supervisorId, _workerId);
        _ops.forceMkdir(new File(ConfigUtils.workerPidsRoot(_conf, _workerId)));
        _ops.forceMkdir(new File(ConfigUtils.workerTmpRoot(_conf, _workerId)));
        _ops.forceMkdir(new File(ConfigUtils.workerHeartbeatsRoot(_conf, _workerId)));
        
        File workerArtifacts = new File(ConfigUtils.workerArtifactsRoot(_conf, _topologyId, _port));
        if (!_ops.fileExists(workerArtifacts)) {
            _ops.forceMkdir(workerArtifacts);
            _ops.setupWorkerArtifactsDir(_assignment.get_owner(), workerArtifacts);
        }
    
        String user = getWorkerUser();
        writeLogMetadata(user);
        saveWorkerUser(user);
        createArtifactsLink();
        createBlobstoreLinks();
    }
```
- setup主要做一些创建目录或链接的准备工作
### BasicContainer.launch
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/BasicContainer.java
```
public void launch() throws IOException {
        _type.assertFull();
        LOG.info("Launching worker with assignment {} for this supervisor {} on port {} with id {}", _assignment,
                _supervisorId, _port, _workerId);
        String logPrefix = "Worker Process " + _workerId;
        ProcessExitCallback processExitCallback = new ProcessExitCallback(logPrefix);
        _exitedEarly = false;
        
        final WorkerResources resources = _assignment.get_resources();
        final int memOnheap = getMemOnHeap(resources);
        final String stormRoot = ConfigUtils.supervisorStormDistRoot(_conf, _topologyId);
        final String jlp = javaLibraryPath(stormRoot, _conf);
        
        List<String> commandList = mkLaunchCommand(memOnheap, stormRoot, jlp);
        Map<String, String> topEnvironment = new HashMap<String, String>();
        @SuppressWarnings("unchecked")
        Map<String, String> environment = (Map<String, String>) _topoConf.get(Config.TOPOLOGY_ENVIRONMENT);
        if (environment != null) {
            topEnvironment.putAll(environment);
        }
        topEnvironment.put("LD_LIBRARY_PATH", jlp);
        LOG.info("Launching worker with command: {}. ", Utils.shellCmd(commandList));
        String workerDir = ConfigUtils.workerRoot(_conf, _workerId);
        launchWorkerProcess(commandList, topEnvironment, logPrefix, processExitCallback, new File(workerDir));
    }
    /**
     * Launch the worker process (non-blocking)
     * 
     * @param command
     *            the command to run
     * @param env
     *            the environment to run the command
     * @param processExitcallback
     *            a callback for when the process exits
     * @param logPrefix
     *            the prefix to include in the logs
     * @param targetDir
     *            the working directory to run the command in
     * @return true if it ran successfully, else false
     * @throws IOException
     *             on any error
     */
    protected void launchWorkerProcess(List<String> command, Map<String, String> env, String logPrefix,
            ExitCodeCallback processExitCallback, File targetDir) throws IOException {
        SupervisorUtils.launchProcess(command, env, logPrefix, processExitCallback, targetDir);
    }
```
- 这里通过mkLaunchCommand来准备创建命令
- 然后通过SupervisorUtils.launchProcess启动worker进程
### mkLaunchCommand
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/BasicContainerLauncher.java
```
/**
     * Create the command to launch the worker process
     * @param memOnheap the on heap memory for the worker
     * @param stormRoot the root dist dir for the topology
     * @param jlp java library path for the topology
     * @return the command to run
     * @throws IOException on any error.
     */
    private List<String> mkLaunchCommand(final int memOnheap, final String stormRoot,
            final String jlp) throws IOException {
        final String javaCmd = javaCmd("java");
        final String stormOptions = ConfigUtils.concatIfNotNull(System.getProperty("storm.options"));
        final String stormConfFile = ConfigUtils.concatIfNotNull(System.getProperty("storm.conf.file"));
        final String workerTmpDir = ConfigUtils.workerTmpRoot(_conf, _workerId);
        
        List<String> classPathParams = getClassPathParams(stormRoot);
        List<String> commonParams = getCommonParams();
        
        List<String> commandList = new ArrayList<>();
        //Log Writer Command...
        commandList.add(javaCmd);
        commandList.addAll(classPathParams);
        commandList.addAll(substituteChildopts(_topoConf.get(Config.TOPOLOGY_WORKER_LOGWRITER_CHILDOPTS)));
        commandList.addAll(commonParams);
        commandList.add("org.apache.storm.LogWriter"); //The LogWriter in turn launches the actual worker.
        //Worker Command...
        commandList.add(javaCmd);
        commandList.add("-server");
        commandList.addAll(commonParams);
        commandList.addAll(substituteChildopts(_conf.get(Config.WORKER_CHILDOPTS), memOnheap));
        commandList.addAll(substituteChildopts(_topoConf.get(Config.TOPOLOGY_WORKER_CHILDOPTS), memOnheap));
        commandList.addAll(substituteChildopts(OR(
                _topoConf.get(Config.TOPOLOGY_WORKER_GC_CHILDOPTS),
                _conf.get(Config.WORKER_GC_CHILDOPTS)), memOnheap));
        commandList.addAll(getWorkerProfilerChildOpts(memOnheap));
        commandList.add("-Djava.library.path=" + jlp);
        commandList.add("-Dstorm.conf.file=" + stormConfFile);
        commandList.add("-Dstorm.options=" + stormOptions);
        commandList.add("-Djava.io.tmpdir=" + workerTmpDir);
        commandList.addAll(classPathParams);
        commandList.add("org.apache.storm.daemon.worker");
        commandList.add(_topologyId);
        commandList.add(_supervisorId);
        commandList.add(String.valueOf(_port));
        commandList.add(_workerId);
        
        return commandList;
    }
```
- 启动参数实例
`/usr/lib/jvm/java-1.8-openjdk/jre/bin/java -server -Dlogging.sensitivity=S3 -Dlogfile.name=worker.log -Dstorm.home=/apache-storm-1.2.2 -Dworkers.artifacts=/logs/workers-artifacts -Dstorm.id=DemoTopology-1-1539163962 -Dworker.id=f0f30bc3-11af-4f4f-b2dd-8cc92d8791bf -Dworker.port=6700 -Dstorm.log.dir=/logs -Dlog4j.configurationFile=/apache-storm-1.2.2/log4j2/worker.xml -DLog4jContextSelector=org.apache.logging.log4j.core.selector.BasicContextSelector -Dstorm.local.dir=/data -Xmx768m -XX:+PrintGCDetails -Xloggc:artifacts/gc.log -XX:+PrintGCDateStamps -XX:+PrintGCTimeStamps -XX:+UseGCLogFileRotation -XX:NumberOfGCLogFiles=10 -XX:GCLogFileSize=1M -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=artifacts/heapdump -Djava.library.path=/data/supervisor/stormdist/DemoTopology-1-1539163962/resources/Linux-amd64:/data/supervisor/stormdist/DemoTopology-1-1539163962/resources:/usr/local/lib:/opt/local/lib:/usr/lib -Dstorm.conf.file= -Dstorm.options=storm.local.hostname%3D192.168.99.100 -Djava.io.tmpdir=/data/workers/f0f30bc3-11af-4f4f-b2dd-8cc92d8791bf/tmp -cp /apache-storm-1.2.2/lib/*:/apache-storm-1.2.2/extlib/*:/conf:/data/supervisor/stormdist/DemoTopology-1-1539163962/stormjar.jar org.apache.storm.daemon.worker DemoTopology-1-1539163962 8dd6dc7f-95cb-49f9-9bd1-f0d638fe6fc6 6700 f0f30bc3-11af-4f4f-b2dd-8cc92d8791bf`- org.apache.storm.daemon.worker"的路径为storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/worker.clj
### SupervisorUtils.launchProcess
storm-core-1.2.2-sources.jar!/org/apache/storm/daemon/supervisor/SupervisorUtils.java
```
/**
     * Launch a new process as per {@link java.lang.ProcessBuilder} with a given
     * callback.
     * @param command the command to be executed in the new process
     * @param environment the environment to be applied to the process. Can be
     *                    null.
     * @param logPrefix a prefix for log entries from the output of the process.
     *                  Can be null.
     * @param exitCodeCallback code to be called passing the exit code value
     *                         when the process completes
     * @param dir the working directory of the new process
     * @return the new process
     * @throws IOException
     * @see java.lang.ProcessBuilder
     */
    public static Process launchProcess(List<String> command,
                                        Map<String,String> environment,
                                        final String logPrefix,
                                        final ExitCodeCallback exitCodeCallback,
                                        File dir)
            throws IOException {
        ProcessBuilder builder = new ProcessBuilder(command);
        Map<String,String> procEnv = builder.environment();
        if (dir != null) {
            builder.directory(dir);
        }
        builder.redirectErrorStream(true);
        if (environment != null) {
            procEnv.putAll(environment);
        }
        final Process process = builder.start();
        if (logPrefix != null || exitCodeCallback != null) {
            Utils.asyncLoop(new Callable<Object>() {
                public Object call() {
                    if (logPrefix != null ) {
                        Utils.readAndLogStream(logPrefix,
                                process.getInputStream());
                    }
                    if (exitCodeCallback != null) {
                        try {
                            process.waitFor();
                            exitCodeCallback.call(process.exitValue());
                        } catch (InterruptedException ie) {
                            LOG.info("{} interrupted", logPrefix);
                            exitCodeCallback.call(-1);
                        }
                    }
                    return null; // Run only once.
                }
            });
        }
        return process;
    }
```
- 这里通过ProcessBuilder来启动进程
## 小结
- storm的supervisor启动的时候，会创建ContainerLauncher以及根据SUPERVISOR_SLOTS_PORTS(`supervisor.slots.ports`)创建slots
- slot线程会不断循环state，在WAITING_FOR_BLOB_LOCALIZATION的时候使用ContainerLauncher的launchContainer创建Container并launch
- container launch的时候通过SupervisorUtils.launchProcess(`使用ProcessBuilder`)启动worker进程
## doc
- [Storm Concepts](http://storm.apache.org/releases/1.2.2/Concepts.html)
