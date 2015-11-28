#ifndef TESTCLASS_H
#define TESTCLASS_H


class TestClass
{
  public:
    TestClass(int value);

    static int onRxCallback(void * obj, int value);
  private:
    int mValue;
    int RxCallback(int);
};

#endif // TESTCLASS_H
