project "GLFW"
    location "glfw"
    kind "StaticLib"
    staticruntime "on"
    language "C"

    targetdir ("glfw/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("glfw/bin-obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/glfw_config.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
        }

        defines
        {
            "_GLFW_WIN32",
        }

    filter "system:linux"
        systemversion "latest"

        files
        {
            "glfw/src/x11_init.c",
            "glfw/src/linux_joystick.c",
            "glfw/src/x11_monitor.c",
            --"glfw/src/x11_time.c", -- NOTE(fhomolka): for whatever reason, GLFW removed this
            "glfw/src/posix_time.c", -- NOTE(fhomolka): for whatever reason, GLFW removed this
            "glfw/src/posix_thread.c",
            "glfw/src/x11_window.c",
            --"glfw/src/wgl_context.c",
            "glfw/src/glx_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
        }

        defines
        {
            "_GLFW_X11", --NOTE(fhomolka): Wayland is still experimental, use X11 only
        }


    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"