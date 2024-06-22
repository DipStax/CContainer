#pragma once

#if defined(_WIN32)
    #define EXIT_WITH_SEGV ::testing::ExitedWithCode(3)
#else
    #define EXIT_WITH_SEGV ::testing::KilledBySignal(SIGSEGV)
#endif
