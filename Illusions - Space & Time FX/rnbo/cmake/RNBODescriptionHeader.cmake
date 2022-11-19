function(rnbo_write_description_header DESCRIPTION_JSON OUTPUT_DIR)
	set(PATCHER_DESCRIPTION_JSON ${DESCRIPTION_JSON})
	configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/rnbo_description.h.in ${OUTPUT_DIR}/rnbo_description.h)
endfunction()
