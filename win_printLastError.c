/* 
	A simple function to print error message from GetLastError(), 
	only for windows platform. 
*/
#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#include <stdio.h>
#include <windows.h>
#include <wchar.h>

#define ERROR_MSG_LENGTH_MAX 256

void printLastError(){
	DWORD errNum = GetLastError();
	char errMsg[ERROR_MSG_LENGTH_MAX];
	
    DWORD ret = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
								NULL, 
								errNum,
								0, 
								errMsg,
								ERROR_MSG_LENGTH_MAX, 
								NULL);
	
	if (ret == 0){
		fprintf(stderr, "printLastError() failed, GetLastError() returned: %d.\n", GetLastError());
		return;
	}
	
    printf("errNum: %d => %s", errNum, errMsg);
}

/* 
	To prevent mojibake, before call this function, you should call:
		setlocale(LC_ALL, "");
*/
void printLastErrorW(){
	DWORD errNum = GetLastError();
	wchar_t errMsg[ERROR_MSG_LENGTH_MAX];
	
    DWORD ret = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
								NULL, 
								errNum,
								0, 
								errMsg,
								ERROR_MSG_LENGTH_MAX, 
								NULL);
	
	if (ret == 0){
		fwprintf(stderr, L"printLastError() failed, GetLastError() returned: %d.\n", GetLastError());
		return;
	}
	
    wprintf(L"errNum: %d => %ls", errNum, errMsg);
}

#endif

int main(){
	printLastError();
}
