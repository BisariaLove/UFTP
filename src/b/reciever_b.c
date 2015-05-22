#include "reciever_b.h"

bool is_nack_list_empty() {
    //DBG("NUM NACK LIST = %d", (globals.nackl).num_members);
    if ((globals.nackl).num_members == 0) {
        return true;
    } else {
        // Print the list
        //DBG("---------");
        //print_list(&globals.nackl);
        //DBG("---------");
        // Send nack packet
        //send_nack_packet();
        return false;
    }
}

bool is_last_packet_recieved() {
    return globals.last_bit_arrived;
}

void *reciever(void *val){

    while (1){
        // If last packet is recieved
        // break out of the loop
        if (is_last_packet_recieved() &&
            is_nack_list_empty()) {
            // Delete the nack timer
            // Break out of the loop
            // Return
            // TODO: Delete the nack timer
            DBG("NACK is EMPTY");
            goto COMPLETE_FILE_REACHED;
        }

        //DBG(".........Waiting.......");
        int n=recv_packet();
    }
COMPLETE_FILE_REACHED:
    gettimeofday(&globals.b_reciever_end, NULL);
    DBG("[TIME] END RECIEVER %u", to_micro(globals.b_reciever_end));
    DBG("Complete file is downloaded");
    write_data_list_to_file(globals.recv_filename);
}

int recv_packet(){
    char buffer[globals.config.read_buffer_size];
    bzero(buffer,globals.config.read_buffer_size);

    struct sockaddr_in from;
    int fromlen = sizeof(struct sockaddr_in);

    int size_recieved=recvfrom(globals.b_recv_fd, buffer, globals.config.read_buffer_size, 0,
                               (struct sockaddr *)&from, &fromlen);
    if (size_recieved < 0) {
        perror("Error in recv");
        exit(1);
    }

    // Check the packet with checksum
    // If no match then return i.e. drop packet
    int packet_type = get_recieved_packet_type(buffer);

    switch (packet_type) {
        case DATA_PACKET:
            data_packet_handler(buffer, size_recieved);
            break;
        case NACK_PACKET:
            //nack_packet_handler(size_recieved);
            break;
        case DUMMY_PACKET:
            dummy_packet_handler(buffer, size_recieved);
            break;
    }
    return size_recieved;
}

void data_packet_handler(char *buffer, int size_recieved) {

    char *seq_num, *checksum, *payload;
    vlong payload_size = get_packet_data(buffer, size_recieved, &seq_num, &checksum, &payload);
    //DBG("CHECKSUM : [%s]", checksum);
    vlong sq_num= atoll(seq_num);

    vlong seq_num_int = atoi(seq_num);

    // Check if duplicate packet
    if (is_duplicate(seq_num_int)) {
        DBG("[DUPLICATE RECV] SEQ NUM: %llu", seq_num_int);
        // Free all the memory taken
        return;
    }

    // Update the maximum sequence read
    if (seq_num_int > globals.current_seq) {
        globals.current_seq = seq_num_int;
    }

    //DBG("[DATA RECV] SIZE RECV: %d, SEQ: %s,  CURR MAX: %llu",
     //   size_recieved, seq_num, globals.current_seq);

    // Checksum matched and sequence number known
    // Update the memory pointer
    update_mem_ptr_data_link(payload, sq_num, payload_size);

    // Remove the node from the nack list
    delete_node_nack_list(sq_num);

    //TODO: Free the seq_num and checksum
    free(checksum);
}

void dummy_packet_handler(char *buffer, int size_recieved) {

    if (globals.last_bit_arrived) {
        DBG("[DUPLICATE DUMMY]");
        return;
    }

    gettimeofday(&globals.dummy_reached, NULL);
    // On the bit for last bit arrived
    globals.last_bit_arrived = true;
    DBG("[DUMMY RECV]: [%s]", buffer);

    // Get checksum and filename of the destination
    char *checksum, *payload;
    vlong payload_size = get_packet_data_dummy(buffer, size_recieved, &checksum, &payload);
    // Payload here is the filename
    //TODO: Free the checksum
    free(checksum);
}
