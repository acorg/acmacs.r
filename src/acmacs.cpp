#include <limits>
#include <Rcpp.h>

#include "acmacs-chart-2/chart-modify.hh"
#include "acmacs-chart-2/factory-import.hh"
#include "acmacs-chart-2/factory-export.hh"
#include "acmacs-chart-2/procrustes.hh"

// ----------------------------------------------------------------------

template <typename T> class wrapper
{
 public:
    // using owning_t = T;
    wrapper() = default;
    wrapper(std::shared_ptr<T> obj) : obj_(obj) {}
      //~wrapper() { Rprintf("~wrapper %s\n", typeid(this).name()); }
    // std::shared_ptr<T> operator*() { return obj_; }
    std::shared_ptr<T> obj_;

    template <auto (T::*property)() const> auto get() const
        {
            return ((*obj_).*property)();
        }

    // template <typename R, typename ... Args, R (T::*func)(Args ...) const> R method(Args ... args) const
    //     {
    //         return ((*obj_).*func)(args ...);
    //     }

    // template <void (T::*func)() const> void method_void_no_args() const
    //     {
    //         ((*obj_).*func)();
    //     }

    template <typename R, auto (T::*property)() const> R getT() const
        {
            return ((*obj_).*property)();
        }

    // template <typename Arg, auto (T::*property)(const Arg&)> void setT(const Arg& a) const
    //     {
    //         ((*obj_).*property)(a);
    //     }

    template <typename Wrapper, auto (T::*property)() const> Rcpp::List getList() const
        {
            auto elements = ((*obj_).*property)();
            auto result = Rcpp::List::create();
            for (auto elt: *elements)
                result.push_back(Wrapper(elt));
            return result;
        }

    template <typename Wrapper, auto (T::*property)()> Rcpp::List getListViaAt()
        {
            auto elements = ((*obj_).*property)();
            const auto num_elements = elements->size();
            auto result = Rcpp::List::create();
            for (size_t no = 0; no < num_elements; ++no)
                result.push_back(Wrapper(elements->at(no)));
            return result;
        }

    template <auto (T::*property)() const> Rcpp::StringVector getSV() const
        {
            auto elements = ((*obj_).*property)();
            Rcpp::StringVector result(elements.size());
            for (size_t i = 0; i < elements.size(); ++i)
                result[i] = elements[i];
            return result;
        }

      // ----------------------------------------------------------------------

    template <typename Wrapper, typename V> Rcpp::List getList(V elements) const
        {
            auto result = Rcpp::List::create();
            for (size_t index = 0; index < elements->size(); ++index)
                result.push_back(Wrapper(elements->ptr_at(index)));
            return result;
        }

    template <typename V> Rcpp::StringVector getStringVector(const V& elements) const
        {
            Rcpp::StringVector result(elements.size());
            for (size_t i = 0; i < elements.size(); ++i)
                result[i] = elements[i];
            return result;
        }

}; // class wrapper<>

// ----------------------------------------------------------------------

class PlotSpec;
class Titers;
class Projection;

class Chart : public wrapper<acmacs::chart::ChartModify>
{
 public:
    Chart(std::string aFilename)
        : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_file(aFilename, acmacs::chart::Verify::None, report_time::No))) {}
    Chart(Rcpp::RawVector aData)
        : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_data(std::string_view(reinterpret_cast<const char*>(aData.cbegin()), aData.size()), acmacs::chart::Verify::None, report_time::No))) {}
    Chart(int number_of_antigens, int number_of_sera)
        : wrapper(std::make_shared<acmacs::chart::ChartNew>(number_of_antigens, number_of_sera)) {}
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
    void save(std::string aFilename) { acmacs::chart::export_factory(*obj_, aFilename, "acmacs.r", report_time::No); }

      // https://stackoverflow.com/questions/42579207/rcpp-modules-validator-function-for-exposed-constructors-with-same-number-of-pa
    template <typename T> static inline bool validate_constructor(SEXP* args, int nargs) { return nargs == 1 && Rcpp::is<T>(args[0]); }

    Projection relax2(std::string minimum_column_basis, size_t number_of_dimensions);
    Projection relax3(std::string minimum_column_basis, size_t number_of_dimensions, bool rough);
    void relax_many(std::string minimum_column_basis, size_t number_of_dimensions, size_t number_of_optimizations, bool rough);
    void sort_projections() { obj_->projections_modify()->sort(); }

}; // class Chart
RCPP_EXPOSED_CLASS_NODECL(Chart);

template <> inline bool Chart::validate_constructor<int>(SEXP* args, int nargs) { return nargs == 2 && Rcpp::is<double>(args[0]) && Rcpp::is<double>(args[1]); }

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

RCPP_EXPOSED_CLASS_NODECL(acmacs::chart::Passage);
inline Rcpp::StringVector passage_as_character(acmacs::chart::Passage* aPassage) { return {*aPassage}; }
inline Rcpp::List Chart::get_antigens() const { return getList<Antigen>(obj_->antigens_modify()); }
inline Rcpp::List Chart::get_sera() const { return getList<Serum>(obj_->sera_modify()); }

