#include <Windows.h>
#include <stdio.h>
#include <winternl.h>

typedef NTSTATUS(*MYPROC) (HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

int Error(const char* msg) {
	printf("%s (%u)", msg, GetLastError());
	return -1;
}

//  GetProcAddress + LoadLibrary(ntdll.dll) => NtQueryInformationProcess address

int main() {
	HANDLE h = GetCurrentProcess();
	PROCESS_BASIC_INFORMATION ProcessInformation;
	ULONG lenght = 0;
	HINSTANCE ntdll;
	MYPROC GetProcessInformation;
	ntdll = LoadLibrary(TEXT("Ntdll.dll"));

	//resolve address of NtQueryInformationProcess in ntdll.dll
	GetProcessInformation = (MYPROC)GetProcAddress(ntdll, "NtQueryInformationProcess");

	//get _PEB object
	(GetProcessInformation)(h, ProcessBasicInformation, &ProcessInformation, sizeof(ProcessInformation), &lenght);

	//replace commandline and imagepathname
	BYTE BeingDebugged = ProcessInformation.PebBaseAddress->BeingDebugged;
	if (!BeingDebugged) {
		MessageBox(NULL, L"Behave as u want ;)", L"Not Debugged", MB_OK);
		return 0;
	}

	MessageBox(NULL, L"Behave Nicely", L"Debugged", MB_OK);
	return -1;
}