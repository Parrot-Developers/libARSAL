package com.parrot.arsdk.libsal;

/**
 * Interface for all C data that may pass through different libs of AR.SDK<br>
 * <br>
 * AR.SDK Is mainly implemented in C, and thus libs often use C structures to hold data.<br>
 * This is a wrapper interface onto these kind of data, so java can pass them around libs without copy into JVM memory space.
 */
public interface SALNativeData {

    /**
     * Get the C data pointer (equivalent C type : uint8_t *)<br>
     * This function must return 0 (C-NULL) if the data is not valid (i.e. disposed or uninitialized)
     * @return Native data pointer
     */
    public long getData ();

    /**
     * Get the C data size (in bytes)<br>
     * This function must return 0 (no data) if the data is not valid
     * @return Size of the native data
     */
    public int getDataSize ();

    /**
     * Get a byte array which is a copy of the C native data<br>
     * This function allow Java code to access (but not modify) the content of a SALNativeData
     * @note This function creates a new byte [], and thus should not be called in a loop
     * @return A Java copy of the Native data (byte equivalent)
     */
    public byte [] getByteData ();

    /**
     * Mark a native data as unused (so C-allocated memory can be freed)<br>
     * Using a disposed data leads to undefined behavior, and must be avoided !
     */
    public void dispose ();

}
