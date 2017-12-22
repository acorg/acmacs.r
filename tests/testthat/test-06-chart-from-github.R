context("Test reading chart from remote file")

raw_data <- readBin(file("https://github.com/acorg/acmacs.r/blob/master/tests/testthat/2004-3.ace?raw=true", open="rb"), "raw", n=999999)
chart <- new(acmacs.Chart, raw_data)
