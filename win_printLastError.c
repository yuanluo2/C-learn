/* 
	A simple function to print error message from GetLastError(), 
	only for windows platform. 
*/
#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#include <stdio.h>
#include <windows.h>

#define ERROR_MSG_LENGTH_MAX 256

void printLastError(){
	DWORD errNum = GetLastError();
	char errMsg[ERROR_MSG_LENGTH_MAX];
	
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, 
					errNum,
					0, 
					errMsg,
					ERROR_MSG_LENGTH_MAX, 
					NULL);
					
    printf("errNum: %d => %s", errNum, errMsg);
}

/* 
	To prevent mojibake, before call this function, you should call:
		setlocale(LC_ALL, "");
*/
void printLastErrorW(){
	DWORD errNum = GetLastError();
	wchar_t errMsg[ERROR_MSG_LENGTH_MAX];
	
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, 
					errNum,
					0, 
					errMsg,
					ERROR_MSG_LENGTH_MAX, 
					NULL);
					
    wprintf(L"errNum: %d => %ls", errNum, errMsg);
}

#endif

int main(){
	printLastError();
}
