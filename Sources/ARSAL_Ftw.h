/**
 * @file ARSAL_Ftw.h
 * @brief libARSAL Ftw header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARSAL_FTW_PRIVATE_H_
#define _ARSAL_FTW_PRIVATE_H_

#ifdef HAVE_FTW_H
//The ftw.h will provide all defined values
#else
/**
 * Custom incomplete ftw/nftw implementation for system without support
 * (Android as of ndk r7, r8, r9)
 */

/**
 * Internal result values get from ARSAL_NftwCallback return analyse 
 */
typedef enum 
{
    ARSAL_FTW_FAIL = 0,
    ARSAL_FTW_OK,
} eARSAL_FTW_RESULT;

/**
 * @brief ftw-like function legacy support implementation, Recursively descends the directory hierarchy
 * @param dirpath The directory to descend
 * @param cb The callback recursively on each element of run through the directories
 * @param nopenfd The maximum number of directories depth
 * @retval On success, returns 0. Otherwise, it returns -1, or callack user value
 * @see ftw standard documentation
 */
int ARSAL_Ftw_internal(const char *dirPath, ARSAL_FtwCallback cb, int nopenfd);

 /**
 * @brief nftw-like function legacy support implementation, recursively descends the directory hierarchy
 * @param dirpath The directory to descend
 * @param cb The callback recursively on each element of run through the directories
 * @param nopenfd The maximum number of directories depth
 * @param flags The flag of the type of tree explore
 * @param currentLevel The current depth level, should be 0
 * @param currentBase The base depth level, should be 0
 * @retval On success, returns 0. Otherwise, it returns -1, or callack user value
 * @see nftw standard documentation
 */
int ARSAL_Nftw_internal(const char *dirPath, ARSAL_NftwCallback cb, int nopenfd, eARSAL_FTW_FLAG flags, int currentLevel, int currentBase);

#endif /* HAVE_FTW_H */
#endif /* _ARSAL_FTW_PRIVATE_H_ */

