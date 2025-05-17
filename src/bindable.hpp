#ifndef BINDABLE_HPP
#define BINDABLE_HPP

class bindable_t {
public:
  virtual void bind(int channel) = 0;
};

using bindable_ref_t = std::reference_wrapper<bindable_t>;

#endif
