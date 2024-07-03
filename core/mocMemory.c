#include <stdlib.h>

#include <core/mocMemory.h>
#define MMEMORY_HEAP_SIZE 1024

static void mHeapInit();
void *mMalloc(size_t len);

static mMemory_t mMemoryObj = {.init = mHeapInit, .malloc = mMalloc};

mMemory_t *mMemoryMgr = &mMemoryObj;

static void *mMemoryPoolPtr = NULL;
static void *mMemoryPoolEndPtr = NULL;



typedef struct {
    void *namespace;
    size_t len;
} mNamespace_t;

typedef struct {
    

}

static void mHeapInit(void)
{
    mMemoryPoolPtr = malloc(MMEMORY_HEAP_SIZE);
    mMemoryPoolEndPtr = mMemoryPoolPtr + MMEMORY_HEAP_SIZE;


}

void *mMalloc(size_t len)
{
    if(mMemoryPoolPtr + len > mMemoryPoolEndPtr)
        return NULL;
    
    void *ptrToReturn = mMemoryPoolPtr;
    memset(ptrToReturn, 0, len);
    mMemoryPoolPtr += len;
    return ptrToReturn;

}
void mFree(void *ptr)
{

    mMemoryPoolPtr -= 
}

