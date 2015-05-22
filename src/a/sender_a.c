#include "sender_a.h"

void* sender(void *v){
    // Iterate the data list and send data
    My402ListElem *elem=NULL;
    bool is_retransmitted = false;

    for (elem=My402ListFirst(&globals.datal); elem != NULL; elem=My402ListNext(&globals.datal, elem)) {
        struct node *data_node = (elem->obj);
        int n = send_packet(data_node, is_retransmitted);
        if (n < 0) {
            perror("Error on send");
            exit(1);
        }
    }

    // The nodeA knows that last bit is send
    globals.last_bit_send = true;

    // Send dummy data denotes the end of sending data
    send_dummy_packet();
    DBG("[SUMMARY] START SENDER %u, RETRANS %llu", to_micro(globals.a_sender_start), globals.total_retrans);
}

int send_packet(struct node *data_node, bool is_retransmitted){
    struct sockaddr *to = (struct sockaddr *)&globals.serv_addr;
    int tolen = sizeof(struct sockaddr);

    // Create data packet
    // Add checksum and packet_type bit
    char *buffer;
    vlong buffer_len = create_data_packet(data_node->mem_ptr, data_node->size,
                                          data_node->seq_num, &buffer, is_retransmitted);

    int n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);

    free(buffer);
    return n;
}

int send_dummy_packet(){
    struct sockaddr *to = (struct sockaddr *)&globals.serv_addr;
    int tolen = sizeof(struct sockaddr);

    // Create data packet
    // Add checksum and packet_type bit
    char *buffer;
    vlong buffer_len = create_dummy_packet(&buffer);

    int n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    n = sendto(globals.a_sender_fd, buffer, buffer_len, 0, to, tolen);
    free(buffer);
    return n;
}
