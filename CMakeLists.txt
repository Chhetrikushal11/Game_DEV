cmake_minimum_required(VERSION 3.10)



project(GameDevelopmentProject
		VERSION 1.0.0
		LANGUAGES C CXX
		)

	# to make sure the CXX standrad is 14
set(CMAKE_CXX_STANDARD          14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS       OFF)


add_subdirectory(GameDev)