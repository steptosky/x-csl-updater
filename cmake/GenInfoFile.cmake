# generator of the .h info  file witch is describing the project
function(genInfoFile destinationFile descriptionFile)

    message("Auto-generating info file to: <${destinationFile}>")

    include(${descriptionFile}) 
    loadDescription()
    
    #-----------------------------------------------------------#
    
    set(CONTENT  "#pragma once\n\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}VERSION_MAJOR  ${VersionMajorString}\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}VERSION_MINOR  ${VersionMinorString}\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}VERSION_PATCH  ${VersionPatchString}\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}VERSION_STRING \"${VersionMajorString}.${VersionMinorString}.${VersionPatchString}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}VERSION ((${InfoFilePrefix}VERSION_MAJOR << 16) + (${InfoFilePrefix}VERSION_MINOR << 8) + ${InfoFilePrefix}VERSION_PATCH)\n\n")
    
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}IDENTIFICATION_NAME \"${IdentificationName}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}ORGANIZATION_NAME \"${OrganizationName}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}ORGANIZATION_WEBLINK \"${OrganizationWebLink}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}PROJECT_NAME \"${ProjectName}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}PROJECT_SHORT_NAME \"${ProjectShortName}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}PROJECT_DESCRIPTION \"${ProjectDescription}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}PROJECT_WEBLINK \"${ProjectWebLink}\"\n\n")
    
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}AUTHOR \"${Author}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COPYRIGHT \"${CopyRight}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COPYRIGHT_STS \"${CopyRightSTS}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COPYRIGHT_XAR \"${CopyRightXAR}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COPYLEFT \"${CopyLeft}\"\n\n")
    
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}LICENSE_TYPE \"${LicenseType}\"\n\n")  
    
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}RELEASE_TYPE \"${ReleaseTypeString}\"\n\n")
    
    set(CONTENT  "${CONTENT}/*! \\warning For internal use only */\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COMPILE_DATE __DATE__\n")
    set(CONTENT  "${CONTENT}/*! \\warning For internal use only */\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COMPILE_TIME __TIME__\n\n")

    if (NOT vcs_revision)
        execute_process(
                COMMAND "git" "log" "-1" "--pretty=format:%h"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE vcs_result
                OUTPUT_VARIABLE vcs_revision
        )
    endif()
    if (NOT vcs_branch)
        execute_process(
                COMMAND "git" "rev-parse" "--abbrev-ref" "HEAD"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE vcs_result
                OUTPUT_VARIABLE vcs_branch
        )
        if(vcs_branch)
            string(STRIP ${vcs_branch} vcs_branch)
            set(CONTENT "${CONTENT}/* git */\n")
        endif()
    endif()

    #-------------------------------#
    if(NOT vcs_revision)
        set(vcs_revision "undefined")
    endif()
    if(NOT vcs_branch)
        set(vcs_branch "undefined")
    endif()
    
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}REVISION \"${vcs_revision}\"\n")
    set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}BRANCH \"${vcs_branch}\"\n")
        
    #-----------------------------------------------------------#
        
    file(WRITE ${destinationFile} "${CONTENT}")
    message("Auto-generating info file is DONE. File: <${destinationFile}>")

    return(PROPAGATE vcs_revision vcs_branch)
    
endfunction()
