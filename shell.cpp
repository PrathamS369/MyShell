// myshell.cpp
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// Split a line into tokens by whitespace
static std::vector<std::string> split(const std::string &line)
{
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string tok;
    while (iss >> tok)
        tokens.push_back(tok);
    return tokens;
}

// Run an external command via Win32 CreateProcess
bool run_command(const std::string &cmd, const std::vector<std::string> &args)
{
    // Build command line: "cmd" "arg1" "arg2" ...
    std::string cmdline = "\"" + cmd + "\"";
    for (auto &a : args)
    {
        cmdline += " \"" + a + "\"";
    }

    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    BOOL ok = CreateProcess(
        nullptr,
        cmdline.data(),   // command line
        nullptr, nullptr, // default security
        FALSE,            // no handle inheritance
        0,                // creation flags
        nullptr, nullptr, // use current env & cwd
        &si, &pi);
    if (!ok)
    {
        std::cerr << "myshell: CreateProcess failed (" << GetLastError() << ")\n";
        return false;
    }
    // wait for completion
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return (exitCode == 0);
}

// Builtin: change directory
bool builtin_cd(const std::vector<std::string> &args)
{
    if (args.size() != 2)
    {
        std::cerr << "cd: usage: cd <directory>\n";
        return false;
    }
    if (!SetCurrentDirectory(args[1].c_str()))
    {
        std::cerr << "cd: cannot change to '" << args[1]
                  << "' (" << GetLastError() << ")\n";
        return false;
    }
    return true;
}

// Builtin: list directory contents
void builtin_ls(const std::vector<std::string> &args)
{
    std::string pattern = "*";
    if (args.size() == 2)
    {
        pattern = args[1];
    }
    else if (args.size() > 2)
    {
        std::cerr << "ls: usage: ls [pattern]\n";
        return;
    }

    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE)
    {
        std::cerr << "ls: failed to list directory or no match\n";
        return;
    }
    do
    {
        std::cout << fd.cFileName << "\n";
    } while (FindNextFile(h, &fd));
    FindClose(h);
}

// Builtin: copy file
bool builtin_cp(const std::vector<std::string> &args)
{
    if (args.size() != 3)
    {
        std::cerr << "cp: usage: cp <source> <destination>\n";
        return false;
    }
    if (!CopyFile(args[1].c_str(), args[2].c_str(), FALSE))
    {
        std::cerr << "cp: error copying '" << args[1]
                  << "' to '" << args[2] << "' (" << GetLastError() << ")\n";
        return false;
    }
    return true;
}

// Builtin: remove file or empty directory
bool builtin_rm(const std::vector<std::string> &args)
{
    if (args.size() != 2)
    {
        std::cerr << "rm: usage: rm <file|directory>\n";
        return false;
    }
    const char *target = args[1].c_str();
    if (DeleteFile(target) || RemoveDirectory(target))
    {
        return true;
    }
    std::cerr << "rm: failed to remove '" << target << "' ("
              << GetLastError() << ")\n";
    return false;
}

// Builtin: move (rename) file or directory
bool builtin_move(const std::vector<std::string> &args)
{
    if (args.size() != 3)
    {
        std::cerr << "move: usage: move <source> <destination>\n";
        return false;
    }
    const char *src = args[1].c_str();
    const char *dst = args[2].c_str();
    // MoveFile will fail if dst exists; MoveFileEx with MOVEFILE_REPLACE_EXISTING could overwrite
    if (!MoveFile(src, dst))
    {
        std::cerr << "move: error moving '" << src
                  << "' to '" << dst << "' (" << GetLastError() << ")\n";
        return false;
    }
    return true;
}

int main()
{
    std::string line;
    char cwd[MAX_PATH];

    while (true)
    {
        // Get & show current directory
        if (GetCurrentDirectory(MAX_PATH, cwd))
        {
            std::cout << cwd << "> ";
        }
        else
        {
            std::cout << "myshell> ";
        }

        if (!std::getline(std::cin, line))
        {
            // EOF — exit
            std::cout << "\n";
            break;
        }
        auto tokens = split(line);
        if (tokens.empty())
            continue;

        const auto &cmd = tokens[0];
        if (cmd == "exit")
        {
            break;

            // Built‑ins
        }
        else if (cmd == "cd")
        {
            builtin_cd(tokens);
        }
        else if (cmd == "ls")
        {
            builtin_ls(tokens);
        }
        else if (cmd == "cp")
        {
            builtin_cp(tokens);
        }
        else if (cmd == "rm")
        {
            builtin_rm(tokens);
        }
        else if (cmd == "move" || cmd == "mv")
        {
            builtin_move(tokens);

            // Otherwise, external command
        }
        else
        {
            std::vector<std::string> args(tokens.begin() + 1, tokens.end());
            run_command(cmd, args);
        }
    }

    return 0;
}
