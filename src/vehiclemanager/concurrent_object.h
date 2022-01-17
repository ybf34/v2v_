
#ifndef CONCURRENT_OBJECT_H_
#define CONCURRENT_OBJECT_H_

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace  v2v {

class ConcurrentObject {
  public:
    
    ~ConcurrentObject();

    virtual void Simulate() {};

  protected:
    std::vector<std::thread> threads;
    static std::mutex mtx_;          
};

}  // namespace  v2v

#endif  // CONCURRENT_OBJECT_H_
