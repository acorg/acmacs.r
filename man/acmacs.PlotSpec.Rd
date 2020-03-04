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

  \item{styles_as_data_frame()}{Returns all point styles as a data frame.}

  \item{set_styles(data_frame)}{Sets all point styles from data frame
  returned by styles_as_data_frame() and possibly modified by a user.}

  \item{drawing_order}{Vector of point indexes in the order of
  drawing. Points with indexes that are last in the vector appear on top of other points.}

  \item{raise(point_indexes)}{Raise points by their indexes.}

  \item{lower(point_indexes)}{Lower points by their indexes.}

  \item{raise_sera(serum_indexes)}{Raise sera by their indexes.}

  \item{lower_sera(serum_indexes)}{Lower sera by their indexes.}

  \item{set_shown(point_indexes, shown)}{Shows/hides points.}

  \item{set_size(point_indexes, size)}{Resize points.}

  \item{set_fill(point_indexes, fill)}{Change fill color for points.}

  \item{set_fill_opacity(point_indexes, opacity)}{Change fill opacity for points, opacity is in range 0.0 (transparent) to 1.0 (opaque).}

  \item{set_outline(point_indexes, outline)}{Change outline color for points.}

  \item{set_outline_opacity(point_indexes, opacity)}{Change outline opacity for points, opacity is in range 0.0 (transparent) to 1.0 (opaque).}

  \item{set_outline_width(point_indexes, outline_width)}{Change outline width for points.}

  \item{set_rotation(point_indexes, rotation)}{}

  \item{set_aspect(point_indexes, aspect)}{}

  \item{set_shape(point_indexes, shape)}{Change point shape,
  supported shapes: "circle", "egg", "triangle", "box", "uglyegg",
  invalid values are ignored.}

  \item{set_label_shown(point_indexes, shown)}{}

  \item{set_label_offset_x(point_indexes, label_offset_x)}{}

  \item{set_label_offset_y(point_indexes, label_offset_y)}{}

  \item{set_label_size(point_indexes, label_size)}{}

  \item{set_label_color(point_indexes, label_color)}{}

  \item{set_label_rotation(point_indexes, rotation)}{}

  \item{set_label_slant(point_indexes, label_slant)}{"normal" or "italic"}

  \item{set_label_weight(point_indexes, label_weight)}{"normal" or
  "bold"}

  \item{set_label_font_family(point_indexes, font_family)}{}

  \item{set_label_text(point_indexes, label_text)}{}

}
\section{PointStyle}{
  chart$plot_spec$style is a list of acmacs.PointStyle objects. Those
  objects have the folling properties:

  \code{shown} - if point is shown (boolean)

  \code{size} - point size (in pixels, if drawn by acmacs engine)

  \code{fill} - point fill color (string)

  \code{fill_opacity} - point fill color opacity (number in range 0.0 to 1.0)

  \code{outline} - point outline color (string)

  \code{outline_opacity} - point outline color opacity (number in range 0.0 to 1.0)

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
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.Relax}}\cr
  \code{\link{acmacs.Example}}\cr
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.GridTest}}\cr
  \code{\link{acmacs.merge}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
