/*
 * testqdb.c - Tests qdb interface.
*/

#include "qdb.h"

int main (int argc, char *argv[])
{	
	Connection conn;

	if ((conn = Qdb_init (conn)) == NULL) {	
		exit (1);
	}
	
	if (Qdb_setOptionFile (conn, "~/.my.cnf")) {
		printf ("The option file was specified successfully!!\n");
	}
	
	if ((conn = Qdb_connect (conn, opt_host_name, opt_user_name, opt_password, opt_db_name,opt_port_num, opt_socket_name, opt_flags)) == NULL) {
		printf ("Qdb_connect() was failed!!\n");
		exit(1);
	}
	
	if (Qdb_changeUser (conn, "temp", "1234", "testdb")) {
		printf ("The user was changed successfully!!\n");	
	}
	else {
		Qdb_error (conn, "Qdb_changeUser() was failed!!");
	}
	
	Qdb_disconnect (conn);
	
	exit (0);
}
