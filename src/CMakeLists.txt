add_library(BS BlackScholesModel.cpp BlackScholesModel.hpp)
add_library(MC MonteCarlo.cpp MonteCarlo.hpp)
add_library(OP Option.cpp Option.hpp
               AsianOption.cpp AsianOption.hpp
               BasketOption.cpp BasketOption.hpp
               PerformanceOption.cpp PerformanceOption.hpp
               QuantoOption.cpp QuantoOption.hpp
	       Eurostral100.cpp Eurostral100.hpp) 
add_library(UT Utilities.cpp Utilities.hpp 
               parser.cpp parser.hpp)


add_subdirectory(tests)
