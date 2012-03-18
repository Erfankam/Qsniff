/*
 * qdb.c - Implementation
*/

#include "qdb.h"

Connection Qdb_connect (const char* host_name, const char* user_name, const char* password, const char* db_name,
					unsigned int port_num, const char* socket_name, unsigned long flags)
{
	Connection conn;
	
	/* initialize client library */
	if (mysql_library_init (0, NULL, NULL))
	{
		print_error (NULL, "Client library initialization was failed.");
		return NULL;
	}
	
	/* initialize connection handler */
	conn = mysql_init (NULL);
	if (conn == NULL)
	{
		print_error (NULL, "Connection handler initialization was failed.");
		return NULL;
	}

	/* connect to server */
	if (mysql_real_connect (conn, host_name, user_name, password, db_name, 
							port_num, socket_name, flags) == NULL)
	{
		print_error (conn, "Connection establishment was failed.");
		mysql_close	(conn);
		return NULL;
	}
	
	return conn;
}

void Qdb_disconnect (Connection conn)
{
	/* disconnect from server, terminate client library */
	mysql_close (conn);
	mysql_library_end ();
}

void print_error (Connection conn, char* message)
{
	fprintf (stderr, "%s\n", message);
	if (conn != NULL)
	{
		fprintf (stderr, "Error %u (%s): %s\n", mysql_errno (conn), mysql_sqlstate (conn), mysql_error (conn));
	}
}
