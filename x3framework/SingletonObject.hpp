
#pragma once

#include <x3framework/Iobject.h>
#include <util/Noncopyable.h>

#include <mutex>
#include <atomic>

namespace x3plugin
{
    //在加载插件时构造的单例对象(全局只能有一个)
    template <class Cls>
    class SingletonObject
        : public Noncopyable
        , public Cls
    {
    public:
        static IObject* create(int64_t iid)
        {
            if (!Instance())
            {
                static std::mutex s_mutex;
                std::lock_guard<std::mutex> lock(s_mutex);
                if (!Instance())
                {
                    SingletonObject<Cls>* p = new SingletonObject<Cls>();
                    Instance() = p;
                }
            }
            IObject* ret = NULL;
            Instance()->queryObject(iid, &ret);
            return ret;
        }

        static bool hasInterface(int64_t iid)
        {
            return Cls::_queryObject(NULL, iid, NULL);
        }

    protected:
        virtual int64_t retainObject() const
        {
            ++_refcount;
            return _refcount.load();
        }

        virtual int64_t releaseObject() const
        {
            --_refcount;
            return _refcount.load();
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
        static SingletonObject<Cls>*& Instance()
        {
            static SingletonObject<Cls>* obj = NULL;
            return obj;
        }

        SingletonObject()
            : _refcount(1)
        {
        }

        ~SingletonObject()
        {
            if (Instance())
            {
                delete(Instance());
                Instance() = NULL;
            }
        }
        mutable std::atomic<int32_t> _refcount;
    };
}