class Projection : public wrapper<acmacs::chart::ProjectionModify>
{
 public:
    Projection(acmacs::chart::ProjectionModifyP projection) : wrapper(projection) {}
      // static Rcpp::StringVector as_character(Projection* aProjection) { return {aProjection->obj_->full_name()}; }

    std::string make_info() const { return obj_->make_info(); }
    std::string comment() const { return obj_->comment(); }

    Rcpp::NumericMatrix transformation() const
        {
            const auto a_tr = obj_->transformation();
            Rcpp::NumericMatrix transformation(2, 2);
            transformation(0, 0) = a_tr.a;
            transformation(0, 1) = a_tr.b;
            transformation(1, 0) = a_tr.c;
            transformation(1, 1) = a_tr.d;
            return transformation;
        }

    Rcpp::NumericVector forced_column_bases() const
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

    Rcpp::NumericMatrix layout() const { return layout_convert(obj_->layout()); }
    Rcpp::NumericMatrix transformed_layout() const { return layout_convert(obj_->transformed_layout()); }

    void rotate_degrees(double aDegrees) { obj_->rotate_degrees(aDegrees); }
    void rotate_radians(double aRadians) { obj_->rotate_radians(aRadians); }
    void flip_east_west() { obj_->flip_east_west(); }
    void flip_north_south() { obj_->flip_north_south(); }
      //void () { obj_->(); }

      // aPointNo is counted from 1 (as in R vector)
    void move_point(size_t aPointNo, const Rcpp::NumericVector& aCoordinates)
        {
            if (aPointNo < 1 || aPointNo > obj_->number_of_points())
                throw std::invalid_argument("invalid point number");
            if (aCoordinates.size() != obj_->number_of_dimensions())
                throw std::invalid_argument("invalid vector size (number of point coordinates)");
            obj_->move_point(aPointNo - 1, {aCoordinates.begin(), aCoordinates.end()});
        }

    double stress() const { return obj_->stress(); }

    std::string minimum_column_basis() const { return obj_->minimum_column_basis(); }

    void relax(std::string method, bool rough)
        {
            using namespace acmacs::chart;
            optimization_method opt_method{optimization_method::alglib_cg_pca};
            if (method == "lbfgs")
                opt_method = optimization_method::alglib_lbfgs_pca;
            else if (!method.empty() && method != "cg")
                throw std::invalid_argument("invalid optimization method");
            obj_->relax(optimization_options(opt_method, rough ? optimization_precision::rough : optimization_precision::fine));
        }

    void relax_default() { relax("cg", false); }

 private:
    Rcpp::NumericMatrix layout_convert(std::shared_ptr<acmacs::chart::Layout> layout) const
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

};
RCPP_EXPOSED_CLASS_NODECL(Projection);

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

class PlotSpec : public wrapper<acmacs::chart::PlotSpecModify>
{
 public:
    PlotSpec(acmacs::chart::PlotSpecModifyP plot_spec) : wrapper(plot_spec) {}

    Rcpp::List styles() const { const auto styles = obj_->all_styles(); return {styles.begin(), styles.end()}; }

    Rcpp::IntegerVector drawing_order() const
        {
            const auto drawing_order = obj_->drawing_order();
            Rcpp::IntegerVector result(drawing_order.size());
            std::transform(drawing_order.begin(), drawing_order.end(), result.begin(), [](auto index) { return index + 1; });
            return result;
            // return {drawing_order.begin(), drawing_order.end()};
        }

    void drawing_order_raise(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->raise(indexes);
        }
    void drawing_order_lower(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->lower(indexes);
        }
    void drawing_order_raise_sera(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->raise_serum(indexes);
        }
    void drawing_order_lower_sera(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->lower_serum(indexes);
        }

    void set_style_size(const Rcpp::IntegerVector& aIndexes, double aSize)
        {
            for (auto index: aIndexes)
                obj_->size(index - 1, Pixels{aSize});
        }
    void set_style_fill(const Rcpp::IntegerVector& aIndexes, std::string aFill)
        {
            for (auto index: aIndexes)
                obj_->fill(index - 1, Color(aFill));
        }
    void set_style_outline(const Rcpp::IntegerVector& aIndexes, std::string aOutline)
        {
            for (auto index: aIndexes)
                obj_->outline(index - 1, Color(aOutline));
        }
};
RCPP_EXPOSED_CLASS_NODECL(PlotSpec);

inline PlotSpec Chart::plot_spec() { return obj_->plot_spec_modify(); }

RCPP_EXPOSED_CLASS_NODECL(acmacs::PointStyle);
inline auto style_shown(acmacs::PointStyle* style) { return *style->shown; }
inline auto style_size(acmacs::PointStyle* style) { return style->size->value(); }
inline std::string style_fill(acmacs::PointStyle* style) { return style->fill->to_string(); }
inline std::string style_outline(acmacs::PointStyle* style) { return style->outline->to_string(); }
inline auto style_outline_width(acmacs::PointStyle* style) { return style->outline_width->value(); }
inline auto style_rotation(acmacs::PointStyle* style) { return style->rotation->value(); }
inline auto style_aspect(acmacs::PointStyle* style) { return style->aspect->value(); }
inline std::string style_shape(acmacs::PointStyle* style) { return *style->shape; }

