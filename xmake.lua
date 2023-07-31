add_rules("mode.debug", "mode.release")

add_requires("pdfhummus")


target("PdfDemo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("pdfhummus")