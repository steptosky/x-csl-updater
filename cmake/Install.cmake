# Use HAS_NO_LIBS=ON If your library does not have any binary artefacts
# Use INSTALL_PDB=ON If you want to install Microsoft .pdb file.
# Use IS_STATIC=ON If you want to install files in other lib dir like lib_static.
# Use STS_ENV_INSTALL If you want installing the project according STS rules.
# PROJECT is used for specify project  target.

#----------------------------------------------------------#

if(IS_STATIC)
	set(LIB_DIR_NAME lib_static)	
else()
	set(LIB_DIR_NAME lib)	
endif(IS_STATIC)

#----------------------------------------------------------#

if(MSVC)
	if(INSTALL_PDB AND NOT IS_STATIC)
		get_target_property(_TMP_POSTFIX ${PROJECT} DEBUG_POSTFIX)
		get_target_property(_TMP_OUTPUT_NAME ${PROJECT} DEBUG_OUTPUT_NAME)
		if(NOT _TMP_OUTPUT_NAME)			
			set(_TMP_SRS_PDB ${CMAKE_SOURCE_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}bin/Debug/${PROJECT}${_TMP_POSTFIX}.pdb)
		else()
			set(_TMP_SRS_PDB ${CMAKE_SOURCE_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}bin/Debug/${_TMP_OUTPUT_NAME}${_TMP_POSTFIX}.pdb)
		endif()
		message("Install .pdb is on.")		
	endif()
endif(MSVC)

#----------------------------------------------------------#
		
if (STS_ENV_INSTALL)

	include(${CMAKE_SOURCE_DIR}/Description.cmake)
	loadDescription()	
	include(${CMAKE_SOURCE_DIR}/cmake/DeployFolder.cmake)
	makeDeployFolderString()
		
	set(install_destination_folder "${STS_ENV_INSTALL}")
	set(install_destination_folder "${install_destination_folder}/${EnvTargetFolder}")
	set(install_destination_folder "${install_destination_folder}/${VersionMajorString}.${VersionMinorString}")
	install (CODE "FILE(REMOVE_RECURSE ${install_destination_folder}/include)")
	install (DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION ${install_destination_folder})
		
	if(NOT HAS_NO_LIBS)
				
		file(WRITE ${CMAKE_BINARY_DIR}/compiler "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
	
		install(TARGETS ${PROJECT} 
			RUNTIME DESTINATION  ${install_destination_folder}/${DEPLOY_FOLDER}/bin
			LIBRARY DESTINATION  ${install_destination_folder}/${DEPLOY_FOLDER}/${LIB_DIR_NAME}
			ARCHIVE DESTINATION  ${install_destination_folder}/${DEPLOY_FOLDER}/${LIB_DIR_NAME}
		)
		install (FILES ${CMAKE_BINARY_DIR}/compiler DESTINATION ${install_destination_folder}/${DEPLOY_FOLDER}) 
		
		#----------------------------------------------------------#
		
		if(MSVC)
			if(INSTALL_PDB)
				set(_TMP_DEST_PDB ${install_destination_folder}/${DEPLOY_FOLDER}/bin)	
				install (FILES ${_TMP_SRS_PDB} DESTINATION ${_TMP_DEST_PDB} CONFIGURATIONS Debug)
			endif()
		endif(MSVC)
		
		#----------------------------------------------------------#
				
	endif()
	
else()

	install(TARGETS ${PROJECT} 
		RUNTIME DESTINATION  bin
		LIBRARY DESTINATION  ${LIB_DIR_NAME}
		ARCHIVE DESTINATION  ${LIB_DIR_NAME}
	)
	install (DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include) 
	
		#----------------------------------------------------------#
		
		if(MSVC)
			if(INSTALL_PDB)	
				install (FILES ${_TMP_SRS_PDB} DESTINATION bin CONFIGURATIONS Debug)
			endif()
		endif(MSVC)
		
		#----------------------------------------------------------#
	
endif()

