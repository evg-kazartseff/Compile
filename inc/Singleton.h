//
// Created by evgenii on 20.05.18.
//

#ifndef COMPILE_SINGLETON_H
#define COMPILE_SINGLETON_H

template <typename T>
class Singleton
{
public:
    virtual ~Singleton () = default;
    static T* getInstance ()
    {
        if (!singletonInstance)
        {
            singletonInstance = new T ();
        }
        return singletonInstance;
    }
protected:
    Singleton () = default;
    Singleton(Singleton const&) = delete;
    Singleton& operator= (Singleton const&) = delete;
    static T* singletonInstance;
};

template <typename T>
T* Singleton<T>::singletonInstance;


#endif //COMPILE_SINGLETON_H
