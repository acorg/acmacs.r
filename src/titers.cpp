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
    if (obj_->number_of_layers() < 2)
        throw std::invalid_argument("chart has no layers");
    const auto number_of_antigens = obj_->number_of_antigens(), number_of_sera = obj_->number_of_sera();
    const auto& data = obj_->layers();
    Rcpp::List result(data.size());
    for (size_t layer_no = 0; layer_no < data.size(); ++layer_no) {
        Rcpp::StringMatrix layer_data(number_of_antigens, number_of_sera);
        std::fill(layer_data.begin(), layer_data.end(), "*");
        for (size_t ag_no = 0; ag_no < number_of_antigens; ++ag_no) {
            for (auto [sr_no, titer] : data[layer_no][ag_no])
                layer_data(ag_no, sr_no) = titer;
        }
        result[layer_no] = std::move(layer_data);
    }
    return result;

} // Titers::all_layers

// ----------------------------------------------------------------------

Rcpp::StringVector Titers::titers_for_all_layers(size_t ag_no, size_t sr_no) const
{
    if (obj_->number_of_layers() < 2)
        throw std::invalid_argument("chart has no layers");
    if (ag_no < 1 || ag_no > obj_->number_of_antigens())
        throw std::invalid_argument("invalid antigen_no");
    if (sr_no < 1 || sr_no > obj_->number_of_sera())
        throw std::invalid_argument("invalid serum_no");
    const auto titers = obj_->titers_for_layers(ag_no - 1, sr_no - 1, acmacs::chart::Titers::include_dotcare::yes);
    Rcpp::StringVector result(std::begin(titers), std::end(titers));
    return result;

} // Titers::titers_for_all_layers

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
