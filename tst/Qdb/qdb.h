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
	static char* opt_db_name = NULL;				/* database name (default=none) */
	static unsigned int opt_port_num = 0;			/* port number (use built-in value) */
	static char* opt_socket_name = NULL;			/* socket name (use built-in value) */
	static unsigned int opt_flags = 0;				/* connection flags (none) */



	typedef my_bool boolean;

	typedef MYSQL* Connection;						/* Pointer to MYSQL structure */
	typedef MYSQL_RES* ResultSet;					/* Pointer to MYSQL_RES structure */
	typedef MYSQL_ROW ResultRow;					/* Pointer to one row of data(result set) */ 

	/*
	 * Wrapping of Connection Management Functions in MySQL C API.
	 */

	/* Initializes the client library, also a connection handler and returns a pointer to it.
	 * Wraps mysql_library_init() and mysql_init() functions.
	 */
	Connection Qdb_init (Connection conn);

	/* Connect to the server and returns a pointer to the connection handler.
	 * Wraps mysql_real_connect() function.
 	 */
	Connection Qdb_connect (Connection conn, const char* host_name, const char* user_name, const char* password, const char* db_name,
						unsigned int port_name, const char* socket_name, unsigned long flags);
	
	/* Closes the connection specified by conn, also terminates the client library.
	 * Wraps mysql_close() and mysql_library_end() functions.
	 */
	void Qdb_disconnect (Connection conn);

	/* Changes the user and the default database for the connection specified by conn.
	 * Wraps mysql_change_user() function.
	 */
	boolean Qdb_changeUser (Connection conn, const char* user_name, const char* password, const char* db_name);

	/* Enables user to set the connection behaviors(options).
	 * Wrapping of mysql_options() function.
	 */

	/* Wraps MYSQL_READ_DEFAULT_FILE option, which specifies an option file to read for connection parameters. */
	boolean Qdb_setOptionFile (Connection conn, const char* optFile);

	/* Wraps MYSQL_READ_DEFAULT_GROUP option, in which to look for option values.*/
	boolean Qdb_setOptionGroup (Connection conn, const char* optGroup);
	
	/* Wraps MYSQL_OPT_PROTOCOL_TCP option, which specifies TCP/IP protocol to use for connecting to the server.*/
	boolean Qdb_setTcpProtocol (Connection conn);

	/* Wraps MYSQL_OPT_PROTOCOL_SOCKET option, which specifies unix socket file protocol to use for connecting to the server.*/	
	boolean Qdb_setSocketProtocol (Connection conn);

	/* Wraps MYSQL_OPT_PROTOCOL_PIPE option, which specifies windows named pipe protocol to use for connecting to the server.*/	
	boolean Qdb_setPipeProtocol (Connection conn);

	/* Wraps MYSQL_OPT_PROTOCOL_MEMORY option, which specifies shared memory protocol to use for connecting to the server.*/	
	boolean Qdb_setMemProtocol (Connection conn);

	/* Wraps MYSQL_OPT_CONNECT_TIMEOUT option, which specifies the connection timeout. */
	boolean Qdb_setTimeout (Connection conn, unsigned int t);

	/* Selects the database named by db.
	 * Wrapping of mysql_select_db() function.
	 */
	boolean Qdb_selectdb (Connection conn, const char* db);

	/*
	 * Wrapping of Error Reporting Functions in MySQL C API.
	 */
	void Qdb_error (Connection conn, const char* message);


#endif
