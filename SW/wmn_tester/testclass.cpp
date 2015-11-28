#include "testclass.h"

TestClass::TestClass(int value)
{
    mValue = value;
}

int TestClass::RxCallback(int value)
{
    return mValue + value;
}

int TestClass::onRxCallback(void *obj, int value)
{
    return ((TestClass *)obj)->RxCallback(value);
}
