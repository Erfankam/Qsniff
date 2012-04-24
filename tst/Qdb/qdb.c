/*
 * qdb.c - Implementation
 */

#include "qdb.h"

Connection Qdb_init (Connection conn)
{	
	/* initializes client library */
	if (mysql_library_init (0, NULL, NULL))
	{
		Qdb_error (NULL, "Client library initialization was failed.");
		return NULL;
	}
	
	/* initializes connection handler */
	conn = mysql_init (NULL);
	if (conn == NULL)
	{
		Qdb_error (NULL, "Connection handler initialization was failed.");
		return NULL;
	}
	return conn;	
}


Connection Qdb_connect (Connection conn, const char* host_name, const char* user_name, const char* password, const char* db_name,
					unsigned int port_num, const char* socket_name, unsigned long flags)
{
	/* connect to server */
	if (mysql_real_connect (conn, host_name, user_name, password, db_name, 
							port_num, socket_name, flags) == NULL)
	{
		Qdb_error (conn, "Connection establishment was failed.");
		mysql_close	(conn);
		return NULL;
	}
	
	return conn;
}

void Qdb_disconnect (Connection conn)
{
	/* Disconnect from server, terminate client library. */
	mysql_close (conn);
	mysql_library_end ();
}

boolean Qdb_changeUser (Connection conn, const char* user_name, const char* password, const char* db_name) {
	/* Change the user and the default database for the connection specified by conn. */
	return (!mysql_change_user (conn, user_name, password, db_name));
}

boolean Qdb_setOptionFile (Connection conn, const char* optFile)
{
	/* Specifies an option file. */
	return (!mysql_options (conn, MYSQL_READ_DEFAULT_FILE, optFile));
}

boolean Qdb_setOptionGroup (Connection conn, const char* optGroup)
{
	/* Specifies an option file group. */
	return (!mysql_options (conn, MYSQL_READ_DEFAULT_GROUP, optGroup));
}

boolean Qdb_setTcpProtocol (Connection conn)
{
	unsigned int tcpPro = MYSQL_PROTOCOL_TCP;

	/* Specifies TCP/IP protocol for connecting to the server. */
	return !(mysql_options (conn, MYSQL_OPT_PROTOCOL, &tcpPro));
}

boolean Qdb_setSocketProtocol (Connection conn)
{
	unsigned int sockPro = MYSQL_PROTOCOL_SOCKET;

	/* Specifies unix socket file protocol for connecting to the server. */
	return !(mysql_options (conn, MYSQL_OPT_PROTOCOL, &sockPro));
}

boolean Qdb_setPipeProtocol (Connection conn)
{
	unsigned int pipePro = MYSQL_PROTOCOL_PIPE;

	/* Specifies windows named pipe protocol for connecting to the server. */
	return !(mysql_options (conn, MYSQL_OPT_PROTOCOL, &pipePro));
}

boolean Qdb_setMemProtocol (Connection conn)
{
	unsigned int memPro = MYSQL_PROTOCOL_MEMORY;

	/* Specifies shared memory protocol for connecting to the server. */
	return !(mysql_options (conn, MYSQL_OPT_PROTOCOL, &memPro));
}

boolean Qdb_setTimeout (Connection conn, unsigned int t)
{
	/* Specifies the connection timeout, in seconds. */
	return !(mysql_options (conn, MYSQL_OPT_CONNECT_TIMEOUT, &t));
}

boolean Qdb_selectdb (Connection conn, const char* db)
{
	return !mysql_select_db (conn, db);
}

/*
 * Wrapping of Error Reporting Functions in MySQL C API.
 */

void Qdb_error (Connection conn, const char* message)
{
	fprintf (stderr, "%s\n", message);
	if (conn != NULL) {
		fprintf (stderr, "Error %u (%s): %s\n", mysql_errno (conn), mysql_sqlstate (conn), mysql_error (conn));
	}
}
