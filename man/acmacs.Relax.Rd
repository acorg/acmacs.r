\name{acmacs.Relax}
\title{Functions to make antigenic maps.}
\alias{acmacs.Relax}
\alias{acmacs.relax}
\alias{acmacs.make_maps}
\docType{package}
\description{
  Antigenic maps can be made in the following ways
  \itemize{
    \item{via chart object: chart$relax() and chart$relax_many()}
    \item{via projection object: projection$relax(),
      projection$relax_one_iteration()}
    \item{using custom optimizer}
  }
}


\section{Making maps via chart object}{
% \preformatted{
      # make new projection in 2d without minimum column basis with random layout and relax it
    new_projection <- chart$relax("none", 2)

      # make new projection in 2d without minimum column basis with random
      # layout and fixed random generator seed, and relax it, use fine optimization
    new_projection <- chart$relax("none", 2, FALSE, 1234)

      # make new projection in 2d without minimum column basis with random
      # layout and relax it, use rough optimization which is 30\% faster
    new_projection <- chart$relax("none", 2, TRUE)

      # make 10 new projections in 2d without minimum column basis with random layout and
      # relax them using fine optimization, relaxation is perfomed in parallel, if multiple CPU cores available
    chart$relax_many("none", 2, 10, FALSE)

  \subsection{relax(number_of_dimensions,\cr option ...)}{creates new
    projection with random layout and optimizes it.\cr
  \bold{number_of_dimensions} is a number,  e.g. 2 or 3.\cr
    Option is a string, up to 9 options can be specified, the following options are supported:\cr
  "rough" - use faster rough optimization\cr
  "fine" - use fine optimization (default)\cr
  "optimization-cg" - use cg optimization method (default)\cr
  "optimization-lbfgs" - use lbfgs optimization method\cr
  "disconnect-having-too-few-titers" - disconnect antigens/sera that have too few numeric titers (default)\cr
  "no-disconnect-having-too-few-titers"\cr
  "dimension-annealing"\cr
  "no-dimension-annealing" (default)\cr
  "none" - no minimum column basis (default)\cr
  "1280" - minimum column basis 1280}

  \subsection{relax_many(number_of_dimensions,\cr  number_of_optimizations,\cr
  option ...)}{creates new projection with random}

  \subsection{relax_incremental(number_of_optimizations,\cr option
  ...)}{relax multiple times upon calling acmacs.merge.2(). Only
  points having NaN coordinates (i.e. points found in the second
  source chart of the merge) randomized.\cr
  Option is a string, up to 5
  options may be specified, the following options are supported:\cr
  "rough" - use faster rough optimization\cr
  "fine" - use fine optimization (default)\cr
  "optimization-cg" - use cg optimization method (default)\cr
  "optimization-lbfgs" - use lbfgs optimization method\cr
  "disconnect-having-too-few-titers" - disconnect antigens/sera that have too few numeric titers (default)\cr
  "no-disconnect-having-too-few-titers"\cr
  "no-unmovable-primary-points" - relax all antigens and sera in the merge (default)\cr
  "unmovable-primary-points" - freeze antigens/sera found in the primary chart\cr
  }

}


\section{Making maps via chart object (legacy interface)}{

  \subsection{relax(minimum_column_basis, number_of_dimensions)}{creates new
  projection with random layout and optimizes it.\cr \bold{minimum_column_basis}
  is a string, e.g. "none" or "1280".\cr \bold{number_of_dimensions} is a number,
  e.g. 2 or 3.}

  \subsection{relax(minimum_column_basis,\cr number_of_dimensions,\cr
  rough)}{creates new projection with random layout and optimizes
  it.\cr \bold{minimum_column_basis} is a string, e.g. "none" or
  "1280".\cr \bold{number_of_dimensions} is a number, e.g. 2 or 3.\cr \bold{rough} is
  boolean, when TRUE optimization is about 30\% faster, result is very
  slightly worse, visually no distinguishable.}

  \subsection{relax(minimum_column_basis,\cr number_of_dimensions,\cr
  rough, \cr seed)}{creates new projection with random layout, seed
  (integer) is used for randomization, and optimizes
  it.\cr \bold{minimum_column_basis} is a string, e.g. "none" or
  "1280".\cr \bold{number_of_dimensions} is a number, e.g. 2 or 3.\cr \bold{rough} is
  boolean, when TRUE optimization is about 30\% faster, result is very
  slightly worse, visually no distinguishable.\cr \bold{seed} is an
  integer to seed the randomization.}

  \subsection{relax_many(minimum_column_basis,\cr number_of_dimensions,\cr
  number_of_optimizations,\cr rough)}{creates new projection with random
  layout and optimizes it.\cr \bold{minimum_column_basis} is a string,
  e.g. "none" or "1280".\cr \bold{number_of_dimensions} is a number,
  e.g. 2 or 3.\cr \bold{number_of_optimizations} is a number, e.g. 10 or
  100, optimization is performed in parallel if multiple CPU cores
  available.\cr \bold{rough} is boolean, when TRUE optimization is about
  30\% faster, result is slightly worse, visually no distinguishable.}

}

\section{Making maps via projection object}{
\preformatted{
      # relax existing projection, e.g. upon moving the first antigen
    chart$projection(1)$relax()

      # create new projection in 2d without minimum column basis with random layout without optimisation (dimension annealing cannot be used)
    projection <- chart$new_projection("none", 2)

      # create new projection in 2d without minimum column basis and with some layout (dimension annealing cannot be used)
    projection <- chart$new_projection_with_layout("none", matrix(1:((chart$number_of_antigens + chart$number_of_sera)*2), ncol=2, byrow=TRUE))

      # randomize layout in the projection using "table-max-distance" method
    projection$randomize_layout()

    # perform optimization step by step
    # relax_one_iteration() - performs one iteration of optimization.
    # returns if further optimization steps available.
    while(projection$relax_one_iteration()) { print(projection$stress); print(projection$layout); }

    # relax_one_iteration(method, rough) - performs one iteration of
    # optimization layout, returns if further optimization steps
    # available. Values of method and rough are used in
    # the first call only.
    projection$relax_one_iteration("cg", FALSE) 
}
}

\section{Relaxing with manually calling dimension annealing}{
\preformatted{
    chart <- new(acmacs.Chart, filename)
    chart$remove_all_projections()
    projection <- chart$new_projection("none", 5) # initially make 5D projection
    projection$relax()
    projection$dimension_annealing(2)  # convert relaxed projection to 2D
    projection$relax()  # relax in 2D
}
}

\section{Relaxing with unmovable points}{
\preformatted{
    chart <- new(acmacs.Chart, filename)
    chart$remove_all_projections()
    projection <- chart$new_projection("none", 2)
    projection$set_unmovable(1:10) # first 10 points are unmovable
    projection$relax()

      # some points can be disconnected, i.e. they do not move and do not contribute to stress
    projection$set_disconnected(1:10) # first 10 points are disconnected

      # some points can be made unmovable is the last dimension
      # e.g. fix antigens in 2D and allow sera to be moved in 3D
    projection$set_unmovable_in_the_last_dimension(1:chart$number_of_antigens)
}
}

\section{Making maps in a custom way}{
\preformatted{
      # obtaining stress object from a chart
    stress <- chart$stress_evaluator(number_of_dimensions, minimum_column_basis)

      # creating stress object for given distance matrix (regular distances only)
    stress <- acmacs.stress_from_distances(matrix(...), number_of_dimensions)

      # calculating stress value for given layout
    value <- stress$value(matrix(...))

      # calculating stress contibution for a point, point_no is either antigen_no or (serum_no + number_of_antigens)
    value <- stress$contribution(point_no, matrix(...))

    calculating stress gradient for given layout
    gradient_vector <- stress$gradient(matrix(...))
}
}

\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}\cr
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.GridTest}}\cr
  \code{\link{acmacs.Example}}\cr
  \code{\link{acmacs.merge}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
