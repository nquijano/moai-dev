# find_package ( OpenGL REQUIRED )
if (MSVC)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP -DNDEBUG")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP -DNDEBUG")

    #include debug info inside the libs (not as .pdb files)
    string (REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string (REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
     message("Windows Visual Studio Build Detected")
  else ()
     #work around bug with mingw in release mode failing at link time due to different sized duplicate segments! Optimise for size instead
     set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")
     set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")
     set ( GCC_FORCE_32_BIT TRUE) #lets have 32bit for windows (for luajits sake)
     
     set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -std=gnu99 -static -Werror-implicit-function-declaration -DHAVE_MEMMOVE" )
     set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -static" )
  endif(MSVC)
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 -D_WIN32_IE=0x0500 " )
  SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 -D_WIN32_IE=0x0500"  )