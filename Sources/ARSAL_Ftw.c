/**
 * @file ARSAL_Ftw.c
 * @brief libARSAL Ftw file provides ftw/nftw abstraction UNIX iOS, or implementation for Android.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com 
 **/

#include <config.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <libARSAL/ARSAL_Ftw.h>
#include <libARSAL/ARSAL_Print.h>

#ifdef HAVE_FTW_H
#define __USE_XOPEN_EXTENDED    1
#include <ftw.h>
#else
#include <dirent.h>
#include "ARSAL_Ftw.h"
#endif

#define ARSAL_FTW_TAG   "Ftw"

#ifdef HAVE_FTW_H

int ARSAL_Ftw(const char *dirpath,
        int (*fn) (const char *fpath, const struct stat *sb, int typeflag),
        int nopenfd)
{
    return ftw(dirpath, fn, nopenfd);
}

int ARSAL_Nftw(const char *dirpath,
        int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
        int nopenfd,
        int flags)
{
    return nftw(dirpath, fn, nopenfd, flags);
}

#else

int ARSAL_Ftw(const char *dirpath,
        int (*fn) (const char *fpath, const struct stat *sb, int typeflag),
        int nopenfd)
{
    return ARSAL_Ftw_internal(dirpath, fn, (nopenfd > 3) ? nopenfd-3 : nopenfd);
}

int ARSAL_Nftw(const char *dirpath,
        int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
        int nopenfd,
        int flags)
{
    return ARSAL_Nftw_internal(dirpath, fn, (nopenfd > 3) ? nopenfd-3 : nopenfd, flags, 0, 0);
}

static int typeFlagGet (struct stat *sb)
{
    if (S_ISDIR (sb->st_mode))
    {
        return FTW_D;
    }
    return FTW_F;
}

/**
 * fwt-like function
 */
int ARSAL_Ftw_internal(const char *dirPath,
        int (*cb) (const char *fpath, const struct stat *sb, int typeflag),
        int nopenfd)
{
    int retVal = 0;
    struct stat sb;
	int typeFlag;
        
    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s", dirPath ? dirPath : "null");

    if (0 >= nopenfd)
    {
        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Not enough FD");
        retVal = -1;
        return retVal;
	}  // Call stat and the callback

    lstat (dirPath, &sb);
    typeFlag = typeFlagGet(&sb);
    retVal = cb (dirPath, &sb, typeFlag);

    if (0 != retVal)
    {
        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
        return retVal;
    }

    // If we're searching a directory, call this on all this directory entries
    if (FTW_D == typeFlag)
    {
        // List the directory
        DIR *dir;
        struct dirent *ent;
        char *newName = NULL;
        int rootSize = strlen (dirPath);
        int nameSize = rootSize + 2;

		ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s is a directory !", dirPath);
		newName = (char*) malloc (nameSize);
        if (NULL == newName)
        {
            retVal = -1;
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to alloc buffer for filename");
            return retVal;
        }
        strncpy (newName, dirPath, nameSize);
        newName[rootSize] = '/';
        if (NULL == (dir = opendir (dirPath)))
        {
            retVal=-1;
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to open dir");
            return retVal;
        }

        while (NULL != (ent = readdir (dir)))
        {
			int l_nameSize;

            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Working on file %s for dir %s", ent->d_name, dirPath);
            if ('.' == ent->d_name[0] && // first char is .
                (('.' == ent->d_name[1] && '\0' == ent->d_name[2]) // second char is . (".."), or a null char (".")
                 || '\0' == ent->d_name[1])) // Third char is null ("..")
            {
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Skipping");
                // Skip "." and ".."
                continue;
            }
            l_nameSize = rootSize + strlen (ent->d_name) + 2;
            if (nameSize < l_nameSize)
            {
                nameSize = l_nameSize;
                newName = (char*) realloc (newName, nameSize);
                if (NULL == newName)
                {
                    retVal = -1;
                    closedir (dir);
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to realloc buffer");
                    return retVal;
                }
            }
            strncpy (&newName[rootSize+1], ent->d_name, strlen (ent->d_name)+1);
            retVal = ARSAL_Ftw_internal (newName, cb, nopenfd-1);
            if (0 != retVal)
            {
                closedir (dir);
                free (newName);
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
                return retVal;
            }
        }
        free (newName);
        closedir (dir);
    }
    return retVal;
}

