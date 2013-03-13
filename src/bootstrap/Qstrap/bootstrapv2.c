/* include general useful header files, such as stdio.h */
#include <my_global.h>	
/* contains macros and definitions for structures and functions used by client library */ 
#include <my_sys.h>	
/* defines the primarily MySQL-related constants and data structures */	
#include <mysql.h>
/* libconfig header file */
#include<libconfig.h>
		
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

	/*
	 * PART 1 : MYSQL DATABASES CREATION
	*/

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
	if (mysql_query(conn, "create database if not exists db;"))
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
	strcpy(qry, "create table if not exists ip_header (");
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


	/*
	 *	PART 2 : CONFIG FILE CREATION
	*/

	/* Definition of the configuration object, 'cfg'. */
	config_t cfg;

	/* Definition of settings pointers. */		
	config_setting_t *root, *group, *array, *setting;

	/* Initialization of the 'cfg'. */		
	config_init(&cfg);

	/* Returning the root setting of the 'config', to the 'root' setting pointer. */
	root = config_root_setting(&cfg);

	/* Addition of the 'mysql_settings' group to the 'root' setting. */
	group = config_setting_add(root, "mysql_settings", CONFIG_TYPE_GROUP);
	
	/* Addition of settings to the 'mysql_settings' group. */
	setting = config_setting_add (group, "host", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "localhost");

	setting = config_setting_add (group, "user", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "temp");

	setting = config_setting_add(group, "password", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "pass");

	setting = config_setting_add(group, "database", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "testdb");

	setting = config_setting_add(group, "port", CONFIG_TYPE_INT);
	config_setting_set_int(setting, 0);

	/* Addition of settings to the 'mysql_tables' group. */
	group = config_setting_add(root, "mysql_tables", CONFIG_TYPE_GROUP);
	
	/* Addition of the 'ip_header' group to the 'mysql_tables' group. */
	group = config_setting_add(group, "ip_header", CONFIG_TYPE_GROUP);
		
	array = config_setting_add(group, "timestamp", CONFIG_TYPE_ARRAY);
	config_setting_set_string_elem(array, -1, "bigint unsigned");
	config_setting_set_string_elem(array, -1, "primary key");

	array = config_setting_add(group, "protocol", CONFIG_TYPE_ARRAY);
	config_setting_set_string_elem(array, -1, "tinyint unsigned");
	config_setting_set_string_elem(array, -1, "not null");

	array = config_setting_add(group, "source_ip", CONFIG_TYPE_ARRAY);
	config_setting_set_string_elem(array, -1, "int unsigned");
	config_setting_set_string_elem(array, -1, "not null");

	array = config_setting_add(group, "destination_ip", CONFIG_TYPE_ARRAY);
	config_setting_set_string_elem(array, -1, "int unsigned");
	config_setting_set_string_elem(array, -1, "not null");

	/* Writing of the configuration object to the defined file, 'config' */
	if(!config_write_file(&cfg, "config.cfg")) {
		fprintf(stderr, "Error occured while file writing.");
		config_destroy(&cfg);
		return 1;
	}
		
	/* Destroying of the configuration object, 'config'. */		
	config_destroy(&cfg);

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
