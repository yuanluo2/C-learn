/* This file's charset is utf-8. */
#include <stdio.h>
#include <string.h>
#include <mysql.h>

#define BUFFER_SIZE 256

/*
	CREATE TABLE `m_blog` (
		`id` bigint NOT NULL AUTO_INCREMENT,
		`user_id` bigint NOT NULL,
		`title` varchar(255) NOT NULL,
		`description` varchar(255) NOT NULL,
		`content` longtext,
		`created` datetime NOT NULL ON UPDATE CURRENT_TIMESTAMP,
		`status` tinyint DEFAULT NULL,
		PRIMARY KEY (`id`)
	) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
*/

void test_query(MYSQL* mysql) {
	const char* query = "SELECT id, user_id, title, content, created FROM m_blog";
	
	/* columns. */
	long long id = 0;
	long long user_id = 0;
	char title[256];
	char content[10240];
	bool is_content_null = 0;
	MYSQL_TIME created;
	
	MYSQL_STMT* stmt = NULL;
	MYSQL_BIND binds[5];      /* we want to select 5 columns. */
	
	/* set MYSQL_BIND. */
	memset(binds, 0, sizeof(binds));
	
	binds[0].buffer_type = MYSQL_TYPE_LONGLONG; 
	binds[0].buffer = &id;
	binds[0].is_null = 0;
	
	binds[1].buffer_type = MYSQL_TYPE_LONGLONG; 
	binds[1].buffer = &user_id;
	binds[1].is_null = 0;
	
	binds[2].buffer_type = MYSQL_TYPE_VAR_STRING; 
	binds[2].buffer = title;
	binds[2].buffer_length = sizeof(title)/sizeof(char);
	binds[2].is_null = 0;
	
	binds[3].buffer_type = MYSQL_TYPE_BLOB;
	binds[3].buffer = content;
	binds[3].buffer_length = sizeof(content)/sizeof(char);
	binds[3].is_null = &is_content_null;
	
	binds[4].buffer_type = MYSQL_TYPE_DATETIME; 
	binds[4].buffer = &created;
	binds[4].is_null = 0;
	
	do{
		/* init. */
		if ((stmt = mysql_stmt_init(mysql)) == NULL){
			fprintf(stderr, "test_query(), mysql_stmt_init() error: %s.\n", mysql_stmt_error(stmt));
			break;
		}
		
		if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0){
			fprintf(stderr, "test_query(), mysql_stmt_prepare() error: %s.\n", mysql_stmt_error(stmt));
			break;
		}
		
		if (mysql_stmt_bind_result(stmt, binds) != 0){
			fprintf(stderr, "test_query(), mysql_stmt_bind_result() error: %s.\n", mysql_stmt_error(stmt));
			break;
		}
		
		/* execute the sql. */
		if (mysql_stmt_execute(stmt) != 0){
			fprintf(stderr, "test_query(), mysql_stmt_execute() error: %s.\n", mysql_stmt_error(stmt));
			break;
		}

		/* If you want to get the number of rows, uncomment the code below. */
		/*  
		if (mysql_stmt_store_result(stmt) != 0){
			fprintf(stderr, "test_query(), mysql_stmt_store_result() error: %s.\n", mysql_stmt_error(stmt));
			break;
		}

		printf("result contains %d rows.\n", mysql_stmt_num_rows(stmt));
		*/

		/* fetch results. */
		while (!mysql_stmt_fetch(stmt)){
			printf("%ld %ld %s ", id, user_id, title);
			
			if (is_content_null){
				printf("NULL ");
			}
			else{
				printf("%s ", content);
			}
			
			printf("%04d-%02d-%02d %02d:%02d:%02d\n", 
					created.year, created.month, created.day,
					created.hour, created.minute, created.second);
		}
	}while(0);
	
	if (mysql_stmt_close(stmt) != 0){
		fprintf(stderr, "test_query(), mysql_stmt_close() error: %s.\n", mysql_error(mysql));
	}
}

/* 
	take care, the param stmt is MYSQL_STMT**.
	zero for success, else for failed. 
*/
int stmt_prepare_params(MYSQL* mysql, MYSQL_STMT** stmt, const char* sql, MYSQL_BIND* binds, const char* func_name){
	/* init. */
	if ((*stmt = mysql_stmt_init(mysql)) == NULL){
		fprintf(stderr, "%s, mysql_stmt_init() error: %s.\n", func_name, mysql_stmt_error(*stmt));
		return 1;
	}
	
	if (mysql_stmt_prepare(*stmt, sql, strlen(sql)) != 0){
		fprintf(stderr, "%s, mysql_stmt_prepare() error: %s.\n", func_name, mysql_stmt_error(*stmt));
		return 1;
	}
	
	if (mysql_stmt_bind_param(*stmt, binds) != 0){
		fprintf(stderr, "%s, mysql_stmt_bind_param() error: %s.\n", func_name, mysql_stmt_error(*stmt));
		return 1;
	}
	
	/* execute the sql. */
	if (mysql_stmt_execute(*stmt) != 0){
		fprintf(stderr, "%s, mysql_stmt_execute() error: %s.\n", func_name, mysql_stmt_error(*stmt));
		return 1;
	}
	
	return 0;
}

