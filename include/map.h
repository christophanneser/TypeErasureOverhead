#pragma once

#include <vector>
#include "IteratorTypeErasure/any_iterator/any_iterator.hpp"

template<typename KeyType, typename DataType>
class Map {
 protected:
  typedef IteratorTypeErasure::any_iterator<std::pair<KeyType, DataType>, std::random_access_iterator_tag> AnyIterator;

 public:
  virtual void Add(const KeyType &key, const DataType &data) = 0;
  virtual bool Contains(const KeyType &key) const = 0;
  virtual DataType Get(const KeyType &key) = 0;
  virtual AnyIterator Find(const KeyType &key) = 0;
};

template<typename KeyType, typename DataType>
class VectorMap : public Map<KeyType, DataType> {
  void Add(const KeyType &key, const DataType &data) final {
    samples_.emplace_back(std::pair<KeyType, DataType>{key, data});
  }
  bool Contains(const KeyType &key) const final {
    for (const auto[k, v] : samples_) if (k == key) return true;
    return false;
  }
  DataType Get(const KeyType &key) final {
    auto it = std::lower_bound(samples_.begin(),
                               samples_.end(),
                               key,
                               [](const std::pair<KeyType, DataType> &a, const KeyType &b) {
                                 return std::get<0>(a) < b;
                               });
    return std::get<1>(*it);
  }
  virtual typename Map<KeyType, DataType>::AnyIterator Find(const KeyType &key) {
    auto it = std::lower_bound(samples_.begin(),
                               samples_.end(),
                               key,
                               [](const std::pair<KeyType, DataType> &a, const KeyType &b) {
                                 return std::get<0>(a) < b;
                               });
    typename Map<KeyType, DataType>::AnyIterator any_it(it);
    return any_it;
  };

 private:
  std::vector<std::pair<KeyType, DataType>> samples_;
};