static int Nftw_retValTest (int retVal, int flags, int isDir)
{
    int testVal = AR_FTW_FAIL;
    if (FTW_ACTIONRETVAL == flags)
    {
        switch(retVal)
        {
        case FTW_CONTINUE:
            testVal = AR_FTW_OK;
            break;
        case FTW_SKIP_SUBTREE:
            testVal = (1 == isDir) ? AR_FTW_FAIL : AR_FTW_OK;
            break;
        case FTW_STOP:
            testVal = AR_FTW_FAIL;
            break;
        default:
            testVal = AR_FTW_FAIL;
            break;
        }
    }
    else
    {
        testVal = (0 == retVal) ? AR_FTW_OK : AR_FTW_FAIL;
    }
    return testVal;
}

/**
 * nftw-like function
 */
int ARSAL_Nftw_internal(const char *dirPath,
        int (*cb) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
        int nopenfd, int flags, int currentLevel, int currentBase)
{
    int retVal = 0;
	struct FTW cbStruct = {currentBase, currentLevel};
	struct stat sb;
	int typeFlag;
        
    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s", dirPath ? dirPath : "null");

    if (FTW_NOFLAGS != flags &&
        FTW_ACTIONRETVAL != flags)
    {
        retVal = -1;
        return retVal;
    }

    if (0 >= nopenfd)
    {
        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Not enough FD");
        retVal = -1;
        return retVal;
    }
    // Call stat and the callback

    lstat (dirPath, &sb);
    typeFlag = typeFlagGet(&sb);
    retVal = cb (dirPath, &sb, typeFlag, &cbStruct);
    if (AR_FTW_FAIL == Nftw_retValTest(retVal, flags, FTW_D == typeFlag))
    {
         ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
        return retVal;
    }

    // If we're searching a directory, call this on all this directory entries
    if (FTW_D == typeFlag)
    {
        // List the directory
        DIR *dir;
        struct dirent *ent;
        char *newName = NULL;
        int rootSize = strlen (dirPath);
        int nameSize = rootSize + 2;

		 ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s is a directory !", dirPath);
		newName = (char*)malloc (nameSize);
        if (NULL == newName)
        {
            retVal = -1;
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to alloc buffer for filename");
            return retVal;
        }
        strncpy (newName, dirPath, nameSize);
        newName[rootSize] = '/';
        if (NULL == (dir = opendir (dirPath)))
        {
            retVal=-1;
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to open dir");
            return retVal;
        }

        while (NULL != (ent = readdir (dir)))
        {
            int l_nameSize;

            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Working on file %s for dir %s", ent->d_name, dirPath);
            if ('.' == ent->d_name[0] && // first char is .
                (('.' == ent->d_name[1] && '\0' == ent->d_name[2]) // second char is . (".."), or a null char (".")
                 || '\0' == ent->d_name[1])) // Third char is null ("..")
            {
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Skipping");
                // Skip "." and ".."
                continue;
            }
            l_nameSize = rootSize + strlen (ent->d_name) + 2;
            if (nameSize < l_nameSize)
            {
                nameSize = l_nameSize;
                newName = (char*)realloc (newName, nameSize);
                if (NULL == newName)
                {
                    retVal = -1;
                    closedir (dir);
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to realloc buffer");
                    return retVal;
                }
            }
            strncpy (&newName[rootSize+1], ent->d_name, strlen (ent->d_name)+1);
            retVal = ARSAL_Nftw_internal (newName, cb, nopenfd-1, flags, currentLevel+1, strlen (dirPath) + 1);
            if (AR_FTW_FAIL == Nftw_retValTest (retVal, flags, 0))
            {
                closedir (dir);
                free (newName);
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
                return retVal;
            }
        }
        free (newName);
        closedir (dir);
    }
    return retVal;
}


#endif /* HAVE_FTW_H */
