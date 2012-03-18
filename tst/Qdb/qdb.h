/*
 * qdb.h - Interface
*/
#ifndef QDB_H
#define QDB_H

	#include <my_global.h>	/* include general useful header files, such as stdio.h */
	#include <my_sys.h>		/* contains macros and definitions for structures and functions used by client library */ 
	#include <mysql.h>		/* defines the primarily MySQL-related constants and data structures */

	static char* opt_host_name = NULL;				/* server host (default=localhost) */
	static char* opt_user_name = NULL;				/* username (default=login name) */
	static char* opt_password = NULL;				/* password (default=none) */
	static unsigned int opt_port_num = 0;			/* port number (use built-in value) */
	static char* opt_socket_name = NULL;			/* socket name (use built-in value) */
	static char* opt_db_name = NULL;				/* database name (default=none) */
	static unsigned int opt_flags = 0;				/* connection flags (none) */

	typedef MYSQL* Connection;				/* pointer to connection handler */

	Connection Qdb_connect (const char* host_name, const char* user_name, const char* password, const char* db_name,
						unsigned int port_name, const char* socket_name, unsigned long flags);

	void Qdb_disconnect (Connection conn); 

	void print_error (Connection conn, char* message);

#endif
