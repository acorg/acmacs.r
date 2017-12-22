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

  \item{forced_column_bases}{vector of doubles, if column bases were forced in projection
  (by user or after merge of tables having more-than titers), otherwise
  NA.}

  \item{transformation}{transformation matrix of the projection (matrix
    2x2)}

  \item{layout}{matrix with point coordinates.}

  \item{transformed_layout}{matrix with point coordinates,
  transformation applied.}

  \item{rotate_degrees(degrees)}{modifies projection transformation
    matrix so transformed_layout would be rotated passed number of
    degrees (double) counter-clockwise. To rotate clockwise pass
    negative value.}

  \item{rotate_radians(radians)}{modifies projection transformation
    matrix so transformed_layout would be rotated passed number of
    radians (double) counter-clockwise. To rotate clockwise pass
    negative value.}

  \item{flip_east_west()}{modifies projection transformation
    matrix so transformed_layout would be flipped across vertical line.}

  \item{flip_north_south()}{modifies projection transformation
    matrix so transformed_layout would be flipped across horizontal line.}

  \item{move_point(point_no, coodinates)}{modifies projection
    non-transformed layout by changing point_no (positive integer)
    coordinates (numeric vector). The number of the first point is 1.}

}
\examples{
  # load chart from file
  chart <- new(acmacs.Chart, "/tmp/table.save")
  # print information about projections
  prj <- chart$projections[[0]]
}
\seealso{
 \code{\link{acmacs.Chart}}
 \code{\link{acmacs.Example}}
}
\keyword{ package }
\author{Eugene Skepner, eu@projectionic-cartography.org.}
