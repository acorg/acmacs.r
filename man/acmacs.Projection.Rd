\name{acmacs.Projection}
\title{Class to access projection related data.}
\alias{acmacs.Projection-class}
\alias{acmacs.Projection}
\docType{package}
\usage{
projection <- chart$projections[[projection_number]]
projection$info
projection$stress
}
\value{An object of class \code{"acmacs.Projection"} to access and modify
  projection data. It has the following properties:

  \item{info}{brief description the projection (string), includes
  stress, dimensionality, minimum column basis, forced column bases.}

  \item{stress}{stress (double). may be absent in lispmds case or for
  modified projections.}

  \item{comment}{projection comment field}

  \item{minimum_column_basis}{(string), "none", "1280"}

}
\examples{
  # load chart from file
  chart <- new(acmacs.Chart, "/tmp/table.save")
  # print information about projections
  prj <- chart$projections[[0]]
}
\seealso{
 \code{\link{acmacs.Chart}}
}
\keyword{ package }
\author{Eugene Skepner, eu@projectionic-cartography.org.}
