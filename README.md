# MyShell

A simple custom Linux‑style shell implemented in C++ for Windows using Win32 APIs. It supports built‑in commands (`cd`, `ls`, `cp`, `rm`, `move`/`mv`, `exit`) and can launch external executables.

---

## Prerequisites

* **Windows 10/11**
* **VS Code** with the **C/C++ Extension** (ms‑vscode.cpptools)
* **MinGW‑w64** (GCC) or another C++ toolchain installed

  * If you choose MinGW‑w64, install via MSYS2 or standalone installer
  * Add its `bin` directory (e.g. `C:\msys64\mingw64\bin`) to your **`PATH`** environment variable

---

## Project Structure

```
myshell/              # root folder
├── shell.cpp       # main source file
└── .vscode/          # VS Code configuration
    ├── tasks.json    # build task
    └── launch.json   # debug configuration (optional)
```

---

## VS Code Configuration

1. **tasks.json** (in `.vscode/tasks.json`):

   ```jsonc
   {
     "version": "2.0.0",
     "tasks": [
       {
         "label": "build-myshell",
         "type": "shell",
         "command": "g++",
         "args": [
           "-std=c++17",
           "-g",
           "shell.cpp",
           "-o",
           "shell.exe"
         ],
         "group": { "kind": "build", "isDefault": true }
       }
     ]
   }
   ```



---

## Build & Run

1. **Open** the `myshell` folder in VS Code.
2. Press **Ctrl+Shift+B** to run the **build-myshell** task. This compiles `myshell.exe`.
3. Open the integrated terminal (**View → Terminal** or `` Ctrl+` ``).
4. Execute:

   ```batch
   .\shell.exe
   ```
5. You should see a prompt like:

   ```
   C:\path\to\Myshell>
   ```

---

## Basic Usage

| Command          | Description                         |                                |
| ---------------- | ----------------------------------- | ------------------------------ |
| `cd <dir>`       | Change directory                    |                                |
| `ls [pattern]`   | List files matching `pattern` (`*`) |                                |
| `cp <src> <dst>` | Copy file                           |                                |
| \`rm \<file      | dir>\`                              | Delete file or empty directory |
| \`move           | mv <src> <dst>\`                    | Rename or move file/directory  |
| `exit`           | Exit the shell                      |                                |

**Examples**:

```none
C:\> ls
file1.txt  docs
C:\> cp file1.txt file1_copy.txt
C:\> move file1_copy.txt docs\file1_copy.txt
C:\> cd docs
C:\docs> ls
file1_copy.txt
```

---

## Troubleshooting

* **`g++: command not found`**: Ensure MinGW‑w64’s `bin` folder is in `PATH` and restart VS Code.
* **Permissions errors**: Run VS Code as administrator if accessing protected directories.
* **Debug issues**: Confirm `miDebuggerPath` matches your GDB installation path.

---

Happy coding! Feel free to extend with piping, redirection, or other custom features.
