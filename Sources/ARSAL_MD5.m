/**
 * @file ARSAL_MD5.m
 * @brief MD5 manager allow compute and check md5.
 * @date 02/06/2014
 * @author david.flattin.ext@parrot.com
 */
 
#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>
#include "libARSAL/ARSAL_Error.h"
#include "libARSAL/ARSAL_Print.h"
#include "libARSAL/ARSAL_MD5_Manager.h"
#include "ARSAL_MD5.h"
//#include "ARSAL_Singleton.h"

#define ARUTILS_MD5_TAG         "Md5"

eARSAL_ERROR ARSAL_MD5_Manager_Init(ARSAL_MD5_Manager_t *manager)
{
    eARSAL_ERROR result = ARSAL_OK;
    
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "");
    
    if (manager == NULL)
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }
    
    if (result == ARSAL_OK)
    {
        manager->md5Check = ARSAL_MD5_Check;
        manager->md5Compute = ARSAL_MD5_Compute;
    }
    
    return result;
}

void ARSAL_MD5_Manager_Close(ARSAL_MD5_Manager_t *manager)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "");
}

eARSAL_ERROR ARSAL_MD5_Check(const char *filePath, const char *md5, int md5Len)
{
    eARSAL_ERROR result = ARSAL_OK;
    char md5Txt[(CC_MD5_DIGEST_LENGTH * 2) + 1];
    
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "");
    
    result = ARSAL_MD5_Compute(filePath, md5Txt, sizeof(md5Txt));
    
    if (result == ARSAL_OK)
    {
        if (strcmp(md5Txt, md5) != 0)
        {
            result = ARSAL_ERROR_MD5;
        }
    }
    
    return result;
}

eARSAL_ERROR ARSAL_MD5_Compute(const char *filePath, char *md5, int md5Len)
{
    eARSAL_ERROR result = ARSAL_OK;
    uint8_t md5Hex[CC_MD5_DIGEST_LENGTH];
    uint8_t block[1024];
    CC_MD5_CTX ctx;
    FILE *file;
    size_t count;
    int i;
    
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "");
    
    if ((filePath == NULL) || (md5 == NULL) || (md5Len < ((CC_MD5_DIGEST_LENGTH * 2) + 1)))
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }
    
    if (result == ARSAL_OK)
    {
        CC_MD5_Init(&ctx);
        file = fopen(filePath, "rb");
        if (file == NULL)
        {
            result = ARSAL_ERROR_FILE;
        }
    }
    
    if (result == ARSAL_OK)
    {
        while ((count = fread(block, sizeof(uint8_t), sizeof(block), file)) > 0)
        {
            CC_MD5_Update(&ctx, block, count);
        }
        
        CC_MD5_Final(md5Hex, &ctx);
        
        for (i= 0; i<CC_MD5_DIGEST_LENGTH; i++)
        {
            sprintf(&md5[i * 2], "%02x", md5Hex[i]);
        }
        
        md5[CC_MD5_DIGEST_LENGTH * 2] = '\0';
    }
    
    if (file != NULL)
    {
        fclose(file);
    }
    
    return result;
}
