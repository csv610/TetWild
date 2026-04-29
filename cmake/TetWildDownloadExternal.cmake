################################################################################
include(DownloadProject)

# Shortcut function
function(tetwild_download_project name)
    download_project(
        PROJ         ${name}
        SOURCE_DIR   ${TETWILD_EXTERNAL}/${name}
        DOWNLOAD_DIR ${TETWILD_EXTERNAL}/.cache/${name}
        ${ARGN}
    )
endfunction()

################################################################################

## libigl
function(tetwild_download_libigl)
    tetwild_download_project(libigl
        GIT_REPOSITORY https://github.com/libigl/libigl.git
        GIT_TAG        main
    )
endfunction()

## geogram
function(tetwild_download_geogram)
    tetwild_download_project(geogram
        GIT_REPOSITORY https://github.com/BrunoLevy/geogram.git
        GIT_TAG        main
    )
endfunction()


## fmt
function(tetwild_download_fmt)
    tetwild_download_project(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        12.1.0
    )
endfunction()

## spdlog
function(tetwild_download_spdlog)
    tetwild_download_project(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.17.0
    )
endfunction()

## CLI11
function(tetwild_download_cli11)
    tetwild_download_project(cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.6.2
    )
endfunction()
