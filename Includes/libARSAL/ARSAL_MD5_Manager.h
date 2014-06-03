/**
 * @file ARSAL_MD5.h
 * @brief libARSAL MD5 header file.
 * @date 02/06/2014
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARSAL_MD5_H_
#define _ARSAL_MD5_H_

#define ARSAL_MD5_LENGTH        16

/**
 * @brief Check an MD5
 * @param filePath The file path onto check its md5
 * @param md5 The md5 string
 * @param md5Len md5 string length
 * @retval On success, returns ARSAL_OK. Otherwise, it returns an error number of eARSAL_ERROR
 * @see ARSAL_MD5_Manager_Init ()
 */
typedef eARSAL_ERROR (*ARSAL_MD5_Check_t)(const char *filePath, const char *md5, int md5Len);

/**
 * @brief Compute an MD5
 * @param filePath The file path onto check its md5
 * @param[out] md5 The md5 string to receive the md5
 * @param md5Len md5 string buffer length
 * @retval On success, returns ARSAL_OK. Otherwise, it returns an error number of eARSAL_ERROR
 * @see ARSAL_MD5_Manager_Init ()
 */
typedef eARSAL_ERROR (*ARSAL_MD5_Compute_t)(const char *filePath, uint8_t *md5, int md5Len);

/**
 * @brief MD5 Manager structure
 * @retval md5Check The Check function
 * @retval md5Compute The Compute function
 * @retval md5Object The md5 object
 * @see ARSAL_MD5_Manager_New
 */
typedef struct _ARSAL_MD5_Manager_t 
{
    ARSAL_MD5_Check_t md5Check;
    ARSAL_MD5_Compute_t md5Compute;
    void *md5Object;
} ARSAL_MD5_Manager_t;


/**
 * @brief Create a new ARSAL MD5 Manager
 * @warning This function allocates memory
 * @param[out] error A pointer on the error output
 * @return Pointer on the new ARSAL MD5 Manager
 * @see ARSAL_MD5_Manager_Delete ()
 */
ARSAL_MD5_Manager_t* ARSAL_MD5_Manager_New(eARSAL_ERROR *error);

/**
 * @brief Delete an ARSAL MD5 Manager
 * @warning This function frees memory
 * @param manager The address of the pointer on the ARSAL MD5 Manager
 * @see ARSAL_MD5_Manager_New ()
 */
void ARSAL_MD5_Manager_Delete(ARSAL_MD5_Manager_t **managerAddr);

/**
 * @brief Initialize an ARSAL MD5 Manager
 * @param manager The MD5 Manager
 * @retval On success, returns ARSAL_OK. Otherwise, it returns an error number of eARSAL_ERROR
 * @see ARSAL_MD5_Manager_Close ()
 */
eARSAL_ERROR ARSAL_MD5_Manager_Init(ARSAL_MD5_Manager_t *manager);

/**
 * @brief Close an ARSAL MD5 Manager
 * @param manager The MD5 Manager
 * @retval On success, returns ARSAL_OK. Otherwise, it returns an error number of eARSAL_ERROR
 * @see ARSAL_MD5_Manager_Init ()
 */
void ARSAL_MD5_Manager_Close(ARSAL_MD5_Manager_t *manager);

/**
 * @brief Check an MD5
 * @param manager The MD5 Manager
 * @param filePath The file path onto check its md5
 * @param md5 The md5 string
 * @param md5Len md5 string length
 * @retval On success, returns ARSAL_OK. Otherwise, it returns an error number of eARSAL_ERROR
 * @see ARSAL_MD5_Manager_New ()
 */
eARSAL_ERROR ARSAL_MD5_Manager_Check(ARSAL_MD5_Manager_t *manager, const char *filePath, const char *md5, int md5Len);

/**
 * @brief Compute an MD5
 * @param manager The MD5 Manager
 * @param filePath The file path onto check its md5
 * @param[out] md5 The md5 string to receive the md5
 * @param md5Len md5 string buffer length
 * @retval On success, returns ARSAL_OK. Otherwise, it returns an error number of eARSAL_ERROR
 * @see ARSAL_MD5_Manager_New ()
 */
eARSAL_ERROR ARSAL_MD5_Manager_Compute(ARSAL_MD5_Manager_t *manager, const char *filePath, uint8_t *md5, int md5Size);


#endif /* _ARSAL_MD5_H_ */


