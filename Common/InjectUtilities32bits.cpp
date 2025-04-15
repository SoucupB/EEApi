#include "InjectUtilities.h"
#include <Windows.h>
#include <psapi.h>
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <stdio.h>

const LPVOID windows_BasePointer = (LPVOID)0x401000;

uint32_t util_FindProcID(const char *procname)
{
  HANDLE hSnapshot;
  PROCESSENTRY32 pe;
  int pid = 0;
  BOOL hResult;
  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (INVALID_HANDLE_VALUE == hSnapshot)
    return 0;
  pe.dwSize = sizeof(PROCESSENTRY32);
  hResult = Process32First(hSnapshot, &pe);
  while (hResult)
  {
    if (strcmp(procname, pe.szExeFile) == 0)
    {
      pid = pe.th32ProcessID;
      break;
    }
    hResult = Process32Next(hSnapshot, &pe);
  }
  CloseHandle(hSnapshot);
  return pid;
}

void util_ReadMethodAtAdress(void *address, SIZE_T methodSize) {
  BYTE *buffer = (BYTE *)address;

  for(SIZE_T i = 0; i < methodSize; i++) {
    printf("0x%02x, ", buffer[i]);
  }
}

uint8_t util_ModifyJumpAddress(DWORD targetProcessId, SIZE_T address, SIZE_T jumpAt) {
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
  if (hProcess == NULL) {
    std::cerr << "Could not open process: " << GetLastError() << std::endl;
    return 0;
  }
  BYTE remoteJumpByteCode[5];
  SIZE_T bytesRead = 0;
  if (!ReadProcessMemory(hProcess, (PVOID)address, remoteJumpByteCode, 5, &bytesRead)) {
    CloseHandle(hProcess);
    return 0;
  }
  if(remoteJumpByteCode[0] != 0xE8) {
    CloseHandle(hProcess);
    return 0;
  }
  int32_t jumpAtBytesCode = (int32_t)jumpAt - ((int32_t)address + 5);
  BYTE *jreAddress = (BYTE *)&jumpAtBytesCode;
  if (!WriteProcessMemory(hProcess, (PVOID)(address + 1), jreAddress, sizeof(int32_t), NULL)) {
    std::cerr << "Failed to write parameter to target process memory." << std::endl;
    CloseHandle(hProcess);
    return 0;
  }
  CloseHandle(hProcess);
  return 1;
}

PVOID util_ReadProcessMemory(DWORD targetProcessId, PVOID at, SIZE_T size) {
  HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, targetProcessId);
  if (hProcess == NULL) {
    std::cerr << "Could not open process: " << GetLastError() << std::endl;
    return NULL;
  }
  PVOID buffer = malloc(size);
  memset(buffer, 0, size);
  SIZE_T bytesRead;
  if (ReadProcessMemory(hProcess, at, buffer, size, &bytesRead)) {
    CloseHandle(hProcess);
    return buffer;
  }
  CloseHandle(hProcess);
  return NULL;
}

