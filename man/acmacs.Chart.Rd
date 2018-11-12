\name{acmacs.Chart}
\title{Main class to access data stored in the chart file.}
\alias{acmacs.Chart-class}
\alias{acmacs.Chart}
\alias{acmacs.ColumnBases}
\docType{package}
\usage{
chart <- new(acmacs.Chart, path.expand(filename))

raw_data <- readBin(file(url, open="rb"), "raw", n=999999)
chart <- new(acmacs.Chart, raw_data)
}
\arguments{
  \item{filename}{A character object specifying a file pathname with
    chart data to load, ~ expansion is not supported, please use path.expand()}
  \item{raw_data}{A raw vector specifying raw data (e.g. read from a
    remote file) to load}
}
\value{An object of class \code{"acmacs.Chart"} to access and modify
  chart data. It has the following properties:

  \item{name}{name of the chart. assigning to it changes chart name or
  name prefix depending on the other fields in the original chart data.}

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

  \item{sort_projections()}{sorts projections by stress, in the result
    the first projection (\code{chart$projections[[1]]}) will be the
    bestone}

  \item{remove_all_projections()}{removes all projections from the chart}

  \item{remove_all_projections_except(projection_number)}{removes all projections except
  one specified by number from the chart}

  \item{plot_spec}{\code{\link{acmacs.PlotSpec}} object.}

  \item{titers}{\code{\link{acmacs.Titers}} object.}

  \item{save(filename)}{Save (modified) chart into a file, output
  format is determined by filename suffix: .ace, .save,
  .save.xz. Please use path.expand() to expand ~ in filenames}

  \item{relax(minimum_column_basis, number_of_dimensions)}{creates new
  projection with random layout and optimizes it.\cr \bold{minimum_column_basis}
  is a string, e.g. "none" or "1280".\cr \bold{number_of_dimensions} is a number,
  e.g. 2 or 3.}

  \item{relax(minimum_column_basis,\cr number_of_dimensions,\cr
  rough)}{creates new projection with random layout and optimizes
  it.\cr \bold{minimum_column_basis} is a string, e.g. "none" or
  "1280".\cr \bold{number_of_dimensions} is a number, e.g. 2 or 3.\cr \bold{rough} is
  boolean, when TRUE optimization is about 30\% faster, result is very
  slightly worse, visually no distinguishable.}

  \item{relax_many(minimum_column_basis,\cr number_of_dimensions,\cr
  number_of_optimizations,\cr rough)}{creates new projection with random
  layout and optimizes it.\cr \bold{minimum_column_basis} is a string,
  e.g. "none" or "1280".\cr \bold{number_of_dimensions} is a number,
  e.g. 2 or 3.\cr \bold{number_of_optimizations} is a number, e.g. 10 or
  100, optimization is performed in parallel if multiple CPU cores
  available.\cr \bold{rough} is boolean, when TRUE optimization is about
  30\% faster, result is slightly worse, visually no distinguishable.}

  \item{remove_layers()}{if chart is a merge, titers cannot be
    modified. if you want to change them anyway, you have to remove
    layers from the chart. merging resulting (flattened) chart with
    other charts usually gives wrong result.}

  \item{clone()}{chart is cloned by exporting it into ace (in memory)
    and re-reading it}

  \item{new_projection(minimum_column_basis,\cr
  number_of_dimensions)}{creates new projection with random layout,
  optimization is not run, dimension annealing cannot be used.}

  \item{new_proj <- clone_projection(projection_no)}{clones the specified
    projection, adds it to the same chart.}

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
\section{Adding/removing antigens/sera}{
  \code{insert_antigen(before)} - inserts new antigen before the
  provided index. chart$insert_antigen(1) inserts new antigen before the
  first one, chart$insert_antigen(chart$number_of_antigens+1) appends
  new antigen. New antigen will have random name and all titers
  will be set to *.

  \code{insert_serum(before)} - inserts new serum before the
  provided index. chart$insert_serum(1) inserts new serum before the
  first one, chart$insert_serum(chart$number_of_sera+1) appends
  new serum. New serum will have random name and all titers
  will be set to *.

  \code{remove_antigens(list_of_indexes)} - removes antigens with the
  specified indexes. chart$remove_antigens(c(1,2,3)) removes the first
  three antigens.

  \code{remove_sera(list_of_indexes)} - removes sera with the
  specified indexes. chart$remove_sera(c(1,2,3)) removes the first
  three sera.

}
\keyword{ package }
\examples{
  # load chart from file
  chart <- new(acmacs.Chart, path.expand("/tmp/table.save"))
  # print information about antigens
  ags <- chart$antigens
  format_antigen <- function(a) \{ paste(c(a$name, a$passage, a$reassortant, a$reference, a$annotations, paste("[", a$date, "]", sep="", collapse=""), a$lab_ids), collapse=" ", sep="") \}
  cat("antigens:", length(ags), "\n ", paste(sapply(ags, format_antigen), collapse="\n  "), "\n")
  # print full names of all antigens (full name includes annotations, reassortant, passage)
  cat(paste(sapply(chart$antigens, toString), collapse="\n"))
  # change chart name
  chart$name <- "modified"
}
\seealso{
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.Example}}
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.GridTest}}\cr
  \code{\link{acmacs.merge}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
