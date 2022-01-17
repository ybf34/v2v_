
#include "concurrent_object.h"
#include <algorithm>

namespace v2v {

std::mutex ConcurrentObject::mtx_;

ConcurrentObject::~ConcurrentObject() {

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

}  // namespace  v2v
