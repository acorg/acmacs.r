#include <typeinfo>
#include <Rcpp.h>

#include "acmacs-chart-2/chart-modify.hh"
#include "acmacs-chart-2/factory-import.hh"

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

class Chart : public wrapper<acmacs::chart::ChartModify>
{
 public:
    inline Chart(std::string aFilename) : wrapper(std::make_shared<acmacs::chart::ChartModify>(acmacs::chart::import_factory(aFilename, acmacs::chart::Verify::None, report_time::No))) {}
    inline std::string name() const { return obj_->make_name(); }
    inline std::string info() const { return obj_->make_info(); }
    inline std::string lineage() const { return obj_->lineage(); }
    inline size_t number_of_antigens() const { return obj_->number_of_antigens(); }
    inline size_t number_of_sera() const { return obj_->number_of_sera(); }
    inline size_t number_of_points() const { return obj_->number_of_points(); }
    inline size_t number_of_projections() const { return obj_->number_of_projections(); }
    static inline Rcpp::StringVector as_character(Chart* aChart) { return {aChart->name()}; }

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
            if (fcb && fcb->exists()) {
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

// ----------------------------------------------------------------------

RCPP_MODULE(acmacs)
{
    using namespace Rcpp;

    class_<Chart>("acmacs.Chart")
            .constructor<std::string>("read chart data from a file")
            .property<size_t>("number_of_antigens", &Chart::number_of_antigens)
            .property<size_t>("number_of_sera", &Chart::number_of_sera)
            .property<size_t>("number_of_points", &Chart::number_of_points)
            .property<size_t>("number_of_projections", &Chart::number_of_projections)
            .property<std::string>("lineage", &Chart::lineage)
            .property<std::string>("info", &Chart::info, "multi-line string brifly describing data stored in the chart")
            .property<std::string>("name", &Chart::name)
            .property<Rcpp::List>("antigens", &Chart::getList<Antigen, &acmacs::chart::ChartModify::antigens>, "")
            .property<Rcpp::List>("sera", &Chart::getList<Serum, &acmacs::chart::ChartModify::sera>)
            .property<Rcpp::List>("projections", &Chart::getListViaAt<Projection, &acmacs::chart::ChartModify::projections_modify>, "")
              // .method("save")
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
            .property<double>("stress", &Projection::get<&acmacs::chart::ProjectionModify::stress>)
            .property<std::string>("comment", &Projection::comment)
            .property<std::string>("minimum_column_basis", &Projection::getT<std::string, &acmacs::chart::ProjectionModify::minimum_column_basis>)
            .property("forced_column_bases", &Projection::forced_column_bases)
            .property("transformation", &Projection::transformation)
            .property("layout", &Projection::layout)
            .property("transformed_layout", &Projection::transformed_layout)
            .method("rotate_degrees", &Projection::rotate_degrees)
            .method("rotate_radians", &Projection::rotate_radians)
              // flip
              // move points
            ;
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
