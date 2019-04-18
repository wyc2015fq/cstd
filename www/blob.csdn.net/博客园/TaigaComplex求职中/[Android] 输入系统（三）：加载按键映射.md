# [Android] 输入系统（三）：加载按键映射 - TaigaComplex求职中 - 博客园







# [[Android] 输入系统（三）：加载按键映射](https://www.cnblogs.com/TaigaCon/p/4763035.html)





# 映射表基本概念

由于Android调用getEvents得到的key是linux发送过来的scan code，而Android处理的是类似于KEY_UP这种统一类型的key code，因此需要有映射表把scan code转换成key code。映射表在板子上的位置是/system/usr/keylayout/xxx.kl，先看一下映射表是什么样子的，下面截选了一段。
key 2     1
key 3     2
key 4     3
key 5     4
key 6     5
key 7     6
key 8     7
key 9     8
key 10    9
key 11    0
key 28    DPAD_CENTER
key 102   HOME
key 103   DPAD_UP           WAKE_DROPPED
key 105   DPAD_LEFT         WAKE_DROPPED
key 106   DPAD_RIGHT        WAKE_DROPPED
key 108   DPAD_DOWN         WAKE_DROPPED
key 111   DEL
key 113   VOLUME_MUTE
key 114   VOLUME_DOWN
key 115   VOLUME_UP
key 116   POWER
可以看到每行都是一个映射项，映射项格式如下：

key  [scan code]  [key label]  [flag label]  [flag label]  ...
- key是关键字，表明这个映射项是作为键值映射
- scan code是从linux device取得的键值
- key label是把scan code映射到key code中间的关键字，通过该关键字可以得到key code。
- flag label即按键的标记的关键字，通过flag label可以得到flag，一行映射项后面可以有多个flag label

从3和4可以知道，还有一个key label到key code的过程，以及flag label到flag的过程



另外，映射表是设备相关的。由于不同设备发送到Android的scan code可能会不同，因此每个设备需要用自身对应的映射表才能正确解析出key code。



# 映射表加载过程

### 1. 获取设备相关信息

在构造EventHub的时候，就决定了需要扫描输入设备。然后会在第一次getEvents进行一次扫描。

扫描输入设备主要有两个目的：
- 得到该设备的各种信息，如：设备名称，设备版本，设备产品码等，这些信息都可以作为该设备的标识。
- 知道该设备所发送事件的类型，如：按键事件，触控事件，滑动事件，开关事件，xy坐标等；通过所发送事件的类型，就能定位出设备的类型。
EventHub::EventHub(void) :
 mNeedToScanDevices(true),
{...}

size_t EventHub::getEvents(int timeoutMillis, RawEvent* buffer, size_t bufferSize) {
        if (mNeedToScanDevices) {
            mNeedToScanDevices = false;
            scanDevicesLocked();
            mNeedToSendFinishedDeviceScan = true;
        }
}

void EventHub::scanDevicesLocked() {
    status_t res = scanDirLocked(DEVICE_PATH);
    if(res < 0) {
        ALOGE("scan dir failed for %s\n", DEVICE_PATH);
    }
    if (mDevices.indexOfKey(VIRTUAL_KEYBOARD_ID) < 0) {
        createVirtualKeyboardLocked();
    }
}


