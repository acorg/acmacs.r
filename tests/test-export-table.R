library(acmacs.r)
# print(getwd())
# chart_filename <- "tests/testthat/2004-3.ace"
chart_filename <- "testthat/2004-3.ace"
chart <- new(acmacs.Chart, chart_filename)

table <- chart$titers$all()
rownames(table) <- lapply(chart$antigens, function(ag) { ag$full_name })
colnames(table) <- lapply(chart$sera, function(sr) { sr$full_name })
write.table(table)
