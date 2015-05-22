#include "packetize.h"

/**
 * @brief Create a character buffer with
 *        certain format so that the packet can
 *        be detected
 *        | PAKCET_TYPE | CHECKSUM | PAY_LOAD |
 *             1Byte        10Byte  packet_size
 *        | Comma seperated list f sequence number |
 *
 * @param List of nack sequence number
 * @return character array with format
 */
vlong create_nack_packet(char **buffer, vlong seq_num){

    vlong header_len = PACKET_TYPE_LEN + CHECKSUM_LEN;
    *buffer = malloc(sizeof(char)*(header_len + SEQ_NUM_LEN));
    char *curr_buffer = *buffer;
    bzero(*buffer, header_len + SEQ_NUM_LEN);

    memcpy(curr_buffer, "3", PACKET_TYPE_LEN);
    curr_buffer += PACKET_TYPE_LEN;

    char checksum[10] = "XXXXXXXXXX";
    memcpy(curr_buffer, checksum, CHECKSUM_LEN);
    curr_buffer += CHECKSUM_LEN;

    sprintf(curr_buffer, "%llu", seq_num);

    vlong buffer_len = header_len + SEQ_NUM_LEN;;
    return buffer_len;
}


/**
 * @brief | PACKET_HEADER | DATA |
 *              21Byte   packet_size
 *
 *        | PACKET_TYPE | SEQ NUM | CHECKSUM |
 *            1Byte        10Byte    10Byte
 *        Copy the data from the memory location
 *
 * @param
 * @return character array with format
 */
vlong create_data_packet(char *mem_ptr, vlong payload_size,
                       vlong seq_num, char **buffer, bool is_retransmitted){

    vlong header_len = PACKET_TYPE_LEN + SEQ_NUM_LEN + CHECKSUM_LEN;

    *buffer = malloc(sizeof(char)*(header_len + payload_size));
    char *curr_buffer = *buffer;
    bzero(*buffer, header_len + payload_size);

    memcpy(curr_buffer, "1", PACKET_TYPE_LEN);
    curr_buffer += PACKET_TYPE_LEN;

    char seq_num_st[SEQ_NUM_LEN];
    snprintf(seq_num_st, SEQ_NUM_LEN, "%llu", seq_num);
    memcpy(curr_buffer, seq_num_st, SEQ_NUM_LEN);
    curr_buffer += SEQ_NUM_LEN;

    char checksum[10];
    if (is_retransmitted && globals.last_bit_send) {
        strcpy(checksum, "1XXXXXXXXX");
    } else {
        strcpy(checksum, "0XXXXXXXXX");
    }
    memcpy(curr_buffer, checksum, CHECKSUM_LEN);
    curr_buffer += CHECKSUM_LEN;

    memcpy(curr_buffer, mem_ptr, (size_t)payload_size);

    vlong buffer_len = header_len + payload_size;;
    //DBG("[DATA SEND] SEQ NUM: %llu, TOTAL SIZE [%llu]", seq_num, buffer_len);
    return buffer_len;
}

/**
 * @brief Create dummy packet
 *        This is the last packet send
 *        with infromation about the filenames
 *        It doesn't contain anything
 */
vlong create_dummy_packet(char **buffer){
    vlong payload_size = sizeof(globals.recv_filename);
    vlong header_len = PACKET_TYPE_LEN + CHECKSUM_LEN;
    *buffer = malloc(sizeof(char)*(header_len + payload_size));
    bzero(*buffer, header_len + payload_size);

    char *curr_buffer = *buffer;

    memcpy(curr_buffer, "2", PACKET_TYPE_LEN);
    curr_buffer += PACKET_TYPE_LEN;

    char checksum[10] = "XXXXXXXXXX";
    memcpy(curr_buffer, checksum, CHECKSUM_LEN);
    curr_buffer += CHECKSUM_LEN;

    memcpy(curr_buffer, globals.recv_filename, (size_t)payload_size);

    vlong buffer_len = header_len + payload_size;;
    //DBG("DUMMY : [%s] [%llu]", *buffer, buffer_len);
    return buffer_len;
}
