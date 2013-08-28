# - Find AngelScript
# Find the native AngelScript includes and library
#
#   ANGELSCRIPT_FOUND       - True if AngelScript found.
#   ANGELSCRIPT_INCLUDE_DIR - where to find angelscript.h
#   ANGELSCRIPT_LIBRARY     - AngelScript Librarie
#

FIND_PATH( ANGELSCRIPT_INCLUDE_DIR "angelscript.h" )

FIND_LIBRARY( ANGELSCRIPT_LIBRARY "angelscript" )

# handle the QUIETLY and REQUIRED arguments and set ANGELSCRIPT_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( "FindPackageHandleStandardArgs" )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "AngelScript" DEFAULT_MSG ANGELSCRIPT_INCLUDE_DIR ANGELSCRIPT_LIBRARY )

MARK_AS_ADVANCED( ANGELSCRIPT_INCLUDE_DIR ANGELSCRIPT_LIBRARY )