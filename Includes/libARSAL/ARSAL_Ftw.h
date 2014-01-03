/**
 * @file ARSAL_Ftw.h
 * @brief libARSAL Ftw header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARSAL_FTW_H_
#define _ARSAL_FTW_H_

#ifdef HAVE_FTW_H
#define __USE_XOPEN_EXTENDED    1
#include <sys/stat.h>
#include <ftw.h>

#else

/**
 * @brief FTW structure
 * @param base The base 
 * @param level The level
 * @see ARSAL_Nftw
 */
struct FTW {
    int base;
    int level;
};

/**
 * @brief FTW type enum
 * @see ARSAL_Ftw, ARSAL_Nftw
 */
enum {
    FTW_F = 0,
    FTW_D,
};

/**
 * @brief ACTIONRETVAL enum
 * @see ARSAL_Nftw
 */
enum {
    FTW_ACTIONRETVAL = 16,
};
 
/**
 * @brief ARSAL_Nftw enum flags
 * @see ARSAL_Nftw
 */
enum {
    FTW_CONTINUE = 0,
    FTW_STOP = 1,
    FTW_SKIP_SUBTREE = 2,
};

#endif /* #ifdef HAVE_FTW_H */

/**
 * @brief Recursively descends the directory hierarchy 
 * @param dirpath The directory to descend
 * @param fn The callback recursively on each element of run through the directories
 * @param nopenfd The maximum number of directories depth
 * @see ftw standard documentation
 */
int ARSAL_Ftw(const char *dirpath,
    int (*fn) (const char *fpath, const struct stat *sb, int typeflag),
    int nopenfd);

/**
 * @brief Recursively descends the directory hierarchy 
 * @param dirpath The directory to descend
 * @param fn The callback recursively on each element of run through the directories
 * @param nopenfd The maximum number of directories depth
 * @param flags The flag of the type of tree explore
 * @see nftw standard documentation
 */
int ARSAL_Nftw(const char *dirpath,
    int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
    int nopenfd, 
    int flags);
        
#endif /* _ARSAL_FTW_H_ */


