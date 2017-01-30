#include <my_global.h>
#include <mysql.h>
#include <string.h>

MYSQL *init_connection(char *host, char *user, char *pass)
{
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return con;
    }
    if (mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        con = NULL;
        return con;
    }
    return con;
}

void connection_query(MYSQL *con, char *query_string)
{
    MYSQL_RES *result;
    unsigned int num_fields;
    unsigned int num_rows;
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
                        while ((field = mysql_fetch_field(result)))
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

void query(char *host, char *user, char *pass, char *db, char *query_string)
{
    MYSQL *con = init_connection(host, user, pass);
    if (con == NULL)
    {
        return;
    }
    if (db != NULL) {
        char *prefix = "USE ";
        char *result = malloc(strlen(prefix)+strlen(db)+1);

        // check for errors in malloc here
        if (result == NULL)
        {
            printf ("No memory\n");
            return;
        }
        strcpy(result, prefix);
        strcat(result, db);

        if (mysql_query(con, result))
        {
            free(result);
            fprintf(stderr, "%s\n", mysql_error(con));
            return;
        }
        free(result);
    }
    connection_query(con, query_string);
    mysql_close(con);
}