#ifndef KENGINE_MEMORY_H_
#define KENGINE_MEMORY_H_

#include "common.h"

#include <TlHelp32.h>

namespace kengine {

class Memory {
 public:
	Memory();
	const bool Attached(const TCHAR *name);
	const PROCESSENTRY32 ProcessInfo(const TCHAR *name);
	void Close();
	const BOOL Memory::Write(BYTE *pAddress, const LPVOID pBuffer, 
													 const DWORD dwSize);
	void kek();
 private:
	HANDLE process_handle_;
};

}

#endif