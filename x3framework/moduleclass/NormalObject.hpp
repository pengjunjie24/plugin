
#pragma once

#include <x3framework/moduleclass/Iobject.h>

namespace x3plugin
{
    //在加载插件时构造的普通对象
    template <class Cls>
    class NormalObject : public Cls
    {
    public:
        static IObject* create(int64_t iid)
        {
            IObject* ret = NULL;
            NormalObject<Cls>* p = new NormalObject<Cls>();

            p->queryObject(iid, &ret);
            p->releaseObject();

            return ret;
        }

        static bool hasInterface(int64_t iid)
        {
            return Cls::_queryObject(NULL, iid, NULL);
        }

    protected:
        NormalObject()
            : _refcount(1)
        {
        }

        virtual ~NormalObject()
        {
        }


        virtual int64_t retainObject() const
        {
            ++_refcount;
            return _refcount.load();
        }

        virtual int64_t releaseObject() const
        {
            --_refcount;
            int64_t ret = _refcount.load();
            if (0 == ret)
            {
                delete this;
            }

            return ret;
        }

        virtual bool queryObject(long iid, IObject** p) const
        {
            return Cls::_queryObject(this, iid, p);
        }

        virtual const char* getClassId() const
        {
            return Cls::_getClassId();
        }

        virtual const char* getClassName() const
        {
            return Cls::_getClassName();
        }

    private:
        mutable std::atomic<int64_t> _refcount;//引用计数
    };

}