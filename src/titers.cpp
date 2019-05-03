#include "titers.hh"

// ----------------------------------------------------------------------

Rcpp::StringMatrix Titers::all() const
{
    Rcpp::StringMatrix result(obj_->number_of_antigens(), obj_->number_of_sera());
    for (size_t ag_no = 0; ag_no < obj_->number_of_antigens(); ++ag_no)
        for (size_t sr_no = 0; sr_no < obj_->number_of_sera(); ++sr_no)
            result(ag_no, sr_no) = static_cast<std::string>(obj_->titer(ag_no, sr_no));
    return result;
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
