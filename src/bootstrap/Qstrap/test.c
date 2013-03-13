const char *value;
	int num;
	char *host_name = (char*) malloc(16*sizeof(char));
	char *user_name = (char*) malloc(32*sizeof(char));
	char *password = (char*) malloc(32*sizeof(char));
	char *db_name = (char*) malloc(32*sizeof(char));
	unsigned int port_num;
	char *socket_name = NULL;			
	unsigned int flags = 0;





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

	char* qry = (char*) malloc (256);
	qry = strcpy(qry, "create database if not exists ");
	qry = strcat(qry, db_name);

	/* Creates 'db' database */
	if (mysql_query(conn, qry))
	{
		Qdb_error(conn, "Database was not created.");
		Qdb_disconnect(conn);
		return 1;
	}	






qry = strcpy(qry, "create table if not exists ");

	setting = config_setting_get_elem(setting, 0);

	printf("%s\n", qry);


	/* Disconnects from the MySQL server, terminates the client library */
	Qdb_disconnect(conn);
	
