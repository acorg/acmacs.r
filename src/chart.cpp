#include <limits>

#include "acmacs-base/rjson-v2.hh"
#include "acmacs-chart-2/randomizer.hh"
#include "acmacs-chart-2/merge.hh"
#include "acmacs-chart-2/map-resolution-test.hh"
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
    return obj_->projections_modify().at(projection_no - 1);
}

// ----------------------------------------------------------------------

Projection Chart::clone_projection(size_t projection_no) const
{
    return static_cast<std::shared_ptr<acmacs::chart::ProjectionModify>>(obj_->projections_modify().at(projection_no - 1)->clone(*obj_));

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

std::string Chart::save_to_string()
{
    return acmacs::chart::export_factory(*obj_, acmacs::chart::export_format::ace, "acmacs.r", report_time::no);

} // Chart::save_to_string

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

Projection Chart::new_projection_with_layout_randomization1(std::string minimum_column_basis, size_t number_of_dimensions, std::string randomization_method, double diameter_multiplier)
{
    auto projection = obj_->projections_modify().new_from_scratch(acmacs::number_of_dimensions_t{number_of_dimensions}, minimum_column_basis);
    projection->randomize_layout(Projection::make_randomizer(randomization_method), diameter_multiplier <= 0.0 ? acmacs::chart::optimization_options{}.randomization_diameter_multiplier: diameter_multiplier);
    return static_cast<std::shared_ptr<acmacs::chart::ProjectionModify>>(projection);

} // Chart::new_projection_with_layout_randomization

Projection Chart::new_projection_with_layout_randomization2(std::string minimum_column_basis, size_t number_of_dimensions, std::string randomization_method)
{
    return new_projection_with_layout_randomization1(minimum_column_basis, number_of_dimensions, randomization_method, -1.0);
}

Projection Chart::new_projection_with_layout_randomization3(std::string minimum_column_basis, size_t number_of_dimensions)
{
    return new_projection_with_layout_randomization1(minimum_column_basis, number_of_dimensions, "sample-optimization", -1.0);
}

// ----------------------------------------------------------------------

Projection Chart::new_projection_with_layout(std::string minimum_column_basis, const Rcpp::NumericMatrix& layout)
{
    auto leyt = Projection::layout_convert(layout);
    auto projection = obj_->projections_modify().new_from_scratch(leyt.number_of_dimensions(), minimum_column_basis);
    projection->set_layout(leyt);
    return static_cast<std::shared_ptr<acmacs::chart::ProjectionModify>>(projection);

} // Chart::new_projection_with_layout

// ----------------------------------------------------------------------

Projection Chart::relax1(size_t number_of_dimensions, std::string opt1, std::string opt2, std::string opt3, std::string opt4, std::string opt5, std::string opt6, std::string opt7, std::string opt8,
                         std::string opt9)
{
    using namespace std::string_view_literals;
    acmacs::chart::optimization_options options{acmacs::chart::optimization_method::alglib_cg_pca, acmacs::chart::optimization_precision::fine, 2.0};
    std::string minimum_column_basis{"none"};
    acmacs::chart::use_dimension_annealing dimension_annealing{acmacs::chart::use_dimension_annealing::no};

    const auto parse_opt = [&](std::string_view opt) {
        if (opt == "rough"sv)
            options.precision = acmacs::chart::optimization_precision::rough;
        else if (opt == "fine"sv) // default
            options.precision = acmacs::chart::optimization_precision::fine;
        else if (opt == "optimization-lbfgs"sv)
            options.method = acmacs::chart::optimization_method::alglib_lbfgs_pca;
        else if (opt == "optimization-cg"sv) // default
            options.method = acmacs::chart::optimization_method::alglib_cg_pca;
        else if (opt == "disconnect-having-too-few-titers"sv) // default
            options.disconnect_too_few_numeric_titers = acmacs::chart::disconnect_few_numeric_titers::yes;
        else if (opt == "no-disconnect-having-too-few-titers"sv)
            options.disconnect_too_few_numeric_titers = acmacs::chart::disconnect_few_numeric_titers::no;
        else if (opt == "none"sv || opt == "1280"sv)
            minimum_column_basis = opt;
        else if (opt == "dimension-annealing"sv)
            dimension_annealing = acmacs::chart::use_dimension_annealing::yes;
        else if (opt == "no-dimension-annealing"sv) // default
            dimension_annealing = acmacs::chart::use_dimension_annealing::no;
        else if (!opt.empty())
            std::cerr << "WARNING: acmacs chart relax: unrecognized argument: \"" << opt << "\" (ignored)\n";
    };

    parse_opt(opt1);
    parse_opt(opt2);
    parse_opt(opt3);
    parse_opt(opt4);
    parse_opt(opt5);
    parse_opt(opt6);
    parse_opt(opt7);
    parse_opt(opt8);
    parse_opt(opt9);

    auto [status, projection] = obj_->relax(minimum_column_basis, acmacs::number_of_dimensions_t{number_of_dimensions}, dimension_annealing, options);
    // obj_->projections_modify().sort();
    return projection;
}

Projection Chart::relax2(std::string minimum_column_basis, size_t number_of_dimensions)
{
    auto [status, projection] = obj_->relax(minimum_column_basis, acmacs::number_of_dimensions_t{number_of_dimensions}, acmacs::chart::use_dimension_annealing::no,
                                            acmacs::chart::optimization_options(acmacs::chart::optimization_method::alglib_cg_pca, acmacs::chart::optimization_precision::fine, 2.0));
      // obj_->projections_modify().sort();
    return projection;
}

Projection Chart::relax3(std::string minimum_column_basis, size_t number_of_dimensions, bool rough)
{
    auto [status, projection] = obj_->relax(minimum_column_basis, acmacs::number_of_dimensions_t{number_of_dimensions}, acmacs::chart::use_dimension_annealing::no,
                                            acmacs::chart::optimization_options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 2.0));
      // obj_->projections_modify().sort();
    return projection;
}

