\name{acmacs.Chart}
\title{Main class to access data stored in the chart file.}
\alias{acmacs.Chart-class}
\alias{acmacs.Chart}
\alias{acmacs.ColumnBases}
\docType{package}
\usage{
\code{new(acmacs.Chart, filename)}

\code{raw_data <- readBin(file(url, open="rb"), "raw", n=999999)}
\code{chart <- new(acmacs.Chart, raw_data)}
}
\arguments{
  \item{filename}{A character object specifying a file pathname with
    chart data to load}
  \item{raw_data}{A raw vector specifying raw data (e.g. read from a
    remote file) to load}
}
\value{An object of class \code{"acmacs.Chart"} to access and modify
  chart data. It has the following properties:

  \item{name}{name of the chart.}

  \item{paste(chart), toString(chart)}{equivalent to chart$name.}

  \item{info}{brief multiline description of the chart.}

  \item{number_of_antigens}{number of antigens in the chart.}

  \item{number_of_sera}{number of sera in the chart.}

  \item{number_of_points}{number of points in the chart, the sum of the
  number of antigens and the number of sera.}

  \item{number_of_projections}{number of projections (relaxed maps) in the chart.}

  \item{column_bases(...)}{see below.}

  \item{lineage}{for influenza B chart only, \code{"VICTORIA"} or \code{"YAMAGATA"}.}

  \item{antigens}{List of \code{\link{acmacs.Antigen}} objects}

  \item{sera}{List of \code{\link{acmacs.Serum}} objects.}

  \item{projections}{List of \code{\link{acmacs.Projection}} objects.}

  \item{plot_spec}{\code{\link{acmacs.PlotSpec}} object.}

  \item{titers}{\code{\link{acmacs.Titers}} object.}

  \item{save(filename)}{Save (modified) chart into a file, output
  format is determined by filename suffix: .ace, .save, .save.xz.}

}
\section{Column bases}{
  Column bases is a propery of a projection. Formally, chart may contain
  multiple projections with different column bases, but that situation
  is rare. When new projection is created, column bases for it are calculated
  from chart's titers; if user supplied minimum column basis on
  projection creation, its value is applied to calculated column bases.

  If chart is a merge of individual HI or Neut tables and those tables
  contain more-than titers, then column bases for the resulting merge
  are forced by the program. In that case new projection receives those
  forced column bases.

  \code{column_bases()} - if chart has projections, returns column bases
  of the first (best) projection. Otherwise returns either forced or
  recalculated (with minimum column basis = "none") column bases.

  \code{column_bases(projection_no)} - if projection_no is valid
  (i.e. in range 0:(number_of_projections-1)), returns column_bases for
  that projection. Otherwise returns either forced or
  recalculated (with minimum column basis = "none") column bases.

  \code{column_bases(-1, "1280")} - Returns either forced or
  recalculated (with minimum column basis = "1280") column bases.

}
\keyword{ package }
\examples{
  # load chart from file
  chart <- new(acmacs.Chart, "/tmp/table.save")
  # print information about antigens
  ags <- chart$antigens
  format_antigen <- function(a) \{ paste(c(a$name, a$passage, a$reassortant, a$reference, a$annotations, paste("[", a$date, "]", sep="", collapse=""), a$lab_ids), collapse=" ", sep="") \}
  cat("antigens:", length(ags), "\n ", paste(sapply(ags, format_antigen), collapse="\n  "), "\n")
  # print full names of all antigens (full name includes annotations, reassortant, passage)
  cat(paste(sapply(chart$antigens, toString), collapse="\n"))
}
\seealso{
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.Example}}
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
