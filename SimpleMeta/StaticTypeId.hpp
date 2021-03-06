#pragma once

#include "BoundType.hpp"
#include <vector>
#include <unordered_map>

template <typename T>
struct StaticTypeId
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = new BoundType();
    return sInstance;
  }
};

template <typename T>
struct StaticTypeId<T*> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    return StaticTypeId<T>::GetBoundType();
  }
};

template <typename T>
struct StaticTypeId<std::vector<T>> : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = new BoundType();
    return sInstance;
  }
};

template <typename KeyType, typename ValueType>
struct StaticTypeId<std::unordered_map<KeyType, ValueType>>// : public StaticTypeId<T>
{
  static BoundType*& GetBoundType()
  {
    static BoundType* sInstance = new BoundType();
    return sInstance;
  }
};

template <typename T>
struct RuntimeTypeId
{
  static BoundType* GetVirtualBoundType(char* data)
  {
    T typedObj = (T)data;
    return typedObj->VirtualGetBoundType();
  }

  static char* Allocate()
  {
    return new T();
  }

  static void AllocateInPlace(char* data)
  {
    new(data) T();
  }
};