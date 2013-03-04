
/* This sample code creates a configuration file
   for a hypothetical GUI application. 
   Output file name is : test.cfg */

/* For executing this code, you should first install libconfig package on your system.
   You can do it by a package manager, such as Synaptic, or by source at 
   http://www.hyperrealm.com/libconfig/libconfig-1.4.8.tar.gz . 
   Also, you can be familiar with libconfig, by reading the documentation file in this
   package. */ 
 
#include<stdio.h>
#include<stdlib.h>

/* libconfig header file */
#include<libconfig.h> 

int main ()
{
	// Creates a configuration object
	config_t cfg;
	// Creates configuration setting objects
	config_setting_t *root, *setting, *group, *list, *array;

	// Initializes the configuration object, cfg.
	config_init (&cfg);
	
	// Creates the root setting of configuration, "root"
	root = config_root_setting (&cfg);
	
	// Creates the "version" setting, child of "root"
	setting = config_setting_add (root, "version", CONFIG_TYPE_STRING);
	config_setting_set_string (setting, "1.0");

	// Creates the "application" group, child of "root"	
	group = config_setting_add (root, "application", CONFIG_TYPE_GROUP);
	
	// Creates the "window" group, member of "application" group
	group = config_setting_add (group, "window", CONFIG_TYPE_GROUP);
	
	// Creates the "title" setting, member of "window" group
	setting = config_setting_add (group, "title", CONFIG_TYPE_STRING);
	// Sets the value of the "title" member
	config_setting_set_string (setting, "My Application");
	
	// Creates the "size" group, member of "window" group
	setting = config_setting_add (group, "size", CONFIG_TYPE_GROUP);

	// Creates the "w" setting, member of "size" group
	setting = config_setting_add (setting, "w", CONFIG_TYPE_INT);
	// Sets the value of the "w" member
	config_setting_set_int (setting, 640);

	// Fetches the "size", member of the "window" group 
	setting = config_setting_get_member (group, "size");

	// Creates the "h" setting, member of "size" group
	setting = config_setting_add (setting, "h", CONFIG_TYPE_INT);
	// Sets the value of the "h" member
	config_setting_set_int (setting, 480);

	// Creates the "pos" group, member of "window" group
	setting = config_setting_add (group, "pos", CONFIG_TYPE_GROUP);
	
	// Creates the "x" setting, member of "pos" group
	setting = config_setting_add (setting, "x", CONFIG_TYPE_INT);
	// Sets the value of the "x" member
	config_setting_set_int (setting, 350);

	// Fetches the "pos", member of the "window" group 
	setting = config_setting_get_member (group, "pos");

	// Creates the "y" setting, member of "pos" group
	setting = config_setting_add (setting, "y", CONFIG_TYPE_INT);
	// Sets the value of the "y" member
	config_setting_set_int (setting, 250);

	// Creates the "list" list, member of "application" group
	list = config_setting_add (root, "list", CONFIG_TYPE_LIST);
	
	// Creates and sets the "list" elements
	setting = config_setting_add (list, NULL, CONFIG_TYPE_LIST);
	config_setting_set_string_elem (setting, -1, "abc");
	config_setting_set_int_elem (setting, -1, 123);
	config_setting_set_bool_elem (setting, -1, 1);

	config_setting_set_float_elem (list, -1, 1.234);

	setting = config_setting_add (list, NULL, CONFIG_TYPE_LIST);

	// Creates and sets the "books" list, member of "application" group
	list = config_setting_add (root, "books", CONFIG_TYPE_LIST);
	
	group = config_setting_add (list, NULL, CONFIG_TYPE_GROUP);
	
	setting = config_setting_add (group, "title", CONFIG_TYPE_STRING);
	config_setting_set_string (setting, "Treasure Island");
	
	setting = config_setting_add (group, "auther", CONFIG_TYPE_STRING);
	config_setting_set_string (setting, "Robert Louis Stevenson");

	setting = config_setting_add (group, "price", CONFIG_TYPE_FLOAT);
	config_setting_set_float (setting, 29.95);

	setting = config_setting_add (group, "qty", CONFIG_TYPE_INT);
	config_setting_set_int (setting, 5);	

	group = config_setting_add (list, NULL, CONFIG_TYPE_GROUP);
	
	setting = config_setting_add (group, "title", CONFIG_TYPE_STRING);
	config_setting_set_string (setting, "Snow Crash");
	
	setting = config_setting_add (group, "auther", CONFIG_TYPE_STRING);
	config_setting_set_string (setting, "Neal Stephenson");

	setting = config_setting_add (group, "price", CONFIG_TYPE_FLOAT);
	config_setting_set_float (setting, 9.99);

	setting = config_setting_add (group, "qty", CONFIG_TYPE_INT);
	config_setting_set_int (setting, 8);	
	
	// Creates and sets the "misc" group, member of "application" group
	group = config_setting_add (root, "misc", CONFIG_TYPE_GROUP);
	
	setting = config_setting_add (group, "pi", CONFIG_TYPE_FLOAT);
	config_setting_set_float (setting, 3.141592654);
	
	setting = config_setting_add (group, "bigint", CONFIG_TYPE_INT64);
	config_setting_set_int64 (setting, 922337203685477807L);

	array = config_setting_add (group, "columns", CONFIG_TYPE_ARRAY);
	config_setting_set_string_elem (array, -1, "Last Name");
	config_setting_set_string_elem (array, -1, "First Name");
	config_setting_set_string_elem (array, -1, "MI");

	setting = config_setting_add (group, "bitmask", CONFIG_TYPE_INT);
	// Sets format of the setting, for hexadecimal integers
	config_setting_set_format (setting, CONFIG_FORMAT_HEX);
	config_setting_set_int (setting, 0x1FC3);

	// Writes configuration to the defined file
	if (!config_write_file (&cfg, "test.cfg"))
	{
		fprintf (stderr, "Error while writing file.\n");
		config_destroy (&cfg);
		return 1;
	}

	// Distroy the configuration object, cfg.	
	config_destroy (&cfg);

	return 0;
}
