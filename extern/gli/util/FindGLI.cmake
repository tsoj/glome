if (NOT GLI_PACKAGE_DIR)
    message("ERROR: GLI_PACKAGE_DIR requires to be set")
endif (NOT GLI_PACKAGE_DIR)

set(GLI_INCLUDE_DIR ${GLI_PACKAGE_DIR})
set(GLI_LIBRARY_DIR)

include_directories(${GLI_INCLUDE_DIR})

if (GLI_PACKAGE_DIR AND GLI_INCLUDE_DIR)
    set(GLI_FOUND "YES")
endif (GLI_PACKAGE_DIR AND GLI_INCLUDE_DIR)
