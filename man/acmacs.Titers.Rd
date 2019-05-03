\name{acmacs.Titers}
\title{Class to access titers of the chart.}
\alias{acmacs.Titers-class}
\alias{acmacs.Titers}
\docType{package}
\usage{
ps <- chart$titers
}
\value{An object of class \code{"acmacs.Titers"} to access titers of the
  chart. It has the following properties and methods:

  \item{all()}{Dense matrix of all titers.}

  \item{titer(antigen_no, serum_no)}{Single titer for antigen and serum.}

  \item{set_titer(antigen_no, serum_no, titer)}{Set titer for antigen
  and serum. If a chart has layers, setting merged titer is not
  possible, use chart$remove_layers() to flatten the chart and be able to modify titers.}

  \item{set_dontcare_for_antigen(antigen_no)}{Set all titers for antigen
    to *.}

  \item{set_dontcare_for_serum(serum_no)}{Set all titers for serum
    to *.}

  \item{multiply_by_for_antigen(antigen_no, multiply_by)}{Multiply all
    titers of the antigen by some number.}

  \item{multiply_by_for_serum(serum_no, multiply_by)}{Multiply all
    titers of the serum by some number.}

  \item{number_of_layers}{If chart is a merge, it has layers. In that
    case titers cannot be modified. Use chart$remove_layers() to get rid
    of them and be able to modify titers.}

  \item{titer_of_layer(layer_no, antigen_no, serum_no)}{Single titer for
  antigen and serum for the specified layer.}

}
\keyword{ package }
\examples{
# extract the first titer in the table
chart$titers$titer(1, 1)
# extract the last titer in the table
chart$titers$titer(chart$number_of_antigens, chart$number_of_sera)
# extract all titers into a matrix (it's a function, not property!)
chart$titers$all()
# invariant
identical(sapply(1:chart$number_of_antigens, function(ag_no) { chart$titers$titer(ag_no, 1) }), chart$titers$all()[,1])
}
\seealso{
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Example}}
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.GridTest}}\cr
  \code{\link{acmacs.merge}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
