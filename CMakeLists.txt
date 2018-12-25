# 设置cmake最低版本号
cmake_minimum_required(VERSION 3.6)

# 设置工程名称
project(SLIC)

# 添加编译选项
set(CMAKE_AUTOMOC ON) # 设置自动生成moc文件，一定要设置
set(CMAKE_AUTOUIC ON) # 设置ui文件被修改后，可以生成对应的ui头文件
set(CMAKE_AUTORCC ON) # 处理资源文件
set(CMAKE_PREFIX_PATH "E:/Toolkits/QT/Qt5.12.0/5.12.0/msvc2017_64")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 添加openmp
find_package( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# 查找opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 查找需要的Qt库文件，最好每一个库都要写，Qt也会根据依赖关系自动添加
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(
	./
	./Qt
    ./Slic
)

set(Srcs
	main.cpp
	Qt/mainwindow.cpp
	Qt/SlicThread.cpp
	Slic/slic.cpp
	)
    
set(Hdrs
	Slic/slic.h
	)

set ( MOC_HEADERS
 	Qt/mainwindow.h
	Qt/SlicThread.h
)

set(UIs 
    Qt/mainwindow.ui
	)

# 通过Ui文件生成对应的头文件，一定要添加
qt5_wrap_ui(UI_Files ${UIs})
qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

# 将ui文件和生成文件整理在一个文件夹中，非必须
# 存放ui和qrc原始文件
source_group("Resources" FILES
	${UIs}
	)

# 存放qt的生成文件(包括moc头文件，资源文件和ui文件)
source_group("Generated" FILES
	${UI_Files}
	${MOC_SRCS}
	)

# 存放qt的源代码文件
source_group("Qt" FILES
	Qt/mainwindow.h
	Qt/mainwindow.cpp
	Qt/SlicThread.h
	Qt/SlicThread.cpp
	)

# 存放算法的源代码文件
source_group("Slic" FILES
	Slic/slic.cpp
	Slic/slic.h
	)

# 存放main.cpp
source_group("Main" FILES
	main.cpp
	)

add_executable(${PROJECT_NAME} ${Srcs} ${Hdrs} ${UI_Files} ${MOC_SRCS})

# 添加静态链接库
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})