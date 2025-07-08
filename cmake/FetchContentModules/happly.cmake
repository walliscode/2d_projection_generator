
include(FetchContent)

FetchContent_Declare(
  happly
  GIT_REPOSITORY https://github.com/nmwsharp/happly.git
  GIT_TAG        master # or a specific commit/tag/branch
)

FetchContent_MakeAvailable(happly)
