/**
 * @file ARSAL_JNI.h
 * @brief libARSAL JNI header file.
 * @date 03/06/2014
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARSAL_JNI_H_
#define _ARSAL_JNI_H_

#ifndef JNI_OK
#define JNI_OK      0
#endif
#ifndef JNI_FAILED
#define JNI_FAILED  -1
#endif

extern JavaVM* ARSAL_JNI_Manager_VM;


/**
 * @brief Throw a new ARSALException
 * @param env The java env
 * @param nativeError The error
 * @retval void
 * @see ARSAL_JNI_Manager_NewARSALException
 */
void ARSAL_JNI_Manager_ThrowARSALException(JNIEnv *env, eARSAL_ERROR nativeError);

/**
 * @brief Create a new ARSALException
 * @param env The java env
 * @param nativeError The error
 * @retval the new ARSALException
 * @see ARSAL_JNI_Manager_ThrowARSALException
 */
jobject ARSAL_JNI_Manager_NewARSALException(JNIEnv *env, eARSAL_ERROR nativeError);

/**
 * @brief Get the ARSALException JNI class
 * @param env The java env
 * @retval JNI_TRUE if Success, else JNI_FALSE
 * @see ARSAL_JNI_Manager_FreeARSALExceptionJNI
 */
int ARSAL_JNI_Manager_NewARSALExceptionJNI(JNIEnv *env);

/**
 * @brief Free the ARSALException JNI class
 * @param env The java env
 * @retval void
 * @see ARSAL_JNI_Manager_NewARSALExceptionJNI
 */
void ARSAL_JNI_Manager_FreeARSALExceptionJNI(JNIEnv *env);

/**
 * @brief Create a new ARSAL_ERROR_ENUM
 * @param env The java env
 * @param nativeError The error
 * @retval the new ARSAL_ERROR_ENUM
 * @see ARSAL_JNI_Manager_NewERROR_ENUM_JNI
 */
jobject ARSAL_JNI_Manager_NewERROR_ENUM(JNIEnv *env, eARSAL_ERROR nativeError);

/**
 * @brief Get the ARSAL_ERROR_ENUM JNI class
 * @param env The java env
 * @retval JNI_TRUE if Success, else JNI_FALSE
 * @see ARSAL_JNI_Manager_FreeERROR_ENUM_JNI
 */
int ARSAL_JNI_Manager_NewERROR_ENUM_JNI(JNIEnv *env);

/**
 * @brief Free the ARSAL_ERROR_ENUM JNI class
 * @param env The java env
 * @retval void
 * @see ARSAL_JNI_Manager_NewERROR_ENUM_JNI
 */
void ARSAL_JNI_Manager_FreeERROR_ENUM_JNI(JNIEnv *env);


#endif /* _ARSAL_JNI_H_ */
