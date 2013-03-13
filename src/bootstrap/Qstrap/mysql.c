
/* include general useful header files, such as stdio.h */
#include <my_global.h>	
/* contains macros and definitions for structures and functions used by client library */ 
#include <my_sys.h>	
/* defines the primarily MySQL-related constants and data structures */	
#include <mysql.h>		
#include <string.h>

	typedef MYSQL* Connection;

	static char* opt_host_name = NULL;				/* server host (default=localhost) */
	static char* opt_user_name = NULL;				/* username (default=login name) */
	static char* opt_password = NULL;				/* password (default=none) */
	static char* opt_db_name = NULL;				/* database name (default=none) */
	static unsigned int opt_port_num = 0;			/* port number (use built-in value) */
	static char* opt_socket_name = NULL;			/* socket name (use built-in value) */
	static unsigned int opt_flags = 0;				/* connection flags (none) */

void Qdb_error (Connection conn, const char* message); 
void Qdb_disconnect (Connection conn);

int main () {

	Connection conn;						/* Pointer to MYSQL structure */
	
	/* initializes client library */
	if (mysql_library_init(0, NULL, NULL))
	{
		Qdb_error(conn, "Client library initialization was failed.");
		return 1;
	}
	
	/* initializes connection handler */
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		Qdb_error(conn, "Initialization was failed.");
		return 1;
	}

	/* Reads connection parameters */
	if (mysql_options(conn, MYSQL_READ_DEFAULT_FILE, "~/.my.cnf"))
	{
		Qdb_error(conn, "Options could not to be set.");
		Qdb_disconnect(conn);
		return 1;
	}

	/* Connects to MySQL server */
	if (mysql_real_connect(conn, opt_host_name, opt_user_name, opt_password, opt_db_name, 
								opt_port_num, opt_socket_name, opt_flags) == NULL)
	{
		Qdb_error(conn, "Connection was failed.");
		Qdb_disconnect(conn);
		return 1;
	}

	/* Creates 'db' database */
	if (mysql_query(conn, "create database db;"))
	{
		Qdb_error(conn, "Database was not created.");
		Qdb_disconnect(conn);
		return 1;
	}

	/* Selects 'db' database */
	if (mysql_select_db(conn, "db"))
	{
		Qdb_error(conn, "Database was not changed.");
		Qdb_disconnect(conn);
		return 1;
	}

	/* Desired query creation */
	char* qry = (char*) malloc (256);
	strcpy(qry, "create table ip_header (");
	strcat(qry, "timestamp bigint unsigned primary key,");
	strcat(qry, "protocol tinyint unsigned not null,");
	strcat(qry, "source_ip int unsigned not null,");
	strcat(qry, "destination_ip int unsigned not null);");	

	/* Executes desired query on 'db' database */
	if (mysql_query(conn, qry))
	{
		Qdb_error(conn, "Query was not executed successfully.");
		Qdb_disconnect(conn);
		return 1;
	}

	/* Disconnects from the MySQL server, terminates the client library */
	Qdb_disconnect(conn);

	return 0;
}

	/*
	 * Wrapping of Error Reporting Functions in MySQL C API.
	 */

	void Qdb_error (MYSQL* conn, const char* message)
	{
		fprintf (stderr, "%s\n", message);
		if (conn != NULL) {
			fprintf (stderr, "Error %u (%s): %s\n", mysql_errno (conn), mysql_sqlstate (conn), mysql_error (conn));
		}
	}
	
	/* Closes the connection specified by conn, also terminates the client library.
	 * Wraps mysql_close() and mysql_library_end() functions.
	 */

	void Qdb_disconnect (Connection conn)
	{
		mysql_close (conn);
		mysql_library_end ();
	}
