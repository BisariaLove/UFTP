#include "list.h"

void list_error(const char *mesg){
    DBG("Append Failed");
    exit(1);
}

/**
 * @brief Print nodes of list
 */
void print_list(My402List *list){
    My402ListElem *elem=NULL;
    for (elem=My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)) {
        struct node *data_node = (elem->obj);
        //printf("[%p] : PRINT SEQ = %llu, DATA = %s\n", data_node->mem_ptr, data_node->seq_num, data_node->mem_ptr);
        DBG("NACK : SEQ = %llu", data_node->seq_num);
    }
}

/**
 * @brief Create sender list
 *
 */
void create_list(char *data_ptr, My402List *list, const char *list_type){
    // Initialize the data list
    if (My402ListInit(list)==0){
        exit(1);
    }

    // Iterate and add nodes with seq_num and mem address
    vlong seq_num = 0;
    for (;seq_num<globals.config.total_size; seq_num += globals.config.packet_size){
        struct node *data_node = malloc(sizeof(struct node));
        data_node->seq_num = seq_num;
        data_node->mem_ptr = data_ptr + seq_num;
        // To handle scenario where the last packet is of size less then
        // required
        vlong size = globals.config.total_size - seq_num > globals.config.packet_size ? globals.config.packet_size : globals.config.total_size - seq_num;
        // size in bytes
        data_node->size = size;

        My402ListElem *link_node;
        if (My402ListAppend(list , data_node, &link_node)==0)
	    list_error("Append Failed");

        //DBG("MEM PTR = %p, SEQ_NUM = %llu", data_node->mem_ptr, data_node->seq_num);

        // Check if DATA or NACK list
        // Add the linked list node pointer to the hashmap node
        if (strcmp(list_type, DATA) == 0) {
            // DATA list
            hashed_link *hash_node = malloc(sizeof(hashed_link));
            hash_node->seq_num = data_node->seq_num;
            hash_node->data_node_ptr = link_node;
            add_hashl(hash_node);
        }
        else {
            // NACK list
            hashed_link *hash_node = (hashed_link *)(find_hashl(data_node->seq_num));
            if (!hash_node) {
                DBG("This should never happen");
                exit(1);
            }
            hash_node->nack_node_ptr = link_node;
        }
    }
}
/**
 * @brief Delete node in the nack list with
 *        seq number mentioned in the param
 */
void delete_node_nack_list(vlong seq_num){
    // Get node address from hashmap
    hashed_link *hash_node = (hashed_link *)(find_hashl(seq_num));
    if (!hash_node) {
        DBG("This should never happen");
        exit(1);
    }

    // Delete from the Nack list
    My402ListUnlink(&globals.nackl, hash_node->nack_node_ptr);
    //DBG("[%llu] Removing node", hash_node->seq_num);
}
/**
 * @brief Get the current nack list
 *        that is less the max read sequence number
 */
void get_current_nack_list(){
    My402ListElem *elem=NULL;
    for (elem=My402ListFirst(&globals.nackl);
        elem != NULL && ((struct node*)(elem->obj))->seq_num < globals.current_seq;
        elem=My402ListNext(&globals.nackl, elem)) {
        struct node *data_node = (elem->obj);
        DBG("[%p] : SEQ = %llu", elem->obj, data_node->seq_num);
    }
}

/**
 * @brief Append node for retransmission
 * NOT USED
 */
void add_retransmission_node(vlong *retrans_list, int num_retrans){
    // Iterate the retrans_list
    int i;
    for (i=0;i<num_retrans;i++){
        vlong seq_num = retrans_list[i];
        DBG("Add node = %llu", seq_num);
        hashed_link *hash_node = (hashed_link *)(find_hashl(seq_num));

        // Create a duplicate node
        struct node *data_node = malloc(sizeof(struct node));
        data_node->seq_num = seq_num;
        data_node->mem_ptr = ((struct node*)((hash_node->data_node_ptr)->obj))->mem_ptr;
        data_node->size = ((struct node*)((hash_node->data_node_ptr)->obj))->size;

        // Append the duplicate node to the end of the data list.
        My402ListElem *link_node;
        if (My402ListAppend(&globals.datal , data_node, &link_node)==0)
            list_error("Error in appending to list");
    }
}

/**
 * @brief Update the memory pointer
 *        Create memory for the buffer.
 *        Assign it at the correct sequence number.
 */
void update_mem_ptr_data_link(char *buffer, vlong seq_num,
                              vlong size){
    hashed_link *hash_node = (hashed_link *)(find_hashl(seq_num));
    //DBG("UPDATING [%llu]", seq_num);

    ((struct node*)((hash_node->data_node_ptr)->obj))->mem_ptr = buffer;
    ((struct node*)((hash_node->data_node_ptr)->obj))->size = size;
}

/**
 * @brief Create list
 *        recv_data_list
 *        recv_nack__list
 *        only sequence number is added
 */
void create_recv_list(My402List *list, const char *list_type){
    // Initialize the data list
    if (My402ListInit(list)==0){
        exit(1);
    }

    // Iterate and add nodes with seq_num and mem address
    vlong seq_num = 0;
    for (;seq_num<globals.config.total_size; seq_num += globals.config.packet_size){
        struct node *data_node = malloc(sizeof(struct node));
        data_node->seq_num = seq_num;
        data_node->mem_ptr = NULL;

        My402ListElem *link_node;
        if (My402ListAppend(list , data_node, &link_node)==0)
	    list_error("Append Failed");

        // Check if DATA or NACK list
        // Add the linked list node pointer to the hashmap node
        if (strcmp(list_type, DATA) == 0) {
            // DATA list
            hashed_link *hash_node = malloc(sizeof(hashed_link));
            hash_node->seq_num = data_node->seq_num;
            hash_node->data_node_ptr = link_node;
            add_hashl(hash_node);
        }
        else {
            // NACK list
            hashed_link *hash_node = (hashed_link *)(find_hashl(data_node->seq_num));
            if (!hash_node) {
                DBG("This should never happen");
                exit(1);
            }
            hash_node->nack_node_ptr = link_node;
        }
    }
}

bool is_duplicate(vlong seq_num) {
    // If memory pointer mapping exist in the linked list
    // then the seq number has already being recieved
    hashed_link *hash_node = (hashed_link *)(find_hashl(seq_num));
    if (!hash_node) {
        DBG("This should never happen");
        exit(1);
    }
    if ( ((struct node*)((hash_node->data_node_ptr)->obj))->mem_ptr == NULL ) {
        return false;
    }
    return true;
}

/**
 * @brief Print nodes data using memcpy
 */
void print_file_packet_wise(char *data_ptr){
    long long int seq_num = 0;
    char packet_data[globals.config.packet_size];

    for (;seq_num < globals.config.total_size;){
        strncpy(packet_data, data_ptr + seq_num, (size_t)globals.config.packet_size);
        memcpy(packet_data, data_ptr + seq_num, (size_t)globals.config.packet_size);
        seq_num += globals.config.packet_size;
    }
}
