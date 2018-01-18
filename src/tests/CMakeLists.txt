add_executable(BlackScholes test_BlackScholes.cpp)
target_link_libraries(BlackScholes BS ${LIBS})

add_executable(AsianOption test_AsianOption.cpp)
target_link_libraries(AsianOption OP ${LIBS})

add_executable(BasketOption test_BasketOption.cpp)
target_link_libraries(BasketOption OP ${LIBS})

add_executable(CallQuanto test_call_quanto.cpp)
target_link_libraries(CallQuanto OP ${LIBS})

add_executable(PerformanceOption test_PerformanceOption.cpp)
target_link_libraries(PerformanceOption OP ${LIBS})

add_executable(Opt test_options.cpp)
target_link_libraries(Opt OP ${LIBS})

add_executable(MonteCarlo test_MonteCarlo.cpp)
target_link_libraries(MonteCarlo BS MC OP UT ${LIBS})

add_executable(Delta test_Delta.cpp)
target_link_libraries(Delta BS MC OP UT ${LIBS}) 

add_executable(PLasian test_PLasian.cpp)
target_link_libraries(PLasian BS MC OP UT ${LIBS})

add_executable(PLbasket test_PL.cpp)
target_link_libraries(PLbasket BS MC OP UT ${LIBS})

add_executable(PLcall test_PLcall.cpp)
target_link_libraries(PLcall BS MC OP UT ${LIBS})

add_executable(Parser test_parser.cpp)
target_link_libraries(Parser UT ${LIBS})

add_executable(PriceSupZero test_price_sup_zero.cpp)
target_link_libraries(PriceSupZero BS MC OP UT ${LIBS})

add_executable(SimulMarket test_simul_market.cpp)
target_link_libraries(SimulMarket BS MC OP UT ${LIBS})

