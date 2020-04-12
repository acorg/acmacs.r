\name{acmacs.merge}
\title{Functions to merge charts.}
\alias{acmacs.merge}
\docType{package}
\usage{
chart1 <- new(acmacs.Chart, path.expand(filename1))
chart2 <- new(acmacs.Chart, path.expand(filename2))

table_merge <- acmacs.merge(chart1, chart2)
incremental_merge <- acmacs.merge_incremental(chart1, chart2, optimizations=100, threads=0)
frozen_merge <- acmacs.merge_frozen(chart1, chart2)
overlay_merge <- acmacs.merge_overlay(chart1, chart2)
report <- acmacs.merge_report(chart1, chart2)
}
\section{Table merge}{
  Merging two tables, if source charts have projections, they are
  ignored.

  \code{table_merge <- acmacs.merge(chart1, chart2, match="auto", merge=1)}

  \describe{
    \item{match}{
      \describe{
        \item{"a" ("auto")}{match antigens and sera automatically. In the most
          cases "auto" performs everything you need. It the default matching.}
        \item{"s" ("strict")}{match antigen passages and serum serum_ids
          strictrly. If two antigens with the same name have no passages, they
          will not be matched.}
        \item{"r" ("relaxed")}{match antigen passages and serum serum_ids
          strictrly. If two antigens with the same name have no passages, they
          will not be matched. If two antigens with the same name have different
          passages but they both are egg or both are cell, they will be matched.}
        \item{"i" ("ignore")}{ignore passages and serum ids when matching.}
    }}

    \item{Warning}{Resulting merge is checked for antigen/serum duplicates, if they are
    present, merge fails.}

    \item{merge}{
      \describe{
        See
        \link{https://github.com/acorg/acmacs-chart-2/blob/master/doc/merge-types.org}

        \item{1}{ignore projections of the source charts, resulting chart has no projections.}
        \item{2}{incremental merge without optimization, call merge$relax_incremental(number_of_optimizations) or merge$relax_incremental(number_of_optimizations, "unmovable-primary-points") afterwards}
        \item{3}{overlay merge without further optimization.}
        \item{4}{frozen merge variant with optimization.}
        \item{5}{another frozen merge variant with optimization.}
      }
    }
  }
}

\section{Merge report}{
  Long text showing how titers were merged

  \code{acmacs.merge_report(chart1, chart2)}
}
\keyword{ package }
\seealso{
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
  \code{\link{acmacs.Relax}}\cr
  \code{\link{acmacs.Example}}\cr
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.GridTest}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
