/* include general useful header files, such as stdio.h */
#include <my_global.h>	
/* contains macros and definitions for structures and functions used by client library */ 
#include <my_sys.h>	
/* defines the primarily MySQL-related constants and data structures */	
#include <mysql.h>
/* libconfig header file */
#include<libconfig.h>

#include <stdio.h>
#include <stdlib.h>		
#include <string.h>

	typedef MYSQL* Connection;

//	static char* opt_host_name = "";				/* server host (default=localhost) */
//	static char* opt_user_name = NULL;				/* username (default=login name) */
//	static char* opt_password = NULL;				/* password (default=none) */
//	static char* opt_db_name = NULL;				/* database name (default=none) */
//	static unsigned int opt_port_num = 0;			/* port number (use built-in value) */
//	static char* opt_socket_name = NULL;			/* socket name (use built-in value) */
//	static unsigned int opt_flags = 0;				/* connection flags (none) */

void Qdb_error (Connection conn, const char* message); 
void Qdb_disconnect (Connection conn);

int main () {

	/*
	 *	PART 2 : CONFIG FILE CREATION
	*/

	const char *value;
	int num;
	char *host_name = (char*) malloc(16*sizeof(char));
	char *user_name = (char*) malloc(32*sizeof(char));
	char *password = (char*) malloc(32*sizeof(char));
	char *db_name = (char*) malloc(32*sizeof(char));
	unsigned int port_num;
	char *socket_name = NULL;			
	unsigned int flags = 0;

	/* Definition of the configuration object, 'cfg'. */
	config_t cfg;
  	config_setting_t *setting, *group, *list;
  					
	config_init(&cfg);

	/* Read the config file. If there is an error, report it and exit. */
	if(!config_read_file(&cfg, "config.cfg")) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
				config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return 1;
	}

	/* Addition of the 'mysql_settings' group to the 'group' setting. */
	setting = config_lookup(&cfg, "Qdb_settings");
	
	if(setting == NULL) {
		fprintf(stderr, "No 'QdbSettings' setting in configuration file.\n");
		config_destroy(&cfg);
		return 1;
	}
	
	if(!config_setting_lookup_string(setting, "host", &value)) {
		fprintf(stderr, "No 'host' setting in configuration file.\n");
		host_name = strcpy(host_name, "localhost");
	}
	host_name = strcpy(host_name, value);	

	if(!config_setting_lookup_string(setting, "user", &value)) {
		fprintf(stderr, "No 'user' setting in configuration file.\n");
		user_name = strcpy(user_name, "root");
	}
	user_name = strcpy(user_name, value);

	if(!config_setting_lookup_string(setting, "password", &value)) {
		fprintf(stderr, "No 'password' setting in configuration file.\n");
		password = strcpy(password, "");
	}
	password = strcpy(password, value);

	if(!config_setting_lookup_int(setting, "port", &num)) {
		fprintf(stderr, "No 'port' setting in configuration file.\n");
		port_num = 0;
	}
	port_num = num;

	/*
	 * PART 2 : MYSQL DATABASES CREATION
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

	/* Connects to MySQL server */
	if (mysql_real_connect(conn, host_name, user_name, password, db_name, 
								port_num, socket_name, flags) == NULL)
	{
		Qdb_error(conn, "Connection was failed.");
		Qdb_disconnect(conn);
		return 1;
	}

	if(!config_setting_lookup_string(setting, "database", &value)) {
		fprintf(stderr, "No 'database' setting in configuration file.\n");
		db_name = strcpy(db_name, "qdb");
	}
	db_name = strcpy(db_name, value);

	char *qry = (char*) malloc (4096 * sizeof (char));

	qry = strcpy(qry, "create database if not exists ");
	qry = strcat(qry, db_name);

	/* Creates 'qdb' database */
	if (mysql_query(conn, qry))
	{
		Qdb_error(conn, "Database was not created.");
		Qdb_disconnect(conn);
		return 1;
	}	
	
	qry = strcpy(qry, "use ");
	qry = strcat(qry, db_name);	

	/* Selects 'qdb' database */
	if (mysql_query(conn, qry))
	{
		Qdb_error(conn, "Database was not created.");
		Qdb_disconnect(conn);
		return 1;
	}

	setting = config_lookup(&cfg, "Qdb_tables");

	if(setting == NULL) {
		fprintf(stderr, "No 'QdbTables' setting in configuration file.\n");
		config_destroy(&cfg);
		return 1;
	}

	/* Variables which hold length of 'setting', 'group' and 'list' */
	int set_len, grp_len, lst_len;
 
	set_len = config_setting_length (setting);
	
	int i, j, k;
	
	for (i = 0; i < set_len; i++) {
		group = config_setting_get_elem (setting, i);
		qry = strcpy (qry, "create table if not exists ");
		qry = strcat (qry, config_setting_name (group));
		qry = strcat (qry, " (\n");
		grp_len = config_setting_length (group);
		for (j = 0; j < grp_len; j++) {
			list = config_setting_get_elem (group, j);
			qry = strcat (qry, config_setting_name (list));
			qry = strcat (qry, " ");
			lst_len = config_setting_length (list);
			for (k = 0; k < lst_len; k++) {
				qry = strcat (qry, config_setting_get_string_elem (list, k));
				qry = strcat (qry, " ");
			}
			qry = strcat (qry, ",\n");			 
		}
		qry[strlen (qry) - 2] = ')';

		/* Executes desired query on 'db' database */
		if (mysql_query(conn, qry))
		{
			Qdb_error(conn, "Query was not executed successfully.");
			Qdb_disconnect(conn);
			return 1;
		}
	}


	/* Disconnects from the MySQL server, terminates the client library */
	Qdb_disconnect(conn);
	
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
