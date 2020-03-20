#pragma once

#include "acmacs-chart-2/grid-test.hh"
#include "chart.hh"

// ----------------------------------------------------------------------

namespace acmacs::chart
{
    class GridTest;
}

class GridTest
{
 public:
    GridTest(Chart& chart, size_t projection_no = 0, double grid_step = 0.1);
    GridTest(Chart& chart, double grid_step) : GridTest(chart, 0, grid_step) {}
    Rcpp::DataFrame test_parallel();
    Rcpp::DataFrame test_single_thread();
    Rcpp::DataFrame results();
    Projection make_new_projection_and_relax();

 private:
    std::shared_ptr<acmacs::chart::GridTest> grid_test_;
    std::shared_ptr<acmacs::chart::GridTest::Results> results_;

}; // GridTest
RCPP_EXPOSED_CLASS_NODECL(GridTest);

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
