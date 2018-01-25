\name{acmacs.r-package}
\title{acmacs.r - the Antigenic Cartography package}
\alias{acmacs.r-package}
\alias{acmacs.r}
\docType{package}
\description{
  This package provides R with ability to read, modify, save the data stored in acmacs and
  lispmds files.

  \bold{Available features:}
  \enumerate{
    \item Loading data from .ace, acd1, .save files (can be compressed
  with bzip2 and xz)
    \item Saving data to .ace and .save files, i.e. possible converting from
    acmacs to lispmds and vice versa.
    \item Reading antigen and serum names, passages and other related data
    \item Reading projection layout, column bases, stress, rotating and
  flipping the map, moving points.
    \item Reading and modifying plot spec
    \item Reading titers
    \item Optimization from scratch
    \item Optimization from existing layout
    }
  \bold{Planned features:}
  \enumerate{
    \item Creating antigenic tables from scratch
    \item Modifying antigens and sera
    \item Modifying titers
    \item Making procrustes
    \item Reporting error line data
    \item Reporting blob data
    }
}
\details{
 Please see \code{\link{acmacs.Chart}} documentation about package usage and examples.
}
\examples{
  # load file
  chart <- new(acmacs.Chart, "/tmp/table.save")
  # print chart name
  cat(chart$name)
}
\keyword{ package }
\seealso{
 \code{\link{acmacs.Chart}}\cr
 \code{\link{acmacs.Example}}
}
\author{Eugene Skepner, \email{eu@antigenic-cartography.org}}