LPVOID util_AllocRemote(DWORD targetProcessId, SIZE_T size, PVOID localBuffer) {
  LPVOID hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
  LPVOID paramAddress = VirtualAllocEx(hProcess, NULL, size, (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
  if (paramAddress == NULL) {
    CloseHandle(hProcess);
    return NULL;
  }
  if (!WriteProcessMemory(hProcess, paramAddress, localBuffer, size, NULL)) {
    VirtualFreeEx(hProcess, paramAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return NULL;
  }
  CloseHandle(hProcess);

  return paramAddress;
}

LPVOID util_RemoteInjector(DWORD targetProcessId, unsigned char *injectedCode, SIZE_T injectedSize, PVOID buffer)
{
  LPVOID hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
  LPVOID paramAddress = VirtualAllocEx(hProcess, NULL, injectedSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
  if (paramAddress == NULL)
  {
    std::cerr << "Failed to allocate memory in target process." << std::endl;
    CloseHandle(hProcess);
    return NULL;
  }

  if (!WriteProcessMemory(hProcess, paramAddress, injectedCode, injectedSize, NULL))
  {
    std::cerr << "Failed to write parameter to target process memory." << std::endl;
    VirtualFreeEx(hProcess, paramAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return NULL;
  }
  printf("Code at %p\n", paramAddress);
  CloseHandle(hProcess);
  return paramAddress;
}

PVOID util_CopyWrittenFile(DWORD targetProcessId, const char *writtenFile) {
  FILE *fd = fopen(writtenFile, "rb");
  std::vector<uint8_t> bufferVector;
  for(;;) {
    if(bufferVector.size() >= (1<<17)) {
      break;
    }
    uint8_t element;
    if(fscanf(fd, "%c", &element) == EOF) {
      break;
    }
    bufferVector.push_back(element);
  }
  fclose(fd);
  if(!bufferVector.size()) {
    return NULL;
  }
  uint8_t *buffer = &bufferVector[0];
  printf("Read %d bytes to the external buffer\n", bufferVector.size());
  return util_AddMethod(targetProcessId, buffer, bufferVector.size());
}

void util_CallRemoveThread(DWORD targetProcessId, PVOID methodAddress, PVOID buffer) {
  LPVOID hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
  HANDLE hThread = CreateRemoteThread(
    hProcess,
    NULL,
    0,
    (LPTHREAD_START_ROUTINE)methodAddress,
    buffer,
    0,
    NULL
  );

  WaitForSingleObject(hThread, INFINITE);
  CloseHandle(hProcess);
}

PVOID util_AddMethod(DWORD targetProcessId, unsigned char *injectedCode, SIZE_T injectedSize) {
  LPVOID hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);

  LPVOID paramAddress = VirtualAllocEx(hProcess, NULL, injectedSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
  if (paramAddress == NULL)
  {
    std::cerr << "Failed to allocate memory in target process." << std::endl;
    CloseHandle(hProcess);
    return NULL;
  }

  if (!WriteProcessMemory(hProcess, paramAddress, injectedCode, injectedSize, NULL))
  {
    std::cerr << "Failed to write parameter to target process memory." << std::endl;
    VirtualFreeEx(hProcess, paramAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return NULL;
  }
  CloseHandle(hProcess);

  return paramAddress;
}

void util_WriteProcessMemory(DWORD targetProcessId, PVOID localBuffer, PVOID remoteBuffer, SIZE_T size) {
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
  if (hProcess == NULL) {
    std::cerr << "Could not open process: " << GetLastError() << std::endl;
    return ;
  }
  if (!WriteProcessMemory(hProcess, remoteBuffer, localBuffer, size, NULL))
  {
    printf("Failed to write parameter to target process memory at address %x %d\n", remoteBuffer, GetLastError());
    CloseHandle(hProcess);
    return ;
  }
  CloseHandle(hProcess);
}

void util_ChangeProtectionWrite(DWORD targetProcessId, PVOID remoteBuffer, size_t size) {
  DWORD oldProtect;
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
  if (hProcess == NULL) {
    std::cerr << "Could not open process: " << GetLastError() << std::endl;
    return ;
  }
  if (!VirtualProtectEx(hProcess, remoteBuffer, size, PAGE_READWRITE, &oldProtect)) {
    std::cerr << "Failed to change memory protection. Error: " << GetLastError() << std::endl;
    return;
  }
}

void util_Method(DWORD processID, HANDLE method, PVOID params, size_t paramsSize) {
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
  if (!hProcess) {
    std::cerr << "Failed to open target process." << std::endl;
    return ;
  }
  LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, paramsSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (!pRemoteMemory) {
    CloseHandle(hProcess);
    return ;
  }
  if (!WriteProcessMemory(hProcess, pRemoteMemory, params, paramsSize, NULL)) {
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return ;
  }
  HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)method, pRemoteMemory, 0, NULL);
  if (!hRemoteThread) {
    std::cerr << "Failed to create remote thread in target process." << std::endl;
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return ;
  }
  WaitForSingleObject(hRemoteThread, INFINITE);
  CloseHandle(hRemoteThread);
  CloseHandle(hProcess);
}

HANDLE util_MethodAddresOffset(const char *method) {
  HMODULE dllBase = LoadLibraryA("EmpireEarth\\Bots.dll");
  if (dllBase == NULL) {
    printf("Failed to load DLL!\n");
    return NULL;
  }
  HMODULE methodsAddress = (HMODULE)GetProcAddress(dllBase, (LPCSTR)method);
  if (!methodsAddress) {
    printf("Failed to load method %s at base %p\n", method, dllBase);
    return NULL;
  }
  return (HANDLE)((size_t)methodsAddress - (size_t)dllBase);
}

HANDLE util_RunRemoteThreadMethod(HANDLE hProcess, const char *methodsName) {
  HMODULE hModules[1024];
  DWORD cbNeeded;
  if (!EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
    return NULL;
  }
  for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
    char szModName[MAX_PATH] = {0};
    if (GetModuleBaseNameA(hProcess, hModules[i], szModName, sizeof(szModName))) {
      if (_stricmp(szModName, "Bots.dll") == 0) { // To do
        HANDLE methodsAddressOffset = util_MethodAddresOffset(methodsName);
        if(!methodsAddressOffset) {
          return NULL;
        }
        return (HANDLE)((SIZE_T)methodsAddressOffset + (SIZE_T)hModules[i]);
      }
    }
  }
  return NULL;
}

void util_StartMethodThread(HANDLE hProcess, HMODULE module) {
  HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, 
                                (LPTHREAD_START_ROUTINE)module, 
                                nullptr, 0, nullptr);
  if (!hThread) {
    std::cerr << "Failed to create remote thread for function call. Error: " << GetLastError() << std::endl;
    return ;
  }
  WaitForSingleObject(hThread, INFINITE);
  CloseHandle(hThread);
}

bool util_LoadDLL(DWORD processID, const std::string& dllPath) {
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
  if (!hProcess) {
    std::cerr << "Failed to open target process." << std::endl;
    return false;
  }
  LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllPath.size() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (!pRemoteMemory) {
    std::cerr << "Failed to allocate memory in target process." << std::endl;
    CloseHandle(hProcess);
    return false;
  }
  printf("Remote buffer at %p\n", pRemoteMemory);
  if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath.c_str(), dllPath.size() + 1, NULL)) {
    std::cerr << "Failed to write DLL path to target process memory." << std::endl;
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return false;
  }

  LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
  if (!pLoadLibrary) {
    std::cerr << "Failed to get address of LoadLibraryA." << std::endl;
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return false;
  }

  HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteMemory, 0, NULL);
  if (!hRemoteThread) {
    std::cerr << "Failed to create remote thread in target process." << std::endl;
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return false;
  }
  WaitForSingleObject(hRemoteThread, INFINITE);
  DWORD dllBaseAddress = 0;
  if (!GetExitCodeThread(hRemoteThread, &dllBaseAddress) || dllBaseAddress == 0) {
    std::cerr << "DLL injection failed. Checking target process error..." << std::endl;
    DWORD errorCode = util_GetLastErrorInTargetProcess(hProcess);
    if (errorCode != -1) {
      std::cerr << "Error code from target process: " << errorCode << std::endl;
    } else {
      std::cerr << "Failed to retrieve the error code from the target process." << std::endl;
    }
    return false;
  } else {
    std::cout << "DLL loaded at address: " << std::hex << dllBaseAddress << std::endl;
  }
  HANDLE dllMethodAddress = util_RunRemoteThreadMethod(hProcess, "__Z11someDllMainv");
  printf("Bots.dll method of someDllMain is loaded at %p\n", dllMethodAddress);
  if(!dllMethodAddress) {
    printf("Main method not found!\n");
  }

  if(dllMethodAddress) {
    util_StartMethodThread(hProcess, (HMODULE)dllMethodAddress);
    printf("Method run\n");
  }

  VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
  CloseHandle(hRemoteThread);
  CloseHandle(hProcess);
  printf("DLL injected successfully\n");
  return true;
}

