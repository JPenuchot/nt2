################################################################################
#         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
#         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
#
#          Distributed under the Boost Software License, Version 1.0.
#                 See accompanying file LICENSE.txt or copy at
#                     http://www.boost.org/LICENSE_1_0.txt
################################################################################

################################################################################
# Find BLAS and LAPACK
# - NT2_BLAS_VENDOR can be set to Intel, Goto2, Netlib or be empty
# - NT2_BLAS_ROOT specify a directory where libraries are. If using Intel,
#                 prefer setting MKLROOT instead.
# - NT2_BLAS_MULTICORE can be set to 1 or 0 to enable/disable multicore handling
#                      if supported by the BLAS implementation.
#                      if not set, defaults to 1 if computer is multicore.
# - NT2_BLAS_STATIC force usage of static variant of libraries.
#                   can be useful on some system to workaround issues with MKL
################################################################################
include(nt2.info)

set(NT2_BLAS_FOUND FALSE)

if(NT2_BLAS_STATIC)
  set(OLD_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib")
endif()

# Intel MKL
if(NT2_BLAS_VENDOR STREQUAL "Intel" OR (NOT DEFINED NT2_BLAS_VENDOR AND NOT NT2_BLAS_FOUND))

  if(NOT DEFINED NT2_BLAS_VENDOR)
    message(STATUS "[nt2.blas] trying Intel...")
  endif()

  set(MKLROOT $ENV{MKLROOT})
  if(NOT MKLROOT)
    set(MKLROOT /opt/intel/mkl)
  endif()

  set(NT2_BLAS_LIBRARIES)
  set(NT2_MKL_LIBRARY_DIR ${MKLROOT}/lib)
  if(NT2_ARCH_X86_64)
    set(NT2_MKL_LIBRARY_SUFFIXES intel64)
    find_library(NT2_MKL_LP64 NAMES mkl_intel_lp64 mkl_intel_lp64_dll
                 PATHS ${NT2_MKL_LIBRARY_DIR}
                 PATH_SUFFIXES ${NT2_MKL_LIBRARY_SUFFIXES}
                )
    set(NT2_BLAS_LIBRARIES ${NT2_BLAS_LIBRARIES} ${NT2_MKL_LP64})
  else()
    set(NT2_MKL_LIBRARY_SUFFIXES ia32)
    set(NT2_MKL_LIBRARY_DIR ${NT2_BLAS_ROOT} ${MKLROOT}/lib/ia32)
    find_library(NT2_MKL_32 NAMES mkl_intel mkl_intel_c_dll
                 PATHS ${NT2_MKL_LIBRARY_DIR}
                 PATH_SUFFIXES ${NT2_MKL_LIBRARY_SUFFIXES}
                )
    set(NT2_BLAS_LIBRARIES ${NT2_BLAS_LIBRARIES} ${NT2_MKL_32})
  endif()

  if(NT2_MKL_LP64 OR NT2_MKL_32)
    if(NOT DEFINED NT2_BLAS_MULTICORE)
      nt2_module_tool(is_multicore RESULT_VARIABLE RESULT_VAR OUTPUT_QUIET)
      if(RESULT_VAR EQUAL 0)
        set(NT2_BLAS_MULTICORE 1)
      endif()
    endif()

    if(NOT NT2_BLAS_MULTICORE)
      find_library(NT2_MKL_SEQ NAMES mkl_sequential mkl_sequential_dll
                   PATHS ${NT2_MKL_LIBRARY_DIR}
                   PATH_SUFFIXES ${NT2_MKL_LIBRARY_SUFFIXES}
                  )
      set(NT2_BLAS_LIBRARIES ${NT2_BLAS_LIBRARIES} ${NT2_MKL_SEQ})
    else()
      # avoid re-checking for OpenMP every time
      if(NOT DEFINED OpenMP_CXX_FLAGS)
        find_package(OpenMP QUIET)
      endif()
      if(NT2_COMPILER_ICC OR NT2_COMPILER_MSVC OR NOT OpenMP_CXX_FLAGS)
        unset(NT2_MKL_GNU_THREAD)
        find_library(NT2_MKL_INTEL_THREAD NAMES mkl_intel_thread mkl_intel_thread_dll
                     PATHS ${NT2_MKL_LIBRARY_DIR}
                     PATH_SUFFIXES ${NT2_MKL_LIBRARY_SUFFIXES}
                    )
        file(TO_CMAKE_PATH "$ENV{LIBRARY_PATH}" LIBRARY_PATH)
        find_library(NT2_INTEL_OMP NAMES iomp5 iomp5md libiomp5md
                     PATHS ${LIBRARY_PATH}
                     PATH_SUFFIXES ${NT2_MKL_LIBRARY_SUFFIXES}
                    )
        set(NT2_BLAS_LIBRARIES ${NT2_BLAS_LIBRARIES} ${NT2_MKL_INTEL_THREAD} ${NT2_INTEL_OMP})
      elseif(NT2_COMPILER_GCC_LIKE)
        unset(NT2_MKL_INTEL_THREAD)
        unset(NT2_INTEL_OMP)
        find_library(NT2_MKL_GNU_THREAD NAMES mkl_gnu_thread
                     PATHS ${NT2_MKL_LIBRARY_DIR}
                     PATH_SUFFIXES ${NT2_MKL_LIBRARY_SUFFIXES}
                    )
        set(NT2_BLAS_LIBRARIES ${NT2_BLAS_LIBRARIES} ${NT2_MKL_GNU_THREAD})
        set(NT2_BLAS_LINK_FLAGS ${OpenMP_CXX_FLAGS})
      endif()

      if(UNIX)
        find_package(Threads)
        set(NT2_BLAS_LIBRARIES ${NT2_BLAS_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
      endif()
    endif()

    find_library(NT2_MKL_CORE NAMES mkl_core mkl_core_dll
                 PATHS ${NT2_MKL_LIBRARY_DIR}
                 PATH_SUFFIXES ${NT2_MKL_LIBRARY_SUFFIXES}
                )
    set(NT2_BLAS_LIBRARIES ${NT2_BLAS_LIBRARIES} ${NT2_MKL_CORE})

    if(NT2_MKL_SEQ AND NT2_MKL_CORE)
      set(NT2_BLAS_FOUND TRUE)
      set(NT2_LAPACK_FOUND TRUE)
    elseif(NT2_MKL_GNU_THREAD OR (NT2_MKL_INTEL_THREAD AND NT2_INTEL_OMP))
      if(NT2_MKL_CORE)
        set(NT2_BLAS_FOUND TRUE)
        set(NT2_LAPACK_FOUND TRUE)
      endif()
    endif()

  endif()

  if(NT2_BLAS_FOUND AND NOT DEFINED NT2_BLAS_VENDOR)
    set(NT2_BLAS_VENDOR Intel)
    message(STATUS "[nt2.blas] Intel found")
  endif()
endif()

if(NT2_BLAS_VENDOR STREQUAL "Goto2" OR (NOT DEFINED NT2_BLAS_VENDOR AND NOT NT2_BLAS_FOUND))
  if(NOT DEFINED NT2_BLAS_VENDOR)
    message(STATUS "[nt2.blas] trying Goto2...")
  endif()

  find_library(NT2_GOTO2 NAMES goto2
               PATHS ${NT2_BLAS_ROOT})
  if(NT2_GOTO2)
    set(NT2_BLAS_FOUND TRUE)
    set(NT2_BLAS_LIBRARIES ${NT2_GOTO2})

    if(NOT DEFINED NT2_BLAS_VENDOR)
      set(NT2_BLAS_VENDOR Goto2)
      message(STATUS "[nt2.linalg] Goto2 found")
    endif()
  endif()
endif()

if(NT2_BLAS_VENDOR STREQUAL "Netlib" OR (NOT DEFINED NT2_BLAS_VENDOR AND NOT NT2_BLAS_FOUND))
  if(NOT DEFINED NT2_BLAS_VENDOR)
    message(STATUS "[nt2.blas] trying Netlib...")
  endif()

  find_library(NT2_NETLIB NAMES blas_LINUX
               PATHS ${NT2_BLAS_ROOT})
  if(NT2_NETLIB)
    set(NT2_BLAS_FOUND TRUE)
    set(NT2_BLAS_LIBRARIES ${NT2_NETLIB})

    if(NOT DEFINED NT2_BLAS_VENDOR)
      set(NT2_BLAS_VENDOR Netlib)
      message(STATUS "[nt2.blas] Netlib found")
    endif()
  endif()
endif()

if(NOT NT2_BLAS_FOUND)

  if(NOT DEFINED NT2_BLAS_VENDOR)
    message(STATUS "[nt2.blas] trying built-in...")
  endif()

  find_library(BLAS_LIBRARY NAMES blas libblas PATHS ${NT2_BLAS_ROOT})
  if(BLAS_LIBRARY)
    set(NT2_BLAS_FOUND 1)
    set(NT2_BLAS_LIBRARIES ${BLAS_LIBRARY})
  endif()

  if(NOT NT2_BLAS_FOUND)
    find_package(BLAS QUIET)
    set(NT2_BLAS_FOUND ${BLAS_FOUND})
    set(NT2_BLAS_LIBRARIES ${BLAS_LIBRARIES})
    set(NT2_BLAS_LINK_FLAGS ${BLAS_LINKER_FLAGS})
  endif()

  if(NT2_BLAS_FOUND AND NOT DEFINED NT2_BLAS_VENDOR)
    set(NT2_BLAS_VENDOR "")
    message(STATUS "[nt2.blas] built-in found")
  endif()

endif()

if(NOT NT2_LAPACK_FOUND) # Find lapack if not MKL
  message(STATUS "[nt2.linalg] looking for LAPACK...")

  find_library(LAPACK_LIBRARY NAMES lapack liblapack PATHS ${NT2_BLAS_ROOT})
  if(LAPACK_LIBRARY)
    set(NT2_LAPACK_FOUND 1)
    set(NT2_LAPACK_LIBRARIES ${LAPACK_LIBRARY})
  endif()

  if(NOT NT2_LAPACK_FOUND)
    find_package(LAPACK QUIET)
    set(NT2_LAPACK_FOUND ${LAPACK_FOUND})
    set(NT2_LAPACK_LIBRARIES ${LAPACK_LIBRARIES})
  endif()

  if(NT2_LAPACK_FOUND) # Find lapack if not MKL
    message(STATUS "[nt2.linalg] LAPACK found")
  endif()
endif()

set(NT2_BLAS_VENDOR ${NT2_BLAS_VENDOR} CACHE STRING "BLAS vendor (supported: \"Intel\", \"Goto2\", \"Netlib\")")
if(NOT NT2_BLAS_FOUND OR NOT NT2_LAPACK_FOUND)
  message(STATUS "[nt2.linalg] BLAS or LAPACK not found, if available please specify NT2_BLAS_ROOT or NT2_BLAS_VENDOR")
endif()

if(NT2_BLAS_STATIC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${OLD_CMAKE_FIND_LIBRARY_SUFFIXES})
endif()
