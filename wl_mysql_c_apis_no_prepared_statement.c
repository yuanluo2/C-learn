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
	const char* query = "SELECT * FROM m_blog";
	MYSQL_RES* result = NULL;
	uint64_t numOfRows = 0;
	unsigned int numOfFields = 0;
	MYSQL_ROW row;
	
	/* execute the sql. */
	if (0 != mysql_real_query(mysql, query, strlen(query))) {
		fprintf(stderr, "test_query, mysql_real_query() error: %s.\n", mysql_error(mysql));
		return;
	}

	/* fetch results. */
	result = mysql_store_result(mysql);
	if (result == NULL) {
		fprintf(stderr, "test_query, mysql_store_result() error: %s.\n", mysql_error(mysql));
		return;
	}

	numOfRows = mysql_num_rows(result);
	printf("result contains %d rows.\n", numOfRows);

	numOfFields = mysql_num_fields(result);

	unsigned int i;
	while (row = mysql_fetch_row(result)) {
		for (i = 0; i < numOfFields;++i) {
			MYSQL_FIELD* field = mysql_fetch_field_direct(result, i);

			/* fetch a specific column from the result. */
			if (strcmp("title", field->name) == 0) {
				printf("%s\n", row[i]);     
			}
		}
	}

	mysql_free_result(result);
}

void test_insert(MYSQL* mysql) {
	char insert[BUFFER_SIZE];
	sprintf(insert, 
			"INSERT INTO m_blog (user_id, title, description, created, status) VALUES (%d, %s, %s, %s, %d)",
			39, "'初音ミク'", "'初音ミク'", "'2023-02-25 00:39:39'", 0);

	if (mysql_real_query(mysql, insert, strlen(insert)) != 0) {
		fprintf(stderr, "test_insert, mysql_real_query() error: %s.\n", mysql_error(mysql));
		return;
	}

	printf("Insert affected rows: %d.\n", mysql_affected_rows(mysql));
}

void test_update(MYSQL* mysql) {
	char update[BUFFER_SIZE];
	sprintf(update,
			"UPDATE m_blog SET description = %s WHERE id = %d",
			"'初音ミク大好き'", 18);

	if (mysql_real_query(mysql, update, strlen(update)) != 0) {
		fprintf(stderr, "test_update, mysql_real_query() error: %s.\n", mysql_error(mysql));
		return;
	}

	printf("Update affected rows: %d.\n", mysql_affected_rows(mysql));
}

void test_delete(MYSQL* mysql) {
	char delete[BUFFER_SIZE];
	sprintf(delete,
			"DELETE FROM m_blog WHERE id = %d",
			16);

	if (mysql_real_query(mysql, delete, strlen(delete)) != 0) {
		fprintf(stderr, "test_delete, mysql_real_query error: %s.\n", mysql_error(mysql));
		return;
	}

	printf("Delete affected rows: %d.\n", mysql_affected_rows(mysql));
}

/*
	Assumes your mysql library's path is D:\mysql, 
	then place the D:\mysql\lib\libmysql.dll in the same directory as this file, and compile with:
		gcc wl_mysql_c_apis_no_prepared_statement.c -I D:\mysql\include -L D:\mysql\lib -l libmysql
	
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

		if (mysql_real_connect(mysql, "localhost", "<user>", "<password>", "<db_name>", 3306, NULL, 0) == NULL) {
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
