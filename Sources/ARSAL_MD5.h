/**
 * @file ARSAL_MD5.h
 * @brief libARSAL MD5 header file.
 * @date 02/06/2014
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARSAL_MD5_PRIVATE_H_
#define _ARSAL_MD5_PRIVATE_H_



eARSAL_ERROR ARSAL_MD5_Check(const char *filePath, const char *md5, int md5Len);

eARSAL_ERROR ARSAL_MD5_Compute(const char *filePath, char *md5, int md5Len);

#endif /* _ARSAL_MD5_PRIVATE_H_ */

