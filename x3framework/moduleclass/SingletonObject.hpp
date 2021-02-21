
#pragma once

#include <x3framework/moduleclass/Iobject.h>
#include <util/Noncopyable.h>

#include <mutex>
#include <atomic>
#include <assert.h>

namespace x3plugin
{
    static std::mutex s_singleMutex;

    //在加载插件时构造的单例对象(全局只能有一个)
    template <class Cls>
    class SingletonObject
        : public Noncopyable
        , public Cls
    {
    public:
        static IObject* create(int64_t iid)
        {
            //FIXME: 双检查锁，但由于内存读写reorder不安全
            //reoeder:
            //逻辑顺序:
            //    分配内存——构造函数——赋值运算(传递地址)
            //实际CPU乱序执行:
            //    分配内存——赋值运算(传递地址)——构造函数
            if (!Instance())
            {
                std::lock_guard<std::mutex> lock(s_singleMutex);
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

        static void deleteObject()
        {
            std::lock_guard<std::mutex> lock(s_singleMutex);
            if (Instance())
            {
                assert(Instance()->_refcount == 0);
                delete(Instance());
                Instance() = NULL;
            }
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
            : _refcount(0)
        {
        }

        ~SingletonObject() = default;

        mutable std::atomic<int32_t> _refcount;
    };
}