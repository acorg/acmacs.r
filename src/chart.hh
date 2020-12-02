#pragma once

#include "acmacs-chart-2/chart-modify.hh"
#include "acmacs-chart-2/procrustes.hh"
#include "acmacs-chart-2/factory-import.hh"
#include "acmacs-chart-2/factory-export.hh"

#include <Rcpp.h>

#include "wrapper.hh"

// ----------------------------------------------------------------------

RCPP_EXPOSED_CLASS_NODECL(acmacs::virus::Passage);
RCPP_EXPOSED_CLASS_NODECL(acmacs::chart::Stress);

class PlotSpec;
class Titers;
class Projection;

// ----------------------------------------------------------------------

class Antigen : public wrapper<acmacs::chart::AntigenModify>
{
 public:
    Antigen(acmacs::chart::AntigenModifyP antigen) : wrapper(antigen) {}
    std::string get_name() { return *obj_->name(); }
    void set_name(std::string name) { obj_->name(name); }
    std::string get_full_name() const { return obj_->full_name(); }
    std::string get_abbreviated_name() const { return obj_->abbreviated_name(); }
    std::string get_date() const { return *obj_->date(); }
    void set_date(std::string date) { obj_->date(date); }
    std::string get_lineage() const { return obj_->lineage().to_string(); }
    void set_lineage(std::string lineage) { obj_->lineage(lineage); }
    std::string get_reassortant() const { return *obj_->reassortant(); }
    void set_reassortant(std::string reassortant) { obj_->reassortant(acmacs::virus::Reassortant{reassortant}); }
    acmacs::virus::Passage get_passage() const { return obj_->passage(); }
    void set_passage(std::string passage) { return obj_->passage(acmacs::virus::Passage{passage}); }
    bool get_reference() const { return obj_->reference(); }
    void set_reference(bool reference) { obj_->reference(reference); }
    Rcpp::StringVector get_lab_ids() const { return getStringVector(obj_->lab_ids()); }
    Rcpp::StringVector get_annotations() const { return getStringVector(obj_->annotations()); }
    void add_annotation(std::string annotation) { obj_->add_annotation(annotation); }
    void remove_annotation(std::string annotation) { obj_->remove_annotation(annotation); }

    static inline Rcpp::StringVector as_character(Antigen* aAntigen) { return {aAntigen->obj_->full_name()}; }
};
RCPP_EXPOSED_CLASS_NODECL(Antigen);

// ----------------------------------------------------------------------

class Serum : public wrapper<acmacs::chart::SerumModify>
{
 public:
    Serum(acmacs::chart::SerumModifyP serum) : wrapper(serum) {}
    std::string get_name() { return *obj_->name(); }
    void set_name(std::string name) { obj_->name(name); }
    std::string get_full_name() const { return obj_->full_name(); }
    std::string get_abbreviated_name() const { return obj_->abbreviated_name(); }
    std::string get_lineage() const { return obj_->lineage().to_string(); }
    void set_lineage(std::string lineage) { obj_->lineage(lineage); }
    std::string get_reassortant() const { return *obj_->reassortant(); }
    void set_reassortant(std::string reassortant) { obj_->reassortant(acmacs::virus::Reassortant{reassortant}); }
    acmacs::virus::Passage get_passage() const { return obj_->passage(); }
    void set_passage(std::string passage) { return obj_->passage(acmacs::virus::Passage{passage}); }
    std::string get_serum_id() const { return *obj_->serum_id(); }
    void set_serum_id(std::string serum_id) { obj_->serum_id(acmacs::chart::SerumId{serum_id}); }
    std::string get_serum_species() const { return *obj_->serum_species(); }
    void set_serum_species(std::string serum_species) { obj_->serum_species(acmacs::chart::SerumSpecies{serum_species}); }
    Rcpp::StringVector get_annotations() const { return getStringVector(obj_->annotations()); }
    void add_annotation(std::string annotation) { obj_->add_annotation(annotation); }
    void remove_annotation(std::string annotation) { obj_->remove_annotation(annotation); }

    static inline Rcpp::StringVector as_character(Serum* aSerum) { return {aSerum->obj_->full_name()}; }
};
RCPP_EXPOSED_CLASS_NODECL(Serum);

// ----------------------------------------------------------------------

class Projection : public wrapper<acmacs::chart::ProjectionModify>
{
 public:
    Projection(const Projection& src) : wrapper(src) {}
    Projection(acmacs::chart::ProjectionModifyP projection) : wrapper(projection) {}
      // static Rcpp::StringVector as_character(Projection* aProjection) { return {aProjection->obj_->full_name()}; }

