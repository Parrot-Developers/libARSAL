
package com.parrot.arsdk.arsal;


public class ARSALMd5Manager
{
    /* Native Functions */
    private native static boolean nativeStaticInit();
    private native long nativeNew() throws ARSALException;
    private native void nativeDelete(long jManager);
    private native int nativeInit(long jManager, ARSALMd5 md5);
    private native int nativeClose(long jManager);
    
    private native int nativeCheck(long jManager, String filePath, String md5Txt);
    private native byte[] nativeCompute(long jManager, String filePath) throws ARSALException;
    
    private long m_managerPtr;
    private boolean m_initOk;

    static
    {
        nativeStaticInit();
    }
    
    /*  Java Methods */
    
    /**
     * Constructor
     */
    public ARSALMd5Manager() throws ARSALException
    {
        m_initOk = false;
        m_managerPtr = nativeNew();

        if( m_managerPtr != 0 )
        {
            m_initOk = true;
        }
    }
    
    public long getNativeManager()
    {
        return m_managerPtr;
    }
    
    public void init() throws ARSALException
    {
        int resultCode = nativeInit(m_managerPtr, new ARSALMd5());
        
        ARSAL_ERROR_ENUM result = ARSAL_ERROR_ENUM.getFromValue(resultCode);
        if (result != ARSAL_ERROR_ENUM.ARSAL_OK)
        {
            throw new ARSALException(result);        
        }
    }
    
    public ARSAL_ERROR_ENUM close()
    {
        int resultCode = nativeClose(m_managerPtr);
        
        ARSAL_ERROR_ENUM result = ARSAL_ERROR_ENUM.getFromValue(resultCode);
        
        return result;
    }

    /**
     * Dispose
     */
    public void dispose()
    {
        if(m_initOk == true)
        {
            nativeDelete(m_managerPtr);
            m_managerPtr = 0;
            m_initOk = false;
        }
    }
    
    public ARSAL_ERROR_ENUM check(String filePath, String md5Txt)
    {
        int resultCode = nativeCheck(m_managerPtr, filePath, md5Txt);
        
        ARSAL_ERROR_ENUM result = ARSAL_ERROR_ENUM.getFromValue(resultCode);

        return result;
    }
    
    public byte[] compute(String filePath) throws ARSALException
    {
        return nativeCompute(m_managerPtr, filePath);
    }
}
    

