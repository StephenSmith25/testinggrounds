#include <iostream>
#include <tbb/enumerable_thread_specific.h>
#include <tbb/parallel_for.h>

struct A {

  A() {
    std::cout << "Default constructor"
              << "\n";
  }
};

int main()
 {
  tbb::enumerable_thread_specific<A> tls;

  tbb::parallel_for(tbb::blocked_range<int>(0, 100000), [&](auto rng) {
    auto &C = tls.local();
    int worker_index = tbb::task_arena::current_thread_index();
    std::cout << worker_index << std::endl;
  });
}
