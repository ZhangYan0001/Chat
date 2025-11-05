#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <mutex>

template <typename T>
class Singleton {
 protected:
  Singleton() = default;
  Singleton(const Singleton<T>&) = delete;
  Singleton& operator=(const Singleton<T>&) = delete;
  static T* _instance;

 public:
  static T* GetInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() { _instance = new T(); });
    return _instance;
  }
};

template <typename T>
T* Singleton<T>::_instance = nullptr;

#endif  // SINGLETON_H
