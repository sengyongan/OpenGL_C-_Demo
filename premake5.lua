--OpenGl_Demo workspace --------------------------------
workspace "OpenGl_Demo"--解决方案
    architecture "x64"--函数调用，简单情况省略（）
    startproject "engine"

    configurations--列表
    {
        "Debug",
        "Release"
    }
        
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"--定义变量：输出目录
        
--Opengl project --------------------------------
project "OpenGl"--项目
    location "OpenGL"--相对输出路径(vcxproj……   )
    kind "SharedLib"--动态库
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("bin/".. outputdir .."/%{prj.name}")--相对premake路径
    objdir ("bin-int/".. outputdir .."/%{prj.name}")--..是连接两个字符串的操作符
    
    files--项目中的源文件和头文件
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/glad/src/glad.c"
    }
    
    includedirs--在编译时需要搜索的头文件目录
    {
        "%{prj.name}/vendor/OpenGL/includes",
        "%{prj.name}/vendor/OpenGL/lib",
        "%{prj.name}/vendor/glm",
        "%{prj.name}/vendor/glad/include",
        "%{prj.name}/vendor/stb_image",
        "%{prj.name}/vendor/assimp/include",
        "%{prj.name}/vendor/freetype/include",
        "OpenGL/src"
    }

    libdirs--链接器在链接时搜索库文件的目录
    {
        "OpenGL/vendor/OpenGL/lib",
        "OpenGL/vendor/freetype/lib",
        "OpenGL/vendor/assimp/lib"
    }

    links --列出了实际要链接的库文件名
	{ 
        "glfw3",
        "freetype",
		"assimp-vc143-mtd.lib"
	}
    --Windows --------------------------------
    filter "system:windows"--仅针对window平台
        systemversion "latest"--SDK版本

        defines
        {
            "BUILD_DLL"
        }

        --构建完成后要执行的--复制%{cfg.buildtarget.relpath}（dll）到，相对bin/outpudir/engine下
        postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/engine")
		}
    filter "configurations:Debug"
        runtime "Debug"--调试版本的运行时库
        symbols "On"--调试

    filter "configurations:Release"
        runtime "Release"
        optimize "On"--优化打开

--engine project --------------------------------
project "engine"--项目
    location "engine"--相对输出路径(vcxproj……   )
    kind "ConsoleAPP"--动态库
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/".. outputdir .."/%{prj.name}")--相对premake路径
    objdir ("bin-int/".. outputdir .."/%{prj.name}")--..是连接两个字符串的操作符

    files--项目包含的文件
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "OpenGL/vendor/OpenGL/includes",
        "OpenGL/vendor/glm",
        "OpenGL/vendor/Glad/include",
        "OpenGL/vendor/assimp/include",
        "%{prj.name}/vendor/freetype/include",
        "OpenGL/src"
    }

    libdirs
    {
        "OpenGL/vendor/OpenGL/lib",
        "%{prj.name}/vendor/freetype/include",
        "OpenGL/vendor/assimp/lib" 
    }

    links
    {
        "glfw3",
        "OpenGL",
        "assimp-vc143-mtd.lib"
    }
    --Windows --------------------------------
    filter "system:windows"--仅针对window平台
        systemversion "latest"--SDK版本

    filter "configurations:Debug"
        runtime "Debug"--调试版本的运行时库
        symbols "On"--调试

    filter "configurations:Release"
        runtime "Release"
        optimize "On"--优化打开

