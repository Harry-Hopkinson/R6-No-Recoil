add_rules("mode.debug", "mode.release")

set_languages("c++20")

target("R6NoRecoil")
    set_kind("binary")
    add_files("src/*.cpp", "src/**/*.cpp")
    add_syslinks("gdi32", "user32", "kernel32")
