#include "plot-spec.hh"

// ----------------------------------------------------------------------

Rcpp::List PlotSpec::styles() const
{
    const auto styles = obj_->all_styles();
    return {styles.begin(), styles.end()};
}

// ----------------------------------------------------------------------

Rcpp::IntegerVector PlotSpec::drawing_order() const
{
    const auto drawing_order = obj_->drawing_order();
    Rcpp::IntegerVector result(drawing_order.size());
    std::transform(drawing_order.begin(), drawing_order.end(), result.begin(), [](auto index) { return index + 1; });
    return result;
    // return {drawing_order.begin(), drawing_order.end()};
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_raise(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->raise(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_lower(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->lower(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_raise_sera(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->raise_serum(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::drawing_order_lower_sera(const Rcpp::IntegerVector& aIndexes)
{
    acmacs::chart::Indexes indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
    obj_->lower_serum(indexes);
}

// ----------------------------------------------------------------------

void PlotSpec::set_style_size(const Rcpp::IntegerVector& aIndexes, double aSize)
{
    for (auto index : aIndexes)
        obj_->size(index - 1, Pixels{aSize});
}

// ----------------------------------------------------------------------

void PlotSpec::set_style_fill(const Rcpp::IntegerVector& aIndexes, std::string aFill)
{
    for (auto index : aIndexes)
        obj_->fill(index - 1, Color(aFill));
}

// ----------------------------------------------------------------------

void PlotSpec::set_style_outline(const Rcpp::IntegerVector& aIndexes, std::string aOutline)
{
    for (auto index : aIndexes)
        obj_->outline(index - 1, Color(aOutline));
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
