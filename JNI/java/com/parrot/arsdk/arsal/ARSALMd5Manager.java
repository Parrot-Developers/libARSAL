
package com.parrot.arsdk.arsal;


class ARSALMd5Manager
{
    /* Native Functions */
    private native static boolean nativeStaticInit();
    private native long nativeNew() throws ARUtilsException;
    private native int nativeDelete(long jManager);
    private native int nativeInit(long jManager, ARSALMd5 md5);
    private native int nativeClose(long jManager);
    
    private native int nativeCheck(String filePath, String md5);
    private native String nativeCompute(String filePath) throws ARSALException;
    
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
    
    //ARSAL_ERROR_ENUM check(String filePath, String md5)
    
    //void compute(String filePath) throws ARSALException
}
    

