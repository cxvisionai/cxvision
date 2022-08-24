#ifndef IMAGEARRAY_H
#define IMAGEARRAY_H
#include <vector>
#include <common/imagecounted.h>

template<typename T> class ImageArray : public ImageCounted {
protected:
public:
  std::vector<T> values_;
  ImageArray() {}
  ImageArray(int n) :
      ImageCounted(), values_(n, T()) {
  }
  ImageArray(T const* ts, int n) :
      ImageCounted(), values_(ts, ts+n) {
  }
  ImageArray(T const* ts, T const* te) :
      ImageCounted(), values_(ts, te) {
  }
  ImageArray(T v, int n) :
      ImageCounted(), values_(n, v) {
  }
  ImageArray(std::vector<T> &v) :
      ImageCounted(), values_(v) {
  }
  ImageArray(ImageArray<T> &other) :
      ImageCounted(), values_(other.values_) {
  }
  ImageArray(ImageArray<T> *other) :
      ImageCounted(), values_(other->values_) {
  }
  virtual ~ImageArray() {
  }
  ImageArray<T>& operator=(const ImageArray<T> &other) {
    values_ = other.values_;
    return *this;
  }
  ImageArray<T>& operator=(const std::vector<T> &array) {
    values_ = array;
    return *this;
  }
  T const& operator[](int i) const {
    return values_[i];
  }
  T& operator[](int i) {
    return values_[i];
  }
  int size() const {
    return int(values_.size());
  }
  bool empty() const {
    return values_.size() == 0;
  }
  std::vector<T> const& values() const {
    return values_;
  }
  std::vector<T>& values() {
    return values_;
  }
  void push_back(T value) {
    values_.push_back(value);
  }
};

template<typename T> class ImageArrayRef : public ImageCounted {
private:
public:
  ImageArray<T> *array_;
  ImageArrayRef() :
      array_(0) {
  }
  explicit ImageArrayRef(int n) :
      array_(0) {
    reset(new ImageArray<T> (n));
  }
  ImageArrayRef(T *ts, int n) :
      array_(0) {
    reset(new ImageArray<T> (ts, n));
  }
  ImageArrayRef(ImageArray<T> *a) :
      array_(0) {
    reset(a);
  }
  ImageArrayRef(const ImageArrayRef &other) :
      ImageCounted(), array_(0) {
    reset(other.array_);
  }

  template<class Y>
  ImageArrayRef(const ImageArrayRef<Y> &other) :
      array_(0) {
    reset(static_cast<const ImageArray<T> *>(other.array_));
  }

  ~ImageArrayRef() {
    if (array_) {
      array_->release();
    }
    array_ = 0;
  }

  T const& operator[](int i) const {
    return (*array_)[i];
  }

  T& operator[](int i) {
    return (*array_)[i];
  }

  void reset(ImageArray<T> *a) {
    if (a) {
      a->retain();
    }
    if (array_) {
      array_->release();
    }
    array_ = a;
  }
  void reset(const ImageArrayRef<T> &other) {
    reset(other.array_);
  }
  ImageArrayRef<T>& operator=(const ImageArrayRef<T> &other) {
    reset(other);
    return *this;
  }
  ImageArrayRef<T>& operator=(ImageArray<T> *a) {
    reset(a);
    return *this;
  }

  ImageArray<T>& operator*() const {
    return *array_;
  }

  ImageArray<T>* operator->() const {
    return array_;
  }

  operator bool () const {
    return array_ != 0;
  }
  bool operator ! () const {
    return array_ == 0;
  }
};


#endif // ImageARRAY_H
