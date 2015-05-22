#include "sender_b.h"

void *sender(void *val){
    /*
    My402ListElem *elem=NULL;
    for (elem=My402ListFirst(&globals.datal); elem != NULL; elem=My402ListNext(&globals.datal, elem)) {
        struct node *data_node = (elem->obj);
        printf("[%llu] : PRIeT SEQ = %llu, DATA = %s\n", data_node->size, data_node->seq_num, data_node->mem_ptr);
        int n = send_packet(data_node);
        if (n < 0) {
            perror("Error on send");
            exit(1);
        }
    }
    */
    register_signal();
}

int send_packet(struct node *data_node){
    struct sockaddr *to = (struct sockaddr *)&globals.serv_addr;
    int tolen = sizeof(struct sockaddr);

    int n = sendto(globals.b_sender_fd, data_node->mem_ptr, data_node->size, 0, to, tolen);
    return n;
}

/*
int send_nack_packet(){
    struct sockaddr *to = (struct sockaddr *)&globals.serv_addr;
    int tolen = sizeof(struct sockaddr);

    // Iterate the nack list
    My402ListElem *elem=NULL;
    for (elem=My402ListFirst(&globals.nackl);
         elem != NULL && ((struct node*)(elem->obj))->seq_num < globals.current_seq;
         elem=My402ListNext(&globals.nackl, elem)) {
        struct node *data_node = (elem->obj);

        // Create nack packet
        char *buffer;
        vlong buffer_len = create_nack_packet(&buffer, data_node->seq_num);
        DBG("[NACK SEND] SEQ NUM: %llu", data_node->seq_num);

        // Send nack packet
        int n = sendto(globals.b_sender_fd, buffer, buffer_len, 0, to, tolen);
        free(buffer);
        if (n < 0) {
            perror("Error in sento");
            exit(1);
        }
    }
    return 0;
}
*/
