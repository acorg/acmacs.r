#include "locationdb/locdb.hh"

#include "chart.hh"
#include "titers.hh"
#include "plot-spec.hh"
#include "grid-test.hh"

// ----------------------------------------------------------------------

inline Rcpp::StringVector passage_as_character(acmacs::virus::Passage* aPassage) { return {**aPassage}; }
inline Rcpp::List Chart::get_antigens() const { return getList<Antigen>(obj_->antigens_modify()); }
inline Rcpp::List Chart::get_sera() const { return getList<Serum>(obj_->sera_modify()); }

inline Titers Chart::titers() { return obj_->titers_modify(); }
inline PlotSpec Chart::plot_spec() { return obj_->plot_spec_modify(); }

inline auto style_shown(acmacs::PointStyle* style) { return *style->shown; }
inline auto style_size(acmacs::PointStyle* style) { return style->size->value(); }
inline std::string style_fill(acmacs::PointStyle* style) { return style->fill->to_string(); }
inline std::string style_outline(acmacs::PointStyle* style) { return style->outline->to_string(); }
inline auto style_outline_width(acmacs::PointStyle* style) { return style->outline_width->value(); }
inline auto style_rotation(acmacs::PointStyle* style) { return style->rotation->value(); }
inline auto style_aspect(acmacs::PointStyle* style) { return style->aspect->value(); }
inline std::string style_shape(acmacs::PointStyle* style) { return *style->shape; }

// ----------------------------------------------------------------------

static inline void acmacs_init()
{
    get_locdb(locdb_suppress_error::yes); // to avoid error message if locdb is requested later and locationdb.json.xz is not available

    std::cout.rdbuf(Rcpp::Rcout.rdbuf());
    std::cerr.rdbuf(Rcpp::Rcerr.rdbuf());
}

// ----------------------------------------------------------------------

