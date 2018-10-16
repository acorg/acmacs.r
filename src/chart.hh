#pragma once

#include "acmacs-chart-2/chart-modify.hh"
#include "acmacs-chart-2/procrustes.hh"
#include "acmacs-chart-2/factory-import.hh"
#include "acmacs-chart-2/factory-export.hh"

#include <Rcpp.h>

#include "wrapper.hh"

// ----------------------------------------------------------------------

RCPP_EXPOSED_CLASS_NODECL(acmacs::chart::Passage);

class PlotSpec;
class Titers;
class Projection;

// ----------------------------------------------------------------------

class Antigen : public wrapper<acmacs::chart::AntigenModify>
{
 public:
    Antigen(acmacs::chart::AntigenModifyP antigen) : wrapper(antigen) {}
    std::string get_name() { return obj_->name(); }
    void set_name(std::string name) { obj_->name(name); }
    std::string get_full_name() const { return obj_->full_name(); }
    std::string get_abbreviated_name() const { return obj_->abbreviated_name(); }
    std::string get_date() const { return obj_->date(); }
    void set_date(std::string date) { obj_->date(date); }
    std::string get_lineage() const { return obj_->lineage(); }
    void set_lineage(std::string lineage) { obj_->lineage(lineage); }
    std::string get_reassortant() const { return obj_->reassortant(); }
    void set_reassortant(std::string reassortant) { obj_->reassortant(reassortant); }
    acmacs::chart::Passage get_passage() const { return obj_->passage(); }
    void set_passage(std::string passage) { return obj_->passage(passage); }
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
    std::string get_name() { return obj_->name(); }
    void set_name(std::string name) { obj_->name(name); }
    std::string get_full_name() const { return obj_->full_name(); }
    std::string get_abbreviated_name() const { return obj_->abbreviated_name(); }
    std::string get_lineage() const { return obj_->lineage(); }
    void set_lineage(std::string lineage) { obj_->lineage(lineage); }
    std::string get_reassortant() const { return obj_->reassortant(); }
    void set_reassortant(std::string reassortant) { obj_->reassortant(reassortant); }
    acmacs::chart::Passage get_passage() const { return obj_->passage(); }
    void set_passage(std::string passage) { return obj_->passage(passage); }
    std::string get_serum_id() const { return obj_->serum_id(); }
    void set_serum_id(std::string serum_id) { obj_->serum_id(serum_id); }
    std::string get_serum_species() const { return obj_->serum_species(); }
    void set_serum_species(std::string serum_species) { obj_->serum_species(serum_species); }
    Rcpp::StringVector get_annotations() const { return getStringVector(obj_->annotations()); }
    void add_annotation(std::string annotation) { obj_->add_annotation(annotation); }
    void remove_annotation(std::string annotation) { obj_->remove_annotation(annotation); }

    static inline Rcpp::StringVector as_character(Serum* aSerum) { return {aSerum->obj_->full_name()}; }
};
RCPP_EXPOSED_CLASS_NODECL(Serum);

// ----------------------------------------------------------------------

class Chart : public wrapper<acmacs::chart::ChartModify>
{
 public:
    Chart(std::string aFilename);
    Chart(Rcpp::RawVector aData);
    Chart(int number_of_antigens, int number_of_sera);
    Chart(std::shared_ptr<acmacs::chart::ChartModify> src);
    std::string name() const { return obj_->make_name(); }
    std::string info() const { return obj_->make_info(); }
    std::string lineage() const { return obj_->lineage(); }
    size_t number_of_antigens() const { return obj_->number_of_antigens(); }
    size_t number_of_sera() const { return obj_->number_of_sera(); }
    size_t number_of_points() const { return obj_->number_of_points(); }
    size_t number_of_projections() const { return obj_->number_of_projections(); }
    Rcpp::NumericVector column_bases_2(size_t aProjectionNo, std::string aMinimumColumnBasis) const;
    Rcpp::NumericVector column_bases_1(size_t aProjectionNo) const { return column_bases_2(aProjectionNo, "none"); }
    Rcpp::NumericVector column_bases_1s(std::string aMinimumColumnBasis) const { return column_bases_2(std::numeric_limits<size_t>::max(), aMinimumColumnBasis); }
    Rcpp::NumericVector column_bases_0() const { return column_bases_2(0, "none"); }
    PlotSpec plot_spec();
    Titers titers();
    static Rcpp::StringVector as_character(Chart* aChart) { return {aChart->name()}; }

    Rcpp::List get_antigens() const;
    Rcpp::List get_sera() const;
    Antigen insert_antigen(size_t before) { return obj_->insert_antigen(before - 1); }
    Serum insert_serum(size_t before) { return obj_->insert_serum(before - 1); }
    void remove_antigens(const Rcpp::NumericVector& aIndexes);
    void remove_sera(const Rcpp::NumericVector& aIndexes);

    void set_name(std::string name) { return obj_->info_modify()->name(name); }
    void save(std::string aFilename);

      // https://stackoverflow.com/questions/42579207/rcpp-modules-validator-function-for-exposed-constructors-with-same-number-of-pa
    template <typename T> static inline bool validate_constructor(SEXP* args, int nargs) { return nargs == 1 && Rcpp::is<T>(args[0]); }

    Projection relax2(std::string minimum_column_basis, size_t number_of_dimensions);
    Projection relax3(std::string minimum_column_basis, size_t number_of_dimensions, bool rough);
    void relax_many(std::string minimum_column_basis, size_t number_of_dimensions, size_t number_of_optimizations, bool rough);
    void sort_projections() { obj_->projections_modify()->sort(); }
    void remove_all_projections() { obj_->projections_modify()->remove_all(); }
    void remove_all_projections_except(size_t projection_no_one_based) { obj_->projections_modify()->remove_all_except(projection_no_one_based - 1); }
    void remove_layers() { obj_->remove_layers(); }
    Chart clone() const;

}; // class Chart

RCPP_EXPOSED_CLASS_NODECL(Chart);

// ----------------------------------------------------------------------

class Projection : public wrapper<acmacs::chart::ProjectionModify>
{
 public:
    Projection(acmacs::chart::ProjectionModifyP projection) : wrapper(projection) {}
      // static Rcpp::StringVector as_character(Projection* aProjection) { return {aProjection->obj_->full_name()}; }

    std::string make_info() const { return obj_->make_info(); }
    std::string comment() const { return obj_->comment(); }

    Rcpp::NumericMatrix transformation() const;
    Rcpp::NumericVector forced_column_bases() const;
    Rcpp::NumericMatrix layout() const { return layout_convert(obj_->layout()); }
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

 private:
    Rcpp::NumericMatrix layout_convert(std::shared_ptr<acmacs::chart::Layout> layout) const;

}; // class Projection
RCPP_EXPOSED_CLASS_NODECL(Projection);

// ----------------------------------------------------------------------

class ProcrustesData : public wrapper<acmacs::chart::ProcrustesData>
{
 public:
    ProcrustesData(std::shared_ptr<acmacs::chart::ProcrustesData> src) : wrapper(src) {}
    double rms() const { return obj_->rms; }

    Rcpp::NumericMatrix transformation() const;
};
RCPP_EXPOSED_CLASS_NODECL(ProcrustesData);

ProcrustesData procrustes(Projection primary, Projection secondary, bool scaling, std::string match);

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
