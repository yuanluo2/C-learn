/*
	This code demonstrates how to recursively traverse a given directory name,
	which is divided into Windows and Linux versions.
*/

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))

#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_FILE_NAME_LENGTH 256

void walkDir(const char* path){
	if (GetFileAttributes(path) != FILE_ATTRIBUTE_DIRECTORY){
		printf("Not a directory: %s\n", path);
		return;
	}
	
	char fileNameString[MAX_FILE_NAME_LENGTH];
	snprintf(fileNameString, MAX_FILE_NAME_LENGTH, "%s\\*.*", path);
	
	WIN32_FIND_DATA fd;
	HANDLE hd = FindFirstFile(fileNameString, &fd);
	
	if (hd == INVALID_HANDLE_VALUE){
		printf("call FindFirstFile() failed.\n");
		return;
	}
	
	while (FindNextFile(hd, &fd)){
		if (strcmp(fd.cFileName, "..") == 0){
			continue;
		}
		
		printf("%s\\%s\n", path, fd.cFileName);
		
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			snprintf(fileNameString, MAX_FILE_NAME_LENGTH, "%s\\%s", path, fd.cFileName);
			walkDir(fileNameString);
		}
	}
	
	FindClose(hd);
}

#else

#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define MAX_FILE_NAME_LENGTH 256

void walkDir(const char* path){
	DIR* dir = opendir(path);
	
	if (dir == NULL){
		printf("Not a directory: %s\n", path);
		return;
	}
	
	struct dirent* d;
	while ((d = readdir(dir)) != NULL){
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0){
			continue;
		}
		
		printf("%s/%s\n", path, d->d_name);
		
		if (d->d_type == DT_DIR){
			char fileNameString[MAX_FILE_NAME_LENGTH];
			snprintf(fileNameString, MAX_FILE_NAME_LENGTH, "%s/%s", path, d->d_name);
			walkDir(fileNameString);	
		}
	}
	
	closedir(dir);
}

#endif

int main(){
	walkDir("E:\\hatsune miku\\music");
}
