#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "InjectUtilities.h"

typedef struct Reader_t {
  string *configPath;
  string *gamePath;
  string *dllPath;
  string *gameName;
  string *dllName;
} Reader;

typedef Reader *PReader;

PReader path_Create(string filePath);
vector<string> split(string &input, string temp);

DWORD startGame(PReader reader) {
  STARTUPINFO si = { sizeof(si) };
  PROCESS_INFORMATION pi;
  const char *exeName = (const char *)reader->gameName->c_str();
  string fullPath = *reader->gamePath + "\\" + *reader->gameName;
  const char *fullPathChar = fullPath.c_str();
  printf("Trying to start the game %s at path %s\n", exeName, reader->gamePath->c_str());
  if (!CreateProcess(
    fullPathChar,     // application name
    nullptr,     // command line arguments
    nullptr,     // process security
    nullptr,     // thread security
    FALSE,       // inherit handles
    0,           // creation flags
    nullptr,     // environment
    (const char *)reader->gamePath->c_str(),     // current directory
    &si,         // startup info
    &pi          // process info
  )) {
    printf("Failed to start process. Error %d\n", GetLastError());
    return 0;
  }
  return pi.dwProcessId;
}

int main(int argc, char *argv[]) {
  PReader reader = path_Create("Config.txt");
  if(!reader) {
    printf("Config not found or either game_path/dll_path/game_name/dll_name are missing!\n");
    exit(1);
  }
  DWORD pid = startGame(reader);
  if(!pid) {
    printf("PID is 0, the process did not start\n");
    exit(1);
  }
  printf("PID of the process is %d\n", pid);
  // DWORD pid = util_FindProcID("EE-AOC.exe");
  // if(!pid) {
  //   printf("Pid not found!\n");
  //   return 0;
  // }
  string fullDLLPath = *reader->dllPath + "\\" + *reader->dllName;
  if(!util_LoadDLL(pid, *reader->dllName, fullDLLPath)) {
    printf("Bots.dll failed to load\n");
    return 0;
  }
  printf("Successfull insert!\n");
}