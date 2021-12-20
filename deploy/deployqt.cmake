if(WIN32)
    list(JOIN LANGS "," QT_LANGS)
    set(DEPLOYQT_ARGS --no-quick-import --translations ${QT_LANGS})

    find_program(DEPLOYQT_EXECUTABLE "windeployqt" HINTS "${_qt_bin_dir}")
    execute_process(
        COMMAND "${DEPLOYQT_EXECUTABLE}" ${DEPLOYQT_ARGS} "${_target_file_dir}"
        RESULT_VARIABLE ret
    )

    if(NOT ret EQUAL "0")
        message(FATAL_ERROR "${DEPLOYQT_EXECUTABLE} returns ${ret}")
    endif()
endif()
