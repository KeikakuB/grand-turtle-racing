#pragma once
#include <memory>

namespace PM3D
{
  template <class T>
  class Singleton
  {
  public:
    template<class... Args>
    static void Init(Args&&... args)
    {
      singleton_ = std::unique_ptr<T>(new T{ std::forward<Args>(args)... });
    }
    static T& Get()
    {
      return *singleton_;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
  protected:
    Singleton() = default;
    ~Singleton() = default;
  private:
    static std::unique_ptr<T> singleton_;
  };
  template <class T> std::unique_ptr<T> Singleton<T>::singleton_ = nullptr;
}