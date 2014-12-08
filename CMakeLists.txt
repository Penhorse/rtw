cmake_minimum_required(VERSION 3.0)

project(rtw)

macro(append_string output str)
	set("${output}" "${${output}}" "${str}")
endmacro()

set(dir_inc include)

set(rtw_include_files
	arrays
	bits
	error
	filesystem
	meta
	rtw
	scoped_op
	linux/filesystem
)

macro(set_include_files output dir)
	foreach(file_name ${ARGN})
		append_string(${output} ${dir}/${file_name}.hpp)
	endforeach()
endmacro()

set_include_files(rtw_includes ${dir_inc}/rtw ${rtw_include_files})

add_custom_target(rtw ALL)

macro(copy_dir_files target)
	foreach(file ${ARGN})
		add_custom_command(
			TARGET rtw
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${file})
	endforeach()
endmacro()

copy_dir_files(rtw ${rtw_includes})
