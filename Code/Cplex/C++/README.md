### How to set cplex on visual studio with c++?

Project -> Property

**[General]**

C:\Program Files\IBM\ILOG\CPLEX_Studio1210\concert\include

C:\Program Files\IBM\ILOG\CPLEX_Studio1210\cplex\include


**[Preprocessor]**

NDEBUG

_CONSOLE

IL_STD


**[Code Generator]**

Multi-threads DLL (/MD)


**Linker:**

C:\Program Files\IBM\ILOG\CPLEX_Studio1210\concert\lib\x64_windows_msvc14\stat_mda\concert.lib

C:\Program Files\IBM\ILOG\CPLEX_Studio1210\cplex\lib\x64_windows_msvc14\stat_mda\cplex12100.lib

C:\Program Files\IBM\ILOG\CPLEX_Studio1210\cplex\lib\x64_windows_msvc14\stat_mda\ilocplex.lib
