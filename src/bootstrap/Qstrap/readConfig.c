
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
	
/* libconfig header file */
#include<libconfig.h>

int main() {

	/* Definition of the configuration object, 'cfg'. */
	config_t cfg;

	/* Definition of settings pointers. */		
	config_setting_t *group, *list, *setting;

	/* Initialization of the 'cfg'. */		
	config_init (&cfg);

	if (!config_read_file (&cfg, "config.cfg")) {
		fprintf (stderr, "%s:%d %s\n", config_error_file (&cfg),
				config_error_line (&cfg), config_error_text (&cfg));
		config_destroy(&cfg);
		return 1;
	}

	/* Looking up for 'mysql_tables' */
	setting = config_lookup (&cfg, "Qdb_tables");

	if (setting == NULL) {
		fprintf (stdout, "the 'Qdb_tables' settings are not in the config file.\n");
		config_destroy (&cfg);
		return 1;
	}

	/* Variables which hold length of 'setting', 'group' and 'list' */
	int set_len, grp_len, lst_len; 
	set_len = config_setting_length (setting);
	
	int i, j, k;
	char *qry = (char*) malloc (4096 * sizeof (char));

	for (i = 0; i < set_len; i++) {
		group = config_setting_get_elem (setting, i);
		qry = strcpy (qry, "create table ");
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
		fprintf (stdout, "qry[%d]: %s\n", i + 1, qry);
	}

	/* Destroying of the configuration object, 'config'. */		
	config_destroy(&cfg); 
	
	return 0;
}
