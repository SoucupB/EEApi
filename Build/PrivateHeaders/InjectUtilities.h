#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>
#include <vector>

using namespace std;

enum DataType {
  POINTER_TYPE,
  FLOAT_TYPE,
  INT32_T_TYPE,
  INT8_T_TYPE
};

uint32_t util_FindProcID(const char *procname);
LPVOID util_RemoteInjector(DWORD targetProcessId, unsigned char *injectedCode, SIZE_T injectedSize, PVOID buffer);
PVOID util_AddMethod(DWORD targetProcessId, unsigned char *injectedCode, SIZE_T injectedSize);
PVOID util_ReadProcessMemory(DWORD targetProcessId, PVOID at, SIZE_T size);
void util_ReadMethodAtAdress(void *address, SIZE_T methodSize);
PVOID util_FindEmptyAddressSpace(DWORD targetProcessId, DWORD bufferSize);
PVOID util_GetBytesAt(DWORD targetProcessId, PVOID remoteBuffer, DWORD bufferSize);
void util_WriteProcessMemory(DWORD targetProcessId, PVOID localBuffer, PVOID remoteBuffer, SIZE_T size);

uint8_t util_ModifyJumpAddress(DWORD targetProcessId, SIZE_T address, SIZE_T jumpAt);
vector<PVOID> util_FindAllCalls(DWORD targetProcessId, SIZE_T callAddress, SIZE_T programSize);
DWORD_PTR util_GetBaseAddress(DWORD processID);
LPVOID util_AllocRemote(DWORD targetProcessId, SIZE_T size, PVOID localBuffer);
void util_CallRemoveThread(DWORD targetProcessId, PVOID methodAddress, PVOID buffer);
void util_ChangeProtectionWrite(DWORD targetProcessId, PVOID remoteBuffer, size_t size);
bool util_LoadDLL(DWORD processID, const string &dllName, const std::string& dllPath);
DWORD util_GetLastErrorInTargetProcess(HANDLE hProcess);
PVOID util_CopyWrittenFile(DWORD targetProcessId, const char *writtenFile);
void util_ModifyTargetProcessCaller(uint8_t *address, SIZE_T jumpAt);

// This method only works for methods that are inside 'method' block and have thee same standard method header with %rsp and %rbp registers
void util_ExtractMethod(PVOID method, BYTE *methodBytes, SIZE_T *size);
HMODULE util_GetRemoteModuleHandle(DWORD processID, const char* dllName);

void *util_Pointer(void *source, size_t offset, DataType type);
void util_MessageBox(const char *title, const char *message);