扫描的目录是/dev/input，linux中每加入一个输入设备，都会在该目录下创建设备文件。
status_t EventHub::scanDirLocked(const char *dirname)
{
    char devname[PATH_MAX];
    char *filename;
    DIR *dir;
    struct dirent *de;
    dir = opendir(dirname);
    if(dir == NULL)
        return -1;
    strcpy(devname, dirname);
    filename = devname + strlen(devname);
    *filename++ = '/';
    while((de = readdir(dir))) {
        if(de->d_name[0] == '.' &&
           (de->d_name[1] == '\0' ||
            (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;
        strcpy(filename, de->d_name);
        openDeviceLocked(devname);
    }
    closedir(dir);
    return 0;
}




在openDeviceLocked中就能清晰分析出扫描设备的两个目的
status_t EventHub::openDeviceLocked(const char *devicePath) {

    int fd = open(devicePath, O_RDWR | O_CLOEXEC); 

    // Get device name.
    if(ioctl(fd, EVIOCGNAME(sizeof(buffer) - 1), &buffer) < 1) {
        //fprintf(stderr, "could not get device name for %s, %s\n", devicePath, strerror(errno));
    } else {
        buffer[sizeof(buffer) - 1] = '\0';
        identifier.name.setTo(buffer);
    }

    // Get device driver version.
    int driverVersion;
    if(ioctl(fd, EVIOCGVERSION, &driverVersion)) {
        ALOGE("could not get driver version for %s, %s\n", devicePath, strerror(errno));
        close(fd);
        return -1;
    }

    struct input_id inputId;
    if(ioctl(fd, EVIOCGID, &inputId)) {
        ALOGE("could not get device input id for %s, %s\n", devicePath, strerror(errno));
        close(fd);
        return -1;
    }
    identifier.bus = inputId.bustype;
    identifier.product = inputId.product;
    identifier.vendor = inputId.vendor;
    identifier.version = inputId.version;

    ...

    Device* device = new Device(fd, deviceId, String8(devicePath), identifier);



    // Figure out the kinds of events the device reports.
    ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(device->keyBitmask)), device->keyBitmask);
    ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(device->absBitmask)), device->absBitmask);
    ioctl(fd, EVIOCGBIT(EV_REL, sizeof(device->relBitmask)), device->relBitmask);
    ioctl(fd, EVIOCGBIT(EV_SW, sizeof(device->swBitmask)), device->swBitmask);
    ioctl(fd, EVIOCGBIT(EV_LED, sizeof(device->ledBitmask)), device->ledBitmask);
    ioctl(fd, EVIOCGBIT(EV_FF, sizeof(device->ffBitmask)), device->ffBitmask);
    ioctl(fd, EVIOCGPROP(sizeof(device->propBitmask)), device->propBitmask);

    //mouse device?
    if (test_bit(BTN_MOUSE, device->keyBitmask)
            && test_bit(REL_X, device->relBitmask)
            && test_bit(REL_Y, device->relBitmask)) {
        device->classes |= INPUT_DEVICE_CLASS_CURSOR;
    }

    // See if this is a touch pad.
    // Is this a new modern multi-touch driver?
    if (test_bit(ABS_MT_POSITION_X, device->absBitmask)
            && test_bit(ABS_MT_POSITION_Y, device->absBitmask)) {
        // Some joysticks such as the PS3 controller report axes that conflict
        // with the ABS_MT range.  Try to confirm that the device really is
        // a touch screen.
        if (test_bit(BTN_TOUCH, device->keyBitmask) || !haveGamepadButtons) {
            device->classes |= INPUT_DEVICE_CLASS_TOUCH | INPUT_DEVICE_CLASS_TOUCH_MT;
        }
    // Is this an old style single-touch driver?
    } else if (test_bit(BTN_TOUCH, device->keyBitmask)
            && test_bit(ABS_X, device->absBitmask)
            && test_bit(ABS_Y, device->absBitmask)) {
        device->classes |= INPUT_DEVICE_CLASS_TOUCH;
    }

    // See if this device is a joystick.
    // Assumes that joysticks always have gamepad buttons in order to distinguish them
    // from other devices such as accelerometers that also have absolute axes.
    if (haveGamepadButtons) {
        uint32_t assumedClasses = device->classes | INPUT_DEVICE_CLASS_JOYSTICK;
        for (int i = 0; i <= ABS_MAX; i++) {
            if (test_bit(i, device->absBitmask)
                    && (getAbsAxisUsage(i, assumedClasses) & INPUT_DEVICE_CLASS_JOYSTICK)) {
                device->classes = assumedClasses;
                break;
            }
        }
    }

    ...
}




### 2. 加载映射表

通过设备信息与设备类型，我们就能去加载正确的映射表了
status_t EventHub::openDeviceLocked(const char *devicePath) {
    ...

    if (device->classes & (INPUT_DEVICE_CLASS_KEYBOARD | INPUT_DEVICE_CLASS_JOYSTICK)) {
        // Load the keymap for the device.
        keyMapStatus = loadKeyMapLocked(device);
    }

    ...
}status_t EventHub::loadKeyMapLocked(Device* device) {
    return device->keyMap.load(device->identifier, device->configuration);
}



