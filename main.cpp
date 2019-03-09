#include <cstring>

#if !defined(BUF1_SZ)
#    define BUF1_SZ 1024
#endif /* BUF1_SZ */

#if !defined(BUF2_SZ)
#    define BUF2_SZ 1024*1024
#endif /* BUF2_SZ */

class SomeClass {
    char *x1;
    char *x2;

public:
    /**
     * Let's allocate some memory and initialize it with zeroes.
     */
    SomeClass() {
        /* The next two blocks are not for limiting some kind of scope,
         * but for the purpose of logical distinction between two
         * initializations. */
        {
            x1 = new char[BUF1_SZ];
            std::memset(x1, 0, BUF1_SZ);
        }
        {
            x2 = new char[BUF2_SZ];
            std::memset(x2, 0, BUF2_SZ);
        }
    }

    /**
     * A simple copy constructor. Please note that it may throw
     * an exception in case there is not enough memory.
     */
    SomeClass(const SomeClass& sc) {
        /* Please note that initialization may be performed
         * implicitly like:
         * x1 = new char[BUF1_SZ]();
         */
        {
            x1 = new char[BUF1_SZ];
            std::memset(x1, 0, BUF1_SZ);
        }
        {
            x2 = new char[BUF2_SZ];
            std::memset(x2, 0, BUF2_SZ);
        }
        /* std::memmove is a way to ensure that copying will occur
         * without UB even in case of buffer overlapping */
        std::memmove(x1, sc.x1, BUF1_SZ);
        std::memmove(x2, sc.x2, BUF2_SZ);
    }

    /**
     * A simple assignment operator. This is needed to avoid the
     * situation when the copy of object points to the same memory
     * areas as the original object.
     */
    SomeClass& operator=(const SomeClass& rval) {
        /* Return the object in case of self-assignment */
        if (this == &rval) {
            return *this;
        }

        x1 = new char[BUF1_SZ]();
        x2 = new char[BUF2_SZ]();
        std::memmove(x1, rval.x1, BUF1_SZ);
        std::memmove(x2, rval.x2, BUF2_SZ);

        return *this;
    }

    /**
     * We need to free the memory used by pointers pointing to the
     * heap.
     */
    ~SomeClass() {
        delete[] x1;
        delete[] x2;
    }
};


int main (int argc, char** argv) {
    {
        SomeClass sc;
    }
    return 0;
}

