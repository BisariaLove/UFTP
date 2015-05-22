#include "reciever_a.h"

void *reciever(void *v){
    char buffer[globals.config.read_buffer_size];
    bzero(buffer,globals.config.read_buffer_size);

    struct sockaddr_in from;
    int fromlen = sizeof(struct sockaddr_in);

    DBG("[START] START SENDER %u", to_micro(globals.a_sender_start));
    while (1){
        int size_recieved=recvfrom(globals.a_recv_fd,
                                   buffer, globals.config.read_buffer_size, 0,
                       (struct sockaddr *)&from, &fromlen);
        if (size_recieved < 0) {
            perror("Error in recv");
            exit(1);
        }
        int packet_type = get_recieved_packet_type(buffer);

        switch (packet_type) {
            case DATA_PACKET:
                data_packet_handler(buffer, size_recieved);
                break;
            case NACK_PACKET:
                nack_packet_handler(buffer, size_recieved);
                break;
            case DUMMY_PACKET:
                dummy_packet_handler(buffer, size_recieved);
                break;
        }
    }
}


void data_packet_handler(char *buffer, int size_recieved) {}

void dummy_packet_handler(char *buffer, int size_recieved) {}

void nack_packet_handler(char *buffer, int size_recieved){
    char *checksum, *payload;
    vlong payload_size = get_packet_data_nack(buffer, size_recieved, &checksum, &payload);

    vlong seq_num = atoi(payload);

    hashed_link *hash_node = (hashed_link *)(find_hashl(seq_num));
    if (hash_node == NULL) {
        DBG("Hash node is null, this is not possible");
        exit(1);
    }
    struct node *data_node = (struct node*)((hash_node->data_node_ptr)->obj);

    // Retransmit the data back again with sequence number
    globals.total_retrans++;
    //printf("[RETRANS SEND]: TOTAL %llu", globals.total_retrans);
    int n = send_packet(data_node);
    if (n < 0) {
        perror("Retransmiston: Error in sending packet");
        exit(1);
    }
    //TODO: Free payload and checksum
    free(payload);
    free(checksum);
}
