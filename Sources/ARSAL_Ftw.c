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

#include "libARSAL/ARSAL_Ftw.h"
#include "libARSAL/ARSAL_Print.h"

#ifdef HAVE_FTW_H
#define __USE_XOPEN_EXTENDED    1
#include <ftw.h>
#else
#include <dirent.h>
#include "ARSAL_Ftw.h"
#endif

#define ARSAL_FTW_TAG   "Ftw"

#ifdef HAVE_FTW_H

int ARSAL_Ftw(const char *dirpath, ARSAL_FtwCallback cb, int nopenfd)
{
    return ftw(dirpath, (int (*) (const char *, const struct stat *, int))cb, nopenfd);
}

int ARSAL_Nftw(const char *dirpath, ARSAL_NftwCallback cb, int nopenfd, eARSAL_FTW_FLAG flags)
{
    return nftw(dirpath, (int (*) (const char *, const struct stat *, int, struct FTW *))cb, nopenfd, flags);
}

#else

int ARSAL_Ftw(const char *dirpath, ARSAL_FtwCallback cb, int nopenfd)
{
    return ARSAL_Ftw_internal(dirpath, cb, nopenfd);
}

int ARSAL_Nftw(const char *dirpath, ARSAL_NftwCallback cb, int nopenfd, eARSAL_FTW_FLAG flags)
{
    return ARSAL_Nftw_internal(dirpath, cb, nopenfd, flags, 0, 0);
}

static eARSAL_FTW_TYPE ARSAL_Ftw_typeFlagGet (struct stat *sb)
{
    if (S_ISDIR (sb->st_mode))
    {
        return ARSAL_FTW_D;
    }
    return ARSAL_FTW_F;
}

/**
 * fwt-like function
 */
int ARSAL_Ftw_internal(const char *dirPath, ARSAL_FtwCallback cb, int nopenfd)
{
    eARSAL_FTW_TYPE typeFlag;
    struct stat sb;
    int retVal = 0;

    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s", dirPath ? dirPath : "null");
    
    if (dirPath == NULL)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, ARSAL_FTW_TAG, "dirPath is NULL !");
        retVal = -1;
    }

    if (cb == NULL)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, ARSAL_FTW_TAG, "Callback is NULL !");
        retVal = -1;
    }
    
    if (nopenfd <= 0)
    {
        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Not enough FD");
        retVal = -1;
    }  
    
    if (retVal == 0)
    {
        // Call stat and the callback
        retVal = lstat (dirPath, &sb);
        if (retVal != 0)
        {
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to lstat");
        }
        else
        {
            typeFlag = ARSAL_Ftw_typeFlagGet(&sb);
            
            if (typeFlag != ARSAL_FTW_D)
            {
                retVal = cb (dirPath, &sb, typeFlag);
                if (retVal != 0)
                {
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
                }
            }
        }
    }

    // If we're searching a directory, call this on all this directory entries
    if ((retVal == 0) && (typeFlag == ARSAL_FTW_D))
    {
        // List the directory
        DIR *dir = NULL;
        struct dirent *ent;
        char *newName = NULL;
        int rootSize = strlen (dirPath);
        int nameSize = rootSize + 2;

        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s is a directory !", dirPath);
        newName = (char*) malloc (nameSize);
        if (newName == NULL)
        {
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to alloc buffer for filename");
            retVal = -1;
        }
        
        if (retVal == 0)
        {
            strncpy (newName, dirPath, nameSize);
            newName[rootSize] = '/';
            if ((dir = opendir (dirPath)) == NULL)
            {
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to open dir");
                retVal = -1;
            }
        }
        
        if (retVal == 0)
        {
            retVal = cb (dirPath, &sb, typeFlag);
            if (retVal != 0)
            {
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
            }
        }

        while ((retVal == 0) && ((ent = readdir (dir)) != NULL))
        {
            int l_nameSize;

            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Working on file %s for dir %s", ent->d_name, dirPath);
            if ((ent->d_name[0] == '.') && // first char is .
                (((ent->d_name[1] == '.') && (ent->d_name[2] == '\0')) // second char is . (".."), or a null char (".")
                 || (ent->d_name[1] == '\0'))) // Third char is null ("..")
            {
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Skipping");
                // Skip "." and ".."
                continue;
            }
            l_nameSize = rootSize + strlen (ent->d_name) + 2;
            if (nameSize < l_nameSize)
            {
                nameSize = l_nameSize;
                char *prevNewName = newName;
                newName = (char*) realloc (newName, nameSize);
                if (newName == NULL)
                {
                    retVal = -1;
                    free (prevNewName);
                    closedir (dir);
                    dir = NULL;
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to realloc buffer");
                }
            }
            
            if (retVal == 0)
            {
                strncpy (&newName[rootSize + 1], ent->d_name, strlen (ent->d_name) + 1);
                retVal = ARSAL_Ftw_internal (newName, cb, nopenfd - 1);
                if (retVal != 0)
                {
                    closedir (dir);
                    dir = NULL;
                    free (newName);
                    newName = NULL;
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
                }
            }
        }
        
        if (newName != NULL)
        {
            free (newName);
            newName = NULL;
        }
        if (dir != NULL)
        {
            closedir (dir);
            dir = NULL;
        }
    }
    
    return retVal;
}

