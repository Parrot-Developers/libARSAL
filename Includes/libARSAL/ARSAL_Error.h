#ifndef _ARSAL_ERROR_H_
#define _ARSAL_ERROR_H_

/**
 * @brief libARSAL errors known.
 */
typedef enum
{
    ARSAL_OK = 0,                              /**< No error */

    ARSAL_ERROR = -1000,                       /**< ARSAL Generic error */
    ARSAL_ERROR_ALLOC,                         /**< ARSAL alloc error */
    ARSAL_ERROR_SYSTEM,                        /**< ARSAL system error */
    ARSAL_ERROR_BAD_PARAMETER,                 /**< ARSAL bad parameter error */
    ARSAL_ERROR_FILE,                          /**< ARSAL file error */
    
    ARSAL_ERROR_MD5 = -2000,                   /**< ARSAL md5 error */

    ARSAL_ERROR_BLE_CONNECTION = -5000,        /**< BLE connection generic error */
    ARSAL_ERROR_BLE_NOT_CONNECTED,             /**< BLE is not connected */
    ARSAL_ERROR_BLE_DISCONNECTION,             /**< BLE disconnection error */
    ARSAL_ERROR_BLE_SERVICES_DISCOVERING,      /**< BLE network services discovering error */
    ARSAL_ERROR_BLE_CHARACTERISTICS_DISCOVERING,    /**< BLE network characteristics discovering error */
    ARSAL_ERROR_BLE_CHARACTERISTIC_CONFIGURING     /**< BLE network characteristic configuring error */

} eARSAL_ERROR;

/**
 * @brief Gets the error string associated with an eARSAL_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARSAL_Error_ToString (eARSAL_ERROR error);

#endif /* _ARSAL_ERROR_H_ */