Projection Chart::relax_seed(std::string minimum_column_basis, size_t number_of_dimensions, bool rough, unsigned seed)
{
    auto [status, projection] = obj_->relax(minimum_column_basis, acmacs::number_of_dimensions_t{number_of_dimensions}, acmacs::chart::use_dimension_annealing::no,
                                            acmacs::chart::optimization_options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 2.0),
                                            seed);
      // obj_->projections_modify().sort();
    return projection;
}

void Chart::relax_many(std::string minimum_column_basis, size_t number_of_dimensions, size_t number_of_optimizations, bool rough)
{
    acmacs::chart::optimization_options options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 2.0);
    obj_->relax(acmacs::chart::number_of_optimizations_t{number_of_optimizations}, minimum_column_basis, acmacs::number_of_dimensions_t{number_of_dimensions}, acmacs::chart::use_dimension_annealing::no, options, acmacs::chart::DisconnectedPoints{});
    obj_->projections_modify().sort();
}

// ----------------------------------------------------------------------

void Chart::relax_incremental_1(size_t number_of_optimizations, bool rough)
{
    acmacs::chart::optimization_options options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 2.0);
    constexpr const size_t projection_no{0};
    obj_->relax_incremental(projection_no, acmacs::chart::number_of_optimizations_t{number_of_optimizations}, options);
    obj_->projections_modify().sort();
}

void Chart::relax_incremental_2(size_t number_of_optimizations, std::string opt1, std::string opt2, std::string opt3, std::string opt4, std::string opt5)
{
    using namespace std::string_view_literals;

    acmacs::chart::optimization_options options;
    auto remove_source_projection{acmacs::chart::remove_source_projection::yes};
    auto unmovable_non_nan_points{acmacs::chart::unmovable_non_nan_points::no};

    const auto parse_opt = [&](std::string_view opt) {
        if (opt == "rough"sv)
            options.precision = acmacs::chart::optimization_precision::rough;
        else if (opt == "fine"sv) // default
            options.precision = acmacs::chart::optimization_precision::fine;
        else if (opt == "optimization-lbfgs"sv)
            options.method = acmacs::chart::optimization_method::alglib_lbfgs_pca;
        else if (opt == "optimization-cg"sv) // default
            options.method = acmacs::chart::optimization_method::alglib_cg_pca;
        else if (opt == "disconnect-having-too-few-titers"sv) // default
            options.disconnect_too_few_numeric_titers = acmacs::chart::disconnect_few_numeric_titers::yes;
        else if (opt == "no-disconnect-having-too-few-titers"sv)
            options.disconnect_too_few_numeric_titers = acmacs::chart::disconnect_few_numeric_titers::no;
        else if (opt == "unmovable-primary-points"sv)
            unmovable_non_nan_points = acmacs::chart::unmovable_non_nan_points::yes;
        else if (opt == "no-unmovable-primary-points"sv) // default
            unmovable_non_nan_points = acmacs::chart::unmovable_non_nan_points::no;
        else if (!opt.empty())
            std::cerr << "WARNING: acmacs chart relax_incremental: unrecognized argument: \"" << opt << "\" (ignored)\n";
    };

    parse_opt(opt1);
    parse_opt(opt2);
    parse_opt(opt3);
    parse_opt(opt4);
    parse_opt(opt5);

    constexpr const size_t projection_no{0};
    obj_->relax_incremental(projection_no, acmacs::chart::number_of_optimizations_t{number_of_optimizations}, options, remove_source_projection, unmovable_non_nan_points);
    obj_->projections_modify().sort();

} // Chart::relax_incremental_2

