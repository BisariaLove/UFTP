#include "globals.h"

vlong create_nack_packet(char **buffer, vlong seq_num);

vlong create_data_packet(char *mem_ptr, vlong payload_size,
                       vlong seq_num, char **buffer, bool is_retransmitted);

vlong create_dummy_packet(char **buffer);
