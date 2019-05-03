#include <limits>

#include "acmacs-chart-2/randomizer.hh"
#include "acmacs-chart-2/merge.hh"
#include "chart.hh"
#include "titers.hh"
#include "plot-spec.hh"

// ----------------------------------------------------------------------

Chart::Chart(std::shared_ptr<acmacs::chart::ChartModify> src)
    : wrapper(src)
{
}

Chart::Chart(std::string aFilename)
    : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_file(aFilename, acmacs::chart::Verify::None, report_time::no)))
{
}

Chart::Chart(Rcpp::RawVector aData)
    : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_data(std::string_view(reinterpret_cast<const char*>(aData.cbegin()), aData.size()), acmacs::chart::Verify::None, report_time::no)))
{
}

Chart::Chart(int number_of_antigens, int number_of_sera)
    : wrapper(std::make_shared<acmacs::chart::ChartNew>(number_of_antigens, number_of_sera))
{
}

// ----------------------------------------------------------------------

Chart Chart::clone() const
{
    const auto data = acmacs::chart::export_factory(*obj_, acmacs::chart::export_format::ace, "acmacs.r", report_time::no);
    return std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_data(data, acmacs::chart::Verify::None, report_time::no));

} // Chart::clone

// ----------------------------------------------------------------------

Projection Chart::projection(size_t projection_no)
{
    return obj_->projections_modify()->at(projection_no - 1);
}

// ----------------------------------------------------------------------

Projection Chart::clone_projection(size_t projection_no) const
{
    return static_cast<std::shared_ptr<acmacs::chart::ProjectionModify>>(obj_->projections_modify()->at(projection_no - 1)->clone(*obj_));

} // Chart::clone_projection

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
    acmacs::chart::export_factory(*obj_, aFilename, "acmacs.r", report_time::no);
}

// ----------------------------------------------------------------------

