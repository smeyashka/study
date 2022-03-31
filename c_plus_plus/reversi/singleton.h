#ifndef SINGLETON_H
#define SINGLETON_H

//! template for singleton. T - creating object. C - constructor parameter 
struct NIL {};
template<class T, typename C=NIL> class Singleton {
public:
  static T* instance(C p) {
    if( 0 == _instance ) {
      _instance = new T(p);
    }
    return _instance;
  }

  static T* instance() {
    if( 0 == _instance ) {
      _instance = new T;
    }
    return _instance;
  }


  static void free() {
    if (_instance) delete _instance;
    _instance = 0;
  }

private:
  Singleton();
  Singleton(const Singleton&);
  Singleton& operator=(const Singleton&);
  virtual ~Singleton();
  
private:
  static T* _instance;
};

#endif
