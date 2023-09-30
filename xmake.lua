add_rules("mode.debug", "mode.release")

target("czyshot")
    add_rules("qt.widgetapp")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_includedirs("src")
    -- add_includedirs("/usr/include/libnotify")
    -- 添加库搜索路径
    -- add_linkdirs("/usr/lib")

    -- 添加需要链接的库
    -- add_links("notify")

    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files("src/show/showpic.h")
    add_files("src/show/cappic.h")
