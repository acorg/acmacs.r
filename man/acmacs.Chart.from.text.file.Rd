\name{acmacs.Chart.from.text.file}
\title{Example of reading chart from a text file.}
\alias{acmacs.Chart.from.text.file}
\section{Example}{
\bold{# ========= Source text file =========}\cr
\preformatted{# The first row is space separated serum names
# other rows have antigen name and titers for that antigen

        OH/1/17 Br/2/17 Br/3/17 TX/2/17 FL/33/17
OH/1/17 1280 160 40 40 20
Br/2/17 640 640 640 320 160
Br/3/17 40 320 640 160 320
TX/4/17 20 160 640 320 640
FL/5/17 20 320 640 160 320
SP/6/17 640 320 320 160 160
SP/7/17 20 320 640 320 320
MD/8/17 640 160 80 40 10
MD/9/17 80 160 80 40 40
AL/1/17 1280 320 80 80 20
}
\bold{# ========= Reading text file into a chart =========}\cr
\preformatted{
library(acmacs.r)
rows <- readLines("tab.tab")
serum_names <- scan(text=rows[[1]], what="c", quiet=TRUE)
chart <- new(acmacs.Chart, length(rows) - 1, length(serum_names))
for (sr_no in 1:length(serum_names)) \{
  chart$sera[[sr_no]]$set_name(serum_names[[sr_no]])
\}
for (row_no in 2:length(rows)) \{
   name_titers <- scan(text=rows[[row_no]], what="c", quiet=TRUE)
   chart$antigens[[row_no-1]]$set_name(name_titers[[1]])
   for (sr_no in 1:length(serum_names)) \{
     chart$titers$set_titer(row_no-1, sr_no, name_titers[[sr_no + 1]])
   \}
\}
}
\bold{# ========= Make a map =========}\cr
\preformatted{
# optimize chart 100 times in 2d without minimum column basis using fine optimization
# table is small, 100 optimizations take less than a second
chart$relax_many("none", 2, 100, FALSE)
# report best map info
cat(chart$info)
}
}
\docType{package}
\keyword{ package }
\seealso{
  \code{\link{acmacs.r-package}}\cr
  \code{\link{acmacs.Chart}}\cr
  \code{\link{acmacs.Chart.from.scratch}}\cr
  \code{\link{acmacs.Example}}\cr
  \code{\link{acmacs.Antigen}}\cr
  \code{\link{acmacs.Serum}}\cr
  \code{\link{acmacs.Projection}}\cr
  \code{\link{acmacs.PlotSpec}}\cr
  \code{\link{acmacs.Titers}}\cr
}
\author{Eugene Skepner, eu@antigenic-cartography.org.}
