# ------------------------------------------------------------------------------
# Doxygen
find_package(Doxygen OPTIONAL_COMPONENTS dot)
if(DOXYGEN_FOUND)
  set(DOXYGEN_PROJECT_BRIEF "${PROJECT_NAME}")
  set(DOXYGEN_GENERATE_TREEVIEW YES)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ../README.md)

  doxygen_add_docs(
    doxygen
    ../README.md
    ../cmake/README.md
    ../docs
    ../include
    COMMENT
    "Generate doxygen documentation"
  )

else()
  message(
    AUTHOR_WARNING
      "Doxygen needs to be installed to generate developer documentation"
  )

endif()
