#include "memory.h"

#include <psapi.h>
#include <tchar.h>

namespace kengine {

Memory::Memory() {
  process_handle_ = NULL;
}

const bool Memory::Attached(const TCHAR* name) {
  PROCESSENTRY32 pe32 = ProcessInfo(name);
  if (-1 == pe32.th32ProcessID) {
    return false;
  }
  process_handle_ = OpenProcess(PROCESS_ALL_ACCESS, 0, pe32.th32ProcessID);
  if (0 == process_handle_) {
    return false;
  }
  return true;
}

const PROCESSENTRY32 Memory::ProcessInfo(const TCHAR* name) {
  PROCESSENTRY32 pe32 = { 0 };
  HANDLE hSnapshot = NULL;
  pe32.dwSize = sizeof(PROCESSENTRY32);
  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (Process32First(hSnapshot, &pe32)) {
    do {
      if (0 == _tcsicmp(pe32.szExeFile, name)) {
        CloseHandle(hSnapshot);
        return pe32;
      }
    } while (Process32Next(hSnapshot, &pe32));
  }
  if (INVALID_HANDLE_VALUE != hSnapshot) {
    CloseHandle(hSnapshot);
  }
  memset(&pe32, -1, sizeof(PROCESSENTRY32));
  return pe32;
}

void Memory::Close() {
  if (0 == process_handle_) {
    return;
  }
  CloseHandle(process_handle_);
  process_handle_ = INVALID_HANDLE_VALUE;
}

const BOOL Memory::Write(BYTE* pAddress, const LPVOID pBuffer, 
                         const DWORD dwSize) {
  if (INVALID_HANDLE_VALUE == process_handle_) {
    return FALSE;
  }
  DWORD dwOldProtect = NULL;
  BOOL bProt = VirtualProtectEx(process_handle_, pAddress, dwSize,
                                PAGE_EXECUTE_READWRITE, &dwOldProtect);
  BOOL bWrite = WriteProcessMemory(process_handle_, pAddress, pBuffer, dwSize,
                                   NULL);
  VirtualProtectEx(process_handle_, pAddress, dwSize, dwOldProtect, 
                   &dwOldProtect);
  return bWrite;
}

void Memory::kek()
{

}
 
} // namespace kengine