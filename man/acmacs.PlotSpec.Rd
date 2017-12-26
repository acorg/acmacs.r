\name{acmacs.PlotSpec}
\title{Class to access and modify plot specification of the chart.}
\alias{acmacs.PlotSpec-class}
\alias{acmacs.PlotSpec}
\docType{package}
\usage{
ps <- chart$plot_spec
}
\value{An object of class \code{"acmacs.PlotSpec"} to access and modify
  plot specification of the chart. It has the following properties and methods:

  \item{styles}{List of point styles (see below).}

  \item{drawing_order}{Vector of point indexes in the order of
  drawing. Points with indexes that are last in the vector appear on top of other points.}

  \item{raise(point_indexes)}{Raise points by their indexes.}

  \item{lower(point_indexes)}{Lower points by their indexes.}

  \item{raise_sera(serum_indexes)}{Raise sera by their indexes.}

  \item{lower_sera(serum_indexes)}{Lower sera by their indexes.}

  \item{set_size(point_indexes, size)}{Resize points.}

  \item{set_fill(point_indexes, fill)}{Change fill color for points.}

  \item{set_outline(point_indexes, outline)}{Change outline color for points.}

}
\section{PointStyle}{
  chart$plot_spec$style is a list of acmacs.PointStyle objects. Those
  objects have the folling properties:

  \code{shown} - if point is shown (boolean)

  \code{size} - point size (in pixels, if drawn by acmacs engine)

  \code{fill} - point fill color (string)

  \code{outline} - point outline color (string)

  \code{outline_width} - point outline width (in pixels, if drawn by acmacs engine)

  \code{rotation} - point shape rotation (double, radians)

  \code{aspect} - point shape aspect ratio (double)

  \code{shape} - point shape (string): "BOX", "CIRCLE", "TRIANGLE"

}
\keyword{ package }
\examples{
# raise reference antigens
ref_indexes <- 1:chart$number_of_antigens
ref_indexes <- ref_indexes[lapply(ref_indexes, function(no) { chart$antigens[[no]]$reference})==TRUE]
chart$plot_spec$raise(ref_indexes)
chart$plot_spec$drawing_order
}
\seealso{
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.Example}}
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
