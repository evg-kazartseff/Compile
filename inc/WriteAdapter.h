//
// Created by evgenii on 19.05.18.
//

#ifndef COMPILE_WRITEADAPTER_H
#define COMPILE_WRITEADAPTER_H

#include <fstream>

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

class WriteAdapter {
private:
    std::ofstream file;
public:
    WriteAdapter() = default;
    ~WriteAdapter ();
    void SetFile(const std::string& file);
    void Print(const std::string& str);
};

#endif //COMPILE_WRITEADAPTER_H
