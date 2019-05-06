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
  make new projection in 2d without minimum column basis with random layout and relax it\cr
  \bold{new_projection <- chart$relax("none", 2)}

  make new projection in 2d without minimum column basis with random
  layout and relax it, use rough optimization which is 30\% faster\cr
  \bold{new_projection <- chart$relax("none", 2, TRUE)}

  make 10 new projections in 2d without minimum column basis with random layout and relax them using fine optimization, relaxation is perfomed in parallel, if multiple CPU cores available\cr
  \bold{chart$relax_many("none", 2, 10, FALSE)}
}


\section{Making maps via projection object}{
  relax existing projection, e.g. upon moving the first antigen\cr
  \bold{chart$projection(1)$relax()}

  create new projection in 2d without minimum column basis with random layout without optimisation (dimension annealing cannot be used)\cr
  \bold{projection <- chart$new_projection("none", 2)}\cr
  randomize layout in the projection using "table-max-distance" method\cr
  \bold{projection$randomize_layout()}\cr
  perform optimization step by step\cr
  \bold{while(projection$relax_one_iteration()) { print(projection$stress); print(projection$layout); }}
}


\section{Making maps in a custom way}{
  obtaining stress object from a chart\cr
  \bold{stress <- chart$stress_evaluator(number_of_dimensions,
    minimum_column_basis)}

  creating stress object for given distance matrix (regular distances only)\cr
  \bold{stress <- acmacs.stress_from_distances(matrix(...), number_of_dimensions)}

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
