package com.parrot.arsdk.arsal;

import com.parrot.arsdk.arsal.ARSAL_PRINT_LEVEL_ENUM;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.Date;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.UnknownHostException;

public class ARSALPrint
{

    private static boolean hasLog;
    private static Method logwtf;
    private static Method loge;
    private static Method logw;
    private static Method logi;
    private static Method logd;
    private static Method logv;

    private static boolean displayDebugPrints = false;
    private static boolean displayVerbosePrints = false;

    static
    {
        try
        {
            Class<?> logClass = Class.forName ("android.util.Log");
            logwtf = logClass.getDeclaredMethod ("wtf", String.class, String.class);
            loge = logClass.getDeclaredMethod ("e", String.class, String.class);
            logw = logClass.getDeclaredMethod ("w", String.class, String.class);
            logi = logClass.getDeclaredMethod ("i", String.class, String.class);
            logd = logClass.getDeclaredMethod ("d", String.class, String.class);
            logv = logClass.getDeclaredMethod ("v", String.class, String.class);
            hasLog = true;
        }
        catch (Exception e)
        {
            logwtf = null;
            loge = null;
            logw = null;
            logi = null;
            logd = null;
            logv = null;
            hasLog = false;
        }
    }

    private ARSALPrint ()
    {
        // Do nothing in this constructor
    }

    private static String getStackTraceString(Throwable tr)
    {
        if (tr == null)
        {
            return "";
        }

        // This is to reduce the amount of log spew that apps do in the non-error
        // condition of the network being unavailable.
        Throwable t = tr;
        while (t != null)
        {
            if (t instanceof UnknownHostException)
            {
                return "";
            }
            t = t.getCause();
        }

        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        tr.printStackTrace(pw);
        return sw.toString();
    }

    public static void v (String tag, String message, Throwable t)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_VERBOSE, tag, message + '\n' + getStackTraceString(t));
    }

    public static void wtf (String tag, String message, Throwable t)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_FATAL, tag, message + '\n' + getStackTraceString(t));
    }

    public static void i (String tag, String message, Throwable t)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_INFO, tag, message + '\n' + getStackTraceString(t));
    }

    public static void d (String tag, String message, Throwable t)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_DEBUG, tag, message + '\n' + getStackTraceString(t));
    }

    public static void w (String tag, String message, Throwable t)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_WARNING, tag, message + '\n' + getStackTraceString(t));
    }

    public static void e (String tag, String message, Throwable t)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_ERROR, tag, message + '\n' + getStackTraceString(t));
    }

    public static void v (String tag, String message)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_VERBOSE, tag, message);
    }

    public static void wtf (String tag, String message)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_FATAL, tag, message);
    }

    public static void i (String tag, String message)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_INFO, tag, message);
    }

    public static void d (String tag, String message)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_DEBUG, tag, message);
    }

    public static void w (String tag, String message)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_WARNING, tag, message);
    }

    public static void e (String tag, String message)
    {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_ERROR, tag, message);
    }

    public static void print (ARSAL_PRINT_LEVEL_ENUM level, String tag, String message)
    {
        if (message.endsWith ("\n"))
        {
            internalPrint (level, tag, message);
        }
        else
        {
            internalPrint (level, tag, message + "\n");
        }
    }

    private static void internalPrint (ARSAL_PRINT_LEVEL_ENUM level, String tag, String message)
    {
        if (hasLog)
        {
            try
            {
                switch (level)
                {
                case ARSAL_PRINT_FATAL:
                    logwtf.invoke (null, tag, message);
                    break;
                case ARSAL_PRINT_ERROR:
                    loge.invoke (null, tag, message);
                    break;
                case ARSAL_PRINT_WARNING:
                    logw.invoke (null, tag, message);
                    break;
                case ARSAL_PRINT_INFO:
                    logi.invoke (null, tag, message);
                    break;
                case ARSAL_PRINT_DEBUG:
                    if (displayDebugPrints)
                    {
                        logd.invoke (null, tag, message);
                    }
                    break;
                case ARSAL_PRINT_VERBOSE:
                    if (displayVerbosePrints)
                    {
                        logv.invoke (null, tag, message);
                    }
                    break;
                default:
                    System.err.println ("Unknown print level tag : " + level);
                    logwtf.invoke (null, tag, message);
                    break;
                }
            }
            catch (Exception e)
            {
                e.printStackTrace ();
            }
        }
        else
        {
            Date now = new Date ();
            SimpleDateFormat sdf = new SimpleDateFormat ("HH:mm:ss:SSS");
            String formattedDate = sdf.format (now);
            switch (level)
            {
            case ARSAL_PRINT_FATAL:
                System.err.print ("[WTF] " + tag + " | " + formattedDate + " | " + message);
                break;
            case ARSAL_PRINT_ERROR:
                System.err.print ("[ERR] " + tag + " | " + formattedDate + " | " + message);
                break;
            case ARSAL_PRINT_WARNING:
                System.out.print ("[WNG] " + tag + " | " + formattedDate + " | " + message);
                break;
            case ARSAL_PRINT_INFO:
                System.out.print ("[INF] " + tag + " | " + formattedDate + " | " + message);
                break;
            case ARSAL_PRINT_DEBUG:
                if (displayDebugPrints)
                {
                    System.out.print ("[DGB] " + tag + " | " + formattedDate + " | " + message);
                }
                break;
            case ARSAL_PRINT_VERBOSE:
                if (displayVerbosePrints)
                {
                    System.out.print ("[VRB] " + tag + " | " + formattedDate + " | " + message);
                }
                break;
            default:
                System.err.println ("Unknown print level tag : " + level);
                System.err.print ("[WTF] " + tag + " | " + formattedDate + " | " + message);
                break;
            }
        }
    }

    public static void enableDebugPrints ()
    {
        displayDebugPrints = true;
    }

    public static void disableDebugPrints ()
    {
        displayDebugPrints = false;
    }

    public static void enableVerbosePrints ()
    {
        displayVerbosePrints = true;
    }

    public static void disableVerbosePrints ()
    {
        displayVerbosePrints = false;
    }
}
