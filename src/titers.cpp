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

Rcpp::List Titers::all_layers() const
{
    const auto& data = obj_->rjson_layers();
    Rcpp::List result(data);
    return result;
    
} // Titers::all_layers

// ----------------------------------------------------------------------

Rcpp::StringVector Titers::titers_for_all_layers(size_t ag_no, size_t sr_no) const
{
    const auto titers = obj_->titers_for_layers(ag_no - 1, sr_no - 1, acmacs::chart::Titers::include_dotcare::yes);
    Rcpp::StringVector result(std::begin(titers), std::end(titers));
    return result;

} // Titers::titers_for_all_layers

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
