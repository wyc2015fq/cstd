# VSCode笔记

Ctrl+Shift+P 输入 json 找到配置文件

## setting.json

```json
{
    "code-runner.saveAllFilesBeforeRun": true,

    "code-runner.executorMap": {
        "cpp": "cd $dir && g++ $fileName -o $fileNameWithoutExt -I/root/cstd/include && $dir$fileNameWithoutExt"
    },
    "files.associations": {
        "typeinfo": "cpp"
    },
    "code-runner.runInTerminal": true,
    "explorer.confirmDelete": false,
}
```



## launch.json

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "/root/testc/testcpp",
            "args": [],
            "stopAtEntry": false,
            "cwd": "/root/testc",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

## c_cpp_properties.json

```
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "D:/code/git/ywlydd/cstd"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "8.1",
            "compilerPath": "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/cl.exe",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "msvc-x64"
        }
    ],
    "version": 4
}
```

## keybindings.json

```
[
    {
        "key": "f7",
        "command": "workbench.action.tasks.build"
    },
    {
        "key": "ctrl+shift+b",
        "command": "-workbench.action.tasks.build"
    }
]
```

