#ifndef TFS_H_
#define TFS_H_

/* CPSC/ECE 3220 Fall 2019
 *
 * trivial file system
 *
 * trivial file system assumptions
 *
 * - there is a small, fixed number of file blocks
 * - file blocks are mapped with a file allocation table
 *
 * - the directory occupies the first two file blocks
 * - the directory is single-level, unstructured table
 * - file names are up to 11 characters in length and can contain
 *     alphanumeric characters, underscores, and periods; there
 *     is no additionally defined naming syntax
 * - file descriptors are used as indices into the directory
 * - a file descriptor has a valid range of 1-15 (in most cases
 *     a return value of 0 indicates an error, so a file
 *     descriptor of 0 is not used as a valid index)
 * - a starting block of zero means that no file blocks are
 *     allocated to the file
 *
 * - the file allocation table occupies the second two file blocks
 * - a file block number for a file has a valid range of 4-255
 * - a file size has a valid range of 0-MAX_SIZE (note that for
 *     tfs_size(), a return value > MAX_FILE_SIZE is used to
 *     indicate an error)
 *
 * - a file can only have one open at a time => the current byte
 *     offset (i.e., the file pointer) is placed in the directory
 *     entry instead of in the normal per-open data structure
 *
 * - there are no file permissions and no permission checking
 *
 * mapping of 256 x 128 byte file blocks:
 * 0 - 1:   directory, 16 entries x 16 bytes each, fd of 0 unused
 * 2 - 3:   file allocation table, 256 entries x 1 byte each,
 *            0-3 not used as valid blocks, 0 == free, 1 == end
 * 4 - 255: file blocks containing file data
 *
 * a directory entry is 16 bytes (12 bytes for name string)
 * +--------+--------+--------+--------+--------------...--+
 * | status | first_ |  size  |  byte_ |      name         |
 * |        | block  |        | offset |                   |
 * +--------+--------+--------+--------+--------------...--+
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* defined sizes and limits */

#define N_DIRECTORY_ENTRIES 16
#define N_BLOCKS 256
#define BLOCK_SIZE 128
#define BLOCK_SIZE_AS_POWER_OF_2 7
#define N_BYTES (256*128)
#define MAX_FILE_SIZE (252*128)
#define FILENAME_LENGTH 9
#define FIRST_VALID_FD 1
#define FIRST_VALID_BLOCK 4


/* directory entry status */

#define UNUSED 0
#define CLOSED 1
#define OPEN 2


/* special case block index values */

#define FREE 0
#define LAST_BLOCK 1


/* logical values */

#define TRUE 1
#define FALSE 0


/* storage for file system */

char storage[N_BYTES];


/* struct declarations and pointers */

struct file_block{
  char bytes[BLOCK_SIZE];
};

struct directory_entry{
  unsigned char status;
  unsigned char first_block;
  unsigned short size;
  unsigned short byte_offset;
  char name[FILENAME_LENGTH + 1];
};

struct file_block *blocks;
struct directory_entry *directory;
unsigned char *file_allocation_table;


/* public interface */

void tfs_init();

void tfs_list_blocks();

void tfs_list_directory();

unsigned int tfs_create( char *name );

unsigned int tfs_exists( char *name );

unsigned int tfs_open(   char *name );

unsigned int tfs_size(   unsigned int file_descriptor );

unsigned int tfs_seek(   unsigned int file_descriptor,
                         unsigned int offset );

unsigned int tfs_read(   unsigned int file_descriptor,
                         char *buffer,
                         unsigned int byte_count );

unsigned int tfs_write(  unsigned int file_descriptor,
                         char *buffer,
                         unsigned int byte_count );

unsigned int tfs_close(  unsigned int file_descriptor );

unsigned int tfs_delete( unsigned int file_descriptor );


/* helper functions */

unsigned int tfs_check_fd_in_range( unsigned int fd );
unsigned int tfs_check_block_in_range( unsigned int b );
unsigned int tfs_check_file_is_open( unsigned int fd );
unsigned int tfs_check_valid_name( char *name );
unsigned int tfs_size( unsigned int file_descriptor );
unsigned int tfs_new_directory_entry();
unsigned int tfs_map_name_to_fd( char *name );
unsigned int tfs_new_block();


#endif /* TFS_H_ */