void util_ModifyTargetProcessCaller(uint8_t *address, SIZE_T jumpAt) {
  if(address[0] != 0xE8) {
    return ;
  }
  int32_t jumpAtBytesCode = (int32_t)jumpAt - ((int32_t)address + 5);

  for(uint8_t i = 0; i < sizeof(void *); i++) {
    address[i + 1] = ((uint8_t *)&jumpAtBytesCode)[i];
  }
}

void util_MessageBox(const char *title, const char *message) {
  MessageBox(
      NULL,
      (char *)message,
      (char *)title,
      MB_OK | MB_ICONINFORMATION
  );
}

DWORD util_GetLastErrorInTargetProcess(HANDLE hProcess) {
  LPVOID pGetLastError = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetLastError");
  if (!pGetLastError) {
      std::cerr << "Failed to get address of GetLastError." << std::endl;
      return -1;
  }

  HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pGetLastError, NULL, 0, NULL);
  if (!hRemoteThread) {
      std::cerr << "Failed to create remote thread for GetLastError." << std::endl;
      return -1;
  }

  WaitForSingleObject(hRemoteThread, INFINITE);

  DWORD errorCode = 0;
  if (!GetExitCodeThread(hRemoteThread, &errorCode)) {
      std::cerr << "Failed to get the exit code for GetLastError thread." << std::endl;
      errorCode = -1;
  }

  CloseHandle(hRemoteThread);
  return errorCode;
}

