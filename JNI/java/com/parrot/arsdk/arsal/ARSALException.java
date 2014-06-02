package com.parrot.arsdk.arsal;

/**
 * Exception class: ARSALException of ARSAL library
 * @date 30/12/2013
 * @author david.flattin.ext@parrot.com
 */
public class ARSALException extends Exception
{	
    private ARSAL_ERROR_ENUM error;
    
    /**
     * ARSALException constructor
     * @return void
     */
    public ARSALException()
    {
        error = ARSAL_ERROR_ENUM.ARSAL_ERROR;
    }
    
    /**
     * ARSALException constructor
     * @param error ARSAL_ERROR_ENUM error code
     * @return void
     */
    public ARSALException(ARSAL_ERROR_ENUM error) 
    {
        this.error = error;
    }
    
    /**
     * ARSALException constructor
     * @param error int error code
     * @return void
     */
    public ARSALException(int error) 
    {
        this.error = ARSAL_ERROR_ENUM.getFromValue(error);
    }
    
    /**
     * Gets ARSAL ERROR code
     * @return {@link ARSAL_ERROR_ENUM} error code
     */
    public ARSAL_ERROR_ENUM getError()
    {
        return error;
    }
    
    /**
     * Sets ARSAL ERROR code
     * @param error {@link ARSAL_ERROR_ENUM} error code     
     * @return void
     */
    public void setError(ARSAL_ERROR_ENUM error)
    {
        this.error = error;
    }
    
    /**
     * Gets ARSALException string representation
     * @return String Exception representation
     */
    public String toString ()
    {
        String str;
        
        if (null != error)
        {
            str = "ARSALException [" + error.toString() + "]";
        }
        else
        {
            str = super.toString();
        }
        
        return str;
    }
}

