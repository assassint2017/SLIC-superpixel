# 设置cmake版本号
cmake_minimum_required(VERSION 3.6)

# 设置工程名称
project(SLIC)

# 添加编译选项
set(CMAKE_AUTOMOC ON) # 设置自动生成moc文件，一定要设置
set(CMAKE_AUTOUIC ON) # 设置ui文件被修改后，可以生成对应的ui头文件
set(CMAKE_AUTORCC ON) # 自己之后写的带有Q_OBJECT前缀的类，如果没有编译选项的话就会报链接错误
set(CMAKE_PREFIX_PATH "E:/Toolkits/QT/Qt5.12.0/5.12.0/msvc2017_64")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 查找opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 查找需要的Qt库文件，最好每一个库都要写，Qt也会根据依赖关系自动添加
find_package(Qt5Widgets REQUIRED)  
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

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
	Qt/mainwindow.h
	Qt/SlicThread.h
	Slic/slic.h
	)

set(UIs 
    Qt/mainwindow.ui
	)

# 通过Ui文件生成对应的头文件，一定要添加
qt5_wrap_ui(UI_Files ${UIs})

#将ui文件和生成文件整理在一个文件夹中，非必须
source_group("Resources" FILES
	${UIs}
	)
source_group("Generated" FILES
	${UI_Files}
	${MOC_Srcs}
	)
source_group("Qt" FILES
	Qt/mainwindow.h
	Qt/mainwindow.cpp
	Qt/SlicThread.h
	Qt/SlicThread.cpp
	)
source_group("Slic" FILES
	Slic/slic.cpp
	Slic/slic.h
	)

# 创建工程文件
add_executable(${PROJECT_NAME} ${Srcs} ${Hdrs} ${UI_Files})

# 添加静态链接库
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})