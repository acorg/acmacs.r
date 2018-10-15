#include "titers.hh"

RCPP_MODULE(acmacs_titers)
{
    using namespace Rcpp;

    class_<Titers>("acmacs.Titers")
            .method("all", &Titers::all)
            .method("titer", &Titers::titer)
            .method("set_titer", &Titers::set_titer)
            .method("set_dontcare_for_antigen", &Titers::set_dontcare_for_antigen)
            .method("set_dontcare_for_serum", &Titers::set_dontcare_for_serum)
            .method("multiply_by_for_antigen", &Titers::multiply_by_for_antigen)
            .method("multiply_by_for_serum", &Titers::multiply_by_for_serum)
            ;
}


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