// ----------------------------------------------------------------------

acmacs::chart::Stress Chart::stress_evaluator(size_t number_of_dimensions, std::string minimum_column_basis)
{
    return acmacs::chart::stress_factory(*obj_, acmacs::number_of_dimensions_t{number_of_dimensions}, minimum_column_basis, acmacs::chart::multiply_antigen_titer_until_column_adjust::yes, acmacs::chart::dodgy_titer_is_regular::no);
}

// ----------------------------------------------------------------------

Rcpp::StringVector Chart::extension_field(std::string field_name) const
{
    if (const auto& ext = obj_->extension_field(field_name); !ext.is_null())
        return rjson::format(ext);
    else
        return NA_STRING;

} // Chart::extension_field

// ----------------------------------------------------------------------

void Chart::set_extension_field(std::string field_name, std::string value) const
{
    obj_->extension_field_modify(field_name, rjson::parse_string(value));

} // Chart::set_extension_field

// ----------------------------------------------------------------------

Rcpp::NumericMatrix Projection::transformation() const
{
    const auto a_tr = obj_->transformation();
    Rcpp::NumericMatrix transformation(*a_tr.number_of_dimensions, *a_tr.number_of_dimensions);
    for (auto row : acmacs::range(a_tr.number_of_dimensions))
        for (auto column : acmacs::range(a_tr.number_of_dimensions))
            transformation(*row, *column) = a_tr._x(*row, *column);
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
    if (acmacs::number_of_dimensions_t{static_cast<size_t>(aCoordinates.size())} != obj_->number_of_dimensions())
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

void Projection::dimension_annealing(size_t target_number_of_dimensions)
{
    auto layout = obj_->layout_modified();
    if (acmacs::number_of_dimensions_t{target_number_of_dimensions} >= layout->number_of_dimensions() || target_number_of_dimensions == 0)
        throw std::invalid_argument(fmt::format("invalid target number of dimensions ({}) for dimension annealing, expected 1:{}", target_number_of_dimensions, *layout->number_of_dimensions() - 1));
    auto stress = acmacs::chart::stress_factory(*obj_, acmacs::chart::multiply_antigen_titer_until_column_adjust::yes);
    acmacs::chart::dimension_annealing(acmacs::chart::optimization_method::alglib_cg_pca, stress, layout->number_of_dimensions(), acmacs::number_of_dimensions_t{target_number_of_dimensions}, layout->data(), layout->data() + layout->size());
    layout->change_number_of_dimensions(acmacs::number_of_dimensions_t{target_number_of_dimensions});

} // Projection::dimension_annealing

// ----------------------------------------------------------------------

acmacs::chart::ProjectionModify::randomizer Projection::make_randomizer(std::string randomization_method)
{
    using namespace acmacs::chart;
    if (randomization_method == "sample-optimization")
        return ProjectionModify::randomizer::plain_from_sample_optimization;
    else if (randomization_method == "table-max-distance")
        return ProjectionModify::randomizer::plain_with_table_max_distance;
    else if (randomization_method == "current-layout-area")
        return ProjectionModify::randomizer::plain_with_current_layout_area;
    else
        throw std::invalid_argument("invalid randomization method, supported: \"sample-optimization\", \"table-max-distance\", \"current-layout-area\"");

} // Projection::make_randomizer

// ----------------------------------------------------------------------

void Projection::randomize_layout(std::string randomization_method, double diameter_multiplier)
{
    obj_->randomize_layout(make_randomizer(randomization_method), diameter_multiplier);
    intermediate_layouts_.reset();
}

// ----------------------------------------------------------------------

Rcpp::NumericMatrix Projection::layout_convert(std::shared_ptr<acmacs::Layout> layout)
{
    Rcpp::NumericMatrix result(layout->number_of_points(), *layout->number_of_dimensions());
    for (size_t p_no = 0; p_no < layout->number_of_points(); ++p_no) {
        const auto coord = layout->at(p_no);
        if (coord.exists()) {
            for (auto dim : acmacs::range(layout->number_of_dimensions()))
                result(p_no, *dim) = coord[dim];
        }
        else {
            for (auto dim : acmacs::range(layout->number_of_dimensions()))
                result(p_no, *dim) = Rcpp::NumericMatrix::get_na();
        }
    }
    return result;
}

// ----------------------------------------------------------------------

acmacs::Layout Projection::layout_convert(const Rcpp::NumericMatrix& source)
{
    acmacs::Layout result(source.nrow(), acmacs::number_of_dimensions_t{static_cast<size_t>(source.ncol())});
    for (size_t point_no = 0; point_no < result.number_of_points(); ++point_no) {
        for (auto dim_no : acmacs::range(static_cast<size_t>(source.ncol())))
            result(point_no, acmacs::number_of_dimensions_t{dim_no}) = source(point_no, dim_no);
    }
    return result;

} // Projection::layout_convert

// ----------------------------------------------------------------------

acmacs::Transformation Projection::transformation_convert(const Rcpp::NumericMatrix& source) const
{
    if (source.nrow() != source.ncol() || static_cast<size_t>(source.nrow()) != number_of_dimensions())
        throw std::invalid_argument{"invalid transfromation matrix size"};
    acmacs::Transformation result(acmacs::number_of_dimensions_t{number_of_dimensions()});
    for (auto row : acmacs::range(number_of_dimensions()))
        for (auto column : acmacs::range(number_of_dimensions()))
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

void Projection::set_disconnected(const Rcpp::IntegerVector& points)
{
    acmacs::chart::DisconnectedPoints to_disconnect(points.size());
    std::transform(points.begin(), points.end(), to_disconnect.begin(), [](auto index) { return index - 1; });
    obj_->set_disconnected(to_disconnect);

} // Projection::set_disconnected

// ----------------------------------------------------------------------

void Projection::set_unmovable(const Rcpp::IntegerVector& points)
{
    acmacs::chart::UnmovablePoints to_freeze(points.size());
    std::transform(points.begin(), points.end(), to_freeze.begin(), [](auto index) { return index - 1; });
    obj_->set_unmovable(to_freeze);

} // Projection::set_unmovable

// ----------------------------------------------------------------------

void Projection::set_unmovable_in_the_last_dimension(const Rcpp::IntegerVector& points)
{
    acmacs::chart::UnmovableInTheLastDimensionPoints to_freeze(points.size());
    std::transform(points.begin(), points.end(), to_freeze.begin(), [](auto index) { return index - 1; });
    obj_->set_unmovable_in_the_last_dimension(to_freeze);

} // Projection::set_unmovable_in_the_last_dimension

// ----------------------------------------------------------------------

acmacs::chart::Stress stress_from_distances(const Rcpp::NumericMatrix& distances, size_t number_of_dimensions)
{
    acmacs::chart::Stress stress(acmacs::number_of_dimensions_t{number_of_dimensions}, distances.nrow() + distances.ncol(), acmacs::chart::multiply_antigen_titer_until_column_adjust::yes, acmacs::chart::dodgy_titer_is_regular::no);
    auto& td = stress.table_distances();
    for (size_t ag_no = 0; ag_no < distances.nrow(); ++ag_no)
        for (size_t sr_no = 0; sr_no < distances.ncol(); ++sr_no)
            td.add_value(acmacs::chart::Titer::Regular, ag_no, sr_no + distances.nrow(), distances(ag_no, sr_no));
    return stress;
}

// ----------------------------------------------------------------------

double stress_value(acmacs::chart::Stress* stress, const Rcpp::NumericMatrix& layout)
{
    if (acmacs::number_of_dimensions_t{static_cast<size_t>(layout.ncol())} != stress->number_of_dimensions())
        throw std::invalid_argument("Invalid number of columns in the layout matrix");
    return stress->value(Projection::layout_convert(layout));
}

double stress_contribution(acmacs::chart::Stress* stress, size_t point_no, const Rcpp::NumericMatrix& layout)
{
    if (acmacs::number_of_dimensions_t{static_cast<size_t>(layout.ncol())} != stress->number_of_dimensions())
        throw std::invalid_argument("Invalid number of columns in the layout matrix");
    if (point_no < 1 || point_no > layout.nrow())
        throw std::invalid_argument("Invalid point_no");
    return stress->contribution(point_no - 1, Projection::layout_convert(layout));
}

std::vector<double> stress_gradient(acmacs::chart::Stress* stress, const Rcpp::NumericMatrix& layout)
{
    if (acmacs::number_of_dimensions_t{static_cast<size_t>(layout.ncol())} != stress->number_of_dimensions())
        throw std::invalid_argument("Invalid number of columns in the layout matrix");
    return stress->gradient(Projection::layout_convert(layout));
}

// ----------------------------------------------------------------------

Rcpp::NumericMatrix ProcrustesData::transformation() const
{
    const auto& a_tr = obj_->transformation;
    Rcpp::NumericMatrix transformation(*a_tr.number_of_dimensions + 1, *a_tr.number_of_dimensions + 1);
    for (auto row : acmacs::range(a_tr.number_of_dimensions)) {
        for (auto col : acmacs::range(a_tr.number_of_dimensions))
            transformation(*row, *col) = a_tr(row, col);
        transformation(*row, *a_tr.number_of_dimensions) = 1.0;
    }
    for (auto col : acmacs::range(a_tr.number_of_dimensions))
        transformation(*a_tr.number_of_dimensions, *col) = a_tr.translation(col);
    transformation(*a_tr.number_of_dimensions, *a_tr.number_of_dimensions) = 1.0;
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

static acmacs::chart::MergeSettings merge_settinsg(std::string match_level, size_t projection_merge);

acmacs::chart::MergeSettings merge_settinsg(std::string match_level, size_t projection_merge)
{
    acmacs::chart::MergeSettings settings(convert_match_level(match_level));
    switch (projection_merge) {
        case 1:
            settings.projection_merge = acmacs::chart::projection_merge_t::type1;
            break;
        case 2:
            settings.projection_merge = acmacs::chart::projection_merge_t::type2;
            break;
        case 3:
            settings.projection_merge = acmacs::chart::projection_merge_t::type3;
            break;
        case 4:
            settings.projection_merge = acmacs::chart::projection_merge_t::type4;
            break;
        case 5:
            settings.projection_merge = acmacs::chart::projection_merge_t::type5;
            break;
        default:
            throw std::invalid_argument(std::string{"Unrecognized projection merge type "} + std::to_string(projection_merge) + " (1..5 expected)");
    }
    return settings;
}

// ----------------------------------------------------------------------

Chart merge(Chart chart1, Chart chart2, std::string match_level, size_t projection_merge)
{
    auto [result, diagnostics] = acmacs::chart::merge(*chart1.obj_, *chart2.obj_, merge_settinsg(match_level, projection_merge));
    return result;
}

// ----------------------------------------------------------------------

Chart merge_overlay(Chart chart1, Chart chart2)
{
    Rprintf("acmacs.merge_overlay deprecated! use acmacs.merge.3\n");
    auto result = merge(chart1, chart2, "a", 3);
      // std::cout << "overlay 1 " << result.obj_->projection(0)->stress() << '\n';
    result.obj_->projection_modify(0)->relax(acmacs::chart::optimization_options{});
      // std::cout << "overlay 2 " << result.obj_->projection(0)->stress() << '\n';
    return result;
}

// ----------------------------------------------------------------------

Chart merge_frozen(Chart chart1, Chart chart2)
{
    Rprintf("acmacs.merge_frozen deprecated! use acmacs.merge.4\n");
    auto result = merge(chart1, chart2, "a", 4);
      // std::cout << "frozen " << result.obj_->projection(0)->stress() << '\n';
    return result;
}

// ----------------------------------------------------------------------

Chart merge_incremental(Chart chart1, Chart chart2, size_t number_of_optimizations, size_t num_threads)
{
    Rprintf("acmacs.merge_incremental deprecated! use acmacs.merge.2() followed by merge$relax_incremental(number_of_optimizations)\n");
    auto result = merge(chart1, chart2, "a", 2);
    acmacs::chart::optimization_options options(acmacs::chart::optimization_method::alglib_cg_pca, acmacs::chart::optimization_precision::fine, 2.0);
    options.num_threads = num_threads;
    constexpr const size_t projection_no{0};
    result.obj_->relax_incremental(projection_no, acmacs::chart::number_of_optimizations_t{number_of_optimizations}, options);
    return result;
}

// ----------------------------------------------------------------------

std::string merge_report(Chart chart1, Chart chart2, std::string match_level)
{
    auto [result, diagnostics] = acmacs::chart::merge(*chart1.obj_, *chart2.obj_, merge_settinsg(match_level, 1));
    return diagnostics.titer_merge_report(*result);
}

// ----------------------------------------------------------------------

Rcpp::DataFrame map_resolution_test(Chart chart, const Rcpp::IntegerVector& number_of_dimensions, const Rcpp::NumericVector& proportions_to_dont_care, std::string minimum_column_basis,
                                    bool column_bases_from_master, bool relax_from_full_table,
                                    size_t number_of_random_replicates_for_each_proportion, size_t number_of_optimizations, std::string save_charts_to)
{
    using namespace Rcpp;

    acmacs::chart::map_resolution_test_data::Parameters parameters{
        std::vector<acmacs::number_of_dimensions_t>(number_of_dimensions.size(), acmacs::number_of_dimensions_t{1}),
        acmacs::chart::number_of_optimizations_t{number_of_optimizations},
        number_of_random_replicates_for_each_proportion,
        std::vector<double>(std::begin(proportions_to_dont_care), std::end(proportions_to_dont_care)),
        acmacs::chart::MinimumColumnBasis{minimum_column_basis},
        column_bases_from_master ? acmacs::chart::map_resolution_test_data::column_bases_from_master::yes : acmacs::chart::map_resolution_test_data::column_bases_from_master::no,
        acmacs::chart::optimization_precision::rough,
        relax_from_full_table ? acmacs::chart::map_resolution_test_data::relax_from_full_table::yes : acmacs::chart::map_resolution_test_data::relax_from_full_table::no,
        save_charts_to
    };
    std::transform(std::begin(number_of_dimensions), std::end(number_of_dimensions), std::begin(parameters.number_of_dimensions), [](size_t val) { return acmacs::number_of_dimensions_t{val}; });
    // fmt::print(stderr, "DEBUG: {}\n", parameters);

    const auto results = acmacs::chart::map_resolution_test(*chart.obj_, parameters);
    const auto& predictions = results.predictions();

    auto df = DataFrame::create(_["number_of_dimensions"] = IntegerVector(predictions.size()), _["proportion_to_dont_care"] = NumericVector(predictions.size()),
                                _["av_abs_error"] = NumericVector(predictions.size()), _["av_abs_error_sd"] = NumericVector(predictions.size()), _["sd_error"] = NumericVector(predictions.size()),
                                _["sd_error_sd"] = NumericVector(predictions.size()), _["correlation"] = NumericVector(predictions.size()), _["correlation_sd"] = NumericVector(predictions.size()),
                                _["r2"] = NumericVector(predictions.size()), _["r2_sd"] = NumericVector(predictions.size()), _["number_of_samples"] = IntegerVector(predictions.size()));
    IntegerVector df_number_of_dimensions = df["number_of_dimensions"];
    NumericVector df_proportion_to_dont_care = df["proportion_to_dont_care"];
    NumericVector df_av_abs_error = df["av_abs_error"];
    NumericVector df_av_abs_error_sd = df["av_abs_error_sd"];
    NumericVector df_sd_error = df["sd_error"];
    NumericVector df_sd_error_sd = df["sd_error_sd"];
    NumericVector df_correlation = df["correlation"];
    NumericVector df_correlation_sd = df["correlation_sd"];
    NumericVector df_r2 = df["r2"];
    NumericVector df_r2_sd = df["r2_sd"];
    IntegerVector df_number_of_samples = df["number_of_samples"];
    for (auto [index, entry] : acmacs::enumerate(predictions)) {
        df_number_of_dimensions[index] = *entry.number_of_dimensions;
        df_proportion_to_dont_care[index] = entry.proportion_to_dont_care;
        df_av_abs_error[index] = entry.av_abs_error.mean();
        df_av_abs_error_sd[index] = entry.av_abs_error.population_sd();
        df_sd_error[index] = entry.sd_error.mean();
        df_sd_error_sd[index] = entry.sd_error.population_sd();
        df_correlation[index] = entry.correlations.mean();
        df_correlation_sd[index] = entry.correlations.population_sd();
        df_r2[index] = entry.r2.mean();
        df_r2_sd[index] = entry.r2.population_sd();
        df_number_of_samples[index] = entry.number_of_samples;
    }
    return df;

} // map_resolution_test

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
