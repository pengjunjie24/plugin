
#pragma once

#include <x3framework/moduleclass/Iobject.h>
#include <x3framework/moduleclass/PluginDef.h>

#include <stdlib.h>

namespace x3plugin
{
    LOCALAPI bool createObject(const char*clsid, int64_t iid, IObject** point);

    //T必须是IObject的子类
    //封装对插件接口类的基本运算符
    template <class T>
    class Object
    {
    public:
        Object()
            : _pointer(NULL)
        {
        }

        // if clsid is "" then the actual class id will be used just in the current plugin.
        Object(const char* clsid)
            : _pointer(NULL)
        {
            createObject(clsid, T::getIID(), address());
        }

        Object(const IObject* src)
            : _pointer(NULL)
        {
            operator=(src);
        }

        Object(const Object<T>& src)
            : _pointer(NULL)
        {
            operator=(src.pointer());
        }

        template <class T2>
        Object(const Object<T2>& src)
            : _pointer(NULL)
        {
            operator=(src.pointer());
        }

        ~Object()
        {
            release();
        }

        Object<T>& operator=(const IObject* src)
        {
            if (_pointer != src)
            {
                release();
                if (T::getIID() == IObject::getIID() && src)
                {
                    src->retainObject();
                    _pointer = static_cast<T*>(const_cast<IObject*>(src));
                }
                else if(src)
                {
                    src->queryObject(T::getIID(), address());
                }
            }
            return *this;
        }

        template <class T2>
        Object<T>& operator=(const Object<T2>& src)
        {
            if (T::getIID() == T2::getIID()
                || T::getIID() == IObject::getIID())
            {
                if (src)
                {
                    src.pointer()->retainObject();
                }
                if (_pointer)
                {
                    _pointer->releaseObject();
                }
                _pointer = src.pointer();

                return *this;
            }

            return operator=(src.p());
        }

        Object<T>& operator=(const Object<T>& src)
        {
            if (src)
            {
                //src指针加引用
                src.pointer()->retainObject();
            }
            if (_pointer)
            {
                //_pointer指针减引用
                _pointer->releaseObject();
            }
            _pointer = src.pointer();

            return *this;
        }

        operator bool() const
        {
            return (_pointer != NULL);
        }

        bool operator!() const
        {
            return !_pointer;
        }

        bool operator==(const Object<T>& src) const
        {
            return _pointer == src._pointer;
        }

        bool operator!=(const Object<T>& src) const
        {
            return _pointer != src._pointer;
        }

        T* operator->() const
        {
            if (!_pointer)
            {
                fprintf(stderr, "Object pointer is null\n");
                abort();
            }
            return _pointer;
        }

        T* pointer() const
        {
            return _pointer;
        }

        bool valid() const
        {
            return (_pointer =! NULL);
        }

        Object<T>& create(const char* clsid = "")
        {
            createObject(clsid, T::getIID(), address());
            return *this;
        }

        void release()
        {
            if (_pointer)
            {
                _pointer->releaseObject();
                _pointer = NULL;
            }
        }

        IObject** address()
        {
            release();
            return reinterpret_cast<IObject**>(&_pointer);
        }

    private:
        T* _pointer;
    };

    typedef Object<IObject> AnyObject;
}