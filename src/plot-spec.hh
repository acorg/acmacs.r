#pragma once

#include "acmacs-chart-2/chart-modify.hh"

#include <Rcpp.h>

#include "wrapper.hh"

// ----------------------------------------------------------------------

class PlotSpec : public wrapper<acmacs::chart::PlotSpecModify>
{
 public:
    PlotSpec(acmacs::chart::PlotSpecModifyP plot_spec) : wrapper(plot_spec) {}

    Rcpp::List styles() const;
    Rcpp::DataFrame styles_as_data_frame() const;
    void set_styles(const Rcpp::DataFrame& new_styles);

    Rcpp::IntegerVector drawing_order() const;
    void drawing_order_raise(const Rcpp::IntegerVector& aIndexes);
    void drawing_order_lower(const Rcpp::IntegerVector& aIndexes);
    void drawing_order_raise_sera(const Rcpp::IntegerVector& aIndexes);
    void drawing_order_lower_sera(const Rcpp::IntegerVector& aIndexes);
    void set_style_size(const Rcpp::IntegerVector& aIndexes, double aSize);
    void set_style_fill(const Rcpp::IntegerVector& aIndexes, std::string aFill);
    void set_style_outline(const Rcpp::IntegerVector& aIndexes, std::string aOutline);
    void set_style_outline_width(const Rcpp::IntegerVector& aIndexes, double aOutlineWidth);
};

RCPP_EXPOSED_CLASS_NODECL(PlotSpec);
RCPP_EXPOSED_CLASS_NODECL(acmacs::PointStyle);

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
