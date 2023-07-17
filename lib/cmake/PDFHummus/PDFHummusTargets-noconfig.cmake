#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PDFHummus::Zlib" for configuration ""
set_property(TARGET PDFHummus::Zlib APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDFHummus::Zlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libZlib.a"
  )

list(APPEND _cmake_import_check_targets PDFHummus::Zlib )
list(APPEND _cmake_import_check_files_for_PDFHummus::Zlib "${_IMPORT_PREFIX}/lib/libZlib.a" )

# Import target "PDFHummus::FreeType" for configuration ""
set_property(TARGET PDFHummus::FreeType APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDFHummus::FreeType PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libFreeType.a"
  )

list(APPEND _cmake_import_check_targets PDFHummus::FreeType )
list(APPEND _cmake_import_check_files_for_PDFHummus::FreeType "${_IMPORT_PREFIX}/lib/libFreeType.a" )

# Import target "PDFHummus::LibAesgm" for configuration ""
set_property(TARGET PDFHummus::LibAesgm APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDFHummus::LibAesgm PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libLibAesgm.a"
  )

list(APPEND _cmake_import_check_targets PDFHummus::LibAesgm )
list(APPEND _cmake_import_check_files_for_PDFHummus::LibAesgm "${_IMPORT_PREFIX}/lib/libLibAesgm.a" )

# Import target "PDFHummus::LibJpeg" for configuration ""
set_property(TARGET PDFHummus::LibJpeg APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDFHummus::LibJpeg PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libLibJpeg.a"
  )

list(APPEND _cmake_import_check_targets PDFHummus::LibJpeg )
list(APPEND _cmake_import_check_files_for_PDFHummus::LibJpeg "${_IMPORT_PREFIX}/lib/libLibJpeg.a" )

# Import target "PDFHummus::LibTiff" for configuration ""
set_property(TARGET PDFHummus::LibTiff APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDFHummus::LibTiff PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C;CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libLibTiff.a"
  )

list(APPEND _cmake_import_check_targets PDFHummus::LibTiff )
list(APPEND _cmake_import_check_files_for_PDFHummus::LibTiff "${_IMPORT_PREFIX}/lib/libLibTiff.a" )

# Import target "PDFHummus::LibPng" for configuration ""
set_property(TARGET PDFHummus::LibPng APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDFHummus::LibPng PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libLibPng.a"
  )

list(APPEND _cmake_import_check_targets PDFHummus::LibPng )
list(APPEND _cmake_import_check_files_for_PDFHummus::LibPng "${_IMPORT_PREFIX}/lib/libLibPng.a" )

# Import target "PDFHummus::PDFWriter" for configuration ""
set_property(TARGET PDFHummus::PDFWriter APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDFHummus::PDFWriter PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libPDFWriter.a"
  )

list(APPEND _cmake_import_check_targets PDFHummus::PDFWriter )
list(APPEND _cmake_import_check_files_for_PDFHummus::PDFWriter "${_IMPORT_PREFIX}/lib/libPDFWriter.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
