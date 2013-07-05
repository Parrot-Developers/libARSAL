package com.parrot.arsdk.arsal;

import com.parrot.arsdk.arsal.ARSAL_PRINT_LEVEL_ENUM;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.Date;

public class ARSALPrint {

    private static boolean hasLog;
    private static Method loge;
    private static Method logw;
    private static Method logd;

    private static boolean displayDebugPrints = false;

    static {
        try {
            Class<?> logClass = Class.forName ("android.util.Log");
            loge = logClass.getDeclaredMethod ("e", String.class, String.class);
            logw = logClass.getDeclaredMethod ("w", String.class, String.class);
            logd = logClass.getDeclaredMethod ("d", String.class, String.class);
            hasLog = true;
        } catch (Exception e) {
            loge = null;
            logw = null;
            logd = null;
            hasLog = false;
        }
    }

    private ARSALPrint () {
        // Do nothing in this constructor
    }

    public static void d (String tag, String message) {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_DEBUG, tag, message);
    }

    public static void w (String tag, String message) {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_WARNING, tag, message);
    }

    public static void e (String tag, String message) {
        print (ARSAL_PRINT_LEVEL_ENUM.ARSAL_PRINT_ERROR, tag, message);
    }

    public static void print (ARSAL_PRINT_LEVEL_ENUM level, String tag, String message) {
        if (message.endsWith ("\n")) {
            internalPrint (level, tag, message);
        } else {
            internalPrint (level, tag, message + "\n");
        }
    }

    private static void internalPrint (ARSAL_PRINT_LEVEL_ENUM level, String tag, String message) {
        if (hasLog) {
            try {
                switch (level) {
                case ARSAL_PRINT_ERROR:
                    loge.invoke (null, tag, message);
                    break;
                case ARSAL_PRINT_WARNING:
                    logw.invoke (null, tag, message);
                    break;
                case ARSAL_PRINT_DEBUG:
                    if (displayDebugPrints) {
                        logd.invoke (null, tag, message);
                    }
                    break;
                default:
                    System.err.println ("Unknown print level tag : " + level);
                    loge.invoke (null, tag, message);
                    break;
                }
            } catch (Exception e) {
                e.printStackTrace ();
            }
        } else {
            Date now = new Date ();
            SimpleDateFormat sdf = new SimpleDateFormat ("HH:mm:ss:SSS");
            String formattedDate = sdf.format (now);
            switch (level) {
            case ARSAL_PRINT_ERROR:
                System.err.print ("[ERR] " + tag + " | " + formattedDate + " | " + message);
                break;
            case ARSAL_PRINT_WARNING:
                System.out.print ("[WNG] " + tag + " | " + formattedDate + " | " + message);
                break;
            case ARSAL_PRINT_DEBUG:
                if (displayDebugPrints) {
                    System.out.print ("[DGB] " + tag + " | " + formattedDate + " | " + message);
                }
                break;
            default:
                System.err.println ("Unknown print level tag : " + level);
                System.err.print ("[ERR] " + tag + " | " + formattedDate + " | " + message);
                break;
            }
        }
    }

    public static void enableDebugPrints () {
        displayDebugPrints = true;
    }

    public static void disableDebugPrints () {
        displayDebugPrints = false;
    }
}