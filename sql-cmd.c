#include <my_global.h>
#include <mysql.h>
#include <string.h>
/* Maximum packet size + 1. */
#define MAX_QUERY_SZ 1048577

int main(int argc, char **argv)
{
    MYSQL *con = mysql_init(NULL);
    MYSQL_RES *result;
    unsigned int num_fields;
    unsigned int num_rows;

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }


    if (mysql_real_connect(con, "localhost", "test_user", "test_pass", NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    void query(char *query_string) {
        if (mysql_query(con, query_string))
        {
            fprintf(stderr, "%s\n", mysql_error(con));
        } else
        {
            result = mysql_store_result(con);
            if (result) {
                num_fields = mysql_num_fields(result);
                // retrieve rows, then call mysql_free_result(result)

                MYSQL_ROW row;
                MYSQL_FIELD *field;

                while ((row = mysql_fetch_row(result)))
                {
                    for (int i = 0; i < num_fields; i++)
                    {
                        if (i == 0)
                        {
                            while (field = mysql_fetch_field(result))
                            {
                                printf("%s\t", field->name);
                            }

                            printf("\n");
                        }
                        printf("%s\t", row[i] ? row[i] : "NULL");
                    }
                    printf("\n");
                }
            }
            else
            {
                if (mysql_field_count(con) == 0)
                {
                    // query does not return data
                    // (it was not a SELECT)
                    num_rows = mysql_affected_rows(con);
                    printf("%d row(s) affected.\n", num_rows);
                } else {
                    fprintf(stderr, "Error: %s\n", mysql_error(con));
                }
            }
            mysql_free_result(result);
        }
    }

    while (1)
    {
        /* Allocate memory and check if okay. */
        char *user_query = malloc (MAX_QUERY_SZ);
        if (user_query == NULL)
        {
            printf ("No memory\n");
            return 1;
        }
        /* Prompt user for query. */
        printf(">");

        /* Get the query, with size limit. */
        fgets (user_query, MAX_QUERY_SZ, stdin);

        if (strcmp(user_query,"quit\n") == 0)
        {
            mysql_close(con);
            exit(0);
        }
        /* execute query */
        query(user_query);

        /* Free memory */
        free (user_query);
    }

    mysql_close(con);
    exit(0);
}