#pragma once

#include "acmacs-chart-2/chart-modify.hh"

#include <Rcpp.h>

#include "wrapper.hh"

// ----------------------------------------------------------------------

class Titers : public wrapper<acmacs::chart::TitersModify>
{
 public:
    Titers(acmacs::chart::TitersModifyP titers) : wrapper(titers) {}
    std::string titer(size_t ag_no, size_t sr_no) const { return obj_->titer(ag_no - 1, sr_no - 1); }
    void set_titer(size_t ag_no, size_t sr_no, std::string titer) { obj_->titer(ag_no - 1, sr_no - 1, titer); }
    void set_dontcare_for_antigen(size_t ag_no) { obj_->dontcare_for_antigen(ag_no - 1); }
    void set_dontcare_for_serum(size_t sr_no) { obj_->dontcare_for_serum(sr_no - 1); }
    void multiply_by_for_antigen(size_t ag_no, double multiply_by) { obj_->multiply_by_for_antigen(ag_no - 1, multiply_by); }
    void multiply_by_for_serum(size_t sr_no, double multiply_by) { obj_->multiply_by_for_serum(sr_no - 1, multiply_by); }
    size_t number_of_layers() { return obj_->number_of_layers(); }
    Rcpp::StringMatrix all() const;
};
RCPP_EXPOSED_CLASS_NODECL(Titers);

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
