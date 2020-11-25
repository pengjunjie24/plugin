#pragma once

#include <util/Noncopyable.h>

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

//单例模式类
template<typename T>
class Singleton : Noncopyable
{
public:
    static T* instance()
    {
        pthread_once(&ponce_, &Singleton::init);
        assert(value_ != NULL);
        return value_;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    static void init()
    {
        value_ = new T();
        ::atexit(destroy);
    }

    static void destroy()
    {
        delete value_;
        value_ = NULL;
    }

    static pthread_once_t ponce_;
    static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;