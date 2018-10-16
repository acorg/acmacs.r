#include <limits>

#include "chart.hh"
#include "titers.hh"
#include "plot-spec.hh"

// ----------------------------------------------------------------------

inline auto style_shown(acmacs::PointStyle* style) { return *style->shown; }
inline auto style_size(acmacs::PointStyle* style) { return style->size->value(); }
inline std::string style_fill(acmacs::PointStyle* style) { return style->fill->to_string(); }
inline std::string style_outline(acmacs::PointStyle* style) { return style->outline->to_string(); }
inline auto style_outline_width(acmacs::PointStyle* style) { return style->outline_width->value(); }
inline auto style_rotation(acmacs::PointStyle* style) { return style->rotation->value(); }
inline auto style_aspect(acmacs::PointStyle* style) { return style->aspect->value(); }
inline std::string style_shape(acmacs::PointStyle* style) { return *style->shape; }

inline Rcpp::StringVector passage_as_character(acmacs::chart::Passage* aPassage) { return {*aPassage}; }
inline Rcpp::List Chart::get_antigens() const { return getList<Antigen>(obj_->antigens_modify()); }
inline Rcpp::List Chart::get_sera() const { return getList<Serum>(obj_->sera_modify()); }
inline Titers Chart::titers() { return obj_->titers_modify(); }
inline PlotSpec Chart::plot_spec() { return obj_->plot_spec_modify(); }

// ----------------------------------------------------------------------

