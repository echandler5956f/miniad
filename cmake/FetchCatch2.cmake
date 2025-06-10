include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2  # Pinned stable version
)
FetchContent_MakeAvailable(catch2) 