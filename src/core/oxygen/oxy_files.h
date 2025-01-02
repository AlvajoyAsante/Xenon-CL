#ifndef OXY_FILES_H
#define OXY_FILES_H

#include <tice.h>
#include <graphx.h>

#define OXY_BASIC_TYPE 0
#define OXY_PBASIC_TYPE 1
#define OXY_ICES_TYPE 2
#define OXY_ICE_TYPE 3
#define OXY_C_TYPE 4
#define OXY_ASM_TYPE 5
#define OXY_APPVAR_TYPE 6
#define OXY_ERROR_TYPE 7

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** 
 * @brief file dynamic pointer.
 */
struct oxy_files_t {
    uint8_t user_id;
	
	char name[9];
	uint8_t type;
	
	gfx_sprite_t *icon;
    char *description;
    
	bool archived;
    bool locked;
	bool hidden;
	
    int location;
    int size;
    
    bool pinned;
};
extern struct oxy_files_t *oxy_file;

/** 
 * @brief Folder dynamic pointer.
 */
struct oxy_folders_t {
	uint8_t user_id;
    
	char name[9];
    gfx_sprite_t *icon;
	
	bool locked;
	
    int location;
	int position;
};
extern struct oxy_folders_t *oxy_folder;

/** 
 * @brief File system information.
 */
struct oxy_file_system_t {
    int numfiles;
    int numfolders;
	int numpins;
};
extern struct oxy_file_system_t oxy_file_system;


/**
 * @brief Init the file system for use.  
 */
void oxy_InitFilesStystem(void);

void oxy_CheckFileSystem(void);

void oxy_RescanFileSystem(void);

/** 
 * @brief Detects all files on calculator (programs and app-vars).   
 */
void oxy_DetectAllFiles(void);

/* Deleting folders and files */
/** Removes folder from index in pointer.
 *  @param index   
 */
void oxy_DeleteFolder(int index);

/** Removes file from index in pointer.
 *  @param index   
 */
void oxy_DeleteFile(int index);

/* Creating folders and files */ 
/**
 * @brief Creates space to add a file.  
 */
bool oxy_AddFile(void);

/** Adds folder to position.
 * @param name Name of the 
 * @param location Int of wanted folder location.
 */
bool oxy_AddFolder(char *name, int position);

/* Detecting files icons */
/**
 * @brief Gets Asm Icon and stores into file pointer.  
 */ 
void oxy_GetAsmIcons(void);

/**
 * @brief Gets Basic Icon and stores into file pointer.  
 */ 
void oxy_GetBasicIcons(void);

/* Sorting all files */
/**
 * @brief Sorts all folders.
 */ 
void oxy_SortFolders(void);

/**
 * @brief Sorts Files.  
 */ 
void oxy_SortFiles(void);

/**
 * Gets the amount file type from oxygen's files type. 
 * @param type oxygen file type uint8_t.
 * @returns Returns TI file type.
 */ 
uint8_t oxy_GetFileType(uint8_t type);

/* Other */
/**
 * @brief Delete a file or folder.
 * @param isfolder Is the item a folder?
 * @param index Item index?
 * @returns Returns true when a file is delete, and 0 otherwise.
 */ 
bool oxy_Delete(bool isfolder, int index);

/**
 * @brief Edit a program.  
 * @returns Returns false if theres an error.
 */ 
bool oxy_EditPrgm(char *prog_name, const char *editor_name, os_runprgm_callback_t callback);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_FILES_H__ */