RCPP_MODULE(acmacs_chart)
{
    using namespace Rcpp;

    class_<Chart>("acmacs.Chart")
            .constructor<std::string>("read chart data from a file", &Chart::validate_constructor<std::string>)
            .constructor<Rcpp::RawVector>("read chart from raw inline data", &Chart::validate_constructor<Rcpp::RawVector>)
            .constructor<int, int>("create new chart from scratch", &Chart::validate_constructor<int>)
            .property<size_t>("number_of_antigens", &Chart::number_of_antigens)
            .property<size_t>("number_of_sera", &Chart::number_of_sera)
            .property<size_t>("number_of_points", &Chart::number_of_points)
            .property<size_t>("number_of_projections", &Chart::number_of_projections)
            .property<std::string>("lineage", &Chart::lineage)
            .property<std::string>("info", &Chart::info, "multi-line string brifly describing data stored in the chart")
            .property<std::string>("name", &Chart::name, &Chart::set_name)
            .property<Rcpp::List>("antigens", &Chart::get_antigens)
            .property<Rcpp::List>("sera", &Chart::get_sera)
            .method("insert_antigen", &Chart::insert_antigen)
            .method("insert_serum", &Chart::insert_serum)
            .method("remove_antigens", &Chart::remove_antigens)
            .method("remove_sera", &Chart::remove_sera)
            .property<Rcpp::List>("projections", &Chart::getListViaAt<Projection, &acmacs::chart::ChartModify::projections_modify>)
            .method("remove_all_projections_except", &Chart::remove_all_projections_except)
            .method("remove_all_projections", &Chart::remove_all_projections)
            .property<PlotSpec>("plot_spec", &Chart::plot_spec)
            .property<Titers>("titers", &Chart::titers)
            .method("remove_layers", &Chart::remove_layers)
            .method("column_bases", &Chart::column_bases_2, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            .method("column_bases", &Chart::column_bases_1, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            // .method("column_bases", &Chart::column_bases_1s, "either forced or calculated column bases with the provided minimum_column_basis")
            .method("column_bases", &Chart::column_bases_0, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            .method("save", &Chart::save)
            .method("relax", &Chart::relax2)
            .method("relax", &Chart::relax3)
            .method("relax_many", &Chart::relax_many, "generate maps multiple times from random starts\n\targuments:\n\tminimum column basis, e.g. \"none\", \"1280\"\n\tnumber of dimensions, e.g. 2\n\tnumber of optimizations, e.g. 10\n\tuse rough optimization (30% faster): TRUE or FALSE\n")
            .method("sort_projections", &Chart::sort_projections)
            ;
    function("as.character.Rcpp_acmacs.Chart", &Chart::as_character, "as.character.Rcpp_acmacs.Chart(chart) - shows brief info about passed chart");

    class_<Antigen>("acmacs.Antigen")
            .property("name", &Antigen::get_name, &Antigen::set_name, "antigen name")
            .method("set_name", &Antigen::set_name)
            .property("full_name", &Antigen::get_full_name)
            .property("abbreviated_name", &Antigen::get_abbreviated_name)
            .property("date", &Antigen::get_date, &Antigen::set_date)
            .method("set_date", &Antigen::set_date)
            .property<acmacs::chart::Passage>("passage", &Antigen::get_passage)
            .method("set_passage", &Antigen::set_passage)
            .property("lineage", &Antigen::get_lineage, &Antigen::set_lineage)
            .method("set_lineage", &Antigen::set_lineage)
            .property("reassortant", &Antigen::get_reassortant, &Antigen::set_reassortant)
            .method("set_reassortant", &Antigen::set_reassortant)
            .property("reference", &Antigen::get_reference, &Antigen::set_reference)
            .method("set_reference", &Antigen::set_reference)
            .property<Rcpp::StringVector>("lab_ids", &Antigen::get_lab_ids)
            .property<Rcpp::StringVector>("annotations", &Antigen::get_annotations)
            .method("add_annotation", &Antigen::add_annotation)
            .method("remove_annotation", &Antigen::remove_annotation)
            ;
    function("as.character.Rcpp_acmacs.Antigen", &Antigen::as_character);

    class_<Serum>("acmacs.Serum")
            .property("name", &Serum::get_name, &Serum::set_name)
            .method("set_name", &Serum::set_name)
            .property("full_name", &Serum::get_full_name)
            .property("abbreviated_name", &Serum::get_abbreviated_name)
            .property("passage", &Serum::get_passage)
            .method("set_passage", &Serum::set_passage)
            .property("lineage", &Serum::get_lineage, &Serum::set_lineage)
            .method("set_lineage", &Serum::set_lineage)
            .property("reassortant", &Serum::get_reassortant, &Serum::set_reassortant)
            .method("set_reassortant", &Serum::set_reassortant)
            .property("serum_id", &Serum::get_serum_id, &Serum::set_serum_id)
            .method("set_serum_id", &Serum::set_serum_id)
            .property("serum_species", &Serum::get_serum_species, &Serum::set_serum_species)
            .method("set_serum_species", &Serum::set_serum_species)
            .property<Rcpp::StringVector>("annotations", &Serum::get_annotations)
            .method("add_annotation", &Serum::add_annotation)
            .method("remove_annotation", &Serum::remove_annotation)
            ;
    function("as.character.Rcpp_acmacs.Serum", &Serum::as_character);

    class_<acmacs::chart::Passage>("acmacs.Passage")
            .method("is_egg", &acmacs::chart::Passage::is_egg)
            .method("is_cell", &acmacs::chart::Passage::is_cell)
            .method("without_date", &acmacs::chart::Passage::without_date)
            .method("type", &acmacs::chart::Passage::passage_type)
            ;
    function("as.character.Rcpp_acmacs.Passage", &passage_as_character);

    class_<Projection>("acmacs.Projection")
            .property<std::string>("info", &Projection::make_info)
            .property("stress", &Projection::stress)
            .method("recalculate_stress", &Projection::recalculate_stress)
            .property<std::string>("comment", &Projection::comment)
            .property<std::string>("minimum_column_basis", &Projection::minimum_column_basis)
            .property("forced_column_bases", &Projection::forced_column_bases)
            .property("transformation", &Projection::transformation)
            .property("layout", &Projection::layout)
            .property("transformed_layout", &Projection::transformed_layout)
            .method("rotate_degrees", &Projection::rotate_degrees)
            .method("rotate_radians", &Projection::rotate_radians)
            .method("flip_east_west", &Projection::flip_east_west)
            .method("flip_north_south", &Projection::flip_north_south)
            .method("move_point", &Projection::move_point)
            .method("relax", &Projection::relax)
            .method("relax", &Projection::relax_default)
            ;

    class_<acmacs::PointStyle>("acmacs.PointStyle")
            .property("shown", &style_shown, nullptr)
            .property("size", &style_size, nullptr)
            .property("fill", &style_fill, nullptr)
            .property("outline", &style_outline, nullptr)
            .property("outline_width", &style_outline_width, nullptr)
            .property("rotation", &style_rotation, nullptr)
            .property("aspect", &style_aspect, nullptr)
            .property("shape", &style_shape, nullptr)
              // LabelStyle label;
              // field<std::string> label_text;
            ;

    function("acmacs.procrustes", &procrustes);

    class_<ProcrustesData>("acmacs.ProcrustesData")
            .property("rms", &ProcrustesData::rms, nullptr)
            .property("transformation", &ProcrustesData::transformation, nullptr)
            ;

}

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