void _util_FindAddresses(PVOID localBuffer, DWORD bufferSize) {
  uint32_t *addressesHolders = (uint32_t *)malloc(sizeof(uint32_t) * bufferSize);
  memset(addressesHolders, 0, sizeof(uint32_t) * bufferSize);
  BYTE *bytesBuffer = (BYTE *)localBuffer;

  for(SIZE_T i = 1; i < bufferSize; i++) {
    if(bytesBuffer[i] == 0x90) {
      addressesHolders[i] = addressesHolders[i - 1] + 1;
    }
  }

  for(SIZE_T i = 0, c = bufferSize - 1; i < c; i++) {
    if(addressesHolders[i] && !addressesHolders[i + 1]) {
      printf("At index 0x%p with size %d\n", i + (SIZE_T)windows_BasePointer - (SIZE_T)addressesHolders[i] + 1, addressesHolders[i]);
    }
  }
}

DWORD_PTR util_GetBaseAddress(DWORD processID) {
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
  if (hSnapshot == INVALID_HANDLE_VALUE) {
      return 0;
  }

  MODULEENTRY32 me32;
  me32.dwSize = sizeof(MODULEENTRY32);
  if (Module32First(hSnapshot, &me32)) {
      CloseHandle(hSnapshot);
      return (DWORD_PTR)me32.modBaseAddr;
  }

  CloseHandle(hSnapshot);
  return 0;
}

PVOID util_FindEmptyAddressSpace(DWORD targetProcessId, DWORD bufferSize) {
  HANDLE processHandle = OpenProcess(PROCESS_VM_READ, FALSE, targetProcessId);
  PVOID localBuffer = malloc(bufferSize * sizeof(BYTE));

  if (processHandle == NULL) {
    free(localBuffer);
    printf("Error on reading the process\n");
    return NULL;
  }
  SIZE_T bytesRead = 0;
  if(!ReadProcessMemory(processHandle, windows_BasePointer, localBuffer, bufferSize * sizeof(BYTE), &bytesRead)) {
    free(localBuffer);
    CloseHandle(processHandle);
    printf("Could not read from target process\n");
    return NULL;
  }
  CloseHandle(processHandle);
  _util_FindAddresses(localBuffer, bufferSize);
  return localBuffer;
}

