\name{acmacs.Antigen}
\title{Class to access antigen related data.}
\alias{acmacs.Antigen-class}
\alias{acmacs.Antigen}
\alias{acmacs.Passage}
\alias{acmacs.Passage-class}
\docType{package}
\usage{
antigen <- chart$antigens[[antigen_number]]
antigen$name
antigen$date
antigen$passage
antigen$passage$type()
}
\value{An object of class \code{"acmacs.Antigen"} to access and modify
  antigen data. It has the following properties:

  \item{name}{name of the antigen.}

  \item{full_name}{name of the antigen including annotations, reassortant, passage.}

  \item{paste(antigen), toString(antigen)}{return full name.}

  Properties below are not available for lispmds saves and always return
  default (empty or falsy values).

  \item{abbreviated_name}{name with the abbreviated location and
  two-digit year, available only for charts with parsed names.}

  \item{date}{isolation date of antigen in format YYYY-MM-DD.}

  \item{passage}{passage history object (see below).}

  \item{lineage}{Influenza B lineage, \code{"VICTORIA"} or \code{"YAMAGATA"}.}

  \item{reassortant}{reassortant information.}

  \item{reference}{boolean flag indicating if antigen was declared as
  reference one.}

  \item{lab_ids}{List of character objects in format LAB#LAB_ID.}

  \item{annotations}{List of character objects with additional data
  associated with the antigen. Special annotation \code{"DISTINCT"} is
  used to avoid duplicating antigens in the chart, DISTINCT antigens are
  never merged when tables are merged.}

}
\section{Passage}{
  antigen$passage is an object with the following properties:

  \code{is_egg()} returns if passage is egg (logical).

  \code{is_cell()} returns if passage is cell (logical).

  \code{without_date()} returns passage without date (string), makes sense
  for CDC passages.

  \code{type()} returns passage type (string), "egg" or "cell"

  \code{paste(antigen$passage)} converts passage to string.
}
\section{Changing fields}{
  \code{set_name(new_name)}

  \code{set_date(new_date)} - date is expected in format YYYY-MM-DD

  \code{set_passage(new_passage)}

  \code{set_lineage(lineage)} - lineage is either VICTORIA or YAMAGATA

  \code{set_reassortant(reassortant)}

  \code{set_reference(TRUE)}

  \code{add_annotation(annotation)} - annotations is a set of unique strings

  \code{remove_annotation(annotation)}
}
\section{Adding/removing antigens}{
  Adding and removing antigens is done via Chart object because it affects
  other chart fields (titers, projections, etc.)
  see \code{\link{acmacs.Chart}} and \code{\link{acmacs.Chart.from.scratch}}
}
\examples{
  # load chart from file
  chart <- new(acmacs.Chart, "/tmp/table.save")
  # print information about antigens
  ags <- chart$antigens
  format_antigen <- function(a) \{
    paste(c(a$name, a$passage, a$reassortant, a$reference, a$annotations, paste("[", a$date, "]", sep="", collapse=""), a$lab_ids), collapse=" ", sep="")
  \}
  cat("antigens:", length(ags), "\n ", paste(sapply(ags, format_antigen), collapse="\n  "), "\n")
  # print full names of all antigens
  cat(paste(sapply(ags, toString), collapse="\n"))
}
\seealso{
 \code{\link{acmacs.Serum}}\cr
 \code{\link{acmacs.Example}}
}
\keyword{ package }
\author{Eugene Skepner, eu@antigenic-cartography.org.}
