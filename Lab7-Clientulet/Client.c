#include <Windows.h>
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define STATUS_SUCCESS					0
#define INVALID_NUMBER_OF_ARGUMENTS		1
#define INVALID_ARGUMENT_PROVIDED		2
#define INVALID_SHARED_MEMORY			3
#define SH_BUFFER_SIZE					0x400


//
// IsBlacklisted
//
BOOLEAN
IsPathBlacklisted(
	__in PCHAR Path
	)
/*
	Checks for blacklisted paths (not relative to executable path)
*/
{
	BOOLEAN status = FALSE;
	CHAR chFileFullPath[MAX_PATH] = { 0 };
	CHAR chCurrentExecutablePath[MAX_PATH] = { 0 };

	__try
	{
		if (MAX_PATH < strlen(Path))
		{
			printf("Path too long\n");
			status = TRUE;
			__leave;
		}

		if (0 == GetFullPathNameA(
			Path,
			MAX_PATH,
			chFileFullPath,
			NULL
			))
		{
			printf("GetFullPathName failed with 0x%x\n", GetLastError());
			status = TRUE;
			__leave;
		}

		if (0 == GetCurrentDirectoryA(
			MAX_PATH,
			chCurrentExecutablePath
			))
		{
			printf("GetCurrentDirectory failed with 0x%x\n", GetLastError());
			status = TRUE;
			__leave;
		}

		printf("File in: %s\n", chFileFullPath);
		printf("Current %s\n", chCurrentExecutablePath);

		int result = _strnicmp(
			chFileFullPath,
			chCurrentExecutablePath,
			strlen(chCurrentExecutablePath));

		if (0 != result)
		{
			printf("strnicmp returned %d\n", result);
			status = TRUE;
		}
	}
	__finally
	{

	}

	return status;
}

int main(DWORD argc, PCHAR argv[])
{	
	int status = STATUS_SUCCESS;
	PCHAR pChShMemoryName = "Global\\Testulet";
	HANDLE hShMemory = NULL;
	PBYTE pShBuffer = NULL;
	PCHAR pChFileName = NULL;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	__try
	{
		if (2 != argc)
		{
			status = INVALID_NUMBER_OF_ARGUMENTS;
			printf("INVALID_NUMBER_OF_ARGUMENTS");
			__leave;
		}

		pChFileName = argv[1];
		if (IsPathBlacklisted(pChFileName))
		{
			status = INVALID_ARGUMENT_PROVIDED;
			printf("INVALID_ARGUMENT_PROVIDED");
			__leave;
		}

		hShMemory = CreateFileMappingA(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			SH_BUFFER_SIZE,
			pChShMemoryName);
		if (NULL == hShMemory)
		{
			status = INVALID_SHARED_MEMORY;
			printf("CreateFileMappingA failed with 0x%x\n", GetLastError());
			__leave;
		}

		pShBuffer = MapViewOfFile(
			hShMemory,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			SH_BUFFER_SIZE
			);
		if (NULL == pShBuffer)
		{
			printf("MapViewOfFile failed with 0x%x\n", GetLastError());
			
			__leave;
		}

		hFile = CreateFileA(
			pChFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			// status = INVALID_SHARED_MEMORY;
			printf("CreateFileMappingA failed with 0x%x\n", GetLastError());
			__leave;
		}


	}
	__finally
	{
		if (INVALID_HANDLE_VALUE != hFile)
		{
			CloseHandle(hFile);
		}

		if (NULL != pShBuffer)
		{
			UnmapViewOfFile(pShBuffer);
		}

		if (NULL != hShMemory)
		{
			CloseHandle(hShMemory);
		}
	}

	return status;
}