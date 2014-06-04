/**
 * @file ARSAL_MD5.h
 * @brief libARSAL MD5 header file.
 * @date 02/06/2014
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARSAL_MD5_PRIVATE_H_
#define _ARSAL_MD5_PRIVATE_H_



eARSAL_ERROR ARSAL_MD5_Check(void *md5Object, const char *filePath, const char *md5Txt);

eARSAL_ERROR ARSAL_MD5_Compute(void *md5Object, const char *filePath, uint8_t *md5, int md5Len);

eARSAL_ERROR ARSAL_MD5_GetMd5AsTxt(const uint8_t *md5, int md5Len, char *md5Txt, int md5TxtLen);

#endif /* _ARSAL_MD5_PRIVATE_H_ */

