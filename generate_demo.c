// a program that connects to greenplum database and create a database named airline-demo

#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

int main() {
    const char *conninfo = "host=localhost port=5432 dbname=postgres user=gpadmin";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    const char *create_db_sql = "CREATE DATABASE \"airline-demo\";";
    PGresult *res = PQexec(conn, create_db_sql);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "CREATE DATABASE failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 1;
    }

    printf("Database 'airline-demo' created successfully.\n");

    PQclear(res);
    PQfinish(conn);
    return 0;
}
