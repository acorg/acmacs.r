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

  \item{number_of_dimensions}{(integer)}

  \item{forced_column_bases}{vector of doubles, if column bases were forced in projection
  (by user or after merge of tables having more-than titers), otherwise
  NA.}

  \item{transformation}{transformation matrix of the projection (matrix
    2x2 for 2D projection, matrix 3x3 for 3D projection)}

  \item{transformation <- matrix(c(1, 0, 0, 1), nrow=2, byrow=TRUE)}{set
    transformation matrix for 2D projection}

  \item{transformation <- matrix(c(1, 0, 0, 0, 1, 0, 0, 0, 1), nrow=3, byrow=TRUE)}{set
    transformation matrix for 3D projection}

  \item{layout}{matrix with point coordinates.}

  \item{layout <- matrix(c(0), nrow=chart$number_of_points,
  ncol=projection$number_of_dimensions)}{set layout to matrix.}

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

  \item{randomize_layout()}{randomizes layout using table-max-distance randomizer and diameter_multiplier=2.0}

  \item{randomize_layout(randomization_method,
  diameter_multiplier)}{randomizes layout\cr
  \bold{randomization_method} is a string: "sample-optimization"
  (layout is randomzed using "table-max-distance", then relaxed to
  find out resulting point area, diameter of that area used for the
  final randomization), "table-max-distance" (randomization diameter
  is based on the maximum distance between antigen and serum in the
  table), "current-layout-area" (current layout area is used to infer
  randomization diameter)\cr \bold{diameter_multiplier} is a number to
  multiply inferred diameter for "sample-optimization" and
  "current-layout-area" }

  \item{relax()}{relaxes projection from the current layout}

  \item{relax(method, rough)}{relaxes projection from the current
  layout\cr\bold{method} is a string: "cg" or "lbfgs"\cr\bold{rough} is boolean}

  \item{relax_one_iteration()}{performs one iteration of optimization. returns if further optimization steps available.}

  \item{relax_one_iteration(method, rough)}{performs one iteration of
  optimization layout, returns if further optimization steps
  available.\cr\bold{method} is a string: "cg" or
  "lbfgs"\cr\bold{rough} is boolean\cr method and rough are used in
  the first call only.}

  \item{reorient(master_projection)}{reorients this projection to match
  master_projection (of the same or different chart) orientation.}

  \item{reorient(master_projection, "auto", "all")}{reorients this projection to match
  master_projection (of the same or different chart) orientation. The
  second argument is passage and serum_id matching ("auto", "strict",
  "relaxed", "ignore"), the third argument is sibset ("all", "antigens", "sera")}

}
\examples{
  # load chart from file
  chart <- new(acmacs.Chart, path.expand("/tmp/table.save"))
  # print information about projections
  prj <- chart$projections[[0]]
}
\seealso{
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Example}}
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.GridTest}}\cr
  \code{\link{acmacs.merge}}\cr
}
\keyword{ package }
\author{Eugene Skepner, eu@projectionic-cartography.org.}
