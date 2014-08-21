/**
 * @file ARSAL_MD5_OpenSSL.c
 * @brief MD5 OpenSSL backend for MD5 Manager.
 * @date 08/21/2014
 * @author hugo.grostabussiat@parrot.com
 */
 
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <openssl/evp.h>
#include "libARSAL/ARSAL_Error.h"
#include "libARSAL/ARSAL_Print.h"
#include "libARSAL/ARSAL_MD5_Manager.h"

#define ARUTILS_MD5_TAG         "MD5_OpenSSL"


/* Exported functions prototypes. */
eARSAL_ERROR ARSAL_MD5_Manager_Init(ARSAL_MD5_Manager_t *manager);
void ARSAL_MD5_Manager_Close(ARSAL_MD5_Manager_t *manager);

/* Local function prototypes */
static eARSAL_ERROR ARSAL_MD5_Check(void *md5Object, const char *filePath, const char *md5Txt);
static eARSAL_ERROR ARSAL_MD5_Compute(void *md5Object, const char *filePath, uint8_t *md5, int md5Len);
static eARSAL_ERROR ARSAL_MD5_GetMd5AsTxt(const uint8_t *md5, int md5Len, char *md5Txt, int md5TxtLen);

eARSAL_ERROR ARSAL_MD5_Manager_Init(ARSAL_MD5_Manager_t *manager)
{
    eARSAL_ERROR result = ARSAL_OK;
    
    if (manager == NULL)
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }
    
    if (result == ARSAL_OK)
    {
        /* Load digests. */
        OpenSSL_add_all_digests();
    }

    if (result == ARSAL_OK)
    {
        manager->md5Check = ARSAL_MD5_Check;
        manager->md5Compute = ARSAL_MD5_Compute;
        manager->md5Object = NULL;
    }
    
    return result;
}

void ARSAL_MD5_Manager_Close(ARSAL_MD5_Manager_t *manager)
{
    if (manager != NULL)
    {
        manager->md5Check = NULL;
        manager->md5Compute = NULL;
        manager->md5Object = NULL;
    }
}

static eARSAL_ERROR ARSAL_MD5_Check(void *md5Object, const char *filePath, const char *md5Txt)
{
    eARSAL_ERROR result = ARSAL_OK;
    uint8_t digest[EVP_MAX_MD_SIZE];
    char md5Src[(EVP_MAX_MD_SIZE * 2) + 1];
    
    if ((filePath == NULL) || (md5Txt == NULL))
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }
    
    if (result == ARSAL_OK)
    {
        result = ARSAL_MD5_Compute(md5Object, filePath, digest, sizeof(digest));
    }
    
    if (result == ARSAL_OK)
    {
        result = ARSAL_MD5_GetMd5AsTxt(digest, sizeof(digest), md5Src, sizeof(md5Src));
    }
    
    if (result == ARSAL_OK)
    {
        if (strcmp(md5Txt, md5Src) != 0)
        {
            ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "MD5 mismatch: src=%s dst=%s", md5Src, md5Txt);
            result = ARSAL_ERROR_MD5;
        }
        else
        {
            ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MD5_TAG, "MD5 match: src=dst=%s", md5Src);
        }
    }
    
    return result;
}

static eARSAL_ERROR ARSAL_MD5_Compute(void *md5Object, const char *filePath, uint8_t *md5, int md5Len)
{
    eARSAL_ERROR result = ARSAL_OK;
    EVP_MD_CTX* ctx;
    size_t digest_size;
    FILE *file;
    size_t count;
    uint8_t *block = NULL;
    long blocksize;

    if ((filePath == NULL) || (md5 == NULL))
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }

    if (result == ARSAL_OK)
    {
        ctx = EVP_MD_CTX_create();
        if (ctx == NULL)
        {
            result = ARSAL_ERROR_ALLOC;
        }
    }

    if (result == ARSAL_OK)
    {
        EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
        digest_size = EVP_MD_CTX_size(ctx);
        assert(digest_size > 0);
        if (md5Len < digest_size)
        {
            result = ARSAL_ERROR_BAD_PARAMETER;
        }
    }

    /* Use page size as block size. */
    if (result == ARSAL_OK)
    {
        blocksize = sysconf(_SC_PAGESIZE);
        if (blocksize == -1 && errno == EINVAL)
        {
            /* Default block size value if page size is unavailable. */
            blocksize = 4096;
        }

        block = malloc(blocksize);
        if (block == NULL)
        {
            result = ARSAL_ERROR_ALLOC;
        }
    }
    
    if (result == ARSAL_OK)
    {
        file = fopen(filePath, "rb");
        if (file == NULL)
        {
            result = ARSAL_ERROR_FILE;
        }
    }
    
    if (result == ARSAL_OK)
    {
        while ((count = fread(block, sizeof(uint8_t), blocksize, file)) > 0)
        {
            EVP_DigestUpdate(ctx, block, count);
        }
        EVP_DigestFinal(ctx, md5, NULL);
    }
    
    if (file != NULL)
    {
        fclose(file);
    }

    if (ctx != NULL)
    {
        EVP_MD_CTX_destroy(ctx);
        ctx = NULL;
    }

    free(block);
    block = NULL;
    
    return result;
}

static eARSAL_ERROR ARSAL_MD5_GetMd5AsTxt(const uint8_t *md5, int md5Len, char *md5Txt, int md5TxtLen)
{
    eARSAL_ERROR result = ARSAL_OK;
    int i;
    size_t digest_size = EVP_MD_size(EVP_md5());
    
    if ((md5 == NULL) || (md5Len < digest_size) || (md5Txt == NULL) || (md5TxtLen < ((digest_size *2) + 1)))
    {
        result = ARSAL_ERROR_BAD_PARAMETER;
    }
    
    for (i= 0; i < digest_size; i++)
    {
        sprintf(&md5Txt[i * 2], "%02x", md5[i]);
    }
    md5Txt[digest_size * 2] = '\0';

    return result;
}