void test_insert(MYSQL* mysql) {
	const char* insert = "INSERT INTO m_blog (user_id, title, description, created, status) VALUES (?, ?, ?, ?, ?)";
	
	/* columns. */
	long long user_id = 100;
	char title[] = "Hatsune Miku";
	char description[] = "Virtual Idol";
	unsigned char status = 0;
	
	MYSQL_TIME created;
	created.year   = 2023;
	created.month  = 2;
	created.day    = 26;
	created.hour   = 0;
	created.minute = 39;
	created.second = 39;
	
	MYSQL_STMT* stmt = NULL;
	MYSQL_BIND binds[5];      /* we want to insert 5 columns. */
	
	/* set MYSQL_BIND. */
	memset(binds, 0, sizeof(binds));
	
	binds[0].buffer_type = MYSQL_TYPE_LONGLONG; 
	binds[0].buffer = &user_id;
	binds[0].is_null = 0;
	
	binds[1].buffer_type = MYSQL_TYPE_VAR_STRING; 
	binds[1].buffer = title;
	binds[1].buffer_length = sizeof(title)/sizeof(char);
	binds[1].is_null = 0;
	
	binds[2].buffer_type = MYSQL_TYPE_VAR_STRING; 
	binds[2].buffer = description;
	binds[2].buffer_length = sizeof(description)/sizeof(char);
	binds[2].is_null = 0;
	
	binds[3].buffer_type = MYSQL_TYPE_DATETIME;
	binds[3].buffer = &created;
	binds[3].is_null = 0;
	
	binds[4].buffer_type = MYSQL_TYPE_TINY; 
	binds[4].buffer = &status;
	binds[4].is_null = 0;
	
	do{
		if (stmt_prepare_params(mysql, &stmt, insert, binds, "test_insert()") != 0){
			break;
		}

		printf("Insert affected rows: %d.\n", mysql_stmt_affected_rows(stmt));
	}while(0);
	
	if (mysql_stmt_close(stmt) != 0){
		fprintf(stderr, "test_query(), mysql_stmt_close() error: %s.\n", mysql_error(mysql));
	}
}

void test_update(MYSQL* mysql) {
	const char* update = "UPDATE m_blog SET description = ? WHERE id = ?";
	
	/* columns. */
	char description[] = "初音ミク大好き";
	long long id = 21;
	
	MYSQL_STMT* stmt = NULL;
	MYSQL_BIND binds[2];      /* we only need 2 columns. */
	
	/* set MYSQL_BIND. */
	memset(binds, 0, sizeof(binds));
	
	binds[0].buffer_type = MYSQL_TYPE_VAR_STRING; 
	binds[0].buffer = description;
	binds[0].buffer_length = sizeof(description)/sizeof(char);
	binds[0].is_null = 0;
	
	binds[1].buffer_type = MYSQL_TYPE_LONGLONG; 
	binds[1].buffer = &id;
	binds[1].is_null = 0;
	
	do{
		if (stmt_prepare_params(mysql, &stmt, update, binds, "test_update()") != 0){
			break;
		}
		
		printf("Update affected rows: %d.\n", mysql_stmt_affected_rows(stmt));	
	}while(0);

	if (mysql_stmt_close(stmt) != 0){
		fprintf(stderr, "test_update(), mysql_stmt_close() error: %s.\n", mysql_error(mysql));
	}
}

void test_delete(MYSQL* mysql) {
	const char* delete = "DELETE FROM m_blog WHERE id = ?";
	
	/* columns. */
	long long id = 20;
	
	MYSQL_STMT* stmt = NULL;
	MYSQL_BIND bind;      /* we only need 1 column. */
	
	/* set MYSQL_BIND. */
	memset(&bind, 0, sizeof(bind));
	
	bind.buffer_type = MYSQL_TYPE_LONGLONG; 
	bind.buffer = &id;
	bind.is_null = 0;
	
	do{
		if (stmt_prepare_params(mysql, &stmt, delete, &bind, "test_delete()") != 0){
			break;
		}
		
		printf("Delete affected rows: %d.\n", mysql_stmt_affected_rows(stmt));	
	}while(0);

	if (mysql_stmt_close(stmt) != 0){
		fprintf(stderr, "test_delete(), mysql_stmt_close() error: %s.\n", mysql_error(mysql));
	}
}

/*
	Assumes your mysql library's path is D:\mysql, 
	then place the D:\mysql\lib\libmysql.dll in the same directory as this file, and compile with:
		gcc wl_mysql_c_apis_prepared_statement.c -I D:\mysql\include -L D:\mysql\lib -l libmysql
	
	Because the charset is utf8mb4, if you are on windows platform, to prevent mojibake,
	please enter the following command to temporarily change the charset of the cmd:
		chcp 65001
*/
int main() {
	MYSQL *mysql = NULL;

	do {
		if ((mysql = mysql_init(mysql)) == NULL) {
			fprintf(stderr, "mysql_init() error: %s.\n", mysql_error(mysql));
			break;
		}

		if (mysql_real_connect(mysql, "localhost", "root", "193168", "blog", 3306, NULL, 0) == NULL) {
			fprintf(stderr, "main, mysql_real_connect() error: %s.\n", mysql_error(mysql));
			break;
		}

		/* use utf8mb4. */
		if (mysql_set_character_set(mysql, "utf8mb4") != 0) {
			fprintf(stderr, "mysql_set_character_set() error: %s.\n", mysql_error(mysql));
			break;
		}

		test_query(mysql);
	} while (0);

	if (mysql != NULL) {
		mysql_close(mysql);
	}

	return 0;
}