加载配置文件分为下面几个步骤

1. 通过设备的配置文件去加载配置文件内制定好的映射表

2. 如果1不成功则通过设备信息加载对应的映射表

3. 如果2不成功则加载通用映射表

4. 如果3不成功则加载虚拟映射表


status_t KeyMap::load(const InputDeviceIdentifier& deviceIdenfifier,
        const PropertyMap* deviceConfiguration) {
    // Use the configured key layout if available.
    if (deviceConfiguration) {
        String8 keyLayoutName;
        if (deviceConfiguration->tryGetProperty(String8("keyboard.layout"),
                keyLayoutName)) {
            status_t status = loadKeyLayout(deviceIdenfifier, keyLayoutName);
            if (status == NAME_NOT_FOUND) {
                ALOGE("Configuration for keyboard device '%s' requested keyboard layout '%s' but "
                        "it was not found.",
                        deviceIdenfifier.name.string(), keyLayoutName.string());
            }
        }

        String8 keyCharacterMapName;
        if (deviceConfiguration->tryGetProperty(String8("keyboard.characterMap"),
                keyCharacterMapName)) {
            status_t status = loadKeyCharacterMap(deviceIdenfifier, keyCharacterMapName);
            if (status == NAME_NOT_FOUND) {
                ALOGE("Configuration for keyboard device '%s' requested keyboard character "
                        "map '%s' but it was not found.",
                        deviceIdenfifier.name.string(), keyLayoutName.string());
            }
        }

        if (isComplete()) {
            return OK;
        }
    }

    // Try searching by device identifier.
    if (probeKeyMap(deviceIdenfifier, String8::empty())) {
        return OK;
    }

    // Fall back on the Generic key map.
    // TODO Apply some additional heuristics here to figure out what kind of
    //      generic key map to use (US English, etc.) for typical external keyboards.
    if (probeKeyMap(deviceIdenfifier, String8("Generic"))) {
        return OK;
    }

    // Try the Virtual key map as a last resort.
    if (probeKeyMap(deviceIdenfifier, String8("Virtual"))) {
        return OK;
    }

    // Give up!
    ALOGE("Could not determine key map for device '%s' and no default key maps were found!",
            deviceIdenfifier.name.string());
    return NAME_NOT_FOUND;
}


一般的情况我们会走第2步，因此从probeKeyMap往下分析
bool KeyMap::probeKeyMap(const InputDeviceIdentifier& deviceIdentifier,
        const String8& keyMapName) {
    if (!haveKeyLayout()) {
        loadKeyLayout(deviceIdentifier, keyMapName);
    }
    if (!haveKeyCharacterMap()) {
        loadKeyCharacterMap(deviceIdentifier, keyMapName);
    }
    return isComplete();
}


对于按键，有键盘按键与自定义按键两种，两者加载的文件后缀不同。键盘按键的映射表后缀是.kcm，而自定义按键映射表后缀是.kl。另外两者映射表的格式也不同，我们这里以自定义按键映射表为例，其中有三个步骤：
- 获取映射表文件路径
- 加载映射表文件
- 如果加载映射表文件成功的话，设置该路径为当前设备的自定义映射文件路径。（否则会去解析Generic.kl或者virtual.kl）
status_t KeyMap::loadKeyLayout(const InputDeviceIdentifier& deviceIdentifier,
        const String8& name) {
    String8 path(getPath(deviceIdentifier, name,
            INPUT_DEVICE_CONFIGURATION_FILE_TYPE_KEY_LAYOUT));
    if (path.isEmpty()) {
        return NAME_NOT_FOUND;
    }

    status_t status = KeyLayoutMap::load(path, &keyLayoutMap);
    if (status) {
        return status;
    }

    keyLayoutFile.setTo(path);
    return OK;
}


#### 1. 获取映射表文件路径

