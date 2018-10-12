#include <limits>


#include "chart.hh"

// ----------------------------------------------------------------------

Chart::Chart(std::string aFilename)
    : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_file(aFilename, acmacs::chart::Verify::None, report_time::No)))
{
}

Chart::Chart(Rcpp::RawVector aData)
    : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_data(std::string_view(reinterpret_cast<const char*>(aData.cbegin()), aData.size()), acmacs::chart::Verify::None, report_time::No)))
{
}

Chart::Chart(int number_of_antigens, int number_of_sera)
    : wrapper(std::make_shared<acmacs::chart::ChartNew>(number_of_antigens, number_of_sera))
{
}

// ----------------------------------------------------------------------

void Chart::remove_antigens(const Rcpp::NumericVector& aIndexes)
{
    std::vector<size_t> indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](const auto& ind) -> size_t { return ind - 1; });
    obj_->remove_antigens(acmacs::ReverseSortedIndexes(indexes));
}

void Chart::remove_sera(const Rcpp::NumericVector& aIndexes)
{
    std::vector<size_t> indexes(aIndexes.size());
    std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](const auto& ind) -> size_t { return ind - 1; });
    obj_->remove_sera(acmacs::ReverseSortedIndexes(indexes));
}

// ----------------------------------------------------------------------

void Chart::save(std::string aFilename)
{
    acmacs::chart::export_factory(*obj_, aFilename, "acmacs.r", report_time::No);
}

// ----------------------------------------------------------------------

Rcpp::NumericVector Chart::column_bases_2(size_t aProjectionNo, std::string aMinimumColumnBasis) const
{
    std::shared_ptr<acmacs::chart::ColumnBases> cb;
    acmacs::chart::MinimumColumnBasis mcb(aMinimumColumnBasis);
    auto projections = obj_->projections();
    if (projections && aProjectionNo < projections->size()) {
        if (auto p = (*projections)[aProjectionNo]; p) {
            cb = p->forced_column_bases();
            mcb = p->minimum_column_basis();
        }
    }
    if (!cb)
        cb = obj_->column_bases(mcb);
    if (cb) {
        Rcpp::NumericVector result(cb->size());
        for (size_t sr_no = 0; sr_no < cb->size(); ++sr_no)
            result[sr_no] = cb->column_basis(sr_no);
        return result;
    }
    else
        return Rcpp::NumericVector::create(NA_REAL);

} // Chart::column_bases_2

// ----------------------------------------------------------------------

Projection Chart::relax2(std::string minimum_column_basis, size_t number_of_dimensions)
{
    auto [status, projection] = obj_->relax(minimum_column_basis, number_of_dimensions, true,
                                            acmacs::chart::optimization_options(acmacs::chart::optimization_method::alglib_cg_pca, acmacs::chart::optimization_precision::fine, 1.0));
      // obj_->projections_modify()->sort();
    return projection;
}

Projection Chart::relax3(std::string minimum_column_basis, size_t number_of_dimensions, bool rough)
{
    auto [status, projection] = obj_->relax(minimum_column_basis, number_of_dimensions, true,
                                            acmacs::chart::optimization_options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 1.0));
      // obj_->projections_modify()->sort();
    return projection;
}

void Chart::relax_many(std::string minimum_column_basis, size_t number_of_dimensions, size_t number_of_optimizations, bool rough)
{
    acmacs::chart::optimization_options options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 1.0);
    obj_->relax(number_of_optimizations, minimum_column_basis, number_of_dimensions, true, options, false, acmacs::chart::PointIndexList{});
    obj_->projections_modify()->sort();
}

// ----------------------------------------------------------------------

Rcpp::NumericMatrix Projection::transformation() const
{
    const auto a_tr = obj_->transformation();
    Rcpp::NumericMatrix transformation(2, 2);
    transformation(0, 0) = a_tr.a;
    transformation(0, 1) = a_tr.b;
    transformation(1, 0) = a_tr.c;
    transformation(1, 1) = a_tr.d;
    return transformation;
}

// ----------------------------------------------------------------------

