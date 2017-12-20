#include <typeinfo>
#include <Rcpp.h>

#include "acmacs-chart-2/chart.hh"
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

class Chart : public wrapper<acmacs::chart::Chart>
{
 public:
    inline Chart(std::string aFilename) : wrapper(acmacs::chart::import_factory(aFilename, acmacs::chart::Verify::None, report_time::No)) {}
    inline std::string name() const { return obj_->make_name(); }
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

class Projection : public wrapper<acmacs::chart::Projection>
{
 public:
    inline Projection(acmacs::chart::ProjectionP projection) : wrapper(projection) {}
    // static inline Rcpp::StringVector as_character(Projection* aProjection) { return {aProjection->obj_->full_name()}; }
};
RCPP_EXPOSED_CLASS_NODECL(Projection);

// ----------------------------------------------------------------------

RCPP_MODULE(acmacs)
{
    using namespace Rcpp;

    class_<Chart>("acmacs.Chart")
            .constructor<std::string>("read chart data from a file")
            .property<size_t>("number_of_antigens", &Chart::get<&acmacs::chart::Chart::number_of_antigens>, "number_of_antigens")
            .property<size_t>("number_of_sera", &Chart::get<&acmacs::chart::Chart::number_of_sera>, "number_of_sera")
            .property<size_t>("number_of_points", &Chart::get<&acmacs::chart::Chart::number_of_points>)
            .property<size_t>("number_of_projections", &Chart::get<&acmacs::chart::Chart::number_of_projections>)
            .property<std::string>("lineage", &Chart::get<&acmacs::chart::Chart::lineage>)
            .property<std::string>("info", &Chart::get<&acmacs::chart::Chart::make_info>, "multi-line string brifly describing data stored in the chart")
            .property<std::string>("name", &Chart::name)
            .property<Rcpp::List>("antigens", &Chart::getList<Antigen, &acmacs::chart::Chart::antigens>)
            .property<Rcpp::List>("sera", &Chart::getList<Serum, &acmacs::chart::Chart::sera>)
            .property<Rcpp::List>("projections", &Chart::getList<Projection, &acmacs::chart::Chart::projections>)
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
            .property<std::string>("info", &Projection::get<&acmacs::chart::Projection::make_info>)
            .property<double>("stress", &Projection::get<&acmacs::chart::Projection::stress>)
            .property<std::string>("comment", &Projection::get<&acmacs::chart::Projection::comment>)
            .property<std::string>("minimum_column_basis", &Projection::getT<std::string, &acmacs::chart::Projection::minimum_column_basis>)
        // virtual std::shared_ptr<Layout> layout() const = 0;
        // virtual inline std::shared_ptr<Layout> transformed_layout() const { return std::shared_ptr<Layout>(layout()->transform(transformation())); }
        // virtual std::shared_ptr<ColumnBases> forced_column_bases() const = 0;
        // virtual acmacs::Transformation transformation() const = 0;
              // rotate
              // flip
              // move points
            ;
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