我们从加载映射表文件的步骤2进来，那传入的name为空，则调用到getInputDeviceConfigurationFilePathByDeviceIdentifier，即通过设备标识来产生路径
String8 KeyMap::getPath(const InputDeviceIdentifier& deviceIdentifier,
        const String8& name, InputDeviceConfigurationFileType type) {
    return name.isEmpty()
            ? getInputDeviceConfigurationFilePathByDeviceIdentifier(deviceIdentifier, type)
            : getInputDeviceConfigurationFilePathByName(name, type);
}


如果设备标识中的vendor，product，version都不为0的话，表明可以通过这些信息来组合成一个字符串，这个字符串就是映射表文件的前缀，否则，会设备名称deviceIdentifier.name就是映射表文件的前缀。后缀通过type指定。
String8 getInputDeviceConfigurationFilePathByDeviceIdentifier(
        const InputDeviceIdentifier& deviceIdentifier,
        InputDeviceConfigurationFileType type) {
    if (deviceIdentifier.vendor !=0 && deviceIdentifier.product != 0) {
        if (deviceIdentifier.version != 0) {
            // Try vendor product version.
            String8 versionPath(getInputDeviceConfigurationFilePathByName(
                    String8::format("Vendor_%04x_Product_%04x_Version_%04x",
                            deviceIdentifier.vendor, deviceIdentifier.product,
                            deviceIdentifier.version),
                    type));
            if (!versionPath.isEmpty()) {
                return versionPath;
            }
        }

        // Try vendor product.
        String8 productPath(getInputDeviceConfigurationFilePathByName(
                String8::format("Vendor_%04x_Product_%04x",
                        deviceIdentifier.vendor, deviceIdentifier.product),
                type));
        if (!productPath.isEmpty()) {
            return productPath;
        }
    }

    // Try device name.
    return getInputDeviceConfigurationFilePathByName(deviceIdentifier.name, type);
}
假设当前设备的设备名称是input_ir，传入的type是INPUT_DEVICE_CONFIGURATION_FILE_TYPE_KEY_LAYOUT，则设备的文件名为input_ir.kl



#### 2.加载映射表文件

加载映射表文件最终目的是解析该文件得到映射表，其中也分为三个步骤：
- 打开映射表文件
- 创建映射表
- 解析映射表文件并把映射项加入映射表
status_t KeyLayoutMap::load(const String8& filename, sp<KeyLayoutMap>* outMap) {

    status_t status = Tokenizer::open(filename, &tokenizer);
    
    sp<KeyLayoutMap> map = new KeyLayoutMap();
    
    Parser parser(map.get(), tokenizer);
    status = parser.parse();

}



我们直接看最重要的解析部分

parse函数是一个while循环，一行一行地解析映射表项
status_t KeyLayoutMap::Parser::parse() {
    while (!mTokenizer->isEof()) {

        mTokenizer->skipDelimiters(WHITESPACE);

        if (!mTokenizer->isEol() && mTokenizer->peekChar() != '#') {
            String8 keywordToken = mTokenizer->nextToken(WHITESPACE);
            if (keywordToken == "key") {
                mTokenizer->skipDelimiters(WHITESPACE);
                status_t status = parseKey();
                if (status) return status;
            } else if (keywordToken == "axis") {
                mTokenizer->skipDelimiters(WHITESPACE);
                status_t status = parseAxis();
                if (status) return status;
            } else {
                ALOGE("%s: Expected keyword, got '%s'.", mTokenizer->getLocation().string(),
                        keywordToken.string());
                return BAD_VALUE;
            }

            mTokenizer->skipDelimiters(WHITESPACE);
            if (!mTokenizer->isEol() && mTokenizer->peekChar() != '#') {
                ALOGE("%s: Expected end of line or trailing comment, got '%s'.",
                        mTokenizer->getLocation().string(),
                        mTokenizer->peekRemainderOfLine().string());
                return BAD_VALUE;
            }
        }

        mTokenizer->nextLine();
    }
    return NO_ERROR;
}
每一行的解析步骤如下：
- 跳过行首的空格符
- 如果开头第一个字符是”#”，跳过当前行
- 如果开头的关键词是key，跳过空白分割符，调用parseKey解析，如果解析出错则返回错误
- 如果开头的关键词是axis，跳过空白分隔符，调用parseAxis解析，如果解析出错则返回错误
- 如果开头的关键词是其他的词，说明这个映射表文件有误，返回错误
- 跳过行末的空格符
- 如果行末还有”#”以外的字符，说明这个映射表文件有误，返回错误





