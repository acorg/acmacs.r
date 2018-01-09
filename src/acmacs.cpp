#include <limits>
#include <Rcpp.h>

#include "acmacs-chart-2/chart-modify.hh"
#include "acmacs-chart-2/factory-import.hh"
#include "acmacs-chart-2/factory-export.hh"

// ----------------------------------------------------------------------

template <typename T> class wrapper
{
 public:
    // using owning_t = T;
    inline wrapper() = default;
    inline wrapper(std::shared_ptr<T> obj) : obj_(obj) {}
      //inline ~wrapper() { Rprintf("~wrapper %s\n", typeid(this).name()); }
    // inline std::shared_ptr<T> operator*() { return obj_; }
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

}; // class wrapper<>

// ----------------------------------------------------------------------

class PlotSpec;
class Titers;

class Chart : public wrapper<acmacs::chart::ChartModify>
{
 public:
    inline Chart(std::string aFilename)
        : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_file(aFilename, acmacs::chart::Verify::None, report_time::No))) {}
    inline Chart(Rcpp::RawVector aData)
        : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_from_data(std::string_view(reinterpret_cast<const char*>(aData.cbegin()), aData.size()), acmacs::chart::Verify::None, report_time::No))) {}
    inline std::string name() const { return obj_->make_name(); }
    inline std::string info() const { return obj_->make_info(); }
    inline std::string lineage() const { return obj_->lineage(); }
    inline size_t number_of_antigens() const { return obj_->number_of_antigens(); }
    inline size_t number_of_sera() const { return obj_->number_of_sera(); }
    inline size_t number_of_points() const { return obj_->number_of_points(); }
    inline size_t number_of_projections() const { return obj_->number_of_projections(); }
    Rcpp::NumericVector column_bases_2(size_t aProjectionNo, std::string aMinimumColumnBasis) const;
    inline Rcpp::NumericVector column_bases_1(size_t aProjectionNo) const { return column_bases_2(aProjectionNo, "none"); }
    inline Rcpp::NumericVector column_bases_1s(std::string aMinimumColumnBasis) const { return column_bases_2(std::numeric_limits<size_t>::max(), aMinimumColumnBasis); }
    inline Rcpp::NumericVector column_bases_0() const { return column_bases_2(0, "none"); }
    PlotSpec plot_spec();
    Titers titers();
    static inline Rcpp::StringVector as_character(Chart* aChart) { return {aChart->name()}; }

    inline void save(std::string aFilename) { acmacs::chart::export_factory(*obj_, aFilename, "acmacs.r", report_time::No); }

      // https://stackoverflow.com/questions/42579207/rcpp-modules-validator-function-for-exposed-constructors-with-same-number-of-pa
    template <typename T> static inline bool validate_constructor(SEXP* args, int nargs) { return nargs == 1 && Rcpp::is<T>(args[0]); }

}; // class Chart
RCPP_EXPOSED_CLASS_NODECL(Chart);

class Antigen : public wrapper<acmacs::chart::Antigen>
{
 public:
    inline Antigen(acmacs::chart::AntigenP antigen) : wrapper(antigen) {}
    static inline Rcpp::StringVector as_character(Antigen* aAntigen) { return {aAntigen->obj_->full_name()}; }
};
RCPP_EXPOSED_CLASS_NODECL(Antigen);

class Serum : public wrapper<acmacs::chart::Serum>
{
 public:
    inline Serum(acmacs::chart::SerumP serum) : wrapper(serum) {}
    static inline Rcpp::StringVector as_character(Serum* aSerum) { return {aSerum->obj_->full_name()}; }
};
RCPP_EXPOSED_CLASS_NODECL(Serum);

RCPP_EXPOSED_CLASS_NODECL(acmacs::chart::Passage);
inline Rcpp::StringVector passage_as_character(acmacs::chart::Passage* aPassage) { return {aPassage->data()}; }

class Projection : public wrapper<acmacs::chart::ProjectionModify>
{
 public:
    inline Projection(acmacs::chart::ProjectionModifyP projection) : wrapper(projection) {}
      // static inline Rcpp::StringVector as_character(Projection* aProjection) { return {aProjection->obj_->full_name()}; }

    inline std::string make_info() const { return obj_->make_info(); }
    inline std::string comment() const { return obj_->comment(); }

    inline Rcpp::NumericMatrix transformation() const
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

