#include <config.h>
#include <libSAL/thread.h>

void thread_function(void)
{
    printf("%s : %d\n", __FUNCTION__, __LINE__);
}

