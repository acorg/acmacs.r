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

  \subsection{relax(number_of_dimensions, [seed,] option ...)}{

    Creates new projection with random layout and optimizes it.

    \code{seed} is a number to seed rundom number generator. If not provided,
    hardware random device is used for seeding.

    \code{option} is a string, up to 9 options can be specified.

    \tabular{ll}{
      "fine" \tab use fine optimization (default) \cr
      "rough" \tab use faster rough optimization \cr
      "none" \tab no minimum column basis (default) \cr
      "1280" \tab minimum column basis 1280 \cr
      "optimization-cg" \tab use cg optimization method (default)\cr
      "optimization-lbfgs" \tab use lbfgs optimization method\cr
      "disconnect-having-too-few-titers" \tab disconnect antigens/sera that have too few numeric titers (default)\cr
      "no-disconnect-having-too-few-titers" \tab \cr
      "no-dimension-annealing" \tab (default)\cr
      "dimension-annealing"\cr
    }

    Example:

    \code{
      chart$relax(2, "fine", "1280") \cr
      chart$relax(2, "fine", "1280") \cr
      chart$relax(2, runif(1, 0, 100000000), "fine", "1280")
    }

  }

  \subsection{relax_many(number_of_dimensions, number_of_optimizations, option ...)}{

    Creates new projection with random layout and optimizes it in
    multiple threads in parallel.

    Seeding cannot give repeatable results due to asynchronos nature of
    accessing random number generator by different threads and it is
    therefore not supported.

    Set of options is the same as for relax() above.
  }

  \subsection{relax_incremental(number_of_optimizations, option ...)}{

    Relaxes multiple times upon calling acmacs.merge.2(). Only
    points having \code{NaN} coordinates, i.e. points found in the second
    source chart of the merge, are randomized.

    \code{option} is a string, up to 5 options can be specified.

    \tabular{ll}{
      "fine" \tab use fine optimization (default) \cr
      "rough" \tab use faster rough optimization \cr
      "optimization-cg" \tab use cg optimization method (default)\cr
      "optimization-lbfgs" \tab use lbfgs optimization method\cr
      "disconnect-having-too-few-titers" \tab disconnect antigens/sera that have too few numeric titers (default)\cr
      "no-disconnect-having-too-few-titers" \tab \cr
      "no-unmovable-primary-points" \tab relax all antigens and sera in the merge (default) \cr
      "unmovable-primary-points" \tab freeze antigens/sera found in the primary chart \cr
    }
  }
}


\section{Making maps via chart object (legacy interface)}{

  In the legacy interface dimension annealing is always enabled.

  \subsection{relax(minimum_column_basis, number_of_dimensions)}{

    creates new projection with random layout and optimizes it.

    \code{minimum_column_basis} is a string, e.g. "none" or "1280".

    \code{number_of_dimensions} is a number, e.g. 2 or 3.

    Example:

    \code{
      new_projection <- chart$relax("none", 2) \cr
      new_projection <- chart$relax("1280", 3)
    }
  }

  \subsection{relax(minimum_column_basis, number_of_dimensions, rough)}{

    Creates new projection with random layout and optimizes it.

    \code{minimum_column_basis} is a string, e.g. "none" or
    "1280".

    \code{number_of_dimensions} is a number, e.g. 2 or 3.

    \code{rough} is boolean, when TRUE optimization is about 30\% faster, result is very
    slightly worse, visually no distinguishable.

    Example:

    \code{
      new_projection <- chart$relax("none", 2, TRUE)
    }
  }

  \subsection{relax(minimum_column_basis, number_of_dimensions, rough, seed)}{

    Creates new projection with random layout, seed
    (integer) is used for randomization, and optimizes
    it.

    \code{minimum_column_basis} is a string, e.g. "none" or
    "1280".

    \code{number_of_dimensions} is a number, e.g. 2 or 3.

    \code{rough} is boolean, when TRUE optimization is about 30\% faster, result is very
    slightly worse, visually no distinguishable.

    \code{seed} is an integer to seed the randomization.

    Example:

    \code{
      new_projection <- chart$relax("none", 2, FALSE, 1234)
      new_projection <- chart$relax("none", 2, FALSE, runif(1, 0, 100000000))
    }
  }

  \subsection{relax_many(minimum_column_basis, number_of_dimensions,
    number_of_optimizations, rough)}{

    creates new projection with random
    layout and optimizes it.

    \code{minimum_column_basis} is a string, e.g. "none" or "1280".

    \code{number_of_dimensions} is a number, e.g. 2 or 3.

    \code{number_of_optimizations} is a number, e.g. 10 or 100,
    optimization is performed in parallel if multiple CPU cores
    available.

    \code{rough} is boolean, when TRUE optimization is about 30\%
    faster, result is slightly worse, visually no distinguishable.

    Example:

    \code{
      chart$relax_many("none", 2, 10, FALSE)
    }
  }

}

\section{Making maps via projection object}{

  \subsection{chart$projection(1)$relax()}{
    Relaxes existing projection, e.g. upon moving the first antigen.
  }

  \subsection{Create new projection, randomize layout and relax in
    separate steps}{
    \preformatted{
      projection <- chart$new_projection("none", 2)
      projection$randomize_layout()
      while(projection$relax_one_iteration()) {
        print(projection$stress);
        print(projection$layout);
      }
    }
  }

  \subsection{Create new projection with some layout and relax in
    separate steps}{
    \preformatted{
      projection <- chart$new_projection("none", matrix(1:((chart$number_of_antigens + chart$number_of_sera)*2), ncol=2, byrow=TRUE))
      while(projection$relax_one_iteration()) {
        print(projection$stress);
        print(projection$layout);
      }
    }

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
