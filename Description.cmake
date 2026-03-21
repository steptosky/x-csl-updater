macro(loadDescription)

	String(TIMESTAMP _curr_y_ "%Y" UTC)
	
	#-----------------------------------------------------------#

	set(IdentificationName "XCSLUPDATER")

	set(OrganizationName "VA X-Air Team && StepToSky Team")
	set(OrganizationWebLink "www.x-air.ru")
	
	set(ProjectName "X-CSL-Updater")
	set(ProjectShortName "X-CSL-Updater")
	set(ProjectDescription "This X-CSL-Updater software is a part of X-CSL-Package project.")
	set(ProjectWebLink "csl.x-air.ru")
	
	set(CopyRight "© 2009 - ${_curr_y_} StepToSky")
	set(Author "StepToSky")
	set(LicenseType "The BSD 2-Clause License")
	
	set(VersionMajorString "1")
	set(VersionMinorString "3")
	set(VersionPatchString "2")
	
	set(ReleaseTypeString "")
	set(InfoFilePrefix "STS_XCSL_")
	set(EnvTargetFolder "sts-xcsl-updater")
	
	#-----------------------------------------------------------#
	
endmacro()
