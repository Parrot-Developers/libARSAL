package com.parrot.arsdk.libsal;

/**
 * Interface for all C data that may pass through different libs of AR.SDK<br>
 * <br>
 * AR.SDK Is mainly implemented in C, and thus libs often use C structures to hold data.<br>
 * This is a wrapper interface onto these kind of data, so the Java code can keep track, and read them.
 */
public interface SALNativeData {

    /**
     * Get a java binary copy of the data.
     * @return Copy of the native data. This copy can be modified without any change on the actual data
     */
    public byte [] getData ();

    /**
     * Mark a native data as unused (so C-allocated memory can be freed)<br>
     * Using a disposed data leads to undefined behaviour, and must be avoided !
     */
    public void dispose ();

}