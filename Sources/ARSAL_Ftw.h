/**
 * @file ARSAL_Ftw.h
 * @brief libARSAL Ftw header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com 
 **/

#ifndef _ARSAL_FTW_PRIVATE_H_
#define _ARSAL_FTW_PRIVATE_H_

#ifdef HAVE_FTW_H
//The <ftw.h> will provide all defined values
#else
/**
 * Custom incomplete ftw/nftw implementation for system without support
 * (Android as of ndk r7, r8, r9)
 */

/**
 * Internal result values
 */
enum {
    AR_FTW_FAIL = 0,
    AR_FTW_OK,
};

/**
 * Internal define
 */
#define FTW_NOFLAGS (0)
 
/**
 * @brief ftw-like function legacy support implementation, Recursively descends the directory hierarchy 
 * @param dirpath The directory to descend
 * @param cb The callback recursively on each element of run through the directories
 * @param nopenfd The maximum number of directories depth
 * @see ftw standard documentation
 */ 
int ARSAL_Ftw_internal(const char *dirPath,
        int (*cb) (const char *fpath, const struct stat *sb, int typeflag),
        int nopenfd);


 /**
 * @brief nftw-like function legacy support implementation, recursively descends the directory hierarchy 
 * @param dirpath The directory to descend
 * @param cb The callback recursively on each element of run through the directories
 * @param nopenfd The maximum number of directories depth
 * @param flags The flag of the type of tree explore
 * @param currentLevel The current depth level, should be 0
 * @param currentBase The base depth level, should be 0
 * @see nftw standard documentation
 */
int ARSAL_Nftw_internal(const char *dirPath,
        int (*cb) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
        int nopenfd, 
        int flags, 
        int currentLevel, 
        int currentBase);

#endif /* HAVE_FTW_H */
#endif /* _ARSAL_FTW_PRIVATE_H_ */