    inline Rcpp::NumericMatrix layout() const { return layout_convert(obj_->layout()); }
    inline Rcpp::NumericMatrix transformed_layout() const { return layout_convert(obj_->transformed_layout()); }

    inline void rotate_degrees(double aDegrees) { obj_->rotate_degrees(aDegrees); }
    inline void rotate_radians(double aRadians) { obj_->rotate_radians(aRadians); }
    inline void flip_east_west() { obj_->flip_east_west(); }
    inline void flip_north_south() { obj_->flip_north_south(); }
      //inline void () { obj_->(); }

      // aPointNo is counted from 1 (as in R vector)
    inline void move_point(size_t aPointNo, const Rcpp::NumericVector& aCoordinates)
        {
            if (aPointNo < 1 || aPointNo > obj_->number_of_points())
                throw std::invalid_argument("invalid point number");
            if (aCoordinates.size() != obj_->number_of_dimensions())
                throw std::invalid_argument("invalid vector size (number of point coordinates)");
            obj_->move_point(aPointNo - 1, {aCoordinates.begin(), aCoordinates.end()});
        }

    inline double stress() const { return obj_->stress(); }

    inline std::string minimum_column_basis() const { return obj_->minimum_column_basis(); }

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

class PlotSpec : public wrapper<acmacs::chart::PlotSpecModify>
{
 public:
    inline PlotSpec(acmacs::chart::PlotSpecModifyP plot_spec) : wrapper(plot_spec) {}

    inline Rcpp::List styles() const { const auto styles = obj_->all_styles(); return {styles.begin(), styles.end()}; }

    inline Rcpp::IntegerVector drawing_order() const
        {
            const auto drawing_order = obj_->drawing_order();
            Rcpp::IntegerVector result(drawing_order.size());
            std::transform(drawing_order.begin(), drawing_order.end(), result.begin(), [](auto index) { return index + 1; });
            return result;
            // return {drawing_order.begin(), drawing_order.end()};
        }

    inline void drawing_order_raise(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->raise(indexes);
        }
    inline void drawing_order_lower(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->lower(indexes);
        }
    inline void drawing_order_raise_sera(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->raise_serum(indexes);
        }
    inline void drawing_order_lower_sera(const Rcpp::IntegerVector& aIndexes)
        {
            acmacs::chart::Indexes indexes(aIndexes.size());
            std::transform(aIndexes.begin(), aIndexes.end(), indexes.begin(), [](auto index) { return index - 1; });
            obj_->lower_serum(indexes);
        }

