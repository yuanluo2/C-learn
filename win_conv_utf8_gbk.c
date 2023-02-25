/*
	convert charset between utf8 and gbk.
	only for windows platform.
*/

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
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

/* remember to call free() for the return string. */
char* utf8_to_gbk(const char* src){
    int ret = 0;
    wchar_t* wBuf = NULL;
    char* buf = NULL;
	
    do{
        if ((ret = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0)) <= 0){
            printLastError();
            break;
        }
        
        if ((wBuf = (wchar_t*)malloc(ret + 10)) == NULL){
            perror("malloc() on wBuf failed");
            break;
        }
        
        if ((ret = MultiByteToWideChar(CP_UTF8, 0, src, -1, wBuf, ret + 10)) <= 0){
            printLastError();
            break;
        }
        
        if ((ret = WideCharToMultiByte(CP_ACP, 0, wBuf, -1, NULL, 0, NULL, NULL)) <= 0){
            printLastError();
            break;
        }
        
        if ((buf = (char*)malloc(ret + 10)) == NULL){
            perror("malloc() on buf failed");
            break;
        }
        
        if ((ret = WideCharToMultiByte(CP_ACP, 0, wBuf, -1, buf, ret + 10, NULL, NULL)) <= 0){
            printLastError();
            break;
        }
    }while(0);
	
    if (wBuf != NULL){
        free(wBuf);
    }
	
    return buf;
}

/* remember to call free() for the return string. */
char* gbk_to_utf8(const char* src){
    int ret = 0;
    wchar_t* wBuf = NULL;
    char* buf = NULL;
    
    do{
        if ((ret = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0)) <= 0){
            printLastError();
            break;
        }
        
        if ((wBuf = (wchar_t*)malloc(ret + 10)) == NULL){
            perror("malloc() on wBuf failed");
            break;
        }
        
        if ((ret = MultiByteToWideChar(CP_ACP, 0, src, -1, wBuf, ret + 10)) <= 0){
            printLastError();
            break;
        }
        
        if ((ret = WideCharToMultiByte(CP_UTF8, 0, wBuf, -1, NULL, 0, NULL, NULL)) <= 0){
            printLastError();
            break;
        }
        
        if ((buf = (char*)malloc(ret + 10)) == NULL){
            perror("malloc() on buf failed");
            break;
        }
        
        if ((ret = WideCharToMultiByte(CP_UTF8, 0, wBuf, -1, buf, ret + 10, NULL, NULL)) <= 0){
            printLastError();
            break;
        }
    }while(0);
    
    if (wBuf != NULL){
        free(wBuf);
    }
    
    return buf;
}

#endif

int main(){
    const char* str = "初音ミク";
    
    char* res = utf8_to_gbk(str);
    printf("%s\n", res);
    free(res);      /* remember this. */
    return 0;
}
