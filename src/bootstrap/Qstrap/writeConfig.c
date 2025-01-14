
#include<stdio.h>
#include<stdlib.h>
	
/* libconfig header file */
#include<libconfig.h>

int main() {

	/* Definition of the configuration object, 'cfg'. */
	config_t cfg;

	/* Definition of settings pointers. */		
	config_setting_t *root, *group, *list, *setting;

	/* Initialization of the 'cfg'. */		
	config_init(&cfg);

	/* Returning the root setting of the 'config', to the 'root' setting pointer. */
	root = config_root_setting(&cfg);

	/* Addition of the 'mysql_settings' group to the 'root' setting. */
	group = config_setting_add(root, "Qdb_settings", CONFIG_TYPE_GROUP);
	
	/* Addition of settings to the 'mysql_settings' group. */
	setting = config_setting_add (group, "host", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "localhost");

	setting = config_setting_add (group, "user", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "qsniff");

	setting = config_setting_add(group, "password", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "qq");

	setting = config_setting_add(group, "database", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "qdb");

	setting = config_setting_add(group, "port", CONFIG_TYPE_INT);
	config_setting_set_int(setting, 0);

	/* Addition of settings to the 'mysql_tables' group. */
	group = config_setting_add(root, "Qdb_tables", CONFIG_TYPE_GROUP);
	
	/* Addition of the 'ip_header' group to the 'mysql_tables' group. */
	group = config_setting_add(group, "ip_header", CONFIG_TYPE_GROUP);
		
	list = config_setting_add(group, "timestamp", CONFIG_TYPE_LIST);
	config_setting_set_string_elem(list, -1, "bigint");
	config_setting_set_string_elem(list, -1, "unsigned");
	config_setting_set_string_elem(list, -1, "primary key");

	list = config_setting_add(group, "protocol", CONFIG_TYPE_LIST);
	config_setting_set_string_elem(list, -1, "tinyint");
	config_setting_set_string_elem(list, -1, "unsigned");
	config_setting_set_string_elem(list, -1, "not null");

	list = config_setting_add(group, "source_ip", CONFIG_TYPE_LIST);
	config_setting_set_string_elem(list, -1, "int");
	config_setting_set_string_elem(list, -1, "unsigned");
	config_setting_set_string_elem(list, -1, "not null");

	list = config_setting_add(group, "destination_ip", CONFIG_TYPE_LIST);
	config_setting_set_string_elem(list, -1, "int");
	config_setting_set_string_elem(list, -1, "unsigned");
	config_setting_set_string_elem(list, -1, "not null");

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