    std::string make_info() const { return obj_->make_info(); }
    std::string comment() const { return obj_->comment(); }
    size_t number_of_dimensions() const { return *obj_->number_of_dimensions(); }

    Rcpp::NumericMatrix transformation() const;
    void set_transformation(Rcpp::NumericMatrix src) { obj_->transformation(transformation_convert(src)); }
    Rcpp::NumericVector forced_column_bases() const;
    void set_column_bases(const Rcpp::NumericVector& data);
    void set_column_basis(size_t serum_no, double column_basis);
    Rcpp::NumericMatrix layout() const { return layout_convert(obj_->layout()); }
    void set_layout(Rcpp::NumericMatrix src) { obj_->set_layout(layout_convert(src)); }
    Rcpp::NumericMatrix transformed_layout() const { return layout_convert(obj_->transformed_layout()); }

    void rotate_degrees(double aDegrees) { obj_->rotate_degrees(aDegrees); }
    void rotate_radians(double aRadians) { obj_->rotate_radians(aRadians); }
    void flip_east_west() { obj_->flip_east_west(); }
    void flip_north_south() { obj_->flip_north_south(); }
      //void () { obj_->(); }

      // aPointNo is counted from 1 (as in R vector)
    void move_point(size_t aPointNo, const Rcpp::NumericVector& aCoordinates);

    double stress() const { return obj_->stress(); }
    double recalculate_stress() const { return obj_->stress(acmacs::chart::RecalculateStress::yes); }

    std::string minimum_column_basis() const { return obj_->minimum_column_basis(); }

    void relax(std::string method, bool rough);
    void relax_default() { relax("cg", false); }
    bool relax_one_iteration(std::string method, bool rough); // returns if optimization can be performed further
    bool relax_one_iteration_default() { return relax_one_iteration("cg", false); }
    void randomize_layout(std::string randomization_method, double diameter_multiplier);
    void randomize_layout_default() { randomize_layout("table-max-distance", 2.0); }

    void dimension_annealing(size_t target_number_of_dimensions);

    void set_disconnected(const Rcpp::IntegerVector& points);
    void set_unmovable(const Rcpp::IntegerVector& points);
    void set_unmovable_in_the_last_dimension(const Rcpp::IntegerVector& points);

    void reorient(const Projection& master, std::string match, std::string subset);
    void reorient_default(const Projection& master) { reorient(master, "auto", "all"); }

    static acmacs::Layout layout_convert(const Rcpp::NumericMatrix& source);
    static Rcpp::NumericMatrix layout_convert(std::shared_ptr<acmacs::Layout> layout);
    static acmacs::chart::ProjectionModify::randomizer make_randomizer(std::string randomization_method);

 private:
    std::unique_ptr<acmacs::chart::IntermediateLayouts> intermediate_layouts_;
    size_t next_layout_;

    acmacs::Transformation transformation_convert(const Rcpp::NumericMatrix& source) const;
    acmacs::chart::optimization_method optimization_method(std::string method) const;

}; // class Projection
RCPP_EXPOSED_CLASS_NODECL(Projection);

// ----------------------------------------------------------------------

class Chart : public wrapper<acmacs::chart::ChartModify>
{
 public:
    Chart(std::string aFilename);
    Chart(Rcpp::RawVector aData);
    Chart(int number_of_antigens, int number_of_sera);
    Chart(std::shared_ptr<acmacs::chart::ChartModify> src);
    std::string name() const { return obj_->info()->name(); }
    std::string info() const { return obj_->make_info(); }
    std::string lineage() const { return std::string{*obj_->lineage()}; }
    size_t number_of_antigens() const { return obj_->number_of_antigens(); }
    size_t number_of_sera() const { return obj_->number_of_sera(); }
    size_t number_of_points() const { return obj_->number_of_points(); }
    size_t number_of_projections() const { return obj_->number_of_projections(); }
    Rcpp::NumericVector column_bases_2(size_t aProjectionNo, std::string aMinimumColumnBasis) const;
    Rcpp::NumericVector column_bases_1(size_t aProjectionNo) const { return column_bases_2(aProjectionNo, "none"); }
    Rcpp::NumericVector column_bases_1s(std::string aMinimumColumnBasis) const { return column_bases_2(1, aMinimumColumnBasis); }
    Rcpp::NumericVector column_bases_0() const { return column_bases_2(1, "none"); }
    void set_column_bases(const Rcpp::NumericVector& data);
    void set_column_basis(size_t serum_no, double column_basis);
    PlotSpec plot_spec();
    Titers titers();
    static Rcpp::StringVector as_character(Chart* aChart) { return {aChart->name()}; }

