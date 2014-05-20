/*
 * GENERATED FILE
 *  Do not modify this file, it will be erased during the next configure run
 */

/**
 * @file ARSAL_Error.c
 * @brief ToString function for eARSAL_ERROR enum
 */

#include <libARSAL/ARSAL_Error.h>

char* ARSAL_Error_ToString (eARSAL_ERROR error)
{
    switch (error)
    {
    case ARSAL_OK:
        return "No error";
        break;
    case ARSAL_ERROR_BLE_CONNECTION:
        return "BLE connection generic error";
        break;
    case ARSAL_ERROR_BLE_NOT_CONNECTED:
        return "BLE is not connected";
        break;
    case ARSAL_ERROR_BLE_DISCONNECTION:
        return "BLE disconnection error";
        break;
    case ARSAL_ERROR_BLE_SERVICES_DISCOVERING:
        return "BLE network services discovering error";
        break;
    case ARSAL_ERROR_BLE_CHARACTERISTICS_DISCOVERING:
        return "BLE network characteristics discovering error";
        break;
    case ARSAL_ERROR_BLE_CHARACTERISTIC_CONFIGURING:
        return "BLE network characteristic configuring error";
        break;
    default:
        return "Unknown value";
        break;
    }
    return "Unknown value";
}
