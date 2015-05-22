#include "globals.h"

int get_recieved_packet_type(char *packet, vlong size);

vlong get_packet_data(char *buffer, int recieved_size,
                      char **seq_num, char **checksum,
                      char **payload);

vlong get_packet_data_dummy(char *buffer, int size_recieved,
                            char **checksum, char **payload);

vlong get_packet_data_nack(char *buffer, int size_recieved,
                           char **checksum, char **payload);
