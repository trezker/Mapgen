name = "mapgen"

solution (name)
	configurations { "Debug", "Release" }

--[[	project (name)
		kind "ConsoleApp"
		language "C++"
		location "build"
		files { "src/*.cpp" }
		targetdir "build"
		includedirs { "include", "src" }
		links ("alleg")
		libdirs { "/usr/local/lib" }
]]--
	project (name .. "5")
		kind "ConsoleApp"
		language "C++"
		location "build"
		files { "src5/*.cpp" }
		targetdir "build"
		includedirs { "include", "src5" }
		links { "allegro", "allegro_font", "allegro_ttf", "alledge" }
--		libdirs { "/usr/local/lib" }
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
 
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }
