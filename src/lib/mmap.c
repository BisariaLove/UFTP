#include "mmap.h"

/**
 * @brief Get the complete file into memory
 */
char * get_memory_map_ptr(const char *filename, vlong *total_size){
    int fd;
    char *data;
    struct stat sbuf;

    if ((fd = open(filename, O_RDONLY)) == -1) {
	perror("open");
	exit(1);
    }

   if (stat(filename, &sbuf) == -1) {
	perror("stat");
	exit(1);
   }

    if ((data = mmap((caddr_t)0, sbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == (caddr_t)(-1)) {
	perror("mmap");
	exit(1);
    }

    // Get file size in Bytes
    *total_size = (vlong)sbuf.st_size;
    return data;
}

/**
 * @brief Iterate the data list and write into file
 *        Read size from each node
 */
void write_data_list_to_file(const char *filename){
    // Open the file for writing
    FILE *fp = fopen(filename, "w");
    if (fp == NULL){
        DBG("Can not open file for writing");
        exit(0);
    }

    My402ListElem *elem=NULL;
    // Iterate over the data list
    for (elem=My402ListFirst(&globals.datal); elem != NULL; elem=My402ListNext(&globals.datal, elem)) {
        struct node *data_node = (elem->obj);
        //DBG("[%p] : WRITING SEQ = %llu", elem->obj, data_node->seq_num);
        // Write a packet into the file
        write_packet_to_file(fp, data_node);
    }

    fclose(fp);
}

/**
 * @brief Write one node data into file
 *        size denotes how much to write
 *        one denotes char size (1Bytes)
 */
void write_packet_to_file(FILE *fp, struct node *data_node){
    //DBG("WRITING : %s", data_node->mem_ptr);
    int n = fwrite(data_node->mem_ptr, 1, data_node->size, fp);
    /*
    if (fwrite(buffer, 1, globals.config.packet_size, fp) != globals.config.packet_size){
        DBG("Error in writing");
        exit(0);
    }*/
}
