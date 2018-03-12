\name{acmacs.Serum}
\title{Class to access serum related data.}
\alias{acmacs.Serum-class}
\alias{acmacs.Serum}
\docType{package}
\usage{
serum <- chart$sera[[serum_number]]
serum$name
serum$date
serum$passage
}
\value{An object of class \code{"acmacs.Serum"} to access and modify
  serum data. It has the following properties:

  \item{name}{name of the serum.}

  \item{full_name}{name of the serum including annotations, reassortant,
    serum_id.}

  \item{paste(antigen), toString(antigen)}{return full name.}

  Properties below are not available for lispmds saves and always return
  default (empty or falsy values).

  \item{abbreviated_name}{name with the abbreviated location and
    two-digit year, available only for charts with parsed names.}

  \item{passage}{passage history of the antigen this serum was made from.}

  \item{lineage}{Influenza B lineage, \code{"VICTORIA"} or \code{"YAMAGATA"}.}

  \item{reassortant}{reassortant information.}

  \item{annotations}{List of character objects with additional data
    associated with the serum. Special annotation \code{"DISTINCT"} is
    used to avoid duplicating sera in the chart, DISTINCT sera are
    never merged when tables are merged.}

  \item{serum_id}{Serum Id.}

  \item{serum_species}{Serum species.}
}
\section{Changing fields}{
  \code{set_name(new_name)}

  \code{set_passage(new_passage)}

  \code{set_lineage(lineage)} - lineage is either VICTORIA or YAMAGATA

  \code{set_reassortant(reassortant)}

  \code{set_serum_id(serum_id)}

  \code{set_serum_species(serum_species)}

  \code{add_annotation(annotation)} - annotations is a set of unique strings

  \code{remove_annotation(annotation)}
}
\section{Adding/removing sera}{
  Adding and removing sera is done via Chart object because it affects
  other chart fields (titers, projections, etc.)
  see \code{\link{acmacs.Chart}} and \code{\link{acmacs.Chart.from.scratch}}
}
\examples{
  # load chart from file
  chart <- new(acmacs.Chart, "/tmp/table.save")
  # print information about sera
  srs <- chart$sera
  format_serum <- function(a) \{ paste(c(a$name, a$annotations, a$serum_id, a$serum_species), collapse=" ", sep="") \}
  cat("sera:", length(srs), "\n ", paste(sapply(srs[1:min(80, length(srs))], format_serum), collapse="\n  "), "\n")
  # print full names of all sera
  cat(paste(sapply(srs, toString), collapse="\n"))
}
\seealso{
 \code{\link{acmacs.Antigen}}\cr
 \code{\link{acmacs.Example}}
}
\keyword{ package }
\author{Eugene Skepner, eu@antigenic-cartography.org.}
