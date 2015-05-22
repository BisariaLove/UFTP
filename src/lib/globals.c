#include "globals.h"

// Initializing the global structure with default values
struct globals globals = {
    .config = CONFIG_INIT,
    // This is null as per documentation of uthash
    .hashl = NULL,
    // Current sequence recieved is 0
    .current_seq = 0,
    // Used by nodeB for detecting if dummy packet arrived or not
    .last_bit_arrived = false,
    .last_bit_send = false,
    // Hostname of nodeA
    .hostname_a = "sen",
    .total_retrans = 0
};

/**
 * Calculates difference between two timeval.
 */
unsigned int time_diff_micro(struct timeval end, struct timeval start){
  return ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
}

unsigned int to_micro(struct timeval tv){
  return tv.tv_sec* 1000000 + tv.tv_usec;
}

int send_nack_packet(){
    struct sockaddr *to = (struct sockaddr *)&globals.serv_addr;
    int tolen = sizeof(struct sockaddr);

    vlong total_len = 0;
    // Iterate the nack list
    My402ListElem *elem=NULL;
    //DBG("XXXXXXXX CURR SEQ: %llu\n", globals.current_seq);
    elem=My402ListFirst(&globals.nackl);
    if (elem==NULL) {
        //DBG("First element is NULL");
    } else{
        //printf("[SIGNAL] NACK HEAD = %llu, FILE SEND = %d\n", ((struct node*)(elem->obj))->seq_num, globals.last_bit_arrived);
    }
    for (elem=My402ListFirst(&globals.nackl);
         elem != NULL && ( ((struct node*)(elem->obj))->seq_num < globals.current_seq || globals.last_bit_arrived);
         elem=My402ListNext(&globals.nackl, elem)) {
        struct node *data_node = (elem->obj);

        //DBG("XXXXXXXX SEQ: %llu\n", data_node->seq_num);
        // Create nack packet
        char *buffer;
        vlong buffer_len = create_nack_packet(&buffer, data_node->seq_num);
        //DBG("[NACK SEND] SEQ NUM: %llu", data_node->seq_num);

        // Send nack packet
        int n = sendto(globals.b_sender_fd, buffer, buffer_len, 0, to, tolen);
        free(buffer);
        if (n < 0) {
            perror("Error in sento");
            exit(1);
        }
        total_len++;
    }
    printf("[SIGNAL] NACK LEN: %llu\n", total_len);
    return 0;
}