    Rcpp::List get_antigens() const;
    Rcpp::List get_sera() const;
    Antigen insert_antigen(size_t before) { return obj_->insert_antigen(before - 1); }
    Serum insert_serum(size_t before) { return obj_->insert_serum(before - 1); }
    void remove_antigens(const Rcpp::NumericVector& aIndexes);
    void remove_sera(const Rcpp::NumericVector& aIndexes);

    void set_name(std::string name) { return obj_->info_modify().name(name); }
    void save(std::string aFilename);
    std::string save_to_string();

      // https://stackoverflow.com/questions/42579207/rcpp-modules-validator-function-for-exposed-constructors-with-same-number-of-pa
    template <typename T> static inline bool validate_constructor(SEXP* args, int nargs) { return nargs == 1 && Rcpp::is<T>(args[0]); }

    Projection new_projection_with_layout_randomization1(std::string minimum_column_basis, size_t number_of_dimensions, std::string randomization_method, double diameter_multiplier);
    Projection new_projection_with_layout_randomization2(std::string minimum_column_basis, size_t number_of_dimensions, std::string randomization_method);
    Projection new_projection_with_layout_randomization3(std::string minimum_column_basis, size_t number_of_dimensions);
    Projection new_projection_with_layout(std::string minimum_column_basis, const Rcpp::NumericMatrix& layout);

    Projection relax1(size_t number_of_dimensions, std::string opt1={}, std::string opt2={}, std::string opt3={}, std::string opt4={}, std::string opt5={}, std::string opt6={}, std::string opt7={}, std::string opt8={}, std::string opt9={});
    Projection relax10(size_t number_of_dimensions) { return relax1(number_of_dimensions); }
    Projection relax11(size_t number_of_dimensions, std::string opt1) { return relax1(number_of_dimensions, opt1); }
    Projection relax12(size_t number_of_dimensions, std::string opt1, std::string opt2) { return relax1(number_of_dimensions, opt1, opt2); }
    Projection relax13(size_t number_of_dimensions, std::string opt1, std::string opt2, std::string opt3) { return relax1(number_of_dimensions, opt1, opt2, opt3); }
    Projection relax14(size_t number_of_dimensions, std::string opt1, std::string opt2, std::string opt3, std::string opt4) { return relax1(number_of_dimensions, opt1, opt2, opt3, opt4); }
    Projection relax15(size_t number_of_dimensions, std::string opt1, std::string opt2, std::string opt3, std::string opt4, std::string opt5) { return relax1(number_of_dimensions, opt1, opt2, opt3, opt4, opt5); }
    Projection relax16(size_t number_of_dimensions, std::string opt1, std::string opt2, std::string opt3, std::string opt4, std::string opt5, std::string opt6) { return relax1(number_of_dimensions, opt1, opt2, opt3, opt4, opt5, opt6); }
    Projection relax17(size_t number_of_dimensions, std::string opt1, std::string opt2, std::string opt3, std::string opt4, std::string opt5, std::string opt6, std::string opt7) { return relax1(number_of_dimensions, opt1, opt2, opt3, opt4, opt5, opt6, opt7); }
    Projection relax18(size_t number_of_dimensions, std::string opt1, std::string opt2, std::string opt3, std::string opt4, std::string opt5, std::string opt6, std::string opt7, std::string opt8) { return relax1(number_of_dimensions, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8); }
    Projection relax2(size_t number_of_dimensions, unsigned seed, std::string opt1={}, std::string opt2={}, std::string opt3={}, std::string opt4={}, std::string opt5={}, std::string opt6={}, std::string opt7={}, std::string opt8={}, std::string opt9={});
    Projection relax3(const std::string& minimum_column_basis, size_t number_of_dimensions) { return relax4(minimum_column_basis, number_of_dimensions, false); }
    Projection relax4(const std::string& minimum_column_basis, size_t number_of_dimensions, bool rough);
    Projection relax_seed(const std::string& minimum_column_basis, size_t number_of_dimensions, bool rough, unsigned seed);
    void relax_many(const std::string& minimum_column_basis, size_t number_of_dimensions, size_t number_of_optimizations, bool rough);
    void relax_incremental_1(size_t number_of_optimizations, bool rough);
    void relax_incremental_2(size_t number_of_optimizations, std::string opt1={}, std::string opt2={}, std::string opt3={}, std::string opt4={}, std::string opt5={});
    void relax_incremental_2_1(size_t number_of_optimizations) { relax_incremental_2(number_of_optimizations); }
    void relax_incremental_2_2(size_t number_of_optimizations, std::string opt1) { relax_incremental_2(number_of_optimizations, opt1); }
    void relax_incremental_2_3(size_t number_of_optimizations, std::string opt1, std::string opt2) { relax_incremental_2(number_of_optimizations, opt1, opt2); }
    void relax_incremental_2_4(size_t number_of_optimizations, std::string opt1, std::string opt2, std::string opt3) { relax_incremental_2(number_of_optimizations, opt1, opt2, opt3); }
    void relax_incremental_2_5(size_t number_of_optimizations, std::string opt1, std::string opt2, std::string opt3, std::string opt4) { relax_incremental_2(number_of_optimizations, opt1, opt2, opt3, opt4); }
    void relax_incremental_2_6(size_t number_of_optimizations, std::string opt1, std::string opt2, std::string opt3, std::string opt4, std::string opt5) { relax_incremental_2(number_of_optimizations, opt1, opt2, opt3, opt4, opt5); }
    acmacs::chart::Stress stress_evaluator(size_t number_of_dimensions, std::string minimum_column_basis);
    void sort_projections() { obj_->projections_modify().sort(); }
    void remove_all_projections() { obj_->projections_modify().remove_all(); }
    void remove_all_projections_except(size_t projection_no_based_one) { obj_->projections_modify().remove_all_except(projection_no_based_one - 1); }
    void remove_layers() { obj_->remove_layers(); }
    Chart clone() const;
    Projection clone_projection(size_t projection_no_based_one) const;
    Projection projection(size_t projection_no);

