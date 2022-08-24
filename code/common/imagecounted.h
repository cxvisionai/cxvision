#ifndef IMAGECOUNTED_H
#define IMAGECOUNTED_H


#include <iostream>
typedef uint8_t cx_byte;
typedef bool cx_boolean;
class ImageCounted {
private:
  size_t count_;
public:
  ImageCounted();

  virtual ~ImageCounted();

  ImageCounted *retain();

  void release();

  /* return the current count for denugging purposes or similar */
  size_t count() const;
};

template<typename T> class ImageRef {
private:
public:
  T *object_;
  explicit ImageRef(T *o = 0) :
      object_(0) {
    reset(o);
  }
  ImageRef(const ImageRef &other) :
      object_(0) {
    reset(other.object_);
  }

  template<class Y>
  ImageRef(const ImageRef<Y> &other) :
      object_(0) {
    reset(other.object_);
  }

  ~ImageRef() {
    if (object_) {
      object_->release();
    }
  }

  void reset(T *o) {
    if (o) {
      o->retain();
    }
    if (object_ != 0) {
      object_->release();
    }
    object_ = o;
  }
  ImageRef& operator=(const ImageRef &other) {
    reset(other.object_);
    return *this;
  }
  template<class Y>
  ImageRef& operator=(const ImageRef<Y> &other) {
    reset(other.object_);
    return *this;
  }
  ImageRef& operator=(T* o) {
    reset(o);
    return *this;
  }
  template<class Y>
  ImageRef& operator=(Y* o) {
    reset(o);
    return *this;
  }

  T& operator*() {
    return *object_;
  }
  T* operator->() const {
    return object_;
  }
  operator T*() const {
    return object_;
  }

  bool operator==(const T* that) {
    return object_ == that;
  }
  bool operator==(const ImageRef &other) const {
    return object_ == other.object_ || *object_ == *(other.object_);
  }
  template<class Y>
  bool operator==(const ImageRef<Y> &other) const {
    return object_ == other.object_ || *object_ == *(other.object_);
  }

  bool operator!=(const T* that) {
    return !(*this == that);
  }

  bool empty() const {
    return object_ == 0;
  }
};

#endif // IMAGECOUNTED_H
