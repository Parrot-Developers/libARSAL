
package com.parrot.arsdk.arsal;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.util.Log;

public class ARSALMd5
{
    public final static int MD5_LENGTH = 16;
	
    private MessageDigest digest = null;
    private static final String TAG = "Md5";
    
    public ARSALMd5()
    {
        initialize();
    }
    
    private boolean initialize()
    {
        boolean ret = true;
        try
        {
            digest = java.security.MessageDigest.getInstance("MD5");
        }
        catch (NoSuchAlgorithmException e)
        {
            Log.d(TAG, e.toString());
            ret = false;
        }
        
        return ret;
    }
    
    public String getTextDigest(byte[] hash, int index, int len)
	{
		StringBuffer txt = new StringBuffer();
		
		for (int i=0; i<len; i++)
		{
			String val = String.format("%02x", hash[index + i] & 0x00FF);
			txt.append(val);
		}
		return txt.toString();
	}
    
    public boolean check(String filePath, String md5Txt)
    {
        byte[] md5 = null;
        boolean result = false;
        
        md5 = compute(filePath);
        
        if (md5 != null)
        {            
            if (getTextDigest(md5, 0, md5.length).compareTo(md5Txt) == 0)
            {
                result = true;
            }
        }
        
        return result;
    }
    
    public byte [] compute(String filePath)
    {
        FileInputStream src = null;
        byte[] block = new byte[1024];
        byte[] md5 = null;
        int count;
        
        try
        {
            initialize();
            src = new FileInputStream(filePath);
             
            while ((count = src.read(block, 0, block.length)) > 0)
            {
                digest.update(block, 0, count);
            }
            
            md5 = digest.digest();
            src.close();
            
            //Log.d("DBG", "ARSALMd5.compute: " + getTextDigest(md5, 0, md5.length));
        }
        catch (FileNotFoundException e)
        {
            Log.d("DBG", e.toString());
        }
        catch (IOException e)
        {
            Log.d("DBG", e.toString());
        }
        
        return md5;
    }

    public byte [] compute(byte[] data)
    {
        byte[] md5 = null;
        initialize();
        digest.update(data);
        md5 = digest.digest();
        //Log.d("DBG", "ARSALMd5.compute: " + getTextDigest(md5, 0, md5.length));
        return md5;
    }
}