    Rcpp::StringVector extension_field(std::string field_name) const;
    void set_extension_field(std::string field_name, std::string value) const;

}; // class Chart

RCPP_EXPOSED_CLASS_NODECL(Chart);

// ----------------------------------------------------------------------

acmacs::chart::Stress stress_from_distances(const Rcpp::NumericMatrix& distances, size_t number_of_dimensions);
double stress_value(acmacs::chart::Stress* stress, const Rcpp::NumericMatrix& layout);
double stress_contribution(acmacs::chart::Stress* stress, size_t point_no, const Rcpp::NumericMatrix& layout);
std::vector<double> stress_gradient(acmacs::chart::Stress* stress, const Rcpp::NumericMatrix& layout);
inline size_t stress_number_of_dimensions(acmacs::chart::Stress* stress) { return *stress->number_of_dimensions(); }
inline void stress_change_number_of_dimensions(acmacs::chart::Stress* stress, size_t number_of_dimensions) { stress->change_number_of_dimensions(acmacs::number_of_dimensions_t{number_of_dimensions}); }

// ----------------------------------------------------------------------

acmacs::chart::CommonAntigensSera::match_level_t convert_match_level(std::string source);

// class MatchData
// {
//   public:
//     MatchData(size_t number_of_antigens, size_t number_of_sera) : antigens(number_of_antigens), sera(number_of_sera) {}
//     Rcpp::IntegerVector antigens;
//     Rcpp::IntegerVector sera;
// };
// RCPP_EXPOSED_CLASS_NODECL(MatchData);

Rcpp::List match_antigens_sera(Chart chart1, Chart chart2, std::string match);

class ProcrustesData : public wrapper<acmacs::chart::ProcrustesData>
{
 public:
    ProcrustesData(std::shared_ptr<acmacs::chart::ProcrustesData> src) : wrapper(src) {}
    double rms() const { return obj_->rms; }
    const auto& transformation_raw() const { return obj_->transformation; }
    Rcpp::NumericMatrix transformation() const;
    Rcpp::NumericMatrix apply(const Rcpp::NumericMatrix& source_layout) const;
};
RCPP_EXPOSED_CLASS_NODECL(ProcrustesData);

ProcrustesData procrustes(Projection primary, Projection secondary, bool scaling, std::string match, std::string subset);

Chart merge(Chart chart1, Chart chart2, std::string match_level, size_t projection_merge);
Chart merge_incremental(Chart chart1, Chart chart2, size_t number_of_optimizations, size_t num_threads = 0);
Chart merge_overlay(Chart chart1, Chart chart2);
Chart merge_frozen(Chart chart1, Chart chart2);
std::string merge_report(Chart chart1, Chart chart2, std::string match_level);

Rcpp::DataFrame map_resolution_test(Chart chart, const Rcpp::IntegerVector& number_of_dimensions,
                                    const Rcpp::NumericVector& proportions_to_dont_care, std::string minimum_column_basis,
                                    bool column_bases_from_master, bool relax_from_full_table,
                                    size_t number_of_random_replicates_for_each_proportion, size_t number_of_optimizations, std::string save_charts_to);

// ----------------------------------------------------------------------

template <> inline bool Chart::validate_constructor<int>(SEXP* args, int nargs)
{
    return nargs == 2
            && (Rcpp::is<double>(args[0]) || Rcpp::is<int>(args[0]))
            && (Rcpp::is<double>(args[1]) || Rcpp::is<int>(args[1]));
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
