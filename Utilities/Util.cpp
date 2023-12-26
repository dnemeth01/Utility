#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <tchar.h>

using namespace std;

bool static KillProcessByName(DWORD processID, TCHAR pName[]) {
	TCHAR processName[MAX_PATH] = TEXT("unknown");

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, processID);

	if (hProcess != NULL) {
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR));
		if (_tcscmp(processName, pName) == 0)
			if (TerminateProcess(hProcess, 0) != 0)
				return true;
	}
	return false;
}

void static PrintProcessNameAndID(DWORD processID)
{
	TCHAR processName[MAX_PATH] = TEXT("unknown");

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR));

		// Print the process name and identifier.
		if (_tcscmp(processName, L"unknown") != 0)
			_tprintf(TEXT("%s  (PID: %u)\n"), processName, processID);

		CloseHandle(hProcess);
	}
}

bool static KillProcess() {

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	TCHAR pName[MAX_PATH]{};

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return 1;

	cProcesses = cbNeeded / sizeof(DWORD);

	for (unsigned int i = 0; i < cProcesses; i++)
		if (aProcesses[i] != 0)
			PrintProcessNameAndID(aProcesses[i]);


	cout << "Process name to kill:";
	wcin >> pName;

	if (_tcscmp(pName, L"-1") == 0)
		return false;

	for (unsigned int i = 0; i < cProcesses; i++)
		if (aProcesses[i] != 0)
			KillProcessByName(aProcesses[i], pName);
}

bool static EmptyCB() {
	if (OpenClipboard(GetForegroundWindow()))
		if (EmptyClipboard)
			if (CloseClipboard)
				return true;
	return false;
}

void static print() {
	int a = 0;
	cout << "Utilities" << endl;
	cout << "1:Empty clipboard" << endl;
	cout << "2:Kill a process by name" << endl;
	cout << "Select:";
	cin >> a;

	switch (a) {
	case 1:
		if (EmptyCB())
			cout << "Clipboard Empty!" << endl;
		else
			cout << "Failed to empty clipboard!" << endl;
		break;
	case 2:
		if (KillProcess())
			cout << "Killed process!" << endl;
		else
			cout << "Failed to kill process!" << endl;
		break;
	}
}

int main()
{
	while (true)
		print();

	return 0;
}