    inline void set_style_size(const Rcpp::IntegerVector& aIndexes, double aSize)
        {
            for (auto index: aIndexes)
                obj_->size(index - 1, Pixels{aSize});
        }
    inline void set_style_fill(const Rcpp::IntegerVector& aIndexes, std::string aFill)
        {
            for (auto index: aIndexes)
                obj_->fill(index - 1, aFill);
        }
    inline void set_style_outline(const Rcpp::IntegerVector& aIndexes, std::string aOutline)
        {
            for (auto index: aIndexes)
                obj_->outline(index - 1, aOutline);
        }
};
RCPP_EXPOSED_CLASS_NODECL(PlotSpec);

inline PlotSpec Chart::plot_spec() { return obj_->plot_spec_modify(); }

RCPP_EXPOSED_CLASS_NODECL(acmacs::PointStyle);
inline auto style_shown(acmacs::PointStyle* style) { return *style->shown; }
inline auto style_size(acmacs::PointStyle* style) { return style->size->value(); }
inline std::string style_fill(acmacs::PointStyle* style) { return *style->fill; }
inline std::string style_outline(acmacs::PointStyle* style) { return *style->outline; }
inline auto style_outline_width(acmacs::PointStyle* style) { return style->outline_width->value(); }
inline auto style_rotation(acmacs::PointStyle* style) { return style->rotation->value(); }
inline auto style_aspect(acmacs::PointStyle* style) { return style->aspect->value(); }
inline std::string style_shape(acmacs::PointStyle* style) { return *style->shape; }

class Titers : public wrapper<acmacs::chart::TitersModify>
{
 public:
    inline Titers(acmacs::chart::TitersModifyP titers) : wrapper(titers) {}
    inline std::string titer(size_t ag_no, size_t sr_no) const { return obj_->titer(ag_no - 1, sr_no - 1); }
    inline Rcpp::StringMatrix all() const
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

// ----------------------------------------------------------------------

RCPP_MODULE(acmacs)
{
    using namespace Rcpp;

    class_<Chart>("acmacs.Chart")
            .constructor<std::string>("read chart data from a file", &Chart::validate_constructor<std::string>)
            .constructor<Rcpp::RawVector>("read chart from raw inline data", &Chart::validate_constructor<Rcpp::RawVector>)
            .property<size_t>("number_of_antigens", &Chart::number_of_antigens)
            .property<size_t>("number_of_sera", &Chart::number_of_sera)
            .property<size_t>("number_of_points", &Chart::number_of_points)
            .property<size_t>("number_of_projections", &Chart::number_of_projections)
            .property<std::string>("lineage", &Chart::lineage)
            .property<std::string>("info", &Chart::info, "multi-line string brifly describing data stored in the chart")
            .property<std::string>("name", &Chart::name)
            .property<Rcpp::List>("antigens", &Chart::getList<Antigen, &acmacs::chart::ChartModify::antigens>)
            .property<Rcpp::List>("sera", &Chart::getList<Serum, &acmacs::chart::ChartModify::sera>)
            .property<Rcpp::List>("projections", &Chart::getListViaAt<Projection, &acmacs::chart::ChartModify::projections_modify>)
            .property<PlotSpec>("plot_spec", &Chart::plot_spec)
            .property<Titers>("titers", &Chart::titers)
            .method("column_bases", &Chart::column_bases_2, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            .method("column_bases", &Chart::column_bases_1, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            // .method("column_bases", &Chart::column_bases_1s, "either forced or calculated column bases with the provided minimum_column_basis")
            .method("column_bases", &Chart::column_bases_0, "either forced or calculated column bases, i.e. the ones used for stress calculation and optimization")
            .method("save", &Chart::save)
            ;
    function("as.character.Rcpp_acmacs.Chart", &Chart::as_character);

    class_<Antigen>("acmacs.Antigen")
            .property<std::string>("name", &Antigen::getT<std::string, &acmacs::chart::Antigen::name>)
            .property<std::string>("full_name", &Antigen::getT<std::string, &acmacs::chart::Antigen::full_name>)
            .property<std::string>("abbreviated_name", &Antigen::getT<std::string, &acmacs::chart::Antigen::abbreviated_name>)
            .property<std::string>("date", &Antigen::getT<std::string, &acmacs::chart::Antigen::date>)
            .property<acmacs::chart::Passage>("passage", &Antigen::getT<acmacs::chart::Passage, &acmacs::chart::Antigen::passage>)
            .property<std::string>("lineage", &Antigen::getT<std::string, &acmacs::chart::Antigen::lineage>)
            .property<std::string>("reassortant", &Antigen::getT<std::string, &acmacs::chart::Antigen::reassortant>)
            .property<bool>("reference", &Antigen::get<&acmacs::chart::Antigen::reference>)
            .property<Rcpp::StringVector>("lab_ids", &Antigen::getSV<&acmacs::chart::Antigen::lab_ids>)
            .property<Rcpp::StringVector>("annotations", &Antigen::getSV<&acmacs::chart::Antigen::annotations>)
            ;
    function("as.character.Rcpp_acmacs.Antigen", &Antigen::as_character);

    class_<Serum>("acmacs.Serum")
            .property<std::string>("name", &Serum::getT<std::string, &acmacs::chart::Serum::name>)
            .property<std::string>("full_name", &Serum::getT<std::string, &acmacs::chart::Serum::full_name>)
            .property<std::string>("abbreviated_name", &Serum::getT<std::string, &acmacs::chart::Serum::abbreviated_name>)
            .property<std::string>("passage", &Serum::getT<std::string, &acmacs::chart::Serum::passage>)
            .property<std::string>("lineage", &Serum::getT<std::string, &acmacs::chart::Serum::lineage>)
            .property<std::string>("reassortant", &Serum::getT<std::string, &acmacs::chart::Serum::reassortant>)
            .property<Rcpp::StringVector>("annotations", &Serum::getSV<&acmacs::chart::Serum::annotations>)
            .property<std::string>("serum_id", &Serum::getT<std::string, &acmacs::chart::Serum::serum_id>)
            .property<std::string>("serum_species", &Serum::getT<std::string, &acmacs::chart::Serum::serum_species>)
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
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
