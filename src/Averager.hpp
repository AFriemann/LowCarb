/**
 * @file   Averager.hpp
 * @author see AUTHORS
 * @brief  Averager header file.
 */

#ifndef AVERAGER_HPP
#define AVERAGER_HPP

/**
 * @class Averager
 */
template <typename T>
class Averager {
 private:
    T sum;
    unsigned count;

 public:
    Averager();
    explicit Averager(int i);
    Averager(int i, int j);

    ~Averager();

    void add(const T & value);

    T get() const;
};

template <typename T>
Averager<T>::Averager() {
  this->count = 0;
}

template <typename T>
Averager<T>::Averager(int i) : sum(i) {
  this->count = 0;
}

template <typename T>
Averager<T>::Averager(int i, int j) : sum(i, j) {
  this->count = 0;
}

template <typename T>
Averager<T>::~Averager() {
}

template <typename T>
void Averager<T>::add(const T & value) {
  if (this->count == 0) {
    this->sum = value;
  } else {
    this->sum += value;
  }
  this->count++;
}


template <typename T>
T Averager<T>::get() const {
    T result(this->sum);
    result /= this->count;
    return result;
}
#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
