# RTW_INCLUDE_DIRS
# RTW_FOUND

find_path(
	RTW_INCLUDE_DIR
	PATHS
		rtw/error.hpp
		rtw/filesystem.hpp
		rtw/rtw.hpp
	PATH_SUFFIXES
		include
	HINTS
		${RTW_ROOT}
	)

set(RTW_INCLUDE_DIRS ${RTW_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	RTW
	REQUIRED_VARS
	RTW_INCLUDE_DIR
)

mark_as_advanced(
	RTW_INCLUDE_DIR
)
