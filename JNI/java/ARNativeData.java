package com.parrot.arsdk.arsal;

/**
 * Interface for all C data that may pass through different libs of AR.SDK<br>
 * <br>
 * AR.SDK Is mainly implemented in C, and thus libs often use C structures to hold data.<br>
 * This is a wrapper interface onto these kind of data, so java can pass them around libs without copy into JVM memory space.
 */
public interface ARNativeData {

    /**
     * Gets the C data pointer (equivalent C type : uint8_t *)<br>
     * This function must return 0 (C-NULL) if the data is not valid (i.e. disposed or uninitialized)
     * @return Native data pointer
     */
    public long getData ();

    /**
     * Gets the C data size (in bytes)<br>
     * This function must return 0 (no data) if the data is not valid
     * @return Size of the native data
     */
    public int getDataSize ();

    /**
     * Gets a byte array which is a copy of the C native data<br>
     * This function allow Java code to access (but not modify) the content of an <code>ARNativeData</code>
     * @note This function creates a new byte [], and thus should not be called in a loop
     * @return A Java copy of the native data (byte equivalent)
     */
    public byte [] getByteData ();

    /**
     * Marks a native data as unused (so C-allocated memory can be freed)<br>
     * Using a disposed data leads to undefined behavior, and must be avoided !<br>
     * Application MUST call <code>dispose</code> on all <code>ARNativeData</code> before they are GC'd
     */
    public void dispose ();

}
