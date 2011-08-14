name = "mapgen"

solution (name)
	configurations { "Debug", "Release" }

	project (name)
		kind "ConsoleApp"
		language "C++"
		location "build"
		files { "src/*.cpp" }
		targetdir "build"
		includedirs { "include", "src" }
		links ("alleg")
		libdirs { "/usr/local/lib" }
