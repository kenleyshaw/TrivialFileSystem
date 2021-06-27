#include "tfs.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* implementation of assigned functions */


/* you are welcome to use helper functions of your own */



/* tfs_delete()
 *
 * deletes a closed directory entry having the given file descriptor
 *   (changes the status of the entry to unused) and releases all
 *   allocated file blocks
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is closed
 *
 * postconditions:
 *   (1) the status of the directory entry is set to unused
 *   (2) all file blocks have been set to free
 *
 * input parameter is a file descriptor
 *
 * return value is TRUE when successful or FALSE when failure
 */

unsigned int tfs_delete( unsigned int file_descriptor ){
	unsigned int block;
	// checking for valid preconditions
	// if the file descriptor is 0 or not in range or if it is not open, then it is not valid
	if (file_descriptor == 0 || tfs_check_fd_in_range(file_descriptor == FALSE ||
			tfs_check_file_is_open(file_descriptor) != FALSE)) {
		return FALSE;
	}
	else {
		// assigning the first block in the directory to an unsigned int
		block = directory[file_descriptor].first_block;
		// if that block is 0, then it is unused
		if (block == 0) {
			directory[file_descriptor].status = UNUSED;
			return TRUE;
		}
		else {
			block = 0;
			// iterates through each block, and if it is not free then it manually frees space
			for (block = FIRST_VALID_BLOCK; block < N_BLOCKS; block++) {
				if (file_allocation_table[block] != FREE) {
					free(&file_allocation_table[block]);
				}
			}
		}
		directory[file_descriptor].status = UNUSED;
	}
	return TRUE;
}


/* tfs_read()
 *
 * reads a specified number of bytes from a file starting
 *   at the byte offset in the directory into the specified
 *   buffer; the byte offset in the directory entry is
 *   incremented by the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * the function will read fewer bytes than specified if the
 *   end of the file is encountered before the specified number
 *   of bytes have been transferred
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has allocated file blocks
 *
 * postconditions:
 *   (1) the buffer contains bytes transferred from file blocks
 *   (2) the specified number of bytes has been transferred
 *         except in the case that end of file was encountered
 *         before the transfer was complete
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_read( unsigned int file_descriptor,
                       char *buffer,
                       unsigned int byte_count ){
	unsigned int block;
	// if the file descriptor not in range or if it is not open, then it is not valid
	if (tfs_check_fd_in_range(file_descriptor == FALSE &&
			tfs_check_file_is_open(file_descriptor) == FALSE)) {
		return 0;
	}
	else {
		// assigns block as the first block of the file descriptor
		block = directory[file_descriptor].first_block;
		// size is going to be the final size read
		unsigned int size = 0;
		// gets name of file to read
		char* file_to_read = directory[file_descriptor].name;
		// iterates through the maximum number of directory entries, comparing the name of them
		// to the name of the file to read
		for(int i = 0; i < N_DIRECTORY_ENTRIES; i++){
			if(strcmp(directory[i].name, file_to_read) == 0)
				{
				// assigns that size to the appropriate directory entry
				size = directory[i].size;
		break;
				}
		}
		// returns the size of bytes read
		return size;
	}
}

/* tfs_write()
 *
 * writes a specified number of bytes from a specified buffer
 *   into a file starting at the byte offset in the directory;
 *   the byte offset in the directory entry is incremented by
 *   the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * furthermore, depending on the starting byte offset and the
 *   specified number of bytes to transfer, additional file
 *   blocks, if available, will be added to the file as needed;
 *   in this case, the size of the file will be adjusted
 *   based on the number of bytes transferred beyond the
 *   original size of the file
 *
 * the function will read fewer bytes than specified if file
 *   blocks are not available
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *
 * postconditions:
 *   (1) the file contains bytes transferred from the buffer
 *   (2) the specified number of bytes has been transferred
 *         except in the case that file blocks needed to
 *         complete the transfer were not available
 *   (3) the size of the file is increased as appropriate
 *         when transferred bytes extend beyond the previous
 *         end of the file
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_write( unsigned int file_descriptor,
                        char *buffer,
                        unsigned int byte_count ) {
	/*unsigned int block;
	if (tfs_check_fd_in_range(file_descriptor == FALSE &&
				tfs_check_file_is_open(file_descriptor) == FALSE)) {
		return block;
	}
	else {
		unsigned int bytes_left = byte_count;
		if(byte_count > BLOCK_SIZE) {
				bytes_left = BLOCK_SIZE;
			}
		unsigned int bytes_written = 0;
		int num_blocks = 0;
		char* file_to_write = directory[file_descriptor].name;
		char* write_block = directory[file_descriptor].first_block;
			int block_array[byte_count];
		for(int i = 0; i < N_DIRECTORY_ENTRIES; i++) {
		if(strcmp(directory[file_descriptor].name, file_to_write) == 0) {
					write_block = directory[i].name;
				}
			}
		if(write_block == 0) {
		int find_index = 0;
		// finds the index of the first block that's empty
		for(int i = 0; i < block_array[byte_count]; i++) {
		if(file_allocation_table[i] == 0) {
						find_index = i;
		break;
					}
				}
				write_block = find_index;
			}
		int index = write_block;
		int total_index = index + block_array[byte_count];
		int dec_count = byte_count;
		for(int i = ((BLOCK_SIZE + byte_count - 1) / BLOCK_SIZE); i >= 1; i--) {
		int find_index = 0;
		// finds the index of the first block that's empty
		for(int i = 0; i < block_array[byte_count]; i++) {
		if(file_allocation_table[i] == 0) {
						find_index = i;
		break;
					}
				}
		// sets values in the fat_array and writes to disk
		file_allocation_table[index] = find_index;
		index = file_allocation_table[index];
		file_allocation_table[index] = 0xFFFF;
		int *buff = (void*)block_array + (BLOCK_SIZE * (num_blocks++));
		//block_write(total_index, buff);
				dec_count = dec_count - bytes_left;
		size_t size = dec_count;
		if(dec_count > BLOCK_SIZE) {
					size = BLOCK_SIZE;
				}
		// calculates amount of bytes written and amount of bytes left
		bytes_written = bytes_written + bytes_left;
				write_block = write_block + bytes_left;
				bytes_left = size;
			}
		return bytes_written;*/
	return 0;
}

