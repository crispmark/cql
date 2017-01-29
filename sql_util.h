#ifndef SQL_UTIL_H_
#define SQL_UTIL_H_

MYSQL *init_connection(char *host, char *user, char *pass);

void connection_query(MYSQL *con, char *query_string);

void query(char *host, char *user, char *pass, char *db, char *query_string);

#endif