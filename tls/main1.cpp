#include <iostream>
#include <numeric>
#include <tbb/combinable.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/task_scheduler_init.h>
#include <vector>

struct A {

  A() = default;
  A(int val) : val_(val) { std::cout << "Default constructor" << std::endl; }

  A operator+(const A &a) const {

    A ret;
    ret.val_ = this->val_ + a.val_;
    return ret;
  }
  int val_;
};

int main()

{

  int initval = 0;
  tbb::task_scheduler_init init(4);

  std::vector<int> sequence(10000);
  std::iota(sequence.begin(), sequence.end(), 0.0);
  tbb::combinable<A> count([initval]() { return A(initval); });
  tbb::parallel_for_each(sequence.cbegin(), sequence.cend(), [&count](int i) {
    count.local().val_ += i > 200 ? 1 : 0;
  });
  auto val = count.combine(std::plus<A>());

  std::cout << "val is " << val.val_ << std::endl;
}