Rcpp::NumericVector Projection::forced_column_bases() const
{
    auto fcb = obj_->forced_column_bases();
    if (fcb) {
        Rcpp::NumericVector result(fcb->size());
        for (size_t sr_no = 0; sr_no < fcb->size(); ++sr_no)
            result[sr_no] = fcb->column_basis(sr_no);
        return result;
    }
    else {
        return Rcpp::NumericVector::create(NA_REAL);
    }
}

// ----------------------------------------------------------------------

void Projection::move_point(size_t aPointNo, const Rcpp::NumericVector& aCoordinates)
{
    if (aPointNo < 1 || aPointNo > obj_->number_of_points())
        throw std::invalid_argument("invalid point number");
    if (aCoordinates.size() != obj_->number_of_dimensions())
        throw std::invalid_argument("invalid vector size (number of point coordinates)");
    obj_->move_point(aPointNo - 1, {aCoordinates.begin(), aCoordinates.end()});
}

// ----------------------------------------------------------------------

void Projection::relax(std::string method, bool rough)
{
    using namespace acmacs::chart;
    optimization_method opt_method{optimization_method::alglib_cg_pca};
    if (method == "lbfgs")
        opt_method = optimization_method::alglib_lbfgs_pca;
    else if (!method.empty() && method != "cg")
        throw std::invalid_argument("invalid optimization method");
    obj_->relax(optimization_options(opt_method, rough ? optimization_precision::rough : optimization_precision::fine));
}

// ----------------------------------------------------------------------

Rcpp::NumericMatrix Projection::layout_convert(std::shared_ptr<acmacs::chart::Layout> layout) const
{
    Rcpp::NumericMatrix result(layout->number_of_points(), layout->number_of_dimensions());
    for (size_t p_no = 0; p_no < layout->number_of_points(); ++p_no) {
        const auto coord = layout->get(p_no);
        if (coord.not_nan()) {
            for (size_t dim = 0; dim < layout->number_of_dimensions(); ++dim)
                result(p_no, dim) = coord[dim];
        }
        else {
            for (size_t dim = 0; dim < layout->number_of_dimensions(); ++dim)
                result(p_no, dim) = Rcpp::NumericMatrix::get_na();
        }
    }
    return result;
}

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

Rcpp::NumericMatrix ProcrustesData::transformation() const
{
    const auto& a_tr = obj_->transformation;
    Rcpp::NumericMatrix transformation(a_tr.number_of_dimensions() + 1, a_tr.number_of_dimensions() + 1);
    for (size_t row = 0; row < a_tr.number_of_dimensions(); ++row) {
        for (size_t col = 0; col < a_tr.number_of_dimensions(); ++col)
            transformation(row, col) = a_tr(row, col);
        transformation(row, a_tr.number_of_dimensions()) = 1.0;
    }
    for (size_t col = 0; col < a_tr.number_of_dimensions(); ++col)
        transformation(a_tr.number_of_dimensions(), col) = a_tr.translation(col);
    transformation(a_tr.number_of_dimensions(), a_tr.number_of_dimensions()) = 1.0;
    return transformation;
}

// ----------------------------------------------------------------------

ProcrustesData procrustes(Projection primary, Projection secondary, bool scaling, std::string match)
{
    auto match_level{acmacs::chart::CommonAntigensSera::match_level_t::automatic};
    if (!match.empty()) {
        switch (match[0]) {
          case 's': match_level = acmacs::chart::CommonAntigensSera::match_level_t::strict; break;
          case 'r': match_level = acmacs::chart::CommonAntigensSera::match_level_t::relaxed; break;
          case 'i': match_level = acmacs::chart::CommonAntigensSera::match_level_t::ignored; break;
          case 'a': match_level = acmacs::chart::CommonAntigensSera::match_level_t::automatic; break;
          default:
              std::cerr << "Unrecognized --match argument, automatic assumed" << '\n';
              break;
        }
    }

    const acmacs::chart::CommonAntigensSera common(primary.obj_->chart(), secondary.obj_->chart(), match_level);
    return ProcrustesData(std::make_shared<acmacs::chart::ProcrustesData>(acmacs::chart::procrustes(*primary.obj_, *secondary.obj_, common.points(), scaling ? acmacs::chart::procrustes_scaling_t::yes : acmacs::chart::procrustes_scaling_t::no)));
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
