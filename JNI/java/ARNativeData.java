package com.parrot.arsdk.arsal;

/**
 * Root class for all C data that may pass through different libs of AR.SDK<br>
 * <br>
 * AR.SDK Is mainly implemented in C, and thus libs often use C structures to hold data.<br>
 * This is a wrapper class onto these kind of data, so java can pass them around libs without copy into JVM memory space.
 */
public class ARNativeData {

    /* ************** */
    /* DEFAULT VALUES */
    /* ************** */

    /**
     * Default size for native data
     */
    public static final int DEFAULT_SIZE = 128;

    /* *********************** */
    /* INTERNAL REPRESENTATION */
    /* *********************** */

    /**
     * Storage of the C Pointer
     */
    protected long pointer;
    /**
     * Storage of the allocated size of the C Pointer
     */
    protected int capacity;
    /**
     * How many bytes are used in the buffer
     */
    protected int used;

    /**
     * Check validity before all native calls
     */
    protected boolean valid;

    /* ************ */
    /* CONSTRUCTORS */
    /* ************ */

    /**
     * Generic constructor<br>
     * This constructor allocate a <code>ARNativeData</code> with the given capacity
     * @param size Initial capacity of the <code>ARNativeData</code>
     */
    public ARNativeData (int size) {
        this.pointer = allocateData (size);
        this.capacity = 0;
        this.valid = false;
        if (this.pointer != 0) {
            this.capacity = size;
            this.valid = true;
        }
        this.used = 0;
    }

    /**
     * Default size constructor
     * This constructor allocate a <code>ARNativeData</code> with the default capacity
     */
    public ARNativeData () {
        this (DEFAULT_SIZE);
    }

    /**
     * Copy constructor<br>
     * This constructor creates a new <code>ARNativeData</code> with a capacity<br>
     * equal to the original objects data size
     * @param data The original <code>ARNativeData</code> to copy
     */
    public ARNativeData (ARNativeData data) {
        this (data.getDataSize ());
        if (! copyData (pointer, capacity, data.getData (), data.getDataSize ())) {
            dispose ();
        }
    }

    /**
     * Copy constructor, with specified allocation size<br>
     * This constructor creates a new <code>ARNativeData</code> with a capacity<br>
     * equal to the greatest value between the specified <code>capacity</code> and the original<br>
     * object data size
     * @param data The original <code>ARNativeData</code> to copy
     * @param capacity The minimum capacity in bytes for this <code>ARNativeData</code>
     */
    public ARNativeData (ARNativeData data, int capacity) {
        int totalSize = data.getDataSize ();
        if (capacity > totalSize) {
            totalSize = capacity;
        }
        this.pointer = allocateData (totalSize);
        this.capacity = 0;
        this.valid = false;
        if (this.pointer != 0) {
            this.capacity = totalSize;
            this.valid = true;
        }
        this.used = 0;
        if (! copyData (this.pointer, this.capacity, data.getData (), data.getDataSize ())) {
            dispose ();
        }
    }

    /* ********** */
    /* DESTRUCTOR */
    /* ********** */

    protected void finalize () throws Throwable {
        try {
            if (valid) {
                dispose ();
                System.err.println (this + ": Finalize error -> dispose () was not called !");
            }
        }
        finally {
            super.finalize ();
        }
    }

    /**
     * Checks the object validity
     * @return <code>true</code> if the object is valid (buffer properly alloc and usable)
     * @return <code>false</code> if the object is invalid (alloc error, disposed object)
     */
    public boolean isValid () {
        return valid;
    }

    /**
     * Sets the used size of the <code>ARNativeData</code>
     * @param size New used size of the command
     * @return <code>true</code> if the size was set
     * @return <code>false</code> if the size was invalid (bigger than capacity) or if the <code>ARNativeData</code> is invalid
     */
    public boolean setUsedSize (int size) {
        if ((! valid) || (size > capacity))
            return false;
        this.used = size;
        return true;
    }

    /**
     * Gets the capacity of the <code>ARNativeData</code><br>
     * This function returns 0 (no capacity) if the data is not valid
     * @return Capacity of the native data buffer
     */
    public int getCapacity () {
        if (valid)
            return capacity;
        return 0;
    }

    /**
     * Gets the C data pointer (equivalent C type : uint8_t *)<br>
     * This function returns 0 (C-NULL) if the data is not valid (i.e. disposed or uninitialized)
     * @return Native data pointer
     */
    public long getData () {
        if (valid)
            return pointer;
        return 0;
    }

    /**
     * Get the C data size (in bytes)<br>
     * This function returns 0 (no data) if the data is not valid
     * @return Size of the native data
     */
    public int getDataSize () {
        if (valid)
            return used;
        return 0;
    }

    /**
     * Gets a byte array which is a copy of the C native data<br>
     * This function allow Java code to access (but not modify) the content of the <code>ARNativeData</code>
     * @note This function creates a new byte [], and thus should not be called in a loop
     * @return A Java copy of the native data (byte equivalent)
     */
    public byte [] getByteData () {
        if (valid)
            return generateByteArray (pointer, capacity, used);
        return null;
    }

    /**
     * Marks a native data as unused (so C-allocated memory can be freed)<br>
     * A disposed data is marked as invalid
     */
    public void dispose () {
        if (valid)
            freeData (pointer);
        this.valid = false;
        this.pointer = 0;
        this.capacity = 0;
        this.used = 0;
    }

    /* **************** */
    /* NATIVE FUNCTIONS */
    /* **************** */

    /**
     * Memory allocation in native memory space<br>
     * Allocates a memory buffer of size <code>capacity</code> and return its C-Pointer
     * @param capacity Size, in bytes, of the buffer to allocate
     * @return C-Pointer on the buffer, or 0 (C-NULL) if the alloc failed
     */
    private native long allocateData (int capacity);

    /**
     * Memory release in native memory space<br>
     * Frees a memory buffer from its C-Pointer<br>
     * This call is needed because JVM do not know about native memory allocs
     * @param data C-Pointer on the buffer to free
     */
    private native void freeData (long data);

    /**
     * Copy data to a byte array<br>
     * This function is needed to implement the <code>getByteData</code> function
     * @param data C-Pointer to the internal buffer
     * @param capacity Capacity of the internal buffer (avoid overflows)
     * @param used Used bytes in the internal buffer (size to copy)
     * @return A byte array, which content is the byte-equal copy of the native buffer
     */
    private native byte [] generateByteArray (long data, int capacity, int used);

    /**
     * Copy data from another <code>ARNativeData</code><br>
     * This function is used in the copy constructors.
     * @param dst C-Pointer on this object's internal buffer
     * @param dstCapacity Capacity of this object's internal buffer
     * @param src C-Pointer on the initial object's internal buffer
     * @param srcLen Used bytes in the initial object's internal buffer
     * @return <code>true</code> if the copy was done without error
     * @return <code>false</code> if an error occured. In this case, the content of the internal buffer is undefined, so <code>used</code> should be set to zero
     */
    private native boolean copyData (long dst, int dstCapacity, long src, int srcLen);

}