下面以parseKey为例，分析它是怎么解析出scan code与key code的（由于我们没用到usage code，所以忽略usage，直接分析scan code流程）
status_t KeyLayoutMap::Parser::parseKey() {
    String8 codeToken = mTokenizer->nextToken(WHITESPACE);

    //scan code从字符串转换成数字
    int32_t code = int32_t(strtol(codeToken.string(), &end, 0));
    if (*end) {
        return BAD_VALUE;
    }

    //我们用的是scan code
    KeyedVector<int32_t, Key>& map =
            mapUsage ? mMap->mKeysByUsageCode : mMap->mKeysByScanCode;

    //如果有重复的scan code，会出错返回
    if (map.indexOfKey(code) >= 0) {
        ALOGE("%s: Duplicate entry for key %s '%s'.", mTokenizer->getLocation().string(),
                mapUsage ? "usage" : "scan code", codeToken.string());
        return BAD_VALUE;
    }


    mTokenizer->skipDelimiters(WHITESPACE);
    String8 keyCodeToken = mTokenizer->nextToken(WHITESPACE);

    //通过label获取key code
    int32_t keyCode = getKeyCodeByLabel(keyCodeToken.string());
    if (!keyCode) {
        ALOGE("%s: Expected key code label, got '%s'.", mTokenizer->getLocation().string(),
                keyCodeToken.string());
        return BAD_VALUE;
    }

    //key label后可以接flag，flag从getKeyFlagByLabel解析
    uint32_t flags = 0;
    for (;;) {
        mTokenizer->skipDelimiters(WHITESPACE);
        if (mTokenizer->isEol() || mTokenizer->peekChar() == '#') break;

        String8 flagToken = mTokenizer->nextToken(WHITESPACE);
        uint32_t flag = getKeyFlagByLabel(flagToken.string());
        if (!flag) {
            ALOGE("%s: Expected key flag label, got '%s'.", mTokenizer->getLocation().string(),
                    flagToken.string());
            return BAD_VALUE;
        }
        if (flags & flag) {
            ALOGE("%s: Duplicate key flag '%s'.", mTokenizer->getLocation().string(),
                    flagToken.string());
            return BAD_VALUE;
        }
        flags |= flag;
    }

    Key key;
    key.keyCode = keyCode;
    key.flags = flags;
    map.add(code, key);
    return NO_ERROR;
}


我们在前面说过，还有个从key label到key code的流程，该流程就是在getKeyCodeByLabel中实现的
int32_t getKeyCodeByLabel(const char* label) {
    return int32_t(lookupValueByLabel(label, KEYCODES));
}
最终从KEYCODES这个列表内，根据label查找key code
static const KeycodeLabel KEYCODES[] = {
    { "SOFT_LEFT", 1 },
    { "SOFT_RIGHT", 2 },
    { "HOME", 3 },
    { "BACK", 4 },
    { "CALL", 5 },
    { "ENDCALL", 6 },
    { "0", 7 },
    { "1", 8 },
    { "2", 9 },
    { "3", 10 },
    { "4", 11 },
    { "5", 12 },
    { "6", 13 },
    { "7", 14 },
    { "8", 15 },
    { "9", 16 },
    ...
}


同理，在解析flag的时候也是从FLAGS这个列表内查找flag
uint32_t getKeyFlagByLabel(const char* label) {
    return uint32_t(lookupValueByLabel(label, FLAGS));
}

// NOTE: If you edit these flags, also edit policy flags in Input.h.
static const KeycodeLabel FLAGS[] = {
    { "WAKE", 0x00000001 },
    { "WAKE_DROPPED", 0x00000002 },
    { "SHIFT", 0x00000004 },
    { "CAPS_LOCK", 0x00000008 },
    { "ALT", 0x00000010 },
    { "ALT_GR", 0x00000020 },
    { "MENU", 0x00000040 },
    { "LAUNCHER", 0x00000080 },
    { "VIRTUAL", 0x00000100 },
    { "FUNCTION", 0x00000200 },
    { NULL, 0 }
};











