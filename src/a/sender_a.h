#include "globals.h"

void *sender(void *v);

int send_dummy_packet();

int send_packet(struct node *data_node, bool is_retransmitted);
