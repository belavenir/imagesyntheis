# opencv
FIND_PACKAGE (OpenCV REQUIRED)


ADD_EXECUTABLE (imagesynth syn.cpp)
TARGET_LINK_LIBRARIES (imagesynth ${OpenCV_LIBS})

