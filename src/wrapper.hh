#pragma once

template <typename T> class wrapper
{
 public:
    // using owning_t = T;
    wrapper() = default;
    wrapper(std::shared_ptr<T> obj) : obj_(obj) {}
      //~wrapper() { Rprintf("~wrapper %s\n", typeid(this).name()); }
    // std::shared_ptr<T> operator*() { return obj_; }
    std::shared_ptr<T> obj_;

    template <auto (T::*property)() const> auto get() const
        {
            return ((*obj_).*property)();
        }

    // template <typename R, typename ... Args, R (T::*func)(Args ...) const> R method(Args ... args) const
    //     {
    //         return ((*obj_).*func)(args ...);
    //     }

    // template <void (T::*func)() const> void method_void_no_args() const
    //     {
    //         ((*obj_).*func)();
    //     }

    template <typename R, auto (T::*property)() const> R getT() const
        {
            return ((*obj_).*property)();
        }

    // template <typename Arg, auto (T::*property)(const Arg&)> void setT(const Arg& a) const
    //     {
    //         ((*obj_).*property)(a);
    //     }

    template <typename Wrapper, auto (T::*property)() const> Rcpp::List getList() const
        {
            auto elements = ((*obj_).*property)();
            auto result = Rcpp::List::create();
            for (auto elt: *elements)
                result.push_back(Wrapper(elt));
            return result;
        }

    template <typename Wrapper, auto (T::*property)()> Rcpp::List getListViaAt()
        {
            auto& elements = ((*obj_).*property)();
            const auto num_elements = elements.size();
            auto result = Rcpp::List::create();
            for (size_t no = 0; no < num_elements; ++no)
                result.push_back(Wrapper(elements.at(no)));
            return result;
        }

    template <auto (T::*property)() const> Rcpp::StringVector getSV() const
        {
            auto elements = ((*obj_).*property)();
            Rcpp::StringVector result(elements.size());
            for (size_t i = 0; i < elements.size(); ++i)
                result[i] = elements[i];
            return result;
        }

      // ----------------------------------------------------------------------

    template <typename Wrapper, typename V> Rcpp::List getList(V& elements) const
        {
            auto result = Rcpp::List::create();
            for (size_t index = 0; index < elements.size(); ++index)
                result.push_back(Wrapper(elements.ptr_at(index)));
            return result;
        }

    template <typename V> Rcpp::StringVector getStringVector(const V& elements) const
        {
            Rcpp::StringVector result(elements.size());
            for (size_t i = 0; i < elements.size(); ++i)
                result[i] = elements[i];
            return result;
        }

}; // class wrapper<>

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
