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
	set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COPYLEFT \"${CopyLeft}\"\n\n")
	
	set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}LICENSE_TYPE \"${LicenseType}\"\n\n")	
	
	set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}RELEASE_TYPE \"${ReleaseTypeString}\"\n\n")
	
	set(CONTENT  "${CONTENT}/*! \\warning For internal use only */\n")
	set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COMPILE_DATE __DATE__\n")
	set(CONTENT  "${CONTENT}/*! \\warning For internal use only */\n")
	set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}COMPILE_TIME __TIME__\n\n")

	if (${VcsType} STREQUAL hg)
		execute_process(
			COMMAND "hg" "parent" "--template" "{node|short}"
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			RESULT_VARIABLE vcs_result
			OUTPUT_VARIABLE vcs_revision
		)
		set(CONTENT  "${CONTENT}/* hg */\n")
	elseif (${VcsType} STREQUAL git)
		execute_process(
			COMMAND "git" "log" "-1" "--pretty=format:%h"
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			RESULT_VARIABLE vcs_result
			OUTPUT_VARIABLE vcs_revision
		)
		set(CONTENT  "${CONTENT}/* git */\n")
	else ()
		message ("WARNING: Unknown VCS")
	endif ()
	
	if(${vcs_result} EQUAL 0)
		set(CONTENT  "${CONTENT}#define ${InfoFilePrefix}REVISION \"${vcs_revision}\"\n")		
	else()
		message ("WARNING: VCS is not found, so the ${InfoFilePrefix}REVISION constant is not added.")
	endif ()
		
	#-----------------------------------------------------------#
		
	file(WRITE ${destinationFile} "${CONTENT}")
    message("Auto-generating info file is DONE. File: <${destinationFile}>")
	
endfunction()
