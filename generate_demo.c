// a program that connects to greenplum database and create a database named airline-demo
// gcc -o create_airline_demo generate_demo.c -lpq
#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void create_airline_demo_tables(PGconn *conn);

void create_airline_demo_tables(PGconn *conn) {
    const char *create_airports_sql =
        "CREATE TABLE IF NOT EXISTS airports ("
        "airport_id SERIAL,"
        "name TEXT NOT NULL,"
        "city TEXT NOT NULL,"
        "country TEXT NOT NULL,"
        "iata_code CHAR(3) NOT NULL"
        ") DISTRIBUTED RANDOMLY;";

    const char *create_flights_sql =
        "CREATE TABLE IF NOT EXISTS flights ("
        "flight_id SERIAL,"
        "flight_number TEXT NOT NULL,"
        "departure_airport_id INT,"
        "arrival_airport_id INT,"
        "departure_time TIMESTAMP NOT NULL,"
        "arrival_time TIMESTAMP NOT NULL,"
        "status TEXT"
        ") DISTRIBUTED BY (flight_id);";

    const char *create_passengers_sql =
        "CREATE TABLE IF NOT EXISTS passengers ("
        "passenger_id SERIAL,"
        "first_name TEXT NOT NULL,"
        "last_name TEXT NOT NULL,"
        "email TEXT,"
        "phone TEXT"
        ") DISTRIBUTED RANDOMLY;";

    const char *create_tickets_sql =
        "CREATE TABLE IF NOT EXISTS tickets ("
        "ticket_id SERIAL,"
        "flight_id INT REFERENCES flights(flight_id),"
        "passenger_id INT,"
        "seat_number TEXT NOT NULL,"
        "price NUMERIC(10,2) NOT NULL,"
        "booking_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ") DISTRIBUTED BY (flight_id);";

    const char *sql_statements[] = {
        create_airports_sql,
        create_flights_sql,
        create_passengers_sql,
        create_tickets_sql
    };

    const int num_statements = sizeof(sql_statements) / sizeof(sql_statements[0]);

    for (int i = 0; i < num_statements; ++i) {
        PGresult *res = PQexec(conn, sql_statements[i]);

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Failed to create table %d: %s\n", i + 1, PQerrorMessage(conn));
            PQclear(res);
            return;
        }

        PQclear(res);
    }

    printf("All airline demo tables created successfully (Greenplum optimized).\n");
}

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

    // Connect to the newly created database
    const char *conninfo_airline = "host=localhost port=5432 dbname=airline-demo user=gpadmin";
    PGconn *conn_airline = PQconnectdb(conninfo_airline);

    if (PQstatus(conn_airline) != CONNECTION_OK) {
        fprintf(stderr, "Connection to airline-demo failed: %s", PQerrorMessage(conn_airline));
        PQfinish(conn_airline);
        return 1;
    }

    // Create demo tables
    create_airline_demo_tables(conn_airline);

    PQfinish(conn_airline);
    return 0;
}