PVOID util_GetBytesAt(DWORD targetProcessId, PVOID remoteBuffer, DWORD bufferSize) {
  HANDLE processHandle = OpenProcess(PROCESS_VM_READ, FALSE, targetProcessId);
  PVOID localBuffer = malloc(bufferSize);

  if (processHandle == NULL) {
    free(localBuffer);
    printf("Error on reading the process\n");
    return NULL;
  }
  SIZE_T bytesRead = 0;
  ReadProcessMemory(processHandle, remoteBuffer, localBuffer, bufferSize, &bytesRead);

  return localBuffer;
}

uint8_t _isBufferEqual(PVOID at, PVOID checker, size_t size) {
  BYTE *atBuffer = (BYTE *)&at;
  BYTE *checkerBuffer = (BYTE *)checker;

  for(SIZE_T i = 0; i < size; i++) {
    if(atBuffer[i] != checkerBuffer[i]) {
      return 0;
    }
  }

  return 1;
}

void util_ExtractMethod(PVOID method, BYTE *methodBytes, SIZE_T *size) {
  BYTE *methodBuffer = (BYTE *)method;
  SIZE_T maxBytesPerMethod = 1024 * 10;
  methodBytes[0] = methodBuffer[0];
  SIZE_T index = 1;
  
  BYTE nextMethodHeader[] = {0x55, 0x48, 0x89, 0xe5};
  while(index < maxBytesPerMethod) {
    methodBytes[index] = methodBuffer[index];
    if(!memcmp(methodBuffer + index, nextMethodHeader, sizeof(nextMethodHeader))) {
      break;
    }
    index++;
  }
  *size = index;
}

uint8_t getAddressFromCall(PVOID address, SIZE_T *buffer) {
  BYTE *addressBuffer = (BYTE *)address;
  if(addressBuffer[0] != 0xE8) {
    return 0;
  }
  int32_t addressNumber = *(int32_t *)(addressBuffer + 1);
  int32_t newAddress = (int32_t)(SIZE_T)address + addressNumber + 5;
  memcpy(buffer, &newAddress, sizeof(int32_t));
  return 1;
}

vector<PVOID> util_FindAllCalls(DWORD targetProcessId, SIZE_T callAddress, SIZE_T programSize) {
  vector<PVOID> response;
  if(5 > programSize) {
    return response;
  }

  BYTE *memory = (BYTE *)util_GetBytesAt(targetProcessId, windows_BasePointer, programSize);
  SIZE_T buffer = 0;
  if(!memory) {
    printf("Cannot get bytes from find all calls\n");
    return response;
  }
  for(SIZE_T i = 0; i < programSize - 5; i++) {
    uint8_t isMethodValid = getAddressFromCall((PVOID)(memory + i), &buffer);

    if(isMethodValid && buffer - (SIZE_T)memory == callAddress - (SIZE_T)windows_BasePointer) {
      PVOID addr = (PVOID)(i + (SIZE_T)windows_BasePointer);
      response.push_back(addr);
    }
  }
  free(memory);
  return response;
}

void *util_Pointer(void *source, size_t offset, DataType type) {
  switch (type)
  {
    case POINTER_TYPE: {
      return (void *)((size_t *)((int8_t *)source + offset))[0]; 
      break;
    }
    case FLOAT_TYPE: {
      return (float *)((int8_t *)source + offset); 
      break;
    }
    case INT32_T_TYPE: {
      return (int32_t *)((int8_t *)source + offset); 
      break;
    }
    case INT8_T_TYPE: {
      return (int8_t *)((int8_t *)source + offset); 
      break;
    }
    
    default:
      break;
  }

  return NULL;
}
