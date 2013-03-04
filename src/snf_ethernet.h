#include "ethernet_hdr.h"



/* We just push all packet into db with this format
        id(time stamp)===property===value===type
*/

int push_pck_into_db(pck* packet)

