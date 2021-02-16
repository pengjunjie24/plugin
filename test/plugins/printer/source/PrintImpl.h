#pragma once

#include <x3framework/moduleclass/InterfaceMacro.h>
#include <printer/interface/PrintInterface.h>

class PrintImpl : public PrintInterface
{
    X3BEGIN_CLASS_DECLARE(PrintImpl, c_printClsid)
        X3DEFINE_INTERFACE_ENTRY(PrintImpl)
    X3END_CLASS_DECLARE()
protected:
    PrintImpl();
    virtual ~PrintImpl();

private:
    virtual int add(int a, int b) const;
    virtual int subtract(int a, int b) const;
};