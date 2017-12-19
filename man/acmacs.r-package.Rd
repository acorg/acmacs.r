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
    \item Reading and modifying antigen and serum names, passages and
    other related data
    \item Reading projection layout, column bases, stress
    \item Reading and modifying plot spec
    }
  \bold{Planned features:}
  \enumerate{
    \item Optimization from scratch
    \item Optimization from existing layout
    \item Creating antigenic tables from scratch
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
 \code{\link{acmacs.Chart}}
}
\author{Eugene Skepner, \email{eu@antigenic-cartography.org}}
