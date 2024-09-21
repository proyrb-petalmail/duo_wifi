cmake_minimum_required(VERSION 3.0)

project(wifi VERSION 1.0)

set(CMAKE_C_COMPILER riscv64-unknown-linux-musl-gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g0 -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d")
set(CMAKE_CXX_COMPILER riscv64-unknown-linux-musl-g++)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -g0 -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d")

# set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g0 -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d")
# set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
# set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -g0 -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/boot)

include_directories(
	./main
	./main/argument
	./main/context
	./main/debug
	./main/error
	./main/font
	./main/gui
	
	./module
	./module/cmdline
	./module/configor
	./module/configor/details
	./module/lvgl
	./module/lvgl/src
	./module/lvgl/src/core
	./module/lvgl/src/display
	./module/lvgl/src/draw
	./module/lvgl/src/draw/nxp
	./module/lvgl/src/draw/nxp/pxp
	./module/lvgl/src/draw/nxp/vglite
	./module/lvgl/src/draw/renesas
	./module/lvgl/src/draw/renesas/dave2d
	./module/lvgl/src/draw/sdl
	./module/lvgl/src/draw/sw
	./module/lvgl/src/draw/sw/arm2d
	./module/lvgl/src/draw/sw/blend
	./module/lvgl/src/draw/sw/blend/arm2d
	./module/lvgl/src/draw/sw/blend/helium
	./module/lvgl/src/draw/sw/blend/neon
	./module/lvgl/src/draw/vg_lite
	./module/lvgl/src/drivers
	./module/lvgl/src/drivers/display
	./module/lvgl/src/drivers/display/drm
	./module/lvgl/src/drivers/display/fb
	./module/lvgl/src/drivers/display/ili9341
	./module/lvgl/src/drivers/display/lcd
	./module/lvgl/src/drivers/display/renesas_glcdc
	./module/lvgl/src/drivers/display/st7735
	./module/lvgl/src/drivers/display/st7789
	./module/lvgl/src/drivers/display/st7796
	./module/lvgl/src/drivers/display/tft_espi
	./module/lvgl/src/drivers/evdev
	./module/lvgl/src/drivers/glfw
	./module/lvgl/src/drivers/libinput
	./module/lvgl/src/drivers/nuttx
	./module/lvgl/src/drivers/qnx
	./module/lvgl/src/drivers/sdl
	./module/lvgl/src/drivers/wayland
	./module/lvgl/src/drivers/windows
	./module/lvgl/src/drivers/x11
	./module/lvgl/src/font
	./module/lvgl/src/indev
	./module/lvgl/src/layouts
	./module/lvgl/src/layouts/flex
	./module/lvgl/src/layouts/grid
	./module/lvgl/src/libs
	# ./module/lvgl/src/libs/barcode
	# ./module/lvgl/src/libs/bin_decoder
	# ./module/lvgl/src/libs/bmp
	# ./module/lvgl/src/libs/ffmpeg
	# ./module/lvgl/src/libs/freetype
	# ./module/lvgl/src/libs/fsdrv
	# ./module/lvgl/src/libs/gif
	# ./module/lvgl/src/libs/libjpeg_turbo
	# ./module/lvgl/src/libs/libpng
	./module/lvgl/src/libs/lodepng
	# ./module/lvgl/src/libs/lz4
	# ./module/lvgl/src/libs/qrcode
	# ./module/lvgl/src/libs/rle
	# ./module/lvgl/src/libs/rlottie
	# ./module/lvgl/src/libs/thorvg
	# ./module/lvgl/src/libs/thorvg/rapidjson
	# ./module/lvgl/src/libs/thorvg/rapidjson/error
	# ./module/lvgl/src/libs/thorvg/rapidjson/internal
	# ./module/lvgl/src/libs/thorvg/rapidjson/msinttypes
	# ./module/lvgl/src/libs/tiny_ttf
	# ./module/lvgl/src/libs/tjpgd
	./module/lvgl/src/misc
	./module/lvgl/src/misc/cache
	./module/lvgl/src/osal
	./module/lvgl/src/others
	./module/lvgl/src/others/file_explorer
	./module/lvgl/src/others/fragment
	./module/lvgl/src/others/gridnav
	./module/lvgl/src/others/ime
	./module/lvgl/src/others/imgfont
	./module/lvgl/src/others/monkey
	./module/lvgl/src/others/observer
	./module/lvgl/src/others/snapshot
	./module/lvgl/src/others/sysmon
	./module/lvgl/src/others/vg_lite_tvg
	./module/lvgl/src/stdlib
	./module/lvgl/src/stdlib/builtin
	./module/lvgl/src/stdlib/clib
	./module/lvgl/src/stdlib/micropython
	./module/lvgl/src/stdlib/rtthread
	./module/lvgl/src/themes
	./module/lvgl/src/themes/default
	./module/lvgl/src/themes/mono
	./module/lvgl/src/themes/simple
	./module/lvgl/src/tick
	./module/lvgl/src/widgets
	./module/lvgl/src/widgets/animimage
	./module/lvgl/src/widgets/arc
	./module/lvgl/src/widgets/bar
	./module/lvgl/src/widgets/button
	./module/lvgl/src/widgets/buttonmatrix
	./module/lvgl/src/widgets/calendar
	./module/lvgl/src/widgets/canvas
	./module/lvgl/src/widgets/chart
	./module/lvgl/src/widgets/checkbox
	./module/lvgl/src/widgets/dropdown
	./module/lvgl/src/widgets/image
	./module/lvgl/src/widgets/imagebutton
	./module/lvgl/src/widgets/keyboard
	./module/lvgl/src/widgets/label
	./module/lvgl/src/widgets/led
	./module/lvgl/src/widgets/line
	./module/lvgl/src/widgets/list
	./module/lvgl/src/widgets/lottie
	./module/lvgl/src/widgets/menu
	./module/lvgl/src/widgets/msgbox
	./module/lvgl/src/widgets/objx_templ
	./module/lvgl/src/widgets/property
	./module/lvgl/src/widgets/roller
	./module/lvgl/src/widgets/scale
	./module/lvgl/src/widgets/slider
	./module/lvgl/src/widgets/span
	./module/lvgl/src/widgets/spinbox
	./module/lvgl/src/widgets/spinner
	./module/lvgl/src/widgets/switch
	./module/lvgl/src/widgets/table
	./module/lvgl/src/widgets/tabview
	./module/lvgl/src/widgets/textarea
	./module/lvgl/src/widgets/tileview
	./module/lvgl/src/widgets/win
)

file(GLOB_RECURSE
	source_file_list
	${CMAKE_CURRENT_SOURCE_DIR}/main/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/main/*.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/module/cmdline/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/module/cmdline/*.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/module/configor/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/module/configor/*.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/module/lvgl/*.S
	${CMAKE_CURRENT_SOURCE_DIR}/module/lvgl/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/module/lvgl/*.cpp
)

add_executable(wifi ${source_file_list})
