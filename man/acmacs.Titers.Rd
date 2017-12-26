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

  \item{titers(antigen_no, serum_no)}{Single titer for antigen and serum.}

  \item{all()}{Dense matrix of all titers.}

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
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
