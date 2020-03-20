\name{acmacs.GridTest}
\title{Chart grid test.}
\alias{acmacs.GridTest}
\docType{package}
\usage{
grid_test <- new(acmacs.GridTest, chart)
grid_test <- new(acmacs.GridTest, chart, 1) - for the second best projection
grid_test <- new(acmacs.GridTest, chart, 0.25) - with step 0.25
grid_test <- new(acmacs.GridTest, chart, 1, 0.25) - for the second best projection with step 0.25
result <- grid_test$test() - multithreaded (omp) version
result <- grid_test$test_single_thread() - single thread version (if multithreaded one crashes R)
new_projection <- grid_test$make_new_projection_and_relax()
new_result <- grid_test$test()
}
\value{An object of class \code{"acmacs.GridTest"} to perform grid test
  and apply its results. Grid test is similar to the
  hemisphering/local/grid test of acmacs-web. It tries to find better
  positions for all points of the best (or any other) projection of the
  chart.

  \item{test()}{perform test and return results (data frame)}

  \item{make_new_projection_and_relax()}{apply test results: make new
  projection with the better point positions and relax it.}
}
\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}\cr
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.Relax}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.Example}}\cr
  \code{\link{acmacs.merge}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