Rcpp::NumericVector Chart::column_bases_2(size_t aProjectionNo, std::string aMinimumColumnBasis) const
{
    auto projections = obj_->projections();
    if (aProjectionNo < 1 || (projections && !projections->empty() && aProjectionNo > projections->size()))
        throw std::invalid_argument("invalid projection number: " + std::to_string(aProjectionNo) + " number-of-projections: " + std::to_string(projections->size()));
    std::shared_ptr<acmacs::chart::ColumnBases> cb;
    acmacs::chart::MinimumColumnBasis mcb(aMinimumColumnBasis);
    if (projections && aProjectionNo <= projections->size()) {
        if (auto p = (*projections)[aProjectionNo - 1]; p) {
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

void Chart::set_column_bases(const Rcpp::NumericVector& data)
{
    if (obj_->number_of_sera() != data.size())
        throw std::invalid_argument("invalid number of entries in column bases vector");
    acmacs::chart::ColumnBasesData col_bases(data.size(), 0.0);
    std::copy(data.begin(), data.end(), col_bases.data().begin());
    obj_->forced_column_bases_modify(col_bases);

} // Chart::set_column_bases

// ----------------------------------------------------------------------

void Chart::set_column_basis(size_t serum_no, double column_basis)
{
    if (serum_no == 0 || obj_->number_of_sera() < serum_no)
        throw std::invalid_argument("invalid serum_no");
    auto cb = obj_->forced_column_bases_modify(0.0);
    cb->set(serum_no - 1, column_basis);

} // Chart::set_column_basis

// ----------------------------------------------------------------------

Projection Chart::new_projection(std::string minimum_column_basis, size_t number_of_dimensions)
{
    auto projection = obj_->projections_modify()->new_from_scratch(number_of_dimensions, minimum_column_basis);
    acmacs::chart::optimization_options options;
    projection->randomize_layout(acmacs::chart::ProjectionModify::randomizer::plain_from_sample_optimization, options.randomization_diameter_multiplier);
    return static_cast<std::shared_ptr<acmacs::chart::ProjectionModify>>(projection);

} // Chart::new_projection

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
    Rcpp::NumericMatrix transformation(a_tr.number_of_dimensions, a_tr.number_of_dimensions);
    for (size_t row = 0; row < a_tr.number_of_dimensions; ++row)
        for (size_t column = 0; column < a_tr.number_of_dimensions; ++column)
            transformation(row, column) = a_tr._x(row, column);
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

void Projection::set_column_bases(const Rcpp::NumericVector& data)
{
    if (obj_->chart().number_of_sera() != data.size())
        throw std::invalid_argument("invalid number of entries in column bases vector");
    auto col_bases = std::make_shared<acmacs::chart::ColumnBasesData>(data.size(), obj_->minimum_column_basis());
    std::copy(data.begin(), data.end(), col_bases->data().begin());
    obj_->set_forced_column_bases(col_bases);

} // Projection::set_column_bases

// ----------------------------------------------------------------------

void Projection::set_column_basis(size_t serum_no, double column_basis)
{
    if (serum_no == 0 || obj_->chart().number_of_sera() < serum_no)
        throw std::invalid_argument("invalid serum_no");
    obj_->set_forced_column_basis(serum_no - 1, column_basis);

} // Projection::set_column_basis

// ----------------------------------------------------------------------

void Projection::move_point(size_t aPointNo, const Rcpp::NumericVector& aCoordinates)
{
    if (aPointNo < 1 || aPointNo > obj_->number_of_points())
        throw std::invalid_argument("invalid point number");
    if (static_cast<size_t>(aCoordinates.size()) != obj_->number_of_dimensions())
        throw std::invalid_argument("invalid vector size (number of point coordinates)");
    obj_->move_point(aPointNo - 1, acmacs::PointCoordinates(aCoordinates.begin(), aCoordinates.end()));
}

// ----------------------------------------------------------------------

acmacs::chart::optimization_method Projection::optimization_method(std::string method) const
{
    if (method == "lbfgs")
        return acmacs::chart::optimization_method::alglib_lbfgs_pca;
    else if (method.empty() || method == "cg")
        return acmacs::chart::optimization_method::alglib_cg_pca;
    else
        throw std::invalid_argument("invalid optimization method");
}

// ----------------------------------------------------------------------

void Projection::relax(std::string method, bool rough)
{
    obj_->relax(acmacs::chart::optimization_options(optimization_method(method), rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine));
    intermediate_layouts_.reset();
}

// ----------------------------------------------------------------------

bool Projection::relax_one_iteration(std::string method, bool rough)
{
    if (!intermediate_layouts_) {
        intermediate_layouts_.reset(new acmacs::chart::IntermediateLayouts{});
        obj_->relax(acmacs::chart::optimization_options(optimization_method(method), rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine), *intermediate_layouts_);
        next_layout_ = 0;
    }
    if (next_layout_ < intermediate_layouts_->size()) {
        obj_->set_layout(intermediate_layouts_->at(next_layout_).layout);
        obj_->set_stress(intermediate_layouts_->at(next_layout_).stress);
        ++next_layout_;
    }
    return intermediate_layouts_ && next_layout_ < intermediate_layouts_->size();
}

// ----------------------------------------------------------------------

void Projection::randomize_layout(std::string randomization_method, double diameter_multiplier)
{
    using namespace acmacs::chart;
    ProjectionModify::randomizer rnd{ProjectionModify::randomizer::plain_from_sample_optimization};
    if (randomization_method == "sample-optimization")
        rnd = ProjectionModify::randomizer::plain_from_sample_optimization;
    else if (randomization_method == "table-max-distance")
        rnd = ProjectionModify::randomizer::plain_with_table_max_distance;
    else if (randomization_method == "current-layout-area")
        rnd = ProjectionModify::randomizer::plain_with_current_layout_area;
    else
        throw std::invalid_argument("invalid randomization method, supported: \"sample-optimization\", \"table-max-distance\", \"current-layout-area\"");
    obj_->randomize_layout(rnd, diameter_multiplier);
    intermediate_layouts_.reset();
}

// ----------------------------------------------------------------------

Rcpp::NumericMatrix Projection::layout_convert(std::shared_ptr<acmacs::Layout> layout)
{
    Rcpp::NumericMatrix result(layout->number_of_points(), layout->number_of_dimensions());
    for (size_t p_no = 0; p_no < layout->number_of_points(); ++p_no) {
        const auto coord = layout->get(p_no);
        if (coord.exists()) {
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

acmacs::Layout Projection::layout_convert(const Rcpp::NumericMatrix& source)
{
    acmacs::Layout result(source.nrow(), source.ncol());
    for (size_t point_no = 0; point_no < result.number_of_points(); ++point_no) {
        for (size_t dim_no = 0; dim_no < result.number_of_dimensions(); ++dim_no)
            result(point_no, dim_no) = source(point_no, dim_no);
    }
    return result;

} // Projection::layout_convert

// ----------------------------------------------------------------------

acmacs::Transformation Projection::transformation_convert(const Rcpp::NumericMatrix& source) const
{
    if (source.nrow() != source.ncol() || static_cast<size_t>(source.nrow()) != number_of_dimensions())
        throw std::invalid_argument{"invalid transfromation matrix size"};
    acmacs::Transformation result(number_of_dimensions());
    for (size_t row = 0; row < number_of_dimensions(); ++row)
        for (size_t column = 0; column < number_of_dimensions(); ++column)
            result(row, column) = source(row, column);
    return result;

} // Projection::transformation_convert

// ----------------------------------------------------------------------

void Projection::reorient(const Projection& master, std::string match, std::string subset)
{
    const auto procrustes_data = procrustes(master, *this, false, match, subset);
    const auto& transformation_raw = procrustes_data.transformation_raw();
    obj_->transformation(transformation_raw);

} // Projection::reorient

// ----------------------------------------------------------------------

Rcpp::NumericMatrix ProcrustesData::transformation() const
{
    const auto& a_tr = obj_->transformation;
    Rcpp::NumericMatrix transformation(a_tr.number_of_dimensions + 1, a_tr.number_of_dimensions + 1);
    for (size_t row = 0; row < a_tr.number_of_dimensions; ++row) {
        for (size_t col = 0; col < a_tr.number_of_dimensions; ++col)
            transformation(row, col) = a_tr(row, col);
        transformation(row, a_tr.number_of_dimensions) = 1.0;
    }
    for (size_t col = 0; col < a_tr.number_of_dimensions; ++col)
        transformation(a_tr.number_of_dimensions, col) = a_tr.translation(col);
    transformation(a_tr.number_of_dimensions, a_tr.number_of_dimensions) = 1.0;
    return transformation;
}

// ----------------------------------------------------------------------

acmacs::chart::CommonAntigensSera::match_level_t convert_match_level(std::string source)
{
    auto match_level{acmacs::chart::CommonAntigensSera::match_level_t::automatic};
    if (!source.empty()) {
        switch (std::tolower(source[0])) {
          case 's': match_level = acmacs::chart::CommonAntigensSera::match_level_t::strict; break;
          case 'r': match_level = acmacs::chart::CommonAntigensSera::match_level_t::relaxed; break;
          case 'i': match_level = acmacs::chart::CommonAntigensSera::match_level_t::ignored; break;
          case 'a': match_level = acmacs::chart::CommonAntigensSera::match_level_t::automatic; break;
          default:
              std::cerr << "Unrecognized match argument, automatic assumed" << '\n';
              break;
        }
    }
    return match_level;
}

// ----------------------------------------------------------------------

Rcpp::List match_antigens_sera(Chart chart1, Chart chart2, std::string match)
{
    using namespace Rcpp;
    // MatchData result(chart1.obj_->number_of_antigens(), chart1.obj_->number_of_sera());
    acmacs::chart::CommonAntigensSera common(*chart1.obj_, *chart2.obj_, convert_match_level(match));
    IntegerVector antigens(chart1.obj_->number_of_antigens(), NA_INTEGER);
    for (const auto& entry : common.antigens())
        antigens[entry.primary] = entry.secondary + 1;
    IntegerVector sera(chart1.obj_->number_of_sera(), NA_INTEGER);
    for (const auto& entry : common.sera())
        sera[entry.primary] = entry.secondary + 1;
    return List::create(_["antigens"] = antigens, _["sera"] = sera);

} // match_antigens_sera

// ----------------------------------------------------------------------

ProcrustesData procrustes(Projection primary, Projection secondary, bool scaling, std::string match, std::string subset)
{
    acmacs::chart::CommonAntigensSera common(primary.obj_->chart(), secondary.obj_->chart(), convert_match_level(match));
    if (subset == "antigens")
        common.antigens_only();
    else if (subset == "sera")
        common.sera_only();
    return ProcrustesData(std::make_shared<acmacs::chart::ProcrustesData>(acmacs::chart::procrustes(*primary.obj_, *secondary.obj_, common.points(), scaling ? acmacs::chart::procrustes_scaling_t::yes : acmacs::chart::procrustes_scaling_t::no)));
}

// ----------------------------------------------------------------------

Rcpp::NumericMatrix ProcrustesData::apply(const Rcpp::NumericMatrix& source_layout) const
{
    const auto layout = Projection::layout_convert(source_layout);
    auto resulting_layout = obj_->apply(layout);
    return Projection::layout_convert(resulting_layout);

} // ProcrustesData::apply

// ----------------------------------------------------------------------

static acmacs::chart::MergeSettings merge_settinsg(std::string match_level, std::string projection_merge);

acmacs::chart::MergeSettings merge_settinsg(std::string match_level, std::string projection_merge)
{
    acmacs::chart::MergeSettings settings(convert_match_level(match_level));
    if (!projection_merge.empty()) {
        switch (std::tolower(projection_merge[0])) {
          case 'i':
              settings.projection_merge = acmacs::chart::projection_merge_t::incremental;
              break;
          case 'o':
              settings.projection_merge = acmacs::chart::projection_merge_t::overlay;
              break;
        }
    }
    return settings;
}

// ----------------------------------------------------------------------

Chart merge(Chart chart1, Chart chart2, std::string match_level, std::string projection_merge)
{
    auto [result, diagnostics] = acmacs::chart::merge(*chart1.obj_, *chart2.obj_, merge_settinsg(match_level, projection_merge));
    return result;
}

// ----------------------------------------------------------------------

Chart merge_overlay(Chart chart1, Chart chart2)
{
    auto result = merge(chart1, chart2, "a", "o");
      // std::cout << "overlay 1 " << result.obj_->projection(0)->stress() << '\n';
    result.obj_->projection_modify(0)->relax(acmacs::chart::optimization_options{});
      // std::cout << "overlay 2 " << result.obj_->projection(0)->stress() << '\n';
    return result;
}

// ----------------------------------------------------------------------

Chart merge_frozen(Chart chart1, Chart chart2)
{
    auto result = merge(chart1, chart2, "a", "o");
      // std::cout << "frozen " << result.obj_->projection(0)->stress() << '\n';
    return result;
}

// ----------------------------------------------------------------------

Chart merge_incremental(Chart chart1, Chart chart2, size_t number_of_optimizations, size_t num_threads)
{
    auto result = merge(chart1, chart2, "a", "i");
    acmacs::chart::optimization_options options(acmacs::chart::optimization_method::alglib_cg_pca, acmacs::chart::optimization_precision::fine, 2.0);
    options.num_threads = num_threads;
    result.obj_->relax_incremetal(0, number_of_optimizations, options);
    return result;
}

// ----------------------------------------------------------------------

std::string merge_report(Chart chart1, Chart chart2, std::string match_level)
{
    auto [result, diagnostics] = acmacs::chart::merge(*chart1.obj_, *chart2.obj_, merge_settinsg(match_level, "none"));
    return diagnostics.titer_merge_report(*result);
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
