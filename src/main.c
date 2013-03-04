



int main(int argc, char** argv) {

    /**** Cofiguarations ***/
    /* Read configurations and do what we set */
    read_config(); 
    log()
    config();
    log();
    connect_to_db();
    log();
    start_capturing();

}

