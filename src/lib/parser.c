#include "parser.h"

/**
 * @brief Get recieved packet type
 *
 * @param packet start address of the buffer
 * @param size total size of the packet
 * @return NACK
 *         DUMMY
 *         DATA
 */
int get_recieved_packet_type(char *packet, vlong size){
    if (*packet == DATA_TOKEN) {
        return DATA_PACKET;
    } else if (*packet == NACK_TOKEN) {
        return NACK_PACKET;
    } else if (*packet == DUMMY_TOKEN) {
        return DUMMY_PACKET;
    } else {
        DBG("This should never happen");
        exit(1);
    }
}

void get_meta_info_checksum(char *checksum){
    if (*checksum == '1') {
        globals.last_bit_arrived = true;
    }
}

/**
 * @brief Parse data packet
 *
 * @return payload actual data
 *         seq_num seq num of the paylaod
 *         checksum check of the payload
 *         payload size
 */
vlong get_packet_data(char *buffer, int recieved_size,
                      char **seq_num, char **checksum,
                      char **payload){
    // Create memory for payload
    vlong header_len = PACKET_TYPE_LEN + SEQ_NUM_LEN + CHECKSUM_LEN;
    vlong payload_size = recieved_size - (header_len);

    *payload = malloc(sizeof(char)*payload_size);
    // Create memory for checksum
    *checksum = malloc(sizeof(char)*CHECKSUM_LEN);
    // Create memory for seq_num
    *seq_num = malloc(sizeof(char)*SEQ_NUM_LEN);

    char *curr_buffer = buffer;

    curr_buffer += PACKET_TYPE_LEN;
    memcpy(*seq_num, curr_buffer, SEQ_NUM_LEN);

    curr_buffer += SEQ_NUM_LEN;
    memcpy(*checksum, curr_buffer, CHECKSUM_LEN);

    get_meta_info_checksum(*checksum);

    curr_buffer += CHECKSUM_LEN;
    memcpy(*payload, curr_buffer, payload_size);

    return payload_size;
}

/**
 * @brief Parse dummy packet
 *        This packet denotes the existense of the last bit of file
 *        payload here is the destination filename
 *
 * @return payload actual data
 *         checksum check of the payload
 *         payload size
 */
vlong get_packet_data_dummy(char *buffer, int size_recieved,
                            char **checksum, char **payload){
    vlong header_len = PACKET_TYPE_LEN + CHECKSUM_LEN;
    vlong payload_size = size_recieved - header_len;

    *payload = malloc(sizeof(char)*payload_size);
    // Create memory for checksum
    *checksum = malloc(sizeof(char)*CHECKSUM_LEN);

    char *curr_buffer = buffer;

    curr_buffer += PACKET_TYPE_LEN;
    memcpy(*checksum, curr_buffer, CHECKSUM_LEN);

    curr_buffer += CHECKSUM_LEN;
    memcpy(*payload, curr_buffer, payload_size);
    strcpy(globals.recv_filename, *payload);
    DBG("DUMMY PAYLOAD: %s", *payload);

    return payload_size;
}

/**
 * @brief Parse nack packet
 *        This packet denotes the existense of the last bit of file
 *        payload here is the destination filename
 *
 * @return payload actual data
 *         checksum check of the payload
 *         payload size
 */
vlong get_packet_data_nack(char *buffer, int size_recieved,
                           char **checksum, char **payload){
    vlong header_len = PACKET_TYPE_LEN + CHECKSUM_LEN;
    vlong payload_size = size_recieved - header_len;

    *payload = malloc(sizeof(char)*payload_size);
    // Create memory for checksum
    *checksum = malloc(sizeof(char)*CHECKSUM_LEN);

    char *curr_buffer = buffer;

    curr_buffer += PACKET_TYPE_LEN;
    memcpy(*checksum, curr_buffer, CHECKSUM_LEN);

    curr_buffer += CHECKSUM_LEN;
    memcpy(*payload, curr_buffer, payload_size);
    //printf("[NACK RECV]: SEQ NUMBER: %s\n", *payload);

    return payload_size;
}
