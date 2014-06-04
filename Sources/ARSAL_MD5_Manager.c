/**
 * @file ARSAL_MD5.c
 * @brief MD5 manager allow compute and check md5.
 * @date 02/06/2014
 * @author david.flattin.ext@parrot.com
 */
 
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "libARSAL/ARSAL_Error.h"
#include "libARSAL/ARSAL_Print.h"
#include "libARSAL/ARSAL_MD5_Manager.h"  

#define ARUTILS_MD5_TAG "Md5"

ARSAL_MD5_Manager_t* ARSAL_MD5_Manager_New(eARSAL_ERROR *error)
{
    ARSAL_MD5_Manager_t* newManager = NULL;
    eARSAL_ERROR result = ARSAL_OK;
    
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "");
    
    newManager = calloc(1, sizeof(ARSAL_MD5_Manager_t));
    
    if (newManager == NULL)
    {
        result = ARSAL_ERROR_ALLOC;
    }
    
    *error = result;
    return newManager;
}

void ARSAL_MD5_Manager_Delete(ARSAL_MD5_Manager_t **managerAddr)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "");
    
    if (managerAddr != NULL)
    {
        ARSAL_MD5_Manager_t* manager = *managerAddr;
        
        if (manager != NULL)
        {
            free(manager);
        }
        
        *managerAddr = NULL;
    }
}

eARSAL_ERROR ARSAL_MD5_Manager_Check(ARSAL_MD5_Manager_t *manager, const char *filePath, const char *md5Txt)
{
    eARSAL_ERROR result = ARSAL_OK;
    
    if ((manager == NULL) || (manager->md5Check == NULL))
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }
    
    if (result == ARSAL_OK)
    {
        result = manager->md5Check(manager->md5Object, filePath, md5Txt);
    }
    
    return result;
}

eARSAL_ERROR ARSAL_MD5_Manager_Compute(ARSAL_MD5_Manager_t *manager, const char *filePath, uint8_t *md5, int md5Len)
{
    eARSAL_ERROR result = ARSAL_OK;
    
    if ((manager == NULL) || (manager->md5Compute == NULL))
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }
    
    if (result == ARSAL_OK)
    {
        result = manager->md5Compute(manager->md5Object, filePath, md5, md5Len);
    }
    
    return result;
}