// ----------------------------------------------------------------------

class Titers : public wrapper<acmacs::chart::TitersModify>
{
 public:
    Titers(acmacs::chart::TitersModifyP titers) : wrapper(titers) {}
    std::string titer(size_t ag_no, size_t sr_no) const { return obj_->titer(ag_no - 1, sr_no - 1); }
    Rcpp::StringMatrix all() const
        {
            Rcpp::StringMatrix result(obj_->number_of_antigens(), obj_->number_of_sera());
            for (size_t ag_no = 0; ag_no < obj_->number_of_antigens(); ++ag_no)
                for (size_t sr_no = 0; sr_no < obj_->number_of_sera(); ++sr_no)
                    result(ag_no, sr_no) = static_cast<std::string>(obj_->titer(ag_no, sr_no));
            return result;
        }
};
RCPP_EXPOSED_CLASS_NODECL(Titers);

inline Titers Chart::titers() { return obj_->titers_modify(); }

inline Projection Chart::relax2(std::string minimum_column_basis, size_t number_of_dimensions)
{
    auto [status, projection] = obj_->relax(minimum_column_basis, number_of_dimensions, true,
                                            acmacs::chart::optimization_options(acmacs::chart::optimization_method::alglib_cg_pca, acmacs::chart::optimization_precision::fine, 1.0));
      // obj_->projections_modify()->sort();
    return projection;
}

inline Projection Chart::relax3(std::string minimum_column_basis, size_t number_of_dimensions, bool rough)
{
    auto [status, projection] = obj_->relax(minimum_column_basis, number_of_dimensions, true,
                                            acmacs::chart::optimization_options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 1.0));
      // obj_->projections_modify()->sort();
    return projection;
}

inline void Chart::relax_many(std::string minimum_column_basis, size_t number_of_dimensions, size_t number_of_optimizations, bool rough)
{
    acmacs::chart::optimization_options options(acmacs::chart::optimization_method::alglib_cg_pca, rough ? acmacs::chart::optimization_precision::rough : acmacs::chart::optimization_precision::fine, 1.0);
    for (size_t attempt = 0; attempt < number_of_optimizations; ++attempt) {
        obj_->relax(minimum_column_basis, number_of_dimensions, true, options);
    }
    obj_->projections_modify()->sort();
}

// ----------------------------------------------------------------------

class ProcrustesData : public wrapper<acmacs::chart::ProcrustesData>
{
 public:
    ProcrustesData(std::shared_ptr<acmacs::chart::ProcrustesData> src) : wrapper(src) {}
    double rms() const { return obj_->rms; }

    Rcpp::NumericMatrix transformation() const
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

};
RCPP_EXPOSED_CLASS_NODECL(ProcrustesData);

inline ProcrustesData procrustes(Projection primary, Projection secondary, bool scaling, std::string match)
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

static void init_cout_cerr()
{
    std::cout.rdbuf(Rcpp::Rcout.rdbuf());
    std::cerr.rdbuf(Rcpp::Rcerr.rdbuf());
}

// ----------------------------------------------------------------------

RCPP_MODULE(acmacs)
{
    using namespace Rcpp;

    function("acmacs.init_cout_cerr", &init_cout_cerr, "automatically called on module loading");

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
            .property<std::string>("name", &Chart::name)
            .property<Rcpp::List>("antigens", &Chart::get_antigens)
            .property<Rcpp::List>("sera", &Chart::get_sera)
            .property<Rcpp::List>("projections", &Chart::getListViaAt<Projection, &acmacs::chart::ChartModify::projections_modify>)
            .property<PlotSpec>("plot_spec", &Chart::plot_spec)
            .property<Titers>("titers", &Chart::titers)
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
            .property<double>("stress", &Projection::stress)
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

    class_<PlotSpec>("acmacs.PlotSpec")
            .property("styles", &PlotSpec::styles)
            .property("drawing_order", &PlotSpec::drawing_order)
            .method("raise", &PlotSpec::drawing_order_raise)
            .method("lower", &PlotSpec::drawing_order_lower)
            .method("raise_sera", &PlotSpec::drawing_order_raise_sera)
            .method("lower_sera", &PlotSpec::drawing_order_lower_sera)
            .method("set_size", &PlotSpec::set_style_size)
            .method("set_fill", &PlotSpec::set_style_fill)
            .method("set_outline", &PlotSpec::set_style_outline)
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

    class_<Titers>("acmacs.Titers")
            .method("titer", &Titers::titer)
            .method("all", &Titers::all)
            ;

    function("acmacs.procrustes", &procrustes);

    class_<ProcrustesData>("acmacs.ProcrustesData")
            .property("rms", &ProcrustesData::rms, nullptr)
            .property("transformation", &ProcrustesData::transformation, nullptr)
            ;
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
