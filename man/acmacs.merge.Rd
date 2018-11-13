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

  \code{table_merge <- acmacs.merge(chart1, chart2, match="auto", merge="none")}

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
        \item{"n" ("none")}{ignore projections of the source charts.}
        \item{"i" ("incremental")}{perform incremental merge (see corresponding section below).}
        \item{"m" ("overlay")}{perform frozen or overlay merge (see corresponding section below).}
      }
    }
  }
}
\section{Incremental merge}{
  The best projection of the first (master) chart is copied to the resulting chart, projections
      of the second (merged in) char ignored. Positions of points that are in the
      merged in chart and not in the master chart are randomized and
      resulting layout relaxed.

  \code{incremental_merge <- acmacs.merge_incremental(chart1, chart2, optimizations=100, threads=0)}

  \describe{
    \item{optimizations}{Number of times to randomize positions of the
    points found in the merged in chart and then relax.}
    \item{threads}{Number of threads to use for optimization. 0 -
    autodetect, 1 - use just one thread (in case you use other packages
    linked against OpenMP library that conflict with acmacs), another
    positive number - use that number of threads.}
  }
}
\section{Frozen merge}{
  The best projection of the first chart is copied to the resulting chart.
  The best projection of the second chart is oriented to the best
    projection of the first chart using procrustes, then points that are
    found just in the second chart receive coordinates from that
    reoriented projection. Points that are common for the first and the
    second chart placed to their middle positions between the first
    chart layout and reoriented second chart layout.

  \code{frozen_merge <- acmacs.merge_frozen(chart1, chart2)}
}
\section{Overlay merge}{
  Frozen merge followed by relaxation of the resulting projection.

  \code{overlay_merge <- acmacs.merge_overlay(chart1, chart2)}
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
  \code{\link{acmacs.Example}}
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Chart.from.text.file}}\cr
  \code{\link{acmacs.GridTest}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