RCPP_MODULE(acmacs)
{
    using namespace Rcpp;

    function("acmacs.__init", &acmacs_init, "automatically called on module loading");

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
            .method("projection", &Chart::projection)
            .method("remove_all_projections_except", &Chart::remove_all_projections_except)
            .method("remove_all_projections", &Chart::remove_all_projections)
            .property<PlotSpec>("plot_spec", &Chart::plot_spec)
            .property<Titers>("titers", &Chart::titers)
            .method("remove_layers", &Chart::remove_layers)
            .method("column_bases", &Chart::column_bases_2, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            .method("column_bases", &Chart::column_bases_1, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            // .method("column_bases", &Chart::column_bases_1s, "either forced or calculated column bases with the provided minimum_column_basis")
            .method("column_bases", &Chart::column_bases_0, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            .method("set_column_bases", &Chart::set_column_bases, "set forced column bases for the chart and all its projections")
            .method("set_column_basis", &Chart::set_column_basis, "set forced column basis for a serum in the chart and all its projections")
            .method("save", &Chart::save)
            .method("save", &Chart::save_to_string)
            .method("new_projection", &Chart::new_projection_with_layout_randomization1)
            .method("new_projection", &Chart::new_projection_with_layout_randomization2)
            .method("new_projection", &Chart::new_projection_with_layout_randomization3)
            .method("new_projection_with_layout", &Chart::new_projection_with_layout)
            .method("relax", &Chart::relax2)
            .method("relax", &Chart::relax3)
            .method("relax", &Chart::relax_seed)
            .method("relax_incremetal", &Chart::relax_incremetal)
            .method("relax_many", &Chart::relax_many, "generate maps multiple times from random starts\n\targuments:\n\tminimum column basis, e.g. \"none\", \"1280\"\n\tnumber of dimensions, e.g. 2\n\tnumber of optimizations, e.g. 10\n\tuse rough optimization (30% faster): TRUE or FALSE\n")
            .method("stress_evaluator", &Chart::stress_evaluator, "returns stress evaluator object for the distances constructed from the titers of this chart")
            .method("sort_projections", &Chart::sort_projections)
            .method("clone", &Chart::clone)
            .method("clone_projection", &Chart::clone_projection)
            .method("extension_field", &Chart::extension_field)
            .method("set_extension_field", &Chart::set_extension_field)
            ;
    function("as.character.Rcpp_acmacs.Chart", &Chart::as_character, "as.character.Rcpp_acmacs.Chart(chart) - shows brief info about passed chart");

    class_<Antigen>("acmacs.Antigen")
            .property("name", &Antigen::get_name, &Antigen::set_name, "antigen name")
            .method("set_name", &Antigen::set_name)
            .property("full_name", &Antigen::get_full_name)
            .property("abbreviated_name", &Antigen::get_abbreviated_name)
            .property("date", &Antigen::get_date, &Antigen::set_date)
            .method("set_date", &Antigen::set_date)
            .property<acmacs::virus::Passage>("passage", &Antigen::get_passage)
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

    class_<acmacs::virus::Passage>("acmacs.Passage")
            .method("is_egg", &acmacs::virus::Passage::is_egg)
            .method("is_cell", &acmacs::virus::Passage::is_cell)
            .method("without_date", &acmacs::virus::Passage::without_date)
            .method("type", &acmacs::virus::Passage::passage_type)
            ;
    function("as.character.Rcpp_acmacs.Passage", &passage_as_character);

    class_<Projection>("acmacs.Projection")
            .property<std::string>("info", &Projection::make_info)
            .property("stress", &Projection::stress)
            .method("recalculate_stress", &Projection::recalculate_stress)
            .property<std::string>("comment", &Projection::comment)
            .property<std::string>("minimum_column_basis", &Projection::minimum_column_basis)
            .property("number_of_dimensions", &Projection::number_of_dimensions)
            .property("forced_column_bases", &Projection::forced_column_bases)
            .method("set_column_bases", &Projection::set_column_bases, "set forced column bases for the projections")
            .method("set_column_basis", &Projection::set_column_basis, "set forced column basis for a serum in the chart in the projection")
            .property("transformation", &Projection::transformation)
            .method("set_transformation", &Projection::set_transformation)
            .property("layout", &Projection::layout, &Projection::set_layout)
            .method("set_layout", &Projection::set_layout)
            .property("transformed_layout", &Projection::transformed_layout)
            .method("rotate_degrees", &Projection::rotate_degrees)
            .method("rotate_radians", &Projection::rotate_radians)
            .method("flip_east_west", &Projection::flip_east_west)
            .method("flip_north_south", &Projection::flip_north_south)
            .method("move_point", &Projection::move_point)
            .method("relax", &Projection::relax)
            .method("relax", &Projection::relax_default)
            .method("relax_one_iteration", &Projection::relax_one_iteration)
            .method("relax_one_iteration", &Projection::relax_one_iteration_default)
            .method("randomize_layout", &Projection::randomize_layout)
            .method("randomize_layout", &Projection::randomize_layout_default)
            .method("reorient", &Projection::reorient)
            .method("reorient", &Projection::reorient_default)
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

    class_<PlotSpec>("acmacs.PlotSpec")
            .property("styles", &PlotSpec::styles)
            .method("styles_as_data_frame", &PlotSpec::styles_as_data_frame)
            .method("set_styles", &PlotSpec::set_styles)
            .property("drawing_order", &PlotSpec::drawing_order)
            .method("raise", &PlotSpec::drawing_order_raise)
            .method("lower", &PlotSpec::drawing_order_lower)
            .method("raise_sera", &PlotSpec::drawing_order_raise_sera)
            .method("lower_sera", &PlotSpec::drawing_order_lower_sera)
            .method("set_shown", &PlotSpec::set_style_shown)
            .method("set_size", &PlotSpec::set_style_size)
            .method("set_fill", &PlotSpec::set_style_fill)
            .method("set_outline", &PlotSpec::set_style_outline)
            .method("set_outline_width", &PlotSpec::set_style_outline_width)
            .method("set_rotation", &PlotSpec::set_style_rotation)
            .method("set_aspect", &PlotSpec::set_style_aspect)
            .method("set_shape", &PlotSpec::set_style_shape)
            .method("set_label_shown", &PlotSpec::set_style_label_shown)
            .method("set_label_offset_x", &PlotSpec::set_style_label_offset_x)
            .method("set_label_offset_y", &PlotSpec::set_style_label_offset_y)
            .method("set_label_size", &PlotSpec::set_style_label_size)
            .method("set_label_color", &PlotSpec::set_style_label_color)
            .method("set_label_rotation", &PlotSpec::set_style_label_rotation)
            .method("set_label_slant", &PlotSpec::set_style_label_slant)
            .method("set_label_weight", &PlotSpec::set_style_label_weight)
            .method("set_label_font_family", &PlotSpec::set_style_label_font_family)
            .method("set_label_text", &PlotSpec::set_style_label_text)
            ;

    class_<Titers>("acmacs.Titers")
            .method("all", &Titers::all)
            .method("titer", &Titers::titer)
            .method("set_titer", &Titers::set_titer, "modifying titer for a chart having projections leads to undefined behaviour")
            .method("set_dontcare_for_antigen", &Titers::set_dontcare_for_antigen)
            .method("set_dontcare_for_serum", &Titers::set_dontcare_for_serum)
            .method("multiply_by_for_antigen", &Titers::multiply_by_for_antigen)
            .method("multiply_by_for_serum", &Titers::multiply_by_for_serum)
            .property("number_of_layers", &Titers::number_of_layers)
            .method("titer_of_layer", &Titers::titer_of_layer)
            .method("titers_for_all_layers", &Titers::titers_for_all_layers)
            .method("all_layers", &Titers::all_layers)
            ;

    function("acmacs.match_antigens_sera", &match_antigens_sera, List::create(_["chart1"], _["chart2"], _["match"] = "a"));

    // class_<MatchData>("acmacs.MatchData")
    //         .field("antigens", &MatchData::antigens, "vector of indexes of antigens of chart2 (or NA) for each antigen of chart1")
    //         .field("sera", &MatchData::sera, "vector of indexes of sera of chart2 (or NA) for each serum of chart1")
    //         ;

    function("acmacs.merge", &merge, List::create(_["chart1"], _["chart2"], _["match"] = "a", _["merge"] = 1));
    function("acmacs.merge.1", &merge, List::create(_["chart1"], _["chart2"], _["match"] = "a", _["merge"] = 1));
    function("acmacs.merge.2", &merge, List::create(_["chart1"], _["chart2"], _["match"] = "a", _["merge"] = 2));
    function("acmacs.merge.3", &merge, List::create(_["chart1"], _["chart2"], _["match"] = "a", _["merge"] = 3));
    function("acmacs.merge.4", &merge, List::create(_["chart1"], _["chart2"], _["match"] = "a", _["merge"] = 4));
    function("acmacs.merge.5", &merge, List::create(_["chart1"], _["chart2"], _["match"] = "a", _["merge"] = 5));
    function("acmacs.merge_incremental", &merge_incremental, List::create(_["chart1"], _["chart2"], _["optimizations"] = 100, _["threads"] = 0));
    function("acmacs.merge_frozen", &merge_frozen, List::create(_["chart1"], _["chart2"]));
    function("acmacs.merge_overlay", &merge_overlay, List::create(_["chart1"], _["chart2"]));
    function("acmacs.merge_report", &merge_report, List::create(_["chart1"], _["chart2"], _["match"] = "a"));

    function("acmacs.procrustes", &procrustes, List::create(_["projection_primary"], _["projection_secondary"], _["scaling"] = false, _["match"] = "a", _["subset"] = "a"));

    class_<ProcrustesData>("acmacs.ProcrustesData")
            .property("rms", &ProcrustesData::rms, nullptr)
            .property("transformation", &ProcrustesData::transformation, nullptr)
            .method("apply", &ProcrustesData::apply, "transform and translate layout according to the procrustes")
            ;

    class_<GridTest>("acmacs.GridTest")
            .constructor<Chart&, size_t>()
            .constructor<Chart&>()
            .method("test", &GridTest::test_parallel)
            .method("test_single_thread", &GridTest::test_single_thread)
            .method("results", &GridTest::results)
            .method("make_new_projection_and_relax", &GridTest::make_new_projection_and_relax)
            ;

    function("acmacs.stress_from_distances", &stress_from_distances, List::create(_["distance_matrix"]));
    class_<acmacs::chart::Stress>("acmacs.Stress")
            .method("value", &stress_value)
            .method("contribution", &stress_contribution)
            .method("gradient", &stress_gradient)
            .method("number_of_dimensions", &stress_number_of_dimensions)
            .method("change_number_of_dimensions", &stress_change_number_of_dimensions)
            ;

    function("acmacs.map_resolution_test", &map_resolution_test, List::create(_["chart"], _["number_of_dimensions"] = Rcpp::IntegerVector::create(1, 2, 3, 4, 5), _["proportions_to_dont_care"] = Rcpp::NumericVector::create(0.1, 0.2, 0.3), _["minimum_column_basis"] = "none", _["column_bases_from_master"] = true, _["relax_from_full_table"] = false, _["number_of_random_replicates_for_each_proportion"] = 25, _["number_of_optimizations"] = 100, _["save_charts_to"] = ""));
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
