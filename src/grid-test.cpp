#include "acmacs-chart-2/grid-test.hh"

#include "grid-test.hh"

// ----------------------------------------------------------------------

GridTest::GridTest(Chart& chart, size_t projection_no, double grid_step) :
    grid_test_(new acmacs::chart::GridTest(*chart.obj_, projection_no, grid_step))
{
}

// ----------------------------------------------------------------------

Rcpp::DataFrame GridTest::test_parallel()
{
    results_ = std::make_shared<acmacs::chart::GridTest::Results>(grid_test_->test_all_parallel());
    return results();
}

// ----------------------------------------------------------------------

Rcpp::DataFrame GridTest::test_single_thread()
{
    results_ = std::make_shared<acmacs::chart::GridTest::Results>(grid_test_->test_all());
    return results();
}

// ----------------------------------------------------------------------

Projection GridTest::make_new_projection_and_relax()
{
    if (results_) {
        if (results_->count_trapped_hemisphering() == 0)
            throw std::runtime_error("test found neither trapped nor hemisphering points, nothing to do");
        auto projection = grid_test_->make_new_projection_and_relax(*results_, false);
        results_.reset();
        grid_test_->reset(projection);
        return projection;
    }
    else
        throw std::runtime_error("run test first");
}

// ----------------------------------------------------------------------

Rcpp::DataFrame GridTest::results()
{
    if (results_) {
        const auto size = results_->count_trapped_hemisphering();
        const auto num_dims = results_->num_dimensions();
        Rcpp::NumericVector v_point_no(size);
        Rcpp::StringVector v_diagnosis(size);
        Rcpp::NumericVector v_pos_x(size);
        Rcpp::NumericVector v_pos_y(size);
        Rcpp::NumericVector v_pos_z(size);
        Rcpp::NumericVector v_distance(size);
        // Rcpp::NumericVector v_contribution_diff(size);

        size_t index = 0;
        for (const auto& result : *results_) {
            if (result) {
                v_point_no[index] = result.point_no + 1;
                v_diagnosis[index] = result.diagnosis_str();
                v_pos_x[index] = result.pos.x();
                v_pos_y[index] = result.pos.y();
                if (num_dims > acmacs::number_of_dimensions_t{2})
                    v_pos_z[index] = result.pos.z();
                v_distance[index] = result.distance;
                // v_contribution_diff[index] = result.contribution_diff;
                ++index;
            }
        }
        switch (*num_dims) {
            case 0:
                return Rcpp::DataFrame::create(Rcpp::Named("point_no") = v_point_no);
            case 1:
                return Rcpp::DataFrame::create(Rcpp::Named("point_no") = v_point_no, Rcpp::Named("diagnosis") = v_diagnosis, Rcpp::Named("pos_x") = v_pos_x,
                                               Rcpp::Named("distance") = v_distance); //, Rcpp::Named("contribution_diff") = v_contribution_diff);
            case 2:
                return Rcpp::DataFrame::create(Rcpp::Named("point_no") = v_point_no, Rcpp::Named("diagnosis") = v_diagnosis, Rcpp::Named("pos_x") = v_pos_x, Rcpp::Named("pos_y") = v_pos_y,
                                               Rcpp::Named("distance") = v_distance); //, Rcpp::Named("contribution_diff") = v_contribution_diff);
            default:
                return Rcpp::DataFrame::create(Rcpp::Named("point_no") = v_point_no, Rcpp::Named("diagnosis") = v_diagnosis, Rcpp::Named("pos_x") = v_pos_x, Rcpp::Named("pos_y") = v_pos_y,
                                               Rcpp::Named("pos_z") = v_pos_z,
                                               Rcpp::Named("distance") = v_distance); //, Rcpp::Named("contribution_diff") = v_contribution_diff);
        }
    }
    else {
        Rcpp::NumericVector nv;
        return Rcpp::DataFrame::create(Rcpp::Named("point_no") = nv);
    }
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
