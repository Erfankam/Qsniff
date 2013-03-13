
/* This sample code creates a configuration file
   for a hypothetical GUI application. 
   Output file name is : test.cfg */

/* For executing this code, you should first install libconfig package on your system.
   You can do it by a package manager, such as Synaptic, or by source at 
   http://www.hyperrealm.com/libconfig/libconfig-1.4.8.tar.gz . 
   Also, you can be familiar with libconfig, by reading the documentation file within this
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

	setting = config_setting_add (root, "version", CONFIG_TYPE_STRING);
	config_setting_set_string (setting, "1.0.0");

	if (!config_write_file(&cfg, "test.cfg"))
	{
		fprintf(stderr, "Error while writing!");
		config_destroy(&cfg);
		return(EXIT_FAILURE);
	}

	// Distroy the configuration object, cfg.	
	config_destroy (&cfg);

	return 0;
}