static eARSAL_FTW_RESULT ARSAL_Ftw_Nftw_retValTest (int retVal, eARSAL_FTW_FLAG flags, int isDir)
{
    eARSAL_FTW_RESULT testVal = ARSAL_FTW_FAIL;
    
    if (flags == ARSAL_FTW_ACTIONRETVAL)
    {
        switch(retVal)
        {
        case ARSAL_FTW_CONTINUE:
            testVal = ARSAL_FTW_OK;
            break;
        case ARSAL_FTW_SKIP_SUBTREE:
            testVal = (isDir == 1) ? ARSAL_FTW_FAIL : ARSAL_FTW_OK;
            break;
        case ARSAL_FTW_STOP:
            testVal = ARSAL_FTW_FAIL;
            break;
        default:
            testVal = ARSAL_FTW_FAIL;
            break;
        }
    }
    else
    {
        testVal = (retVal == 0) ? ARSAL_FTW_OK : ARSAL_FTW_FAIL;
    }
    return testVal;
}

/**
 * nftw-like function
 */
int ARSAL_Nftw_internal(const char *dirPath, ARSAL_NftwCallback cb, int nopenfd, eARSAL_FTW_FLAG flags, int currentLevel, int currentBase)
{
    ARSAL_FTW_t cbStruct = {currentBase, currentLevel};
    eARSAL_FTW_TYPE typeFlag;
    struct stat sb;
    int retVal = 0;

    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s", dirPath ? dirPath : "null");

    if (dirPath == NULL)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, ARSAL_FTW_TAG, "dirPath is NULL !");
        retVal = -1;
    }
    
    if (cb == NULL)
    {
        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback is NULL !");
        retVal = -1;
    }    

    if (flags != ARSAL_FTW_NOFLAGS &&
        flags != ARSAL_FTW_ACTIONRETVAL)
    {
        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unsupported flag !");
        retVal = -1;
    }

    if (nopenfd <= 0)
    {
        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Not enough FD");
        retVal = -1;
    }
    
    if (retVal == 0)
    {
        // Call stat and the callback
        retVal = lstat (dirPath, &sb);
        if (retVal != 0)
        {
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to lstat");
        }
        else
        {
            typeFlag = ARSAL_Ftw_typeFlagGet(&sb);
            
            if (typeFlag != ARSAL_FTW_D)
            {
                retVal = cb (dirPath, &sb, typeFlag, &cbStruct);
                
                if (ARSAL_Ftw_Nftw_retValTest(retVal, flags, typeFlag == ARSAL_FTW_D) == ARSAL_FTW_FAIL)
                {
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
                }
                else
                {
                    retVal = 0;
                }
            }
        }
    }

    // If we're searching a directory, call this on all this directory entries
    if ((retVal == 0) && (typeFlag == ARSAL_FTW_D))
    {
        // List the directory
        DIR *dir = NULL;
        struct dirent *ent;
        char *newName = NULL;
        int rootSize = strlen (dirPath);
        int nameSize = rootSize + 2;

        ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "%s is a directory !", dirPath);
        newName = (char*)malloc (nameSize);
        if (newName == NULL)
        {
            retVal = -1;
            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to alloc buffer for filename");
        }
        
        if (retVal == 0)
        { 
            strncpy (newName, dirPath, nameSize);
            newName[rootSize] = '/';
            if ((dir = opendir (dirPath)) == NULL)
            {
                retVal = -1;
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to open dir");
            }
        }
        
        if (retVal == 0)
        {
            retVal = cb (dirPath, &sb, typeFlag, &cbStruct);
            if (ARSAL_Ftw_Nftw_retValTest(retVal, flags, typeFlag == ARSAL_FTW_D) == ARSAL_FTW_FAIL)
            {
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
            }
            else
            {
                retVal = 0;
            }
        }

        while ((retVal == 0) && (ent = readdir (dir)) != NULL)
        {
            int l_nameSize;

            ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Working on file %s for dir %s", ent->d_name, dirPath);
            if ((ent->d_name[0] == '.') && // first char is .
                (((ent->d_name[1] == '.') && (ent->d_name[2] == '\0')) // second char is . (".."), or a null char (".")
                 || (ent->d_name[1] == '\0'))) // Third char is null ("..")
            {
                ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Skipping");
                // Skip "." and ".."
                continue;
            }
            l_nameSize = rootSize + strlen (ent->d_name) + 2;
            if (nameSize < l_nameSize)
            {
                nameSize = l_nameSize;
                char *prevNewName = newName;
                newName = (char*)realloc (newName, nameSize);
                if (newName == NULL)
                {
                    retVal = -1;
                    free (prevNewName);
                    closedir (dir);
                    dir = NULL;
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Unable to realloc buffer");
                }
            }
            
            if (retVal == 0)
            {
                strncpy (&newName[rootSize + 1], ent->d_name, strlen (ent->d_name) + 1);
                retVal = ARSAL_Nftw_internal (newName, cb, nopenfd - 1, flags, currentLevel + 1, strlen (dirPath) + 1);
                
                if (ARSAL_Ftw_Nftw_retValTest (retVal, flags, 0) == ARSAL_FTW_FAIL)
                {
                    closedir (dir);
                    dir = NULL;
                    free (newName);
                    newName = NULL;
                    ARSAL_PRINT (ARSAL_PRINT_DEBUG, ARSAL_FTW_TAG, "Callback said stop");
                }
                else
                {
                    retVal = 0;
                }
            }
        }
        if (newName != NULL)
        {
            free (newName);
        }
        if (dir != NULL)
        {
            closedir (dir);
        }
    }
    return retVal;
}

#endif /* HAVE_FTW_H */
