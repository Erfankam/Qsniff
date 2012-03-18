/*
 * testqdb.c - Tests qdb interface, connect to and disconnect from MySQL server functions
*/

#include "qdb.h"

int main (int argc, char *argv[])
{
	Connection conn;
	
	if ((conn = Qdb_connect ("localhost", "temp", "pass", NULL,0, NULL, 0)) != NULL) {
		print_error (conn, "Connected successfully!!");
	}
	else {
		print_error (NULL, "Qdb_connect() faild!!");
		exit(1);
	}
	
	Qdb_disconnect (conn);
	exit (0);
}
