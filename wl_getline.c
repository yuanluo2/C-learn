#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>

#define BUFFER_INIT_SIZE 16

/*
	1. If the giving buf, capacity or stream is NULL, return -1, and errno will be set to EINVAL.
	2. If malloc(), realloc(), ferror() failed or reach EOF, return -1, no errno will be set.
	3. Otherwise, return the length of the buf, this length does not include '\0'.
	
	This function does not adds the delim to the buf.
*/
ssize_t getdelim(char** buf, size_t* capacity, char delim, FILE* stream){
	char c = '\0';
	ssize_t len = 0;
	
	if (buf == NULL || capacity == NULL || stream == NULL){
		errno = EINVAL;
		return -1;
	}
	
	if (*buf == NULL){
		if ((*buf = (char*)malloc(BUFFER_INIT_SIZE * sizeof(char))) == NULL){
			errno = ENOMEM;
			return -1;
		}
		
		*capacity = BUFFER_INIT_SIZE;
	}
	
	for (;;){
		c = fgetc(stream);
		
		if (ferror(stream) != 0 || c == EOF){
			return -1;
		}
		
		if (len == (*capacity)){
			char* temp = (char*)realloc(*buf, (*capacity) + BUFFER_INIT_SIZE);
			
			if (temp == NULL){
				errno = ENOMEM;
				return -1;
			}
			
			*buf = temp;
			(*capacity) += BUFFER_INIT_SIZE;
		}
		
		if (c == delim){
			break;
		}
		
		(*buf)[len] = c;
		++len;
	}
	
	(*buf)[len] = '\0';
	return len;
}

ssize_t getline(char** buf, size_t* capacity, FILE* stream){
	return getdelim(buf, capacity, '\n', stream);
}

void test_read_file(){
	char* buf = NULL;
	size_t capacity = 0;
	ssize_t len = 0;
	FILE* f = NULL;
	
	if ((f = fopen("C:\\users\\10940\\desktop\\main.c", "r")) == NULL){
		perror("fopen() failed");
		return;
	}
		
	while ((len = getline(&buf, &capacity, f)) >= 0){
		printf("%s\n", buf);
		printf("capacity is %d\n", capacity);
		printf("length is %d\n", len);
		
		free(buf);
		buf = NULL;
	}
	
	if (!feof(f) && len < 0){
		perror("getline() failed");
	}
	
	if (fclose(f) != 0){
		perror("fclose() failed");
	}
}

void test_user_input(){
	char* buf = NULL;
	size_t capacity = 0;
	ssize_t len = 0;
	
	len = getline(&buf, &capacity, stdin);
	
	printf("%s\n", buf);
	printf("capacity is %d\n", capacity);
	printf("length is %d\n", len);
}

int main(){
	test_user_input();
}
