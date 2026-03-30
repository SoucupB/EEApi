#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "InjectUtilities.h"

typedef struct Reader_t {
  string *configPath;
  string *gamePath;
  string *dllPath;
} Reader;
typedef Reader *PReader;

PReader path_Create(string filePath);
vector<string> split(string &input, string temp);

DWORD startGame(PReader reader) {
  STARTUPINFO si = { sizeof(si) };
  PROCESS_INFORMATION pi;
  const char *exePath = (const char *)reader->gamePath->c_str();
  if (!CreateProcess(
    exePath,     // application name
    nullptr,     // command line arguments
    nullptr,     // process security
    nullptr,     // thread security
    FALSE,       // inherit handles
    0,           // creation flags
    nullptr,     // environment
    nullptr,     // current directory
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
    printf("Config not found or either game_path/dll_path are missing!\n");
    exit(1);
  }
  DWORD pid = startGame(reader);
  if(!pid) {
    printf("PID is 0, the process did not start\n");
    exit(1);
  }
  // DWORD pid = util_FindProcID("EE-AOC.exe");
  // if(!pid) {
  //   printf("Pid not found!\n");
  //   return 0;
  // }
  if(!util_LoadDLL(pid, "Bots.dll", argv[1])) {
    printf("Bots.dll failed to load\n");
    return 0;
  }
  printf("Successfull insert!\n");
}