
package com.parrot.arsdk.arsal;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import com.parrot.arsdk.arsal.ARSALPrint;

class ARSALMd5
{
    private static final String TAG = "Md5";
    public final static int MD5_LENGTH = 16;
	
    private MessageDigest digest = null;
    
    public ARSALMd5()
    {
        //initialize();
    }
    
    /*public boolean initialize()
    {
        boolean ret = true;
        try
        {
            digest = java.security.MessageDigest.getInstance("MD5");
        }
        catch (NoSuchAlgorithmException e)
        {
            ARSALPrint.d(TAG, e.toString());
            ret = false;
        }
        
        return ret;
    }
    
    public void update(byte[] buffer, int index, int len)
    {
        digest.update(buffer, index, len);
    }
    
    public byte[] digest()
    {
        return digest.digest();
    }*/
    
    //ARSAL_ERROR_ENUM Check(String filePath, String md5)
    //ARSAL_ERROR_ENUM Compute(String filePath, String md5)
}


