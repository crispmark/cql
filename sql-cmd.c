#include <my_global.h>
#include <mysql.h>
#include <string.h>
/* Maximum packet size + 1. */
#define MAX_QUERY_SZ 1048577

int main(int argc, char **argv) {
    MYSQL *con = mysql_init(NULL);
    MYSQL_RES *result;
    unsigned int num_fields;
    unsigned int num_rows;

    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }


    if (mysql_real_connect(con, "localhost", "test_user", "test_pass", NULL, 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    void query(char *query_string) {
        if (mysql_query(con, query_string)) {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
            exit(1);
        } else {
            result = mysql_store_result(con);
            if (result) {
                num_fields = mysql_num_fields(result);
                printf("%d", num_fields);
                // retrieve rows, then call mysql_free_result(result)
            }
            else {
                if(mysql_field_count(con) == 0) {
                    // query does not return data
                    // (it was not a SELECT)
                    num_rows = mysql_affected_rows(con);
                } else {
                    fprintf(stderr, "Error: %s\n", mysql_error(con));
                }
            }
        }
    }
    query("CREATE DATABASE IF NOT EXISTS testdb");

    query("USE testdb");

    query("CREATE TABLE IF NOT EXISTS test_table (test_id int(11) NOT NULL AUTO_INCREMENT, foreign_id int(11), PRIMARY KEY (test_id))");

    /* Allocate memory and check if okay. */
    char *user_query = malloc (MAX_QUERY_SZ);
    if (user_query == NULL) {
        printf ("No memory\n");
        return 1;
    }
    
    /* Ask user for query. */
    printf("Enter query:");

    /* Get the query, with size limit. */
    fgets (user_query, MAX_QUERY_SZ, stdin);

    /* execute query */
    query(user_query);

    /* Free memory */
    free (user_query);

    mysql_close(con);
    exit(0